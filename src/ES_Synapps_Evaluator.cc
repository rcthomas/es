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

ES::Synapps::Evaluator:: Evaluator( ES::Synow::Grid& grid, ES::Spectrum& target, ES::Spectrum& output, const std::vector< int >& ions, 
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
}

void ES::Synapps::Evaluator::operator() ( int tag, const APPSPACK::Vector& x, APPSPACK::Vector& f, std::string& msg )
{
   (*_setup)( x.getStlVector() );
   (*_grid)( *_setup );

   double score = 0.0;
   for( int i = 0; i < _output->size(); ++ i )
   {
      double term = fabs( ( _output->flux( i ) - _target->flux( i ) ) / _target->flux_error( i ) );
      score += pow( term, _vector_norm );
   }
   score = pow( score, 1.0 / _vector_norm );

   f.resize( 1 );
   f[ 0 ] = score;
   msg = "Success";
}
