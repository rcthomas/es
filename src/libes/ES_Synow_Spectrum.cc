// 
// File    : ES_Synow_Spectrum.cc
// ------------------------------
//
// ES: Elementary Supernova Spectrum Synthesis, Copyright (c) 2010, The
// Regents of the University of California, through Lawrence Berkeley
// National Laboratory (subject to receipt of any required approvals
// from the U.S. Dept. of Energy). All rights reserved.
//
// If you have questions about your rights to use or distribute this
// software, please contact Berkeley Lab's Technology Transfer
// Department at TTD@lbl.gov.
//
// NOTICE. This software was developed under partial funding from the
// U.S. Department of Energy. As such, the U.S. Government has been
// granted for itself and others acting on its behalf a paid-up,
// nonexclusive, irrevocable, worldwide license in the Software to
// reproduce, prepare derivative works, and perform publicly and display
// publicly. Beginning five (5) years after the date permission to
// assert copyright is obtained from the U.S. Department of Energy, and
// subject to any subsequent five (5) year renewals, the U.S. Government
// is granted for itself and others acting on its behalf a paid-up,
// nonexclusive, irrevocable, worldwide license in the Software to
// reproduce, prepare derivative works, distribute copies to the public,
// perform publicly and display publicly, and to permit others to do so. 
//

#include "ES_Spectrum.hh"
#include "ES_Synow_Spectrum.hh"
#include "ES_Synow_Grid.hh"
#include "ES_Synow_Setup.hh"
#include "ES_Blackbody.hh"
#include "ES_Constants.hh"

#include <cmath>
#include <algorithm>

#if __cplusplus <= 199711L
#define nullptr NULL
#endif


ES::Synow::Spectrum::Spectrum( const ES::Synow::Spectrum & i_cRHO) : 
    ES::Synow::Operator( *i_cRHO._grid ),
    _output( i_cRHO._output ),
    _reference( i_cRHO._reference ),
    _flatten( i_cRHO._flatten ),
    _p_size( i_cRHO._p_size ), 
    _p_total( i_cRHO._p_total )
{
	_in = nullptr;
	_p = nullptr;
	_min_shift = nullptr;
	_max_shift = nullptr;
    _alloc( false );
	if (_p != nullptr && i_cRHO._p != nullptr)
		memcpy(_p,i_cRHO._p,_p_total * sizeof(double));
	if (_in != nullptr && i_cRHO._in != nullptr)
		memcpy(_in,i_cRHO._in,_p_total * sizeof(double));
	if (_min_shift != nullptr && i_cRHO._min_shift != nullptr)
		memcpy(_min_shift,i_cRHO._min_shift,_p_total * sizeof(double));
	if (_max_shift != nullptr && i_cRHO._max_shift != nullptr)
		memcpy(_max_shift,i_cRHO._max_shift,_p_total * sizeof(double));
}


