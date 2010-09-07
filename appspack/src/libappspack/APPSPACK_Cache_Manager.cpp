// $Id: APPSPACK_Cache_Manager.cpp,v 1.15 2006/06/30 01:42:08 tgkolda Exp $ 
// $Source: /space/CVS-Acro/acro/packages/appspack/appspack/src/APPSPACK_Cache_Manager.cpp,v $ 

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

/*! \file APPSPACK_Cache_Manager.cpp
    \brief Implementation of APPSPACK::Cache::Manager
*/

#include "APPSPACK_Common.hpp"
#include "APPSPACK_Cache_Manager.hpp"
#include "APPSPACK_Utils.hpp"
#include "APPSPACK_Print.hpp"
#include "APPSPACK_Float.hpp"

APPSPACK::Cache::Manager::Manager(Parameter::List& params, const Vector& scaling) :
  isFout(false)
{
  Cache::Point::setStaticScaling(scaling);
  Cache::Point::setStaticTolerance(params.getParameter("Cache Comparison Tolerance", 
                                   params.getDoubleParameter("Step Tolerance") / 2.0));

  precision = params.getParameter("Cache File Precision", 14);

  treeptr = new SplayTree<Point>;

  parseInputFile(params);
  openOutputFile(params);

  if (params.isParameterVector("Initial X") && params.isParameterVector("Initial F"))
  {
    Vector x = params.getVectorParameter("Initial X");
    Vector f = params.getVectorParameter("Initial F");
    insert (x,f);	
  }

} 

APPSPACK::Cache::Manager::~Manager()
{
  delete treeptr;
  closeOutputFile();
}

bool APPSPACK::Cache::Manager::insert(const Vector& x, const Vector& f)
{
  Point cp(x,f);
  bool isInserted = treeptr->insert(cp);
  if (isInserted)
    writeToOutputFile(x,f);
  return isInserted;
}

bool APPSPACK::Cache::Manager::isCached(const Vector& x, Vector& f)
{
  Point cp(x,f);

  if ( ! (treeptr->find(cp)) )
  {
    return false;
  }

  f = cp.getF();
  return true;
}

void APPSPACK::Cache::Manager::parseInputFile(APPSPACK::Parameter::List& params)
{
  string inname;
  inname = params.getParameter("Cache Input File", inname);
  if (inname.empty())
    return;

  ifstream fin;
  fin.open(inname.c_str());
  
  if (!fin)
  {
    cout << "The cache input file \"" << inname << "\" does not exist." << endl;
    return;
  }

  string line;
  while (!fin.eof())
  {
    getline(fin, line);
    processInputLine(line);
  }

  fin.close();
}

void APPSPACK::Cache::Manager::processInputLine(string& line)
{

  string::size_type line_pos;
  
  string element;
  string::size_type element_pos;
  
  // Skip "f="
  line_pos = 0;

  if (!getNextString(line, line_pos, element))
    return;

  // Do nothing if line does not have the right format
  if (element != "f=[") 
    return;
  
  // Read elements of F 
  Vector f;
  f.resize(0);
  double d;
  while (1)
  {
    // Read the next element of F
    if (!getNextString(line, line_pos, element))
      return;
    
    // Check for termination
    if (element == "]")
      break;

    // Process into a value
    element_pos = 0;
    if (!getNextDouble(element, element_pos, d))
      return;
    f.push_back(d);
  }
  Vector v=f;

  // Skip "x=["
  if (!getNextString(line, line_pos, element))
    return;
  
  // Do nothing if line does not have the right format
  if (element != "x=[") 
    return;

  // Read elements of x 
  Vector x;
  x.resize(0);
  while (1)
  {
    // Read the next element of x
    if (!getNextString(line, line_pos, element))
      return;
    
    // Check for termination
    if (element == "]")
      break;

    // Process into a value
    element_pos = 0;
    if (!getNextDouble(element, element_pos, d))
      return;
    x.push_back(d);
  }

  // Insert into the queue
  insert(x, v);
  
}

void APPSPACK::Cache::Manager::openOutputFile(APPSPACK::Parameter::List& params)
{
  string outname;
  outname = params.getParameter("Cache Output File", outname);
  if (outname.empty())
    return;

  // Open file for append
  fout.open(outname.c_str(), std::ios::out|std::ios::app);
  
  if (!fout)
  {
    cerr << "APPSPACK::Cache::Manager::openOutputFile - Unable to open cache output file" << endl;
    return;
  }

  isFout = true;
}

void APPSPACK::Cache::Manager::writeToOutputFile(const Vector& x, const Vector& f)
{
  if (!isFout)
    return;

  fout << "f=[ ";
  f.leftshift(fout,precision);
  fout << " ]";

  fout << " x=[ ";
  x.leftshift(fout,precision);
  fout << " ]" << endl;
}

void APPSPACK::Cache::Manager::closeOutputFile()
{
  if (!isFout)
    fout.close();
}
