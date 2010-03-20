// 
// File    : ES_Synapps_Evaluator.cc
// ---------------------------------
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

#include "ES_Synapps_Evaluator.hh"
#include "ES_Synow.hh"

#include "APPSPACK_Vector.hpp"

#include <cmath>

ES::Synapps::Evaluator::Evaluator( ES::Synow::Grid& grid, ES::Spectrum& target, ES::Spectrum& output, const std::vector< int >& ions, 
        const std::vector< double >& region_weight, const std::vector< double >& region_lower, const std::vector< double >& region_upper, 
        double const vector_norm ) :
    _grid( &grid ), _target( &target ), _output( &output ), _vector_norm( vector_norm )
{
    _setup = new ES::Synow::Setup();
    _setup->resize( ions.size() );
    for( int i = 0; i < ions.size(); ++ i )
    {
        _setup->ions  [ i ] = ions[ i ];
        _setup->active[ i ] = true;
    }

    // Apply additional weight-factor to target spectrum.

    if( region_weight.empty() ) return;

    std::vector< double > weight( _target->size(), 0.0 );
    for( int ir = 0; ir < region_weight.size(); ++ ir )
    {
        for( int iw = 0; iw < _target->size(); ++ iw )
        {
            if( _target->wl( iw ) < region_lower[ ir ] ) continue;
            if( _target->wl( iw ) > region_upper[ ir ] ) break;
            weight[ iw ] = region_weight[ ir ];
        }
    }

    for( int i = 0; i < _target->size(); ++ i )
    {
        _target->flux_error( i ) = weight[ i ] <= 0 ? -1.0 : _target->flux_error( i ) / weight[ i ];
    }

}

void ES::Synapps::Evaluator::operator() ( int tag, const APPSPACK::Vector& x, APPSPACK::Vector& f, std::string& msg )
{
    (*_setup)( x.getStlVector() );
    (*_grid)( *_setup );

    double score = 0.0;
    for( int i = 0; i < _output->size(); ++ i )
    {
        if( _target->flux_error( i ) <= 0.0 ) continue;
        double term = fabs( ( _output->flux( i ) - _target->flux( i ) ) / _target->flux_error( i ) );
        score += pow( term, _vector_norm );
    }
    score = pow( score, 1.0 / _vector_norm );

    f.resize( 1 );
    f[ 0 ] = score;
    msg = "Success";
}
