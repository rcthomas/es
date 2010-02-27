// 
// File    : ES_Synapps_Evaluator.cc
// ---------------------------------
// Created : Tue Feb 23 16:38:53 2010
// Authors : Rollin C. Thomas (RCT) - rcthomas@lbl.gov
// 

#include "ES_Synapps_Evaluator.hh"
#include "ES_Synow.hh"

#include "APPSPACK_Vector.hpp"

#include <cmath>

void ES::Synapps::Evaluator::operator() ( int tag, const APPSPACK::Vector& x, APPSPACK::Vector& f, std::string& msg )
{
   ES::Synow::Setup setup;

   setup( x.getStlVector() );
   (*_grid)( setup );

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
