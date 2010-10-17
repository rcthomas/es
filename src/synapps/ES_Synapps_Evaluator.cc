// 
// File    : ES_Synapps_Evaluator.cc
// ---------------------------------
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

#include "ES_Synapps_Evaluator.hh"
#include "ES_Synow.hh"

#include <appspack/APPSPACK_Vector.hpp>

#include <cmath>

ES::Synapps::Evaluator::Evaluator( ES::Synow::Grid& grid, ES::Spectrum& target, ES::Spectrum& output, const std::vector< int >& ions, 
        const std::vector< double >& region_weight, const std::vector< double >& region_lower, const std::vector< double >& region_upper, 
        double const vector_norm ) :
    _grid( &grid ), _target( &target ), _output( &output ), _vector_norm( vector_norm )
{
    _setup = new ES::Synow::Setup();
    _setup->resize( ions.size() );
    for( size_t i = 0; i < ions.size(); ++ i )
    {
        _setup->ions  [ i ] = ions[ i ];
        _setup->active[ i ] = true;
    }

    // Apply additional weight-factor to target spectrum.

    if( region_weight.empty() ) return;

    std::vector< double > weight( _target->size(), 0.0 );
    for( size_t ir = 0; ir < region_weight.size(); ++ ir )
    {
        for( size_t iw = 0; iw < _target->size(); ++ iw )
        {
            if( _target->wl( iw ) < region_lower[ ir ] ) continue;
            if( _target->wl( iw ) > region_upper[ ir ] ) break;
            weight[ iw ] = region_weight[ ir ];
        }
    }

    for( size_t i = 0; i < _target->size(); ++ i )
    {
        _target->flux_error( i ) = weight[ i ] <= 0 ? -1.0 : _target->flux_error( i ) / weight[ i ];
    }

}

void ES::Synapps::Evaluator::operator() ( int tag, const APPSPACK::Vector& x, APPSPACK::Vector& f, std::string& msg )
{
    (*_setup)( x.getStlVector() );
    (*_grid)( *_setup );

    double score = 0.0;
    for( size_t i = 0; i < _output->size(); ++ i )
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