ES::Synow::Spectrum::Spectrum( ES::Synow::Grid& grid, ES::Spectrum& output, ES::Spectrum& reference, 
        int const p_size, bool const flatten ) :
    ES::Synow::Operator( grid ),
    _output( &output ),
    _reference( &reference ),
    _flatten( flatten ),
    _p_size( p_size ), 
    _p_total( 5 * p_size )
{
	_in = nullptr;
	_p = nullptr;
	_min_shift = nullptr;
	_max_shift = nullptr;
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
#pragma omp parallel for
    for( int ip = 0; ip < p_outer; ++ ip )
    {
        _p[ ip ] = p_init + ip * p_step;
        _max_shift[ ip ] = 1.0 + sqrt( v_outer * v_outer - _p[ ip ] * _p[ ip ] ) * ES::_inv_c;
        _min_shift[ ip ] = ip < _p_size ? 1.0 + sqrt( v_phot  * v_phot  - _p[ ip ] * _p[ ip ] ) * ES::_inv_c : 1.0 / _max_shift[ ip ];
    }

    // Compute.

    double norm = 0.0;
    for( int ip = 0; ip < _p_size; ++ ip ) norm += _p[ ip ] * p_step;
    norm = 1.0 / norm;

    for( size_t iw = 0; iw < _output->size(); ++ iw )
    {
        int start = std::upper_bound( _grid->wl, _grid->wl + wl_used, _output->wl( iw ) * _min_shift[ _p_size ] ) - _grid->wl;
        int stop  = std::upper_bound( _grid->wl, _grid->wl + wl_used, _output->wl( iw ) * _max_shift[ 0       ] ) - _grid->wl;

        _reference->flux( iw ) = 0.0;
        for( int ip = 0; ip < p_outer; ++ ip ) 
        {
            if( ip < _p_size )
            {
               _in[ ip ] = (*_grid->bb)( _output->wl( iw ) * _min_shift[ ip ] ) * pow( _min_shift[ ip ], 3 );
                _reference->flux( iw ) += _in[ ip ] * _p[ ip ] * p_step;
            }
            else
            {
                _in[ ip ] = 0.0;
            }
        }
        _reference->flux( iw ) *= norm;

        for( int ib = start; ib < stop; ++ ib )
        {
            double zs = _grid->wl[ ib ] / _output->wl( iw );
            int offset = ib * v_size;
            for( int ip = 0; ip < p_outer; ++ ip )
            {
                if( zs < _min_shift[ ip ] ) continue;
                if( zs > _max_shift[ ip ] ) continue;
                double z  = ( 1.0 - zs ) * ES::_c;
                double vv = sqrt( z * z + _p[ ip ] * _p[ ip ] );
				int il;
				if (_grid->v_user)
                	il = int( ( vv - _grid->v[0] ) / v_step );
				else
                	il = int( ( vv - v_phot ) / v_step );
                int    iu = il + 1;
                double cl = ( _grid->v[ iu ] - vv ) / v_step;
                double cu = 1.0 - cl;
                double et = cl * _grid->tau[ offset + il ] + cu * _grid->tau[ offset + iu ];
                double ss = cl * _grid->src[ offset + il ] + cu * _grid->src[ offset + iu ];
                et = exp( - et );
                _in[ ip ] = _in[ ip ] * et + ss * ( 1.0 - et ) * pow( zs, 3 );
            }
        }

        _output->flux( iw ) = 0.0;
        for( int ip = 0; ip < p_outer; ++ ip ) _output->flux( iw ) += _in[ ip ] * _p[ ip ] * p_step;
        _output->flux( iw ) *= norm;

    }

    // Conversion to F-lambda, application of warp, or flattening.

   for( size_t iw = 0; iw < _output->size(); ++ iw )
    {
        if( _flatten )
        {
            _output->flux( iw ) /= _reference->flux( iw );
        }
        else
        {
            double ww = _output->wl( iw ) / 6500.0;
            _output->flux( iw ) /= ww * ww;
            ww -= 1.0;
            _output->flux( iw ) *= setup.a0 + ww * ( setup.a1 + ww * setup.a2 );
        }
    }

}

void ES::Synow::Spectrum::_alloc( bool const clear )
{
    if( clear ) _clear();
    _in        = new double [ _p_total ];
	if (_in == nullptr)
		std::cerr << "synow: unable to allocate 'in' in spectrum." << std::endl;
    _p         = new double [ _p_total ];
	if (_p == nullptr)
		std::cerr << "synow: unable to allocate 'p' in spectrum." << std::endl;
    _min_shift = new double [ _p_total ];
	if (_min_shift == nullptr)
		std::cerr << "synow: unable to allocate min_shift in spectrum." << std::endl;
    _max_shift = new double [ _p_total ];
	if (_max_shift == nullptr)
		std::cerr << "synow: unable to allocate max_shift in spectrum." << std::endl;
}

void ES::Synow::Spectrum::_clear()
{
	if (_in != nullptr)
	    delete [] _in;
	if (_p != nullptr)
	    delete [] _p;
	if (_min_shift != nullptr)
	    delete [] _min_shift;
	if (_max_shift != nullptr)
	    delete [] _max_shift;
	_in = nullptr;
	_p = nullptr;
	_min_shift = nullptr;
	_max_shift = nullptr;
}
