// 
// File    : synapps.cc
// --------------------
// Created : Tue Feb 23 12:59:33 2010
// Authors : Rollin C. Thomas (RCT) - rcthomas@lbl.gov
// 

#include "ES_Synapps.hh"
#include "ES_Synow.hh"

#include "APPSPACK_GCI.hpp"                  // APPSPACK's interface to MPI.
#include "APPSPACK_Executor_MPI.hpp"         // MPI executor.
#include "APPSPACK_Solver.hpp"               // Solver.

#include <yaml-cpp/yaml.h>

#include <fstream>

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
   ES::Spectrum target = ES::Spectrum::create_from_fits_file( target_file.c_str() );
   ES::Spectrum output = ES::Spectrum::create_from_spectrum( target );

   // Grid object.
   
   ES::Synow::Grid grid = ES::Synow::Grid::create( 
         yaml[ "output" ][ "min_wl"      ],
         yaml[ "output" ][ "max_wl"      ],
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

   ES::Synow::Spectrum spectrum( grid, output,
         yaml[ "spectrum" ][ "p_size" ] );

   // Evaluator...

   ES::Synapps::Evaluator evaluator( grid, target, output,
         yaml[ "evaluator" ][ "vector_norm" ] );

   // Master section.

   if( rank == 0 )
   {

      ES::Synapps::Config config( yaml[ "config" ] );

      // Executor, constraints, solver, solve.

      APPSPACK::Executor::MPI       executor;
      APPSPACK::Constraints::Linear linear( config.params.sublist( "Linear" ) );
      APPSPACK::Solver              solver( config.params.sublist( "Solver" ), executor, linear );
      APPSPACK::Solver::State       stat = solver.solve();

      // Problem solved, terminate workers.

      APPSPACK::GCI::initSend();
      APPSPACK::GCI::pack( 1 );
      for( int i = 0; i < workers; ++ i ) APPSPACK::GCI::send( APPSPACK::Executor::MPI::Terminate, i + 1 );

      // would do other things here.

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
