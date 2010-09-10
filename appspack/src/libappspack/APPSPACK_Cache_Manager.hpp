// $Id: APPSPACK_Cache_Manager.hpp,v 1.9 2006/06/30 19:15:20 tgkolda Exp $ 
// $Source: /space/CVS-Acro/acro/packages/appspack/appspack/src/APPSPACK_Cache_Manager.hpp,v $ 

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

/*! \file APPSPACK_Cache_Manager.hpp
    \brief Class definition for APPSPACK::Cache::Manager
*/

#ifndef APPSPACK_CACHE_MANAGER_HPP
#define APPSPACK_CACHE_MANAGER_HPP

#include "APPSPACK_Vector.hpp"
#include "APPSPACK_Cache_SplayTree.hpp" 
#include "APPSPACK_Cache_Point.hpp" 
#include "APPSPACK_Parameter_List.hpp"

namespace APPSPACK 
{

//! Namespace for all the cache-related classes
/*!

The cache supports the ability to read and/or write cached function
values from files.

A single line in a typical cache file will look something like the
following.

\verbatim
f=[ 3.1250e-04 ] x=[ 0.0000e+00 -1.2500e-02  ]
\endverbatim

The line is parsed as follows.
<ul>
<li> The line must begin with "f=[".
<li> This is followed by a minimum of one space.
<li> Entries of f, separated by spaces, are read until "]" is
encountered, surrounded by white space.
<li> This is followed by a minimum of one space.
<li> The next characters are "x=[".
<li> This is followed by a minimum of one space.
<li> Entries of x, separated by spaces, are read until "]" is
encountered, surrounded by white space.
</ul>

Any line that does not conform to the above format is simply ignored.

*/
namespace Cache
{

/*! 
  \brief Actual cache object. Stored the cached points in a splay tree.
  \author H. Alton Patrick, Summer 2000<br>
  Tamara G. Kolda
*/
class Manager 
{

public:

  /*! Constructor */
  Manager(APPSPACK::Parameter::List& params, const Vector& scaling);

  /*! Destructor */
  ~Manager();

  //! Add the given point to the cache.
  bool insert(const Vector& x, const Vector& f);
  
  //! Return true if x is cached and fill in the function value.
  bool isCached(const Vector& x, Vector& f);

private:

  //! Parse the cache input file (reading the name from the parameter list)
  void parseInputFile(APPSPACK::Parameter::List& params);

  //! Process a single line from the input file
  void processInputLine(string& line);

  //! Open the output line for the cache
  void openOutputFile(APPSPACK::Parameter::List& params);

  //! Write a given cache point to the output file
  void writeToOutputFile(const Vector& x, const Vector& f);

  //! Close the output file
  void closeOutputFile();
private:

  //! Pointer to splay tree containing the cache
  SplayTree<Cache::Point>* treeptr;   

  //! Use cache output file?
  bool isFout;

  //! Cache output file
  ofstream fout;

  //! Precision of output (determined by tolerance)
  int precision;
};
} // namespace Cache
} // namespace APPSPACK
#endif
