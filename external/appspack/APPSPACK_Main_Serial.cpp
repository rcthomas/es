// $Id: APPSPACK_Main_Serial.cpp,v 1.13 2005/08/16 00:26:28 jgriffi Exp $ 
// $Source: /space/CVS-Acro/acro/packages/appspack/appspack/src/APPSPACK_Main_Serial.cpp,v $ 

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
  \file APPSPACK_Main_Serial.cpp
  \brief The main() for a serial version of appspack based on a simple system call.
*/

#include "APPSPACK_Common.hpp"
#include "APPSPACK_Parameter_List.hpp"
#include "APPSPACK_Utils.hpp"
#include "APPSPACK_Evaluator_SystemCall.hpp"
#include "APPSPACK_Executor_Serial.hpp"
#include "APPSPACK_Solver.hpp"
#include "APPSPACK_Constraints_Linear.hpp"

//! Main subroutine for default serial version of APPSPACK
int main(int argc, char* argv[])
{
  
  if (argc < 2)
  {
    cout << "Usage: " << argv[0] << " <input file>" << endl;
    return 1;
  }

  APPSPACK::Parameter::List params;

  bool retCode = APPSPACK::parseTextInputFile(argv[1], params);
  if (retCode == false)
    return 1;

  APPSPACK::Evaluator::SystemCall evaluator(params.sublist("Evaluator"));
  
  APPSPACK::Executor::Serial executor(evaluator);

  string constraintType = "Linear";
  if (!params.isParameterSublist("Linear"))
    constraintType = "Bounds";
  
  APPSPACK::Constraints::Linear linear(params.sublist(constraintType));
  
  APPSPACK::Solver solver(params.sublist("Solver"), executor, linear);
  
  APPSPACK::Solver::State state = solver.solve();
}

