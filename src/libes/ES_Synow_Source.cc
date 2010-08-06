// 
// File    : ES_Synow_Source.cc
// ----------------------------
//
// Copyright (C) 2010 Rollin C. Thomas <rcthomas@lbl.gov>
// 
// This file is part of ES.
// 
// ES is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your
// option) any later version.
// 
// ES is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
// for more details.
// 
// You should have received a copy of the GNU General Public License along
// with ES.  If not, see <http://www.gnu.org/licenses/>.
//  

#include <moat.h>       // tmp
#include <moat_ocl.h>   // tmp

#include "ES_Synow_Source.hh"
#include "ES_Synow_Grid.hh"
#include "ES_Synow_Setup.hh"
#include "ES_Blackbody.hh"

#ifdef _OPENMP
#include "omp.h"
#endif

#include <cmath>
#include <iostream>
#include <algorithm>

ES::Synow::Source::Source( ES::Synow::Grid& grid, size_t const mu_size ) :
    ES::Synow::Operator( grid ),
    _mu_size( mu_size )
{
    size_t v_size  = _grid->v_size;
    _mu    = new double [ v_size * _mu_size * 2 ];
    _dmu   = new double [ v_size * _mu_size * 2 ];
    _shift = new double [ v_size * _mu_size * 2 ];
}

ES::Synow::Source::~Source()
{
    delete [] _mu;
    delete [] _dmu;
    delete [] _shift;
}

void ES::Synow::Source::operator() ( const ES::Synow::Setup& setup )
{

    moat_prof_register( "OCL_CPU" , "Total CPU time");
    moat_prof_start( "OCL_CPU" );

    // Local caching.

    double v_phot  = setup.v_phot;
    double v_outer = setup.v_outer;
    double v_step  = _grid->v[ 1 ] - _grid->v[ 0 ];
    size_t v_size  = _grid->v_size;
    size_t wl_used = _grid->wl_used;

    // Prepare the wavelength-independent source metadata.

    for( size_t iv = 0; iv < v_size; ++ iv )
    {

        double v = _grid->v[ iv ];
        double mu_crit = iv > 0 ? sqrt( 1.0 - v_phot * v_phot / v / v ) : 0.0;

        size_t offset, i;
        double dmu, mu_init;

        // Photosphere.

        dmu     = ( 1.0 - mu_crit ) / double( _mu_size );
        mu_init = 1.0 - 0.5 * dmu;

        offset = iv * _mu_size * 2;
        for( size_t im = 0; im < _mu_size; ++ im )
        {
            i = offset + im;
            _mu   [ i ] = mu_init - im * dmu;
            _dmu  [ i ] = dmu;
            _shift[ i ] = 1.0 / ( 1.0 + ( v * _mu[ i ] - sqrt( v * v * ( _mu[ i ] * _mu[ i ] - 1.0 ) + v_phot  * v_phot  ) ) / 299.792 );
        }

        // Sky.

        dmu     = ( mu_crit + 1.0 ) / double( _mu_size );
        mu_init = mu_crit - 0.5 * dmu;

        offset = iv * _mu_size * 2 + _mu_size;
        for( size_t im = 0; im < _mu_size; ++ im )
        {
            i = offset + im;
            _mu   [ i ] = mu_init - im * dmu;
            _dmu  [ i ] = dmu;
            _shift[ i ] = 1.0 / ( 1.0 + ( v * _mu[ i ] + sqrt( v * v * ( _mu[ i ] * _mu[ i ] - 1.0 ) + v_outer * v_outer ) ) / 299.792 );
        }

    }

    // Adjust photosphere SED table as needed.

    {
        double blue_wl = _grid->wl[ 0 ];
        for( size_t i = 0; i < v_size * _mu_size; ++ i )
        {
            double shift_wl = _grid->wl[ 0 ] * _shift[ i ];
            if( shift_wl < blue_wl ) blue_wl = shift_wl;
        }
        (*_grid->bb)( blue_wl );
    }

    // Integration.

    size_t   offset, im, i, start, ib, il, iu;
    double   v, in, d, vd, cl, cu, et, ss;

    for( size_t iw = 0; iw < wl_used; ++ iw )
    {
        #pragma omp parallel for private( v, offset, im, i, in, start, ib, d, vd, il, iu, cl, cu, et, ss ) schedule( static, 1 )
        for( size_t iv = 0; iv < v_size; ++ iv )
        {
            v = _grid->v[ iv ];
            offset = iv * _mu_size * 2;
            for( im = 0; im < _mu_size * 2; ++ im )
            {
                i  = offset + im;
                in = im < _mu_size ? (*_grid->bb)( _grid->wl[ iw ] * _shift[ i ] ) * pow( _shift[ i ], 3 ) : 0.0;
                start = std::upper_bound( _grid->wl, _grid->wl + wl_used, _grid->wl[ iw ] * _shift[ i ] ) - _grid->wl;
                for( ib = start; ib < iw; ++ ib )
                {
                    d  = ( _grid->wl[ iw ] / _grid->wl[ ib ] - 1.0 ) * 299.792;
                    vd = sqrt( v * v + d * d - 2.0 * v * d * _mu[ i ] );
                    il = size_t( ( vd - v_phot ) / v_step );
                    iu = il + 1;
                    cl = ( _grid->v[ iu ] - vd ) / v_step;
                    cu = 1.0 - cl;
                    et = cl * _grid->tau[ ib * v_size + il ] + cu * _grid->tau[ ib * v_size + iu ];
                    ss = cl * _grid->src[ ib * v_size + il ] + cu * _grid->src[ ib * v_size + iu ];
                    et = exp( - et );
                    in = in * et + ss * ( 1.0 - et ) * pow( _grid->wl[ ib ] / _grid->wl[ iw ], 3 );
                }
                _grid->src[ iw * v_size + iv ] += in * _dmu[ i ];
            }
            _grid->src[ iw * v_size + iv ] *= 0.5;
        }
    }

    moat_prof_stop( "OCL_CPU" );
    moat_prof_print( stderr );

}
