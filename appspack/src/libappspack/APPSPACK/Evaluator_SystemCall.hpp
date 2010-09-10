// $Id: APPSPACK_Evaluator_SystemCall.hpp,v 1.12 2006/06/30 01:42:08 tgkolda Exp $ 
// $Source: /space/CVS-Acro/acro/packages/appspack/appspack/src/APPSPACK_Evaluator_SystemCall.hpp,v $ 

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
  \file APPSPACK_Evaluator_SystemCall.hpp
  \brief Class description for APPSPACK::Evaluator::SystemCall
*/

#ifndef APPSPACK_EVALUATOR_SYSTEMCALL
#define APPSPACK_EVALUATOR_SYSTEMCALL

#include "APPSPACK_Common.hpp"
#include "APPSPACK_Vector.hpp"
#include "APPSPACK_Evaluator_Interface.hpp"
#include "APPSPACK_Parameter_List.hpp"

namespace APPSPACK
{

namespace Evaluator 
{

//! SystemCall for (serial) function evaluation
/*!

Default method for handling evaluations. The user provides an
executable that takes an input file with the point to be evaluated and
creates an output file with the result.

See \ref pageExecutables for more information on using APPSPACK with
this object.

\image html Evaluator.gif "Basic system call mode of evaluation"

*/

class SystemCall : public Interface
{
public:
  
  //! Constructor 
  SystemCall(const string& executableName_in, 
	     const string& inputPrefix_in, 
	     const string& outputPrefix_in);

  /*! \brief Constructor (via parameter list)

  Reads "Executable Name", "Input Prefix", and "Output Prefix" from
  the give parameter list.

  See \ref pageExecutables for more information.

  */
  SystemCall(const Parameter::List& params);

  //! Destructor 
  virtual ~SystemCall() {};
  
  // derived
  virtual void operator()(int tag_in, const Vector& x_in, Vector& f_out, string& msg_out);

  //! Prints out the name of the executable, and the input and output file prefixes.
  virtual void print() const;

private:

  //! Create the strings #tag, #inputFileName, #outputFileName, and #execString
  void createStrings(int tag_in);

  //! Write the parameter file
  /*! Create the file named #inputFileName containing the input
    vector \f$x\f$.  The first line of the file contains a single
    integer defining the size of \f$x\f$. This is followed by each
    entry of \f$x\f$ on its own line in scientific format. For
    example, the file might appear as follows.

\verbatim
3
1.23547e-5
2.00000e-3
4.55000e+10
\endverbatim
  */
  void writeInputFile(const Vector& x);


  //! Run the function evaluation using a system command
  void runProgram();

  //! Read the result file containing the function value
  /*! 
    Read the file of the name specified by #outputFileName. The file
    is expected to contain one of the following types of outputs:

    <ul>
    <li>An error message on a single line.
    <li>A sequence of output values, one per line. Typically, this
    will be a single output value. Each value should be a value in
    scientific format or the string "DNE" (without quotes) to indicate
    that a value does not exist.
    </ul>

    An empty function vector will be returned if we cannot read the
    result file or encounter any other problems. Otherwise #f will
    contain the function value for this task.
    
  */
  void readOutputFile();


  //! Delete the input and output files
  void deleteFiles();

private:

  //! Executable name
  const string executableName;

  //! Prefix for name of parameters file
  const string inputPrefix;

  //! Prefix for name of output file
  const string outputPrefix;

  //! Tag for this function evaluation
  string tag;

  //! Name of parameters file for this function evaluation
  string inputFileName;

  //! Name of output file for this function evaluation
  string outputFileName;

  //! Is there currently an input file (that will need to be deleted)?
  bool isInputFile;

  //! Is there currently an output file (that will need to be deleted)?
  bool isOutputFile;

  //! Executable string for this function evaluation
  string execString;

  //! The function value
  Vector f;

  //! Any message about the function value
  string msg;

};

}

}

#endif
