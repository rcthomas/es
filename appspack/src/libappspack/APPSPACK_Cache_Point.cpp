// $Id: APPSPACK_Cache_Point.cpp,v 1.9 2006/06/30 01:42:08 tgkolda Exp $ 
// $Source: /space/CVS-Acro/acro/packages/appspack/appspack/src/APPSPACK_Cache_Point.cpp,v $ 

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
  \file APPSPACK_Cache_Point.cpp
  \brief Implementation of APPSPACK::Cache::Point
*/

#include "APPSPACK_Cache_Point.hpp" 

APPSPACK::Vector APPSPACK::Cache::Point::scaling(0);
double APPSPACK::Cache::Point::tolerance = 0;

APPSPACK::Cache::Point::Point() :
  xPtr(NULL),
  x(*xPtr),
  f()
{
}

APPSPACK::Cache::Point::Point(const Vector& x_in) :
  xPtr(NULL),
  x(x_in),
  f()
{
  checkSize();
}

APPSPACK::Cache::Point::Point(const Vector& x_in, const Vector& f_in) :
  xPtr(NULL),
  x(x_in),
  f(f_in)
{
  checkSize();
}

APPSPACK::Cache::Point::Point(const Point& source) :
  xPtr(new Vector(source.x)),
  x(*xPtr),
  f(source.f)
{
  checkSize();
}

APPSPACK::Cache::Point::~Point()
{
  delete xPtr;
}

void APPSPACK::Cache::Point::copyData(const Point& source)
{
  f = source.f;
}

const APPSPACK::Vector& APPSPACK::Cache::Point::getF()
{
  return f;
}

bool APPSPACK::Cache::Point::operator>(const Point& pt) const
{
  checkSize(pt);

  int n = x.size();

  for(int i = 0; i < n; i++) 
  {
    if ( fabs(x[i] - pt.x[i]) > (scaling[i] * tolerance) ) 
    {
      // unequal with respect to tolerance

      if ( (x[i] - pt.x[i]) > (scaling[i] * tolerance) ) 
      {
	// this > other
	return true;
      }
      else
      {
	// this < other
	return false;
      }

    }
  }

  // this == other
  return false;
}

bool APPSPACK::Cache::Point::operator<(const Point& pt) const
{
  checkSize(pt);

  int n = x.size();

  for(int i = 0; i < n; i++) 
  {
    if ( fabs(x[i] - pt.x[i]) > (scaling[i] * tolerance) ) // unequal 
    {
      if ( (pt.x[i] - x[i]) > (scaling[i] * tolerance) ) // x > pt.x
      {
	// this < other
	return true;
      }
      else
      {
	// this > other
	return false;
      }
    }
  }

  // this == other
  return false;
}

bool APPSPACK::Cache::Point::operator!=(const Point& pt) const
{
  checkSize(pt);

  int n = x.size();

  for(int i = 0; i < n; i++) 
  {
    if ( fabs(pt.x[i] - x[i]) > (scaling[i] * tolerance))
    {
      // this != other
      return true;
    }
  }

  // this == other
  return false;
}

void APPSPACK::Cache::Point::setStaticScaling(const Vector& scaling_in)
{
  scaling = scaling_in;
}

void APPSPACK::Cache::Point::setStaticTolerance(double tolerance_in)
{
  tolerance = tolerance_in;
}


// PRIVATE
void APPSPACK::Cache::Point::checkSize() const
{
  if (x.size() != scaling.size())
  {
    cout << "APPSPACK::Cache::Point::checksize - scaling size mismatch" << endl;
    throw "APPSPACK Error";
  }
}

// PRIVATE
void APPSPACK::Cache::Point::checkSize(const Point& pt) const
{
  if (x.size() != pt.x.size())
  {
    cout << "APPSPACK::Cache::Point::checksize - point size mismatch" << endl;
    throw "APPSPACK Error";
  }
}

