// $Id: APPSPACK_Vector.hpp,v 1.32 2006/06/30 01:42:08 tgkolda Exp $ 
// $Source: /space/CVS-Acro/acro/packages/appspack/appspack/src/APPSPACK_Vector.hpp,v $ 

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
  \file APPSPACK_Vector.hpp
  \brief Class definition for APPSPACK::Vector
*/

#ifndef APPSPACK_VECTOR_HPP
#define APPSPACK_VECTOR_HPP

#include "APPSPACK_Common.hpp"

namespace APPSPACK
{

//! Class Vector -- Extends properties of the Standard Template Library (STL) vector<double>, v.
class Vector
{

public:

  //! Constructor -- creates vector of length 0.
  Vector();

  //! Constructor -- creates vector of length n.
  Vector(int n);

  //! Constructor -- creates vector of length n, each element assigned the value val.
  Vector(int n, double val);

  //! Constructor -- creates a vector by copying the first n elements pointed by x. 
  Vector(int n, double *x);

  //! Copy constructor.
  Vector(const Vector& x);
  
  //! Copy constructor.
  Vector(const vector<double>& x);
  
  //! Destructor.
  ~Vector();

  //@{ \name Memory allocating/altering methods.

  //! Resizes vector length.
  void resize(int n);
  
  //! Sets the capacity of the vector to at least n.
  void reserve(int n);

  //! Appends a to the end of the vector, increasing vector length by 1.
  void push_back(double a);
  
  //! Reset vector to size n and all values to value alpha
  void assign(int n, double alpha);
  
  //! Appends x to end of the vector.
  void append(const Vector& x);

  //! Appends n copies of alpha to the end of the vector.
  void append(int n, double alpha);

  //! Deletes the ith element from the vector, decreasing the length by 1.
  void erase(int i);

  //! Copies x.
  Vector& operator=(const Vector& x);  

  //@}

  //@{ \name Data access methods.
  //! Returns the length of the vector.
  int size() const;

  //! Return true if vector is size zero, false otherwise.
  bool empty() const;

  //! Returns an equivalent STL vector.
  const vector<double>& getStlVector() const;
 
  //! Returns the ith element of the vector.
  double operator[](int i) const;
  
  //! Returns true if two vectors are equal, false otherwise.
  bool operator==(const Vector& x) const;

  //! Returns true if two vectors are not equal, false otherwise.
  bool operator!=(const Vector& x) const;

  //! On exit, b[i] = true if the ith element of the vector is nonzero, false otherwise.  
  /*! b is automatically resized to have appropriate length.*/
  void copyToBool(vector<bool>& b) const;

  //! Prints out vector to specified stream.
  ostream& leftshift(ostream& stream) const;

  //! Prints out vector to specified stream.
  ostream& leftshift(ostream& stream, int precision) const;

  //@}

  //@{ \name Vector mathematics.

  //! Sets all entries of the vector to zero.
  void zero();

  //! Scales all elements of the vector by scalar alpha.
  void scale(double alpha);  
  
  //! On exit, the ith element is scaled by s[i].
  void scale(const Vector& s);  
    
  //! Returns the two norm of the vector.
  double norm() const;

  //! Returns the smallest element of the vector.
  double min() const;

  //! Returns the largest element of the vector.
  double max() const;

  //! Compute the dot product between v (this vector) and x.
  double dot(const Vector& x) const;
  
  //! Adds x to this vector.
  Vector& operator+=(const Vector& x);
  
  //! Substracts x from this vector.
  Vector& operator-=(const Vector& x);

  //! Returns a reference to the ith element.
  double& operator[](int i);

  //@}

private:
  
  //! The vector.
  vector<double> vec;

};

}

//! Printing an APPSPACK::Vector
ostream& operator<<(ostream& stream, const APPSPACK::Vector& x);

#endif
