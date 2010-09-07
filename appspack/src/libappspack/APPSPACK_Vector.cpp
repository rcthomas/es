// $Id: APPSPACK_Vector.cpp,v 1.35 2006/06/30 01:42:08 tgkolda Exp $ 
// $Source: /space/CVS-Acro/acro/packages/appspack/appspack/src/APPSPACK_Vector.cpp,v $ 

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
  \file APPSPACK_Vector.cpp
  \brief Implementation of APPSPACK::Vector
*/

#include "APPSPACK_Vector.hpp"
#include "APPSPACK_Print.hpp"
#include "APPSPACK_LAPACK_Wrappers.hpp"
#include "APPSPACK_Float.hpp"

APPSPACK::Vector::Vector()
{
}

APPSPACK::Vector::Vector(int n) : vec(n)
{
}

APPSPACK::Vector::Vector(int n, double val) : vec(n, val)
{
}

APPSPACK::Vector::Vector(int n, double *x) : vec(x,x+n)
{
}

APPSPACK::Vector::Vector(const APPSPACK::Vector& x) : vec(x.vec)
{
}

APPSPACK::Vector::Vector(const vector<double>& x) : vec(x)
{
}

APPSPACK::Vector::~Vector()
{
}

void APPSPACK::Vector::resize(int n)
{
  vec.resize(n);
}

void APPSPACK::Vector::reserve(int n)
{
  vec.reserve(n);
}

void APPSPACK::Vector::push_back(double a)
{
  vec.push_back(a);
}

void APPSPACK::Vector::assign(int n, double alpha)
{
  vec.assign(n,alpha);
}

void APPSPACK::Vector::append(const Vector& x)
{
  if (x.size() > 0)
    vec.insert(vec.end(), x.vec.begin(), x.vec.end());
}

void APPSPACK::Vector::append(int n, double alpha)
{
  vec.insert(vec.end(), n, alpha);
}

void APPSPACK::Vector::erase(int i)
{
  vec.erase(vec.begin()+i);
}

APPSPACK::Vector& APPSPACK::Vector::operator=(const Vector& x)
{
  vec = x.vec;
  return *this;
}

int APPSPACK::Vector::size() const
{
  return vec.size();
}

bool APPSPACK::Vector::empty() const
{
  return vec.empty();
}

const vector<double>& APPSPACK::Vector::getStlVector() const
{
  return vec;
}

double APPSPACK::Vector::operator[](int i) const
{
  return vec[i];
}

bool APPSPACK::Vector::operator==(const Vector& a) const
{
  return (vec == a.vec);
}

bool APPSPACK::Vector::operator!=(const Vector& a) const
{
  return (vec != a.vec);
}

void  APPSPACK::Vector::copyToBool(vector<bool>& b) const
{
  b.resize(vec.size());
  for (int i = 0; i < vec.size(); i ++)
    b[i] = (vec[i] == 0) ? false : true;
}

ostream& APPSPACK::Vector::leftshift(ostream& stream) const
{
  for (int i = 0; i < vec.size(); i ++) 
  {
    stream << APPSPACK::Print::formatDouble(vec[i]) << " ";
  }
  return stream;
}

ostream& APPSPACK::Vector::leftshift(ostream& stream, int precision) const
{
  for (int i = 0; i < vec.size(); i ++) 
  {
    stream << APPSPACK::Print::formatDouble(vec[i], precision) << " ";
  }
  return stream;
}

ostream& operator<<(ostream& stream, const APPSPACK::Vector& x)
{ 
  stream << "[";
  x.leftshift(stream);
  stream << "]";
  return stream;
}

void APPSPACK::Vector::zero()
{
  vec.assign(vec.size(), 0);
}

void APPSPACK::Vector::scale(double alpha)
{
  for (int i = 0; i < vec.size(); i++)
    vec[i] = alpha * vec[i];
}

void APPSPACK::Vector::scale(const Vector& s)
{
  if (s.vec.size() != vec.size())
  {
    cerr << "APPSPACK::Vector::scale - Size mismatch" << endl;
    throw " APPSPACK Error";
  }
  
  for (int i = 0; i < vec.size(); i++)
    vec[i] = vec[i] * s.vec[i];
}

double APPSPACK::Vector::norm() const
{
  double norm=0;
  for (int i = 0; i < vec.size(); i++)
    norm += vec[i]*vec[i];
  return sqrt(norm);
}

double APPSPACK::Vector::min() const
{
  if (vec.empty())
  {
    cerr << "APPSPACK::Vector::min - empty vector" << endl;
    throw "APPSPACK Error";
  }

  double minvi = vec[0];
  for (int i = 1; i < vec.size(); i++)
    minvi = (minvi < vec[i]) ? minvi : vec[i];
  return minvi;
}

double APPSPACK::Vector::max() const
{
  if (vec.empty())
  {
    cerr << "APPSPACK::Vector::max - empty vector" << endl;
    throw "APPSPACK Error";
  }

  double maxvi = vec[0];
  for (int i = 1; i < vec.size(); i++)
    maxvi = (maxvi > vec[i]) ? maxvi : vec[i];
  return maxvi;
}

double APPSPACK::Vector::dot(const Vector& x) const
{
  if (x.vec.size() != vec.size())
  {
    cerr << "APPSPACK::Vector::dot - Size mismatch" << endl;
    throw " APPSPACK Error";
  }
  
  double z = 0;

#ifdef HAVE_LAPACK
  int inc = 1;
  int n   = x.size();
  // Copy vectors to avoid const compilation errors.
  Vector& xc = const_cast<Vector&>(x);
  Vector& vc = const_cast<Vector&>(*this);
  z = DDOT_F77(&n, &vc[0], &inc, &xc[0], &inc);
#else  
  for (int i = 0; i < x.size(); i ++)
    z += vec[i] * x.vec[i];
#endif 

  return z;
}

APPSPACK::Vector& APPSPACK::Vector::operator+=(const Vector& x)
{
  if (x.vec.size() != vec.size())
  {
    cerr << "APPSPACK::Vector::operator+= - Size mismatch" << endl;
    throw " APPSPACK Error";
  }
  
  for (int i = 0; i < vec.size(); i++)
    vec[i] += x.vec[i];

  return *this;
}

APPSPACK::Vector& APPSPACK::Vector::operator-=(const Vector& x)
{
  if (x.vec.size() != vec.size())
  {
    cerr << "APPSPACK::Vector::operator-= - Size mismatch" << endl;
    throw " APPSPACK Error";
  }
  
  for (int i = 0; i < vec.size(); i++)
    vec[i] -= x.vec[i];

  return *this;
}

double& APPSPACK::Vector::operator[](int i)
{
  return vec[i];
}
