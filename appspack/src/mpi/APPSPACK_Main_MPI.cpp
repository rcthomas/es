// $Id: APPSPACK_Main_MPI.cpp,v 1.15 2006/06/30 01:42:08 tgkolda Exp $ 
// $Source: /space/CVS-Acro/acro/packages/appspack/appspack/src/APPSPACK_Main_MPI.cpp,v $ 

//@HEADER
// ************************************************************************
// 
//          APPSPACK: Asynchronous Parallel Pattern Search
//                 Copyright (2003) Sandia Corporation
// 
// Under terms of Contract DE-AC04-94AL85000, there is a non-exclusive
// license for use of this work by or on behalf of the U.S. Government.
// 
// This library is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as
// published by the Free Software Foundation; either version 2.1 of the
// License, or (at your option) any later version.
//  
// This library is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//                                                                                 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA.                                                                           .
// 
// Questions? Contact Tammy Kolda (tgkolda@sandia.gov) 
// 
// ************************************************************************
//@HEADER

/*!
  \file APPSPACK_Main_MPI.cpp
  \brief The main() for a MPI version of APPSPACK based on a simple system call.
*/

#include "APPSPACK_GCI.hpp"
#include "APPSPACK_Common.hpp"
#include "APPSPACK_Parameter_List.hpp"
#include "APPSPACK_Utils.hpp"
#include "APPSPACK_Evaluator_SystemCall.hpp"
#include "APPSPACK_Executor_MPI.hpp"
#include "APPSPACK_Solver.hpp"
#include "APPSPACK_Vector.hpp"
#include "APPSPACK_Constraints_Linear.hpp"

//! Main subroutine for default MPI version of APPSPACK
/*!
  \note Only works if MPI is enabled at configure time; see \ref
  pageConfigureOptions_MPI.
*/

int main(int argc, char* argv[])
{
  // *** Start MPI ***
  // (also processes argc and argv which may have had extra "stuff"
  // added on by mpirun)
  int rank = APPSPACK::GCI::init(argc, argv);
 
  // *** Check usage, exit if error ***
  if (argc < 2)
  {
    if (rank == 0)
      cout << "Usage: " << argv[0] << " <input file>" << endl;
    APPSPACK::GCI::exit();
    return 1;
  }
    
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

    // Read the input file
    APPSPACK::Parameter::List params;
    bool retCode = APPSPACK::parseTextInputFile(argv[1], params);
    if (retCode == false)
      return 1;

    // Send the evaluator information to each worker
    APPSPACK::GCI::initSend();
    params.sublist("Evaluator").pack();
    for (int i = 0; i < nWorkers; i ++)
      APPSPACK::GCI::send(APPSPACK::Executor::MPI::Init, i+1);

    // Create an executor object
    APPSPACK::Executor::MPI executor;

    // Create a constraints object
    string constraintType = "Linear";
    if (!params.isParameterSublist("Linear"))
      constraintType = "Bounds";

    APPSPACK::Constraints::Linear linear(params.sublist(constraintType));
    
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
    // Unpack parameter list sent by master
    APPSPACK::GCI::recv(APPSPACK::Executor::MPI::Init,0);
    APPSPACK::Parameter::List params;
    params.unpack();

    // Set up evaluator
    APPSPACK::Evaluator::SystemCall evaluator(params);
    
    // Continuously receive and process incoming messages
    while (1)
    {
      // Blocking receive for the next message
      int msgtag, junk;
      APPSPACK::GCI::recv();
      APPSPACK::GCI::bufinfo(msgtag, junk);
      
      // Check for termination
      if (msgtag == APPSPACK::Executor::MPI::Terminate) 
	break;

      // Local vars to be packed and unpacked
      int tag;
      APPSPACK::Vector x;
      bool isF;
      APPSPACK::Vector f;
      string msg;

      // Unpack the latest message
      APPSPACK::GCI::unpack(tag);
      APPSPACK::GCI::unpack(x);

      // Evaluate the function
      evaluator(tag,x,f,msg);

      // Send a reply
      APPSPACK::GCI::initSend();
      APPSPACK::GCI::pack(tag);
      APPSPACK::GCI::pack(f);
      APPSPACK::GCI::pack(msg);
      APPSPACK::GCI::send(APPSPACK::Executor::MPI::Feval,0);

    }

  } // worker

  APPSPACK::GCI::exit();

} // main

