//
// File    : synapps.cc
// --------------------
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

#include "ES_Synapps.hh"
#include "ES_Synow.hh"

#include <appspack/APPSPACK_GCI.hpp>                  // APPSPACK's interface to MPI.
#include <appspack/APPSPACK_Executor_MPI.hpp>         // MPI executor.
#include <appspack/APPSPACK_Solver.hpp>               // Solver.

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

    YAML::Node yaml = YAML::LoadFile( argv [ 1 ] );

    // Target and output spectra.  The output spectrum is sampled
    // at the same wavelengths as the target spectrum.

    std::string target_file = yaml[ "evaluator" ][ "target_file" ].as<std::string>();
    ES::Spectrum target    = ES::Spectrum::create_from_ascii_file( target_file.c_str() );
    ES::Spectrum output    = ES::Spectrum::create_from_spectrum( target );
    ES::Spectrum reference = ES::Spectrum::create_from_spectrum( target );

//  target.rescale_median_flux();

    // Grid object.

    ES::Synow::Grid grid = ES::Synow::Grid::create(
            target.min_wl(),
            target.max_wl(),
            yaml[ "grid"   ][ "bin_width"   ].as<double>(),
            yaml[ "grid"   ][ "v_size"      ].as<int>(),
            yaml[ "grid"   ][ "v_outer_max" ].as<double>() );

    // Opacity operator.

    ES::Synow::Opacity opacity( grid,
            yaml[ "opacity" ][ "line_dir"    ].as<std::string>(),
            yaml[ "opacity" ][ "ref_file"    ].as<std::string>(),
            yaml[ "opacity" ][ "form"        ].as<std::string>(),
            yaml[ "opacity" ][ "v_ref"       ].as<double>(),
            yaml[ "opacity" ][ "log_tau_min" ].as<double>() );

    // Source operator.

    ES::Synow::Source source( grid,
            yaml[ "source" ][ "mu_size" ].as<int>() );

    // Spectrum operator.

    ES::Synow::Spectrum spectrum( grid, output, reference,
            yaml[ "spectrum" ][ "p_size"  ].as<int>(),
            yaml[ "spectrum" ][ "flatten" ].as<bool>() );

    // Evaluator.

    std::vector< int > ions;
    for( size_t i = 0; i < yaml[ "config" ][ "active" ].size(); ++ i )
    {
        if( ! yaml[ "config" ][ "active" ][ i ] ) continue;
        ions.push_back( yaml[ "config" ][ "ions" ][ i ].as<int>() );
    }

    std::vector< double > region_weight;
    std::vector< double > region_lower;
    std::vector< double > region_upper;
    for( size_t i = 0; i < yaml[ "evaluator" ][ "regions" ][ "apply" ].size(); ++ i )
    {
        if( ! yaml[ "evaluator" ][ "regions" ][ "apply" ][ i ].as<bool>() ) continue;
        region_weight.push_back( yaml[ "evaluator" ][ "regions" ][ "weight" ][ i ].as<double>() );
        region_lower.push_back ( yaml[ "evaluator" ][ "regions" ][ "lower"  ][ i ].as<double>() );
        region_upper.push_back ( yaml[ "evaluator" ][ "regions" ][ "upper"  ][ i ].as<double>() );
    }

    ES::Synapps::Evaluator evaluator( grid, target, output, ions, region_weight, region_lower, region_upper,
            yaml[ "evaluator" ][ "vector_norm" ].as<double>() );

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
