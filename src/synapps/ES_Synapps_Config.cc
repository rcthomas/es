//
// File    : ES_Synapps_Config.cc
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

#include "ES_Synapps_Config.hh"

#include <appspack/APPSPACK_Float.hpp>
#include <appspack/APPSPACK_Vector.hpp>
#include <appspack/APPSPACK_Parameter_List.hpp>

#include <yaml-cpp/yaml.h>

#include <set>

ES::Synapps::Config::Config( const YAML::Node& config )
{

    fit_file = config[ "fit_file" ].as<std::string>();
    std::string cache_file = config[ "cache_file" ].as<std::string>();

//  params.sublist( "Solver" ).setParameter( "Debug"                , 4 );
    params.sublist( "Solver" ).setParameter( "Cache Input File"     , cache_file );
    params.sublist( "Solver" ).setParameter( "Cache Output File"    , cache_file );
    params.sublist( "Solver" ).setParameter( "Precision"            , 2    );
    params.sublist( "Solver" ).setParameter( "Use Random Order"     , true );
    params.sublist( "Solver" ).setParameter( "Use Projected Compass", true );

    int num_ions = 0;
    for( size_t i = 0; i < config[ "active" ].size(); ++ i )
    {
        num_ions += config[ "active" ][ i ].as<bool>() ? 1 : 0;
    }

    APPSPACK::Vector buffer( 6 + 5 * num_ions );

    APPSPACK::Matrix ineq_matrix;
    APPSPACK::Matrix eq_matrix;
    APPSPACK::Vector eq_bound;

    int j;

    // Initial value.

    buffer[ 0 ] = config[ "a0"      ][ "start" ].as<double>();
    buffer[ 1 ] = config[ "a1"      ][ "start" ].as<double>();
    buffer[ 2 ] = config[ "a2"      ][ "start" ].as<double>();
    buffer[ 3 ] = config[ "v_phot"  ][ "start" ].as<double>();
    buffer[ 4 ] = config[ "v_outer" ][ "start" ].as<double>();
    buffer[ 5 ] = config[ "t_phot"  ][ "start" ].as<double>();

    j = 6;
    for( size_t i = 0; i < config[ "active" ].size(); ++ i )
    {
        if( ! config[ "active" ][ i ].as<bool>() ) continue;
        buffer[ j + 0 * num_ions ] = config[ "log_tau" ][ "start" ][ i ].as<double>();
        buffer[ j + 1 * num_ions ] = config[ "v_min"   ][ "start" ][ i ].as<double>();
        buffer[ j + 2 * num_ions ] = config[ "v_max"   ][ "start" ][ i ].as<double>();
        buffer[ j + 3 * num_ions ] = config[ "aux"     ][ "start" ][ i ].as<double>();
        buffer[ j + 4 * num_ions ] = config[ "temp"    ][ "start" ][ i ].as<double>();
        ++ j;
    }

    params.sublist( "Solver" ).setParameter( "Initial X", buffer );

    // Lower boundary.

    buffer[ 0 ] = config[ "a0"      ][ "lower" ].as<double>();
    buffer[ 1 ] = config[ "a1"      ][ "lower" ].as<double>();
    buffer[ 2 ] = config[ "a2"      ][ "lower" ].as<double>();
    buffer[ 3 ] = config[ "v_phot"  ][ "lower" ].as<double>();
    buffer[ 4 ] = config[ "v_outer" ][ "lower" ].as<double>();
    buffer[ 5 ] = config[ "t_phot"  ][ "lower" ].as<double>();

    j = 6;
    for( size_t i = 0; i < config[ "active" ].size(); ++ i )
    {
        if( ! config[ "active" ][ i ].as<bool>() ) continue;
        buffer[ j + 0 * num_ions ] = config[ "log_tau" ][ "lower" ][ i ].as<double>();
        buffer[ j + 1 * num_ions ] = config[ "v_min"   ][ "lower" ][ i ].as<double>();
        buffer[ j + 2 * num_ions ] = config[ "v_max"   ][ "lower" ][ i ].as<double>();
        buffer[ j + 3 * num_ions ] = config[ "aux"     ][ "lower" ][ i ].as<double>();
        buffer[ j + 4 * num_ions ] = config[ "temp"    ][ "lower" ][ i ].as<double>();
        ++ j;
    }

    params.sublist( "Linear" ).setParameter( "Lower", buffer );

    // Upper boundary.

    buffer[ 0 ] = config[ "a0"      ][ "upper" ].as<double>();
    buffer[ 1 ] = config[ "a1"      ][ "upper" ].as<double>();
    buffer[ 2 ] = config[ "a2"      ][ "upper" ].as<double>();
    buffer[ 3 ] = config[ "v_phot"  ][ "upper" ].as<double>();
    buffer[ 4 ] = config[ "v_outer" ][ "upper" ].as<double>();
    buffer[ 5 ] = config[ "t_phot"  ][ "upper" ].as<double>();

    j = 6;
    for( size_t i = 0; i < config[ "active" ].size(); ++ i )
    {
        if( ! config[ "active" ][ i ].as<bool>() ) continue;
        buffer[ j + 0 * num_ions ] = config[ "log_tau" ][ "upper" ][ i ].as<double>();
        buffer[ j + 1 * num_ions ] = config[ "v_min"   ][ "upper" ][ i ].as<double>();
        buffer[ j + 2 * num_ions ] = config[ "v_max"   ][ "upper" ][ i ].as<double>();
        buffer[ j + 3 * num_ions ] = config[ "aux"     ][ "upper" ][ i ].as<double>();
        buffer[ j + 4 * num_ions ] = config[ "temp"    ][ "upper" ][ i ].as<double>();
        ++ j;
    }

    params.sublist( "Linear" ).setParameter( "Upper", buffer );

    // Parameter scalings.

    buffer[ 0 ] = config[ "a0"      ][ "scale" ].as<double>();
    buffer[ 1 ] = config[ "a1"      ][ "scale" ].as<double>();
    buffer[ 2 ] = config[ "a2"      ][ "scale" ].as<double>();
    buffer[ 3 ] = config[ "v_phot"  ][ "scale" ].as<double>();
    buffer[ 4 ] = config[ "v_outer" ][ "scale" ].as<double>();
    buffer[ 5 ] = config[ "t_phot"  ][ "scale" ].as<double>();

    j = 6;
    for( size_t i = 0; i < config[ "active" ].size(); ++ i )
    {
        if( ! config[ "active" ][ i ].as<bool>() ) continue;
        buffer[ j + 0 * num_ions ] = config[ "log_tau" ][ "scale" ][ i ].as<double>();
        buffer[ j + 1 * num_ions ] = config[ "v_min"   ][ "scale" ][ i ].as<double>();
        buffer[ j + 2 * num_ions ] = config[ "v_max"   ][ "scale" ][ i ].as<double>();
        buffer[ j + 3 * num_ions ] = config[ "aux"     ][ "scale" ][ i ].as<double>();
        buffer[ j + 4 * num_ions ] = config[ "temp"    ][ "scale" ][ i ].as<double>();
        ++ j;
    }

    params.sublist( "Linear" ).setParameter( "Scaling", buffer );

    // Inequality bounds constraints: v_phot <= v_outer;

    buffer.zero();
    buffer[ 3 ] = - 1.0;
    buffer[ 4 ] =   1.0;
    ineq_matrix.addRow( buffer );

    // Inequality bounds constraints: v_phot <= v_min.

    j = 6;
    for( size_t i = 0; i < config[ "active" ].size(); ++ i )
    {
        if( ! config[ "active" ][ i ].as<bool>() ) continue;
        buffer.zero();
        buffer[ 3 ] = - 1.0;
        buffer[ j + 1 * num_ions ] = 1.0;
        ineq_matrix.addRow( buffer );
        ++ j;
    }

    // Inequality bounds constraints: v_min <= v_max.

    j = 6;
    for( size_t i = 0; i < config[ "active" ].size(); ++ i )
    {
        if( ! config[ "active" ][ i ].as<bool>() ) continue;
        buffer.zero();
        buffer[ j + 1 * num_ions ] = - 1.0;
        buffer[ j + 2 * num_ions ] =   1.0;
        ineq_matrix.addRow( buffer );
        ++ j;
    }

    // Inequality bounds constraints: v_max <= v_outer.

    j = 6;
    for( size_t i = 0; i < config[ "active" ].size(); ++ i )
    {
        if( ! config[ "active" ][ i ].as<bool>() ) continue;
        buffer.zero();
        buffer[ j + 2 * num_ions ] = - 1.0;
        buffer[ 4 ] = 1.0;
        ineq_matrix.addRow( buffer );
        ++ j;
    }

    // Inequality bounds constraints.

    params.sublist( "Linear" ).setParameter( "Inequality Matrix", ineq_matrix );
    params.sublist( "Linear" ).setParameter( "Inequality Lower" , APPSPACK::Vector( ineq_matrix.getNrows(),             0.0 ) );
    params.sublist( "Linear" ).setParameter( "Inequality Upper" , APPSPACK::Vector( ineq_matrix.getNrows(), APPSPACK::dne() ) );

    // Equality constraints: Detached/attached ions (attached: v_phot == v_min ).

    j = 6;
    for( size_t i = 0; i < config[ "active" ].size(); ++ i )
    {
        if( ! config[ "active" ][ i ].as<bool>() ) continue;
        if( ! config[ "detach" ][ i ] )
        {
            buffer.zero();
            buffer[ 3 ] = - 1.0;
            buffer[ j + 1 * num_ions ] = 1.0;
            eq_matrix.addRow( buffer );
            eq_bound.push_back( 0.0 );
        }
        ++ j;
    }

    // Equality constraints: Repeated ions must have same temperature.

    std::set< int > done;

    j = 6;
    for( size_t i = 0; i < config[ "active" ].size(); ++ i )
    {
        if( ! config[ "active" ][ i ].as<bool>() ) continue;
        if( done.find( config[ "ions" ][ i ].as<int>() ) == done.end() )
        {
            int ion = config[ "ions" ][ i ].as<int>();
            int jj = 6;
            for( size_t ii = 0; ii < config[ "active" ].size(); ++ ii )
            {
                if( ! config[ "active" ][ ii ].as<bool>() ) continue;
                if( ii > i && config[ "ions" ][ ii ].as<int>() == ion )
                {
                    buffer.zero();
                    buffer[ j  + 4 * num_ions ] = - 1.0;
                    buffer[ jj + 4 * num_ions ] =   1.0;
                    eq_matrix.addRow( buffer );
                    eq_bound.push_back( 0.0 );
                }
                ++ jj;
            }
        }
        done.insert( config[ "ions" ][ i ].as<int>() );
        ++ j;
    }

    // Equality constraints: Any fixed individual parameters.

    if( config[ "a0" ][ "fixed" ].as<bool>() )
    {
        buffer.zero();
        buffer[ 0 ] = 1.0;
        eq_matrix.addRow( buffer );
        eq_bound.push_back( config[ "a0" ][ "start" ].as<double>() );
    }

    if( config[ "a1" ][ "fixed" ].as<bool>() )
    {
        buffer.zero();
        buffer[ 1 ] = 1.0;
        eq_matrix.addRow( buffer );
        eq_bound.push_back( config[ "a1" ][ "start" ].as<double>() );
    }

    if( config[ "a2" ][ "fixed" ].as<bool>() )
    {
        buffer.zero();
        buffer[ 2 ] = 1.0;
        eq_matrix.addRow( buffer );
        eq_bound.push_back( config[ "a2" ][ "start" ].as<double>() );
    }

    if( config[ "v_phot" ][ "fixed" ].as<bool>() )
    {
        buffer.zero();
        buffer[ 3 ] = 1.0;
        eq_matrix.addRow( buffer );
        eq_bound.push_back( config[ "v_phot" ][ "start" ].as<double>() );
    }

    if( config[ "v_outer" ][ "fixed" ].as<bool>() )
    {
        buffer.zero();
        buffer[ 4 ] = 1.0;
        eq_matrix.addRow( buffer );
        eq_bound.push_back( config[ "v_outer" ][ "start" ].as<double>() );
    }

    if( config[ "t_phot" ][ "fixed" ].as<bool>() )
    {
        buffer.zero();
        buffer[ 5 ] = 1.0;
        eq_matrix.addRow( buffer );
        eq_bound.push_back( config[ "t_phot" ][ "start" ].as<double>() );
    }

    j = 6;
    for( size_t i = 0; i < config[ "active" ].size(); ++ i )
    {
        if( ! config[ "active" ][ i ].as<bool>() ) continue;
        if( config[ "log_tau" ][ "fixed" ][ i ].as<bool>() )
        {
            buffer.zero();
            buffer[ j + 0 * num_ions ] = 1.0;
            eq_matrix.addRow( buffer );
            eq_bound.push_back( config[ "log_tau" ][ "start" ][ i ].as<double>() );
        }
        ++ j;
    }

    j = 6;
    for( size_t i = 0; i < config[ "active" ].size(); ++ i )
    {
        if( ! config[ "active" ][ i ].as<bool>() ) continue;
        if( config[ "v_min" ][ "fixed" ][ i ].as<bool>() )
        {
            buffer.zero();
            buffer[ j + 1 * num_ions ] = 1.0;
            eq_matrix.addRow( buffer );
            eq_bound.push_back( config[ "v_min" ][ "start" ][ i ].as<double>() );
        }
        ++ j;
    }

    j = 6;
    for( size_t i = 0; i < config[ "active" ].size(); ++ i )
    {
        if( ! config[ "active" ][ i ].as<bool>() ) continue;
        if( config[ "v_max" ][ "fixed" ][ i ].as<bool>() )
        {
            buffer.zero();
            buffer[ j + 2 * num_ions ] = 1.0;
            eq_matrix.addRow( buffer );
            eq_bound.push_back( config[ "v_max" ][ "start" ][ i ].as<double>() );
        }
        ++ j;
    }

    j = 6;
    for( size_t i = 0; i < config[ "active" ].size(); ++ i )
    {
        if( ! config[ "active" ][ i ].as<bool>() ) continue;
        if( config[ "aux" ][ "fixed" ][ i ].as<bool>() )
        {
            buffer.zero();
            buffer[ j + 3 * num_ions ] = 1.0;
            eq_matrix.addRow( buffer );
            eq_bound.push_back( config[ "aux"   ][ "start" ][ i ].as<double>() );
        }
        ++ j;
    }

    j = 6;
    for( size_t i = 0; i < config[ "active" ].size(); ++ i )
    {
        if( ! config[ "active" ][ i ].as<bool>() ) continue;
        if( config[ "temp" ][ "fixed" ][ i ].as<bool>() )
        {
            buffer.zero();
            buffer[ j + 4 * num_ions ] = 1.0;
            eq_matrix.addRow( buffer );
            eq_bound.push_back( config[ "temp"  ][ "start" ][ i ].as<double>() );
        }
        ++ j;
    }

    // Equality constraints.

    params.sublist( "Linear" ).setParameter( "Equality Matrix", eq_matrix );
    params.sublist( "Linear" ).setParameter( "Equality Bound" , eq_bound  );

}
