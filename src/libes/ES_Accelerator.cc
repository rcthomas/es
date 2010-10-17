// 
// File    : ES_Accelerator.cc
// ---------------------------
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

#include "ES_Accelerator.hh"

#include <cmath>

double ES::Accelerator::operator() ( double const x )
{
    std::map< double, double >::iterator right = _cache.upper_bound( x );
    if( right != _cache.end() )
    {
        std::map< double, double >::iterator left = right;
        -- left;
        if( left != right )
        {
            double a = ( right->first - x ) / ( right->first - left->first );
            double b = 1.0 - a;
            return a * left->second + b * right->second;
        }
        else
        {
            double y = evaluate( x );
            _insert( x, y );
            return y;
        }
    }
    else
    {
        double y = evaluate( x );
        _insert( x, y );
        return y;
    }
}

void ES::Accelerator::_insert( double const x, double const y )
{
    std::pair< std::map< double, double >::iterator, bool > result = _cache.insert( std::pair< double, double >( x, y ) );
    std::map< double, double >::iterator left  = result.first;
    std::map< double, double >::iterator right = result.first;
    -- left;
    ++ right;
    if( left != result.first && left != _cache.end() )
    {
        double xx = 0.5 * ( left->first  + x );
        double yy = 0.5 * ( left->second + y );
        double yf = evaluate( xx );
        if( fabs( ( yy - yf ) / yf ) > _tolerance ) _insert( xx, yf );
    }
    if( right != _cache.end() )
    {
        double xx = 0.5 * ( x + right->first  );
        double yy = 0.5 * ( y + right->second );
        double yf = evaluate( xx );
        if( fabs( ( yy - yf ) / yf ) > _tolerance ) _insert( xx, yf );
    }
}
