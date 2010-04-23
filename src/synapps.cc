// 
// File    : synapps.cc
// --------------------
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

#include "ES_Synapps.hh"
#include "ES_Synow.hh"

#include "APPSPACK_GCI.hpp"                  // APPSPACK's interface to MPI.
#include "APPSPACK_Executor_MPI.hpp"         // MPI executor.
#include "APPSPACK_Solver.hpp"               // Solver.

#include <yaml-cpp/yaml.h>

#include <fstream>
#include <csignal>
#include <csetjmp>

jmp_buf env;

void signal_handler( int param )
{
    longjmp( env, 1 );
}

int main( int argc, char* argv[] )
{

    // Initialize MPI, 2 or more processors.

    int rank = APPSPACK::GCI::init( argc, argv );
    int workers = APPSPACK::GCI::getNumProcs() - 1;
    if( workers < 1 )
    {
        std::cerr << "ERROR: Program requires at least 2 processors." << std::endl;
        APPSPACK::GCI::exit();
        return 137;
    }

    // Configuration in this application comes from a YAML file.

    YAML::Node yaml;

    {
        std::ifstream  stream( argv[ 1 ] );
        YAML::Parser   parser( stream );
        parser.GetNextDocument( yaml );
        stream.close();
    }

    // Target and output spectra.  The output spectrum is sampled 
    // at the same wavelengths as the target spectrum.

    std::string target_file = yaml[ "evaluator" ][ "target_file" ];
    ES::Spectrum target    = ES::Spectrum::create_from_ascii_file( target_file.c_str() );
    ES::Spectrum output    = ES::Spectrum::create_from_spectrum( target );
    ES::Spectrum reference = ES::Spectrum::create_from_spectrum( target );

//  target.rescale_median_flux();

    // Grid object.

    ES::Synow::Grid grid = ES::Synow::Grid::create( 
            target.min_wl(),
            target.max_wl(),
            yaml[ "grid"   ][ "bin_width"   ], 
            yaml[ "grid"   ][ "v_size"      ],
            yaml[ "grid"   ][ "v_outer_max" ] );

    // Opacity operator.

    ES::Synow::Opacity opacity( grid,
            yaml[ "opacity" ][ "line_dir"    ],
            yaml[ "opacity" ][ "ref_file"    ],
            yaml[ "opacity" ][ "form"        ],
            yaml[ "opacity" ][ "v_ref"       ],
            yaml[ "opacity" ][ "log_tau_min" ] );

    // Source operator.

    ES::Synow::Source source( grid,
            yaml[ "source" ][ "mu_size" ] );

    // Spectrum operator.

    ES::Synow::Spectrum spectrum( grid, output, reference,
            yaml[ "spectrum" ][ "p_size"  ],
            yaml[ "spectrum" ][ "flatten" ] );

    // Evaluator.

    std::vector< int > ions;
    for( size_t i = 0; i < yaml[ "config" ][ "active" ].size(); ++ i )
    {
        if( ! yaml[ "config" ][ "active" ][ i ] ) continue;
        ions.push_back( yaml[ "config" ][ "ions" ][ i ] );
    }

    std::vector< double > region_weight;
    std::vector< double > region_lower;
    std::vector< double > region_upper;
    for( size_t i = 0; i < yaml[ "evaluator" ][ "regions" ][ "apply" ].size(); ++ i )
    {
        if( ! yaml[ "evaluator" ][ "regions" ][ "apply" ][ i ] ) continue;
        region_weight.push_back( yaml[ "evaluator" ][ "regions" ][ "weight" ][ i ] );
        region_lower.push_back ( yaml[ "evaluator" ][ "regions" ][ "lower"  ][ i ] );
        region_upper.push_back ( yaml[ "evaluator" ][ "regions" ][ "upper"  ][ i ] );
    }

    ES::Synapps::Evaluator evaluator( grid, target, output, ions, region_weight, region_lower, region_upper,
            yaml[ "evaluator" ][ "vector_norm" ] );

    // Master section.

    if( rank == 0 )
    {

        ES::Synapps::Config config( yaml[ "config" ] );

        // Executor, constraints, solver.

        APPSPACK::Executor::MPI       executor;
        APPSPACK::Constraints::Linear linear( config.params.sublist( "Linear" ) );
        APPSPACK::Solver              solver( config.params.sublist( "Solver" ), executor, linear );
//      APPSPACK::Solver::State       state = solver.solve();
        APPSPACK::Solver::State       state;
      
        // Signal handler, wrapping solver.

        signal( SIGTERM, signal_handler );

        int terminated = setjmp( env );
        if( ! terminated )
        {
            state = solver.solve();
        }
        else
        {
            std::cerr << std::endl;
            std::cerr << "WARNING!" << std::endl;
            std::cerr << "An abnormal exit condition has been reached, probably due to"  << std::endl;
            std::cerr << "a TERM signal.  Attempting to write out the best fit found so" << std::endl;
            std::cerr << "far, but be forewarned that we probably did not converge."     << std::endl;
            std::cerr << std::endl;
        }

        // Problem solved, terminate workers.

        APPSPACK::GCI::initSend();
        APPSPACK::GCI::pack( 1 );
        for( int i = 0; i < workers; ++ i ) APPSPACK::GCI::send( APPSPACK::Executor::MPI::Terminate, i + 1 );

        // Best spectrum.

        int tag;
        APPSPACK::Vector x;
        APPSPACK::Vector f;
        std::string msg;

        x = solver.getBestX();
        f = solver.getBestF();

        evaluator( tag, x, f, msg );

        std::ofstream stream;
        stream.open( config.fit_file.c_str() );
        stream << std::setprecision( 6 ) << output;
        stream.close();

    }

    // Worker section.

    if( rank != 0 )
    {

        while( true )
        {

            // Blocking receive.

            int msg_tag, junk;
            APPSPACK::GCI::recv();
            APPSPACK::GCI::bufinfo( msg_tag, junk );

            // Check for termination.

            if( msg_tag == APPSPACK::Executor::MPI::Terminate ) break;

            // Local workspace.

            int tag;
            APPSPACK::Vector x;
            APPSPACK::Vector f;
            std::string msg;

            // Unpack latest message -- must be done just like this.

            APPSPACK::GCI::unpack( tag );
            APPSPACK::GCI::unpack( x   );

            // Evaluate the function.

            evaluator( tag, x, f, msg );

            // Send reply -- must be done just like this.

            APPSPACK::GCI::initSend();
            APPSPACK::GCI::pack( tag );
            APPSPACK::GCI::pack( f   );
            APPSPACK::GCI::pack( msg );
            APPSPACK::GCI::send( APPSPACK::Executor::MPI::Feval, 0 );

        }

    }

    // Done.

    APPSPACK::GCI::exit();

    return 0;
}
