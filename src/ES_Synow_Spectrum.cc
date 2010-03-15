// 
// File    : ES_Synow_Spectrum.cc
// ------------------------------
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

#include "ES_Spectrum.hh"
#include "ES_Synow_Spectrum.hh"
#include "ES_Synow_Grid.hh"
#include "ES_Synow_Setup.hh"
#include "ES_Blackbody.hh"

#include <cmath>
#include <algorithm>

ES::Synow::Spectrum::Spectrum( ES::Synow::Grid& grid, ES::Spectrum& output, int const p_size ) :
   ES::Synow::Operator( grid ),
   _output( &output ),
   _p_size( p_size ), 
   _p_total( 5 * p_size )
{
   _alloc( false );
}

ES::Synow::Spectrum::~Spectrum()
{
   _clear();
}

void ES::Synow::Spectrum::operator() ( const ES::Synow::Setup& setup )
{

   // Local caching.

   double v_phot  = setup.v_phot;
   double v_outer = setup.v_outer;
   double v_step  = _grid->v[ 1 ] - _grid->v[ 0 ];
   int    v_size  = _grid->v_size;
   int    wl_used = _grid->wl_used;

   // Set up impact parameters, resizing arrays if needed.

   double p_step = v_phot / double( _p_size );
   double p_init  = 0.5 * p_step;
   int    p_outer = 0;
   while( p_init + p_step * p_outer < v_outer ) ++ p_outer;

   if( p_outer > _p_total )
   {
      _p_total = 2 * p_outer;
      _alloc( true );
   }

   for( int ip = 0; ip < p_outer; ++ ip )
   {
      _p[ ip ] = p_init + ip * p_step;
      _max_shift[ ip ] = 1.0 + sqrt( v_outer * v_outer - _p[ ip ] * _p[ ip ] ) / 299.792;
      _min_shift[ ip ] = ip < _p_size ? 1.0 + sqrt( v_phot  * v_phot  - _p[ ip ] * _p[ ip ] ) / 299.792 : 1.0 / _max_shift[ ip ];
   }

   // Compute.

   double norm = 0.0;
   for( int ip = 0; ip < _p_size; ++ ip ) norm += _p[ ip ] * p_step;
   norm = 1.0 / norm;

   for( int iw = 0; iw < _output->size(); ++ iw )
   {
      int start = std::upper_bound( _grid->wl, _grid->wl + wl_used, _output->wl( iw ) * _min_shift[ _p_size ] ) - _grid->wl;
      int stop  = std::upper_bound( _grid->wl, _grid->wl + wl_used, _output->wl( iw ) * _max_shift[ 0       ] ) - _grid->wl;
      for( int ip = 0; ip < p_outer; ++ ip ) _in[ ip ] = ip < _p_size ? (*_grid->bb)( _output->wl( iw ) * ( 1.0 + sqrt( v_phot * v_phot - _p[ ip ] * _p[ ip ] ) / 299.792 ) ) : 0.0;
      for( int ib = start; ib < stop; ++ ib )
      {
         double zs = _grid->wl[ ib ] / _output->wl( iw );
         int offset = ib * v_size;
         for( int ip = 0; ip < p_outer; ++ ip )
         {
            if( zs < _min_shift[ ip ] ) continue;
            if( zs > _max_shift[ ip ] ) continue;
            double z  = ( 1.0 - zs ) * 299.792;
            double vv = sqrt( z * z + _p[ ip ] * _p[ ip ] );
            int    il = int( ( vv - v_phot ) / v_step );
            int    iu = il + 1;
            double cl = ( _grid->v[ iu ] - vv ) / v_step;
            double cu = 1.0 - cl;
            double et = cl * _grid->tau[ offset + il ] + cu * _grid->tau[ offset + iu ];
            double ss = cl * _grid->src[ offset + il ] + cu * _grid->src[ offset + iu ];
            et = exp( - et );
            _in[ ip ] = _in[ ip ] * et + ss * ( 1.0 - et );
         }
      }
      _output->flux( iw ) = 0.0;
      for( int ip = 0; ip < p_outer; ++ ip ) _output->flux( iw ) += _in[ ip ] * _p[ ip ] * p_step;
      _output->flux( iw ) *= norm;
   }

}

void ES::Synow::Spectrum::_alloc( bool const clear )
{
   if( clear ) _clear();
   _in        = new double [ _p_total ];
   _p         = new double [ _p_total ];
   _min_shift = new double [ _p_total ];
   _max_shift = new double [ _p_total ];
}

void ES::Synow::Spectrum::_clear()
{
   delete [] _in;
   delete [] _p;
   delete [] _min_shift;
   delete [] _max_shift;
}
