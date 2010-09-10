// $Id: APPSPACK_Float.hpp,v 1.6 2006/06/30 01:42:08 tgkolda Exp $ 
// $Source: /space/CVS-Acro/acro/packages/appspack/appspack/src/APPSPACK_Float.hpp,v $ 

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
  \file APPSPACK_Float.hpp
  \brief No classes--Declares function in APPSPACK namespace for handling infinity.
*/

#ifndef APPSPACK_FLOAT_HPP
#define APPSPACK_FLOAT_HPP

#include "APPSPACK_Common.hpp"

/*! \brief APPSPACK_DBL_MAX is the value we use for infinity (DBL_MAX
  should be defined in float.h)
*/ 
#define APPSPACK_DBL_MAX DBL_MAX 

namespace APPSPACK
{
/*! \brief Use x = APPSPACK::dne() to say that the
 value of x does not exist.

  This is done by returning the constant APPSPACK_DBL_MAX.
*/
double dne();

/*! \brief Returns true if value had previously been set by 
  a call to APPSPACK::dne. 

  Checks against the constant APPSPACK_DBL_MAX.
*/
bool exists(double value);

}

#endif

