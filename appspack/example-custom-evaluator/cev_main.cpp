// $Id: cev_main.cpp,v 1.3 2006/06/30 19:14:31 tgkolda Exp $ 
// $Source: /space/CVS-Acro/acro/packages/appspack/appspack/example-custom-evaluator/cev_main.cpp,v $ 

/*! \dir example-custom-evaluator

Directory containing an example of a custom evaluator.

\sa \ref pageCustomize_evaluator

*/

/*! \file cev_main.cpp 

   \brief Main file for custom evaluator example.

   This is the main file. It starts and stops MPI and has the main
   routines for the master and the worker.

   The master is in charge of:
   - Sending any initialization information to the work (see Note 1)
   - Creating the parameter list (see Note 2)
   - Instantiating the MPI executor
   - Instantiating the bounds
   - Creating the solver
   - Running the solver
   - Sending a termination command to each worker

   The worker is in charge of:
   - Receiving initialization information (see Note 1)
   - Processing incoming points
   - Termining upon an appropriate terminate message

   Note 1: In this example, no initialization information is sent to
   the workers because none is needed.

   Note 2: The parameter list can be read from a file or hard-coded or
   some combination. APPSPACK provides tools for reading the parameter
   list form a file; see the function APPSPACK::parseTextInputFile.

   \sa \ref pageCustomize_evaluator

  */

#include "cev_evaluator.hpp"	// Custom evaluator
#include "APPSPACK_GCI.hpp"	// Interface to MPI
#include "APPSPACK_Parameter_List.hpp" // Parameter list
#include "APPSPACK_Executor_MPI.hpp" // MPI executor
#include "APPSPACK_Constraints_Linear.hpp" // constraints
#include "APPSPACK_Solver.hpp"	// solver


//! Main routine
int main(int argc, char* argv[])
{
  // *** Start MPI ***
  // (also processes argc and argv which may have had extra "stuff"
  // added on by mpirun)
  int rank = APPSPACK::GCI::init(argc, argv);

  // *** Check that there are at least two processes ***
  int nWorkers = APPSPACK::GCI::getNumProcs() - 1;
  if (nWorkers < 1)
  {
    cerr << "Error: This program requires at least 2 processes." << endl;
    APPSPACK::GCI::exit();
    return 1;
  }

  // *** Master ***
  if (rank == 0)	
  {

    // Set up the parameter list to be passed to the solver

    APPSPACK::Parameter::List params;

    APPSPACK::Vector initialx(2);
    initialx[0] = 2e-1;
    initialx[1] = 0.3;
    params.sublist("Solver").setParameter("Initial X", initialx);

    params.sublist("Solver").setParameter("Debug", 4);

    APPSPACK::Vector lowerbounds(2,-1);
    params.sublist("Linear").setParameter("Lower", lowerbounds);

    APPSPACK::Vector upperbounds(2,3);
    params.sublist("Linear").setParameter("Upper", upperbounds);

    // Create an executor object
    APPSPACK::Executor::MPI executor;
    
    // Create a constraints object
    APPSPACK::Constraints::Linear linear(params.sublist("Linear"));
    
    // Instantiate the solver
    APPSPACK::Solver solver(params.sublist("Solver"), executor, linear);

    // Solver the problem
    APPSPACK::Solver::State state = solver.solve();

    // Send a termination command to every worker
    APPSPACK::GCI::initSend();
    APPSPACK::GCI::pack(1);
    for (int i = 0; i < nWorkers; i ++)
      APPSPACK::GCI::send(APPSPACK::Executor::MPI::Terminate, i+1);

  }

  // *** Worker ***
  else			
  {
    // Set up custom evaluator
    CustomEvaluator evaluator;
    
    // Continuously receive and process incoming messages
    while (1)
    {
      // Blocking receive for the next message. 
      int msgtag, junk;
      APPSPACK::GCI::recv();	            // Block for next message
      APPSPACK::GCI::bufinfo(msgtag, junk); // Extract the message tag
      
      // Check for termination based on the message tag.
      if (msgtag == APPSPACK::Executor::MPI::Terminate) 
	break;

      // Local vars to be packed and unpacked
      int tag;
      APPSPACK::Vector x;
      APPSPACK::Vector f;
      string msg;

      // Unpack the latest message - must be done just like this.
      APPSPACK::GCI::unpack(tag);
      APPSPACK::GCI::unpack(x);

      // Evaluate the function
      evaluator(tag,x,f,msg);

      // Send a reply - must be done just like this.
      // NOTE FOR USERS UPGRADING TO VERSION 5.0: The variable isf is gone!
      APPSPACK::GCI::initSend();
      APPSPACK::GCI::pack(tag);
      APPSPACK::GCI::pack(f);
      APPSPACK::GCI::pack(msg);
      APPSPACK::GCI::send(APPSPACK::Executor::MPI::Feval,0);

    }

  } // worker

  APPSPACK::GCI::exit();

} // main

