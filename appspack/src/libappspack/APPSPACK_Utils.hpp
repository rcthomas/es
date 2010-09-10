// $Id: APPSPACK_Utils.hpp,v 1.15 2005/10/10 16:47:58 jgriffi Exp $ 
// $Source: /space/CVS-Acro/acro/packages/appspack/appspack/src/APPSPACK_Utils.hpp,v $ 

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
  \file APPSPACK_Utils.hpp
  \brief No classes - just some utility functions in the APPSPACK namespace.
*/

#ifndef APPSPACK_UTILS_HPP
#define APPSPACK_UTILS_HPP

#include "APPSPACK_Common.hpp"
#include "APPSPACK_Parameter_List.hpp"

namespace APPSPACK
{

/*!  Get the next quoted string on the given line, starting at
  position pos. 

  \param line - Line of text from which to read

  \param pos - On input, the starting position in the line. On output,
  the next position after the quoted string (which may be
  std::string::npos). If there is any sort of error, this is set to
  std::string::npos upon return.

  \param value - On output, filled in with the quoted string (without
  the quotes). This is an empty string if no quoted string is found.

  \retval Returns true if the quoted string is successfully found,
  false otherwise.
  
*/
bool getNextQuotedString(const string& line, string::size_type& pos, string& value);

/*!  Get the next string on the given line, starting at
  position pos. 

  \param line - Line of text from which to read

  \param pos - On input, the starting position in the line. On output,
  the next position after the string (which may be
  std::string::npos). If there is any sort of error, this is set to
  std::string::npos upon return.

  \param value - On output, filled in with the next string (i.e., the
  next contiguous block of non-space characters). This is an empty
  string if no string is found.

  \retval Returns true if the string is successfully found,
  false otherwise.
  
*/
bool getNextString(const string& line, string::size_type& pos, string& value);

/*!  Get the next string on the given line, starting at
  position pos, and convert it to a double. 

  \param line - Line of text from which to read

  \param pos - On input, the starting position in the line. On output,
  the next position after the string (which may be
  std::string::npos). If there is any sort of error in reading the
  next string, this is set to std::string::npos upon return.

  \param value - On output, filled in with the double value constrained
  in the next string (i.e., the next contiguous block of non-space
  characters).
  
  \retval Returns true if the next string contains a double,
  false otherwise.
  
*/
bool getNextDouble(const string& line, string::size_type& pos, double& value);


/*!  Get the next string on the given line, starting at
  position pos, and convert it to a int. 

  \param line - Line of text from which to read

  \param pos - On input, the starting position in the line. On output,
  the next position after the string (which may be
  std::string::npos). If there is any sort of error in reading the
  next string, this is set to std::string::npos upon return.

  \param value - On output, filled in with the int value constrained
  in the next string (i.e., the next contguous block of non-space
  characters).
  
  \retval Returns true if the next string contains a int,
  false otherwise.
  
*/
bool getNextInt(const string& line, string::size_type& pos, int& value);


/*!
  \brief Parse an APPSPACK input file and store the data in the given parameter list.

  \param filename - The file name.  See \ref pageExecutables_step2 for details on
  the structure of the input file.
  
  \param params - The parameter list that is to be filled in by this
  function

  \return Returns false if there are any problems parsing the input
  file, true otherwise.


*/
bool parseTextInputFile(const string filename, Parameter::List& params);

//! Process a single line of the text input file.
bool processTextInputFileLine(const string& line, Parameter::List& params, Parameter::List*& subPtr,
				ifstream &fin);

/*! Splits line into a vector of strings based upon whitespace delimiter.
\par Example
The following code segment
\code
string line("one two three");
vector<string> linesplit;
tokenize(line,linesplit);
for (int i = 0; i < linesplit.size(); i++)
  cout << linesplit[i] << endl;
\endcode
will output
\verbatim
one
two
three
\endverbatim
*/
void tokenize(const string& line, vector<string>& linesplit);


}

#endif

