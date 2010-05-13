// $Id: APPSPACK_Evaluator_SystemCall.cpp,v 1.16 2006/06/30 01:42:08 tgkolda Exp $ 
// $Source: /space/CVS-Acro/acro/packages/appspack/appspack/src/APPSPACK_Evaluator_SystemCall.cpp,v $ 

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
  \file APPSPACK_Evaluator_SystemCall.cpp
  \brief Implementation of APPSPACK::Evaluator::SystemCall
*/

#include "APPSPACK_Common.hpp"
#include "APPSPACK_Evaluator_SystemCall.hpp"
#include "APPSPACK_Utils.hpp"
#include "APPSPACK_Float.hpp"

APPSPACK::Evaluator::SystemCall::SystemCall(const string& executableName_in,      
					    const string& inputPrefix_in,      
					    const string& outputPrefix_in) :
  executableName(executableName_in),
  inputPrefix(inputPrefix_in),
  outputPrefix(outputPrefix_in),
  isInputFile(false),
  isOutputFile(false)
{
}

APPSPACK::Evaluator::SystemCall::SystemCall(const Parameter::List& params) :
  executableName(params.getParameter("Executable Name", "a.out")),
  inputPrefix(params.getParameter("Input Prefix", "input")),
  outputPrefix(params.getParameter("Output Prefix", "output")),
  isInputFile(false),
  isOutputFile(false)
{
}

void APPSPACK::Evaluator::SystemCall::operator()(int tag_in, 
						 const Vector& x_in, 
						 Vector& f_out,
						 string&  msg_out)
{
  createStrings(tag_in);
  writeInputFile(x_in);
  runProgram();
  readOutputFile();
  deleteFiles();

  f_out = f;
  msg_out = msg;

  return;
}


void APPSPACK::Evaluator::SystemCall::createStrings(int tag_in)
{
  char tmp[128];
  sprintf(tmp, "%d", tag_in);
  tag = tmp;

  inputFileName = inputPrefix + "." + tag;
  outputFileName = outputPrefix + "." + tag;
  execString = executableName + " " + inputFileName + " " + outputFileName + " " + tag;
}

void APPSPACK::Evaluator::SystemCall::writeInputFile(const Vector& x)
{
  ofstream stream;
  stream.open(inputFileName.c_str());

  if (!stream) 
  {
    cerr << "APPSPACK::Evaluator::SystemCall::writeInputFile - Unable to open the following file: " 
	 << inputFileName << endl;
    return;
  }

  isInputFile = true;

  stream << x.size() << "\n";

  for (int i = 0; i < x.size(); i ++)
    stream << x[i] << "\n";

  stream.close();
}


void APPSPACK::Evaluator::SystemCall::runProgram()
{
  system(execString.c_str());
}

void APPSPACK::Evaluator::SystemCall::readOutputFile()
{
  ifstream fin;
  fin.open(outputFileName.c_str());

  if (!fin) 
  {
    cerr << "\nWarning - no output file for the point corresponding to tag=" << tag << endl;
    f.resize(0);
    msg = "No Output File";
    return;
  }

  isOutputFile = true;
    
  if (fin.eof())
  {
    cerr << "\nWarning - empty output file for the point corresponding to tag=" << tag << endl;
    f.resize(0);
    msg = "Empty Output File";
    fin.close();
    return;
  }

  // Try to fill the vector f
  f.resize(0);
  string line;
  string str;
  double val;
  while(!fin.eof())
  {
    getline(fin, line);
    string::size_type pos = 0;
    getNextString(line,pos,str);

    if (str == "DNE")
      f.push_back(dne());
    else if (sscanf(str.c_str(), "%le", &val) == 1)
      f.push_back(val);
    else if (f.size() == 0) 
    {
      // Error on the 1st line, which may contain a message
      cerr << "\nWarning - error reading function value for tag=" << tag << endl;
      msg = (line.empty()) ? "No Floating Point Value Found in Output File" : line;
      fin.close();
      return;
    }
  }
  
  msg = "Success";
  fin.close();
  return;
}

void APPSPACK::Evaluator::SystemCall::deleteFiles()
{
#ifndef SNL_TFLOPS_ENV
  if (isInputFile)
    unlink(inputFileName.c_str());
  if (isOutputFile)
    unlink(outputFileName.c_str());
#endif

  isInputFile = false;
  isOutputFile = false;
}

void APPSPACK::Evaluator::SystemCall::print() const
{
  cout << "Using SystemCall Evaluator" << endl;
  cout << "Executable Name = " << executableName << endl;
  cout << "Input File Name Prefix = " << inputPrefix << endl;
  cout << "Output File Name Prefix = " << outputPrefix << endl;
}
