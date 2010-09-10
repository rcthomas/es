// $Id: APPSPACK_Cache_Point.hpp,v 1.8 2006/06/30 01:42:08 tgkolda Exp $ 
// $Source: /space/CVS-Acro/acro/packages/appspack/appspack/src/APPSPACK_Cache_Point.hpp,v $ 

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
  \file APPSPACK_Cache_Point.hpp
  \brief Class definition for APPSPACK::Cache::Point
*/

#ifndef APPSPACK_CACHE_POINT_HPP
#define APPSPACK_CACHE_POINT_HPP

#include "APPSPACK_Common.hpp"
#include "APPSPACK_Vector.hpp"

namespace APPSPACK 
{
namespace Cache
{

/*!  \brief Store a cached point. Allow comparisons between points for
  storage in a splay tree. See Cache::Manager.

  \author H. Alton Patrick, Summer 2000
*/

class Point 
{

public:

  //! Empty constructor
  Point();

  //! Constructor
  /*! \note Saves a reference to x_in, but DOES NOT COPY IT. In other
    words, this is only a shallow copy, not a deep copy. We are
    exploiting the way that the splay tree is implemented in order to
    minimize copy of vectors. We know that any Point that is
    created by this constructor will later be copied into a new
    Point (and then we will actually do a deep copy). 
  */
  Point(const Vector& x_in);

  //! Constructor
  /*! \note Saves a reference to x_in, but DOES NOT COPY IT. In other
    words, this is only a shallow copy, not a deep copy. We are
    exploiting the way that the splay tree is implemented in order to
    minimize copy of vectors. We know that any Point that is
    created by this constructor will later be copied into a new
    Point (and then we will actually do a deep copy). 
  */
  Point(const Vector& x_in, const Vector& f_in);
    
  //! Copy constructor
  /*! \note Deep copy. */
  Point(const Point& source);

  //! Destructor 
  ~Point();

  //! Copy the relevant data from another cached point
  void copyData(const Point& source);

  //! Extract data
  const Vector& getF();

  /*! \brief Compare two Points, based on a lexicographic ordering of their
    coordinates.  

    Let x denote the vector corresponding to this object and
    y denote the vector corresponding to the incoming
    Point. Then this operator returns true if
    \f[
    \frac{x_i - y_i}{s_i} > \tau \mbox{ for all } i =1,dots,n
    \f]

    Here \f$s_i\f$ denotes the i-th entry of #scaling and \f$\tau\f$
    represents the #tolerance.

    Otherwise, returns false.
  */
  bool operator>(const Point& pt) const;

  //! Reverse of operator>
  bool operator<(const Point& pt) const;

  /*! \brief Compare two Points for non-equality within the given tolerance.

    Let x denote the vector corresponding to this object and
    y denote the vector corresponding to the incoming
    Point. Then this operator returns true if
    \f[
    \frac{1}{s_i}\left|{x_i} - {y_i}\right| > \tau \mbox{ for any } i = 1,\dots,n
    \f]

    Here \f$s_i\f$ denotes the i-th entry of #scaling and \f$\tau\f$
    represents the #tolerance.

    Otherwise, returns false.
  */
  bool operator!=(const Point& pt) const;

  //! Set the static Point::scaling vector
  static void setStaticScaling(const Vector& scaling_in);

  //! Set the static Point::tol value
  static void setStaticTolerance(double tolerance_in);

private:
  
  //! Check that the size of this point matches that of the scaling
  void checkSize() const;

  //! Check that the size of this point matches that of the given point
  void checkSize(const Point& pt) const;

public:
  
  //! scaling of variables
  static Vector scaling;  

  //! comparison tolerance
  static double tolerance;		  

private:

  //! pointer to data if this data is actually owned by the cached point
  Vector* xPtr;

  //! Reference to the data, either internally or externally owned
  const Vector& x;

  //! Vector of function values
  Vector f;

};

}
}
#endif
