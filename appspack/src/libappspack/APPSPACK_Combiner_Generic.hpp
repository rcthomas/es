// $Id: APPSPACK_Combiner_Generic.hpp,v 1.1 2006/06/30 01:42:08 tgkolda Exp $ 
// $Source: /space/CVS-Acro/acro/packages/appspack/appspack/src/APPSPACK_Combiner_Generic.hpp,v $ 

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
  \file APPSPACK_Combiner_Generic.hpp
  \brief Class description for APPSPACK::Combiner::Generic
*/

#ifndef APPSPACK_COMBINER_GENERIC
#define APPSPACK_COMBINER_GENERIC

#include "APPSPACK_Common.hpp"
#include "APPSPACK_Vector.hpp"

namespace APPSPACK
{

/*! \brief Namespace for combiner classes, i.e., those classes that
  are used to combine a vector of values into a single function
  value. */
namespace Combiner 
{

/*!  

\brief Abstract generic class for combining a vector of values
into a single function value.

The purpose of the combiner is to combine multiple function values
into a single function value. This generic combiner assumes that the
first value (if any) is the objective value and all remaining values
are constraints that need to be nonnegative.
In other words, suppose we have an input vector \f$f\f$ of length \f$n\f$.
Then, we compute a single function value, \f$\hat f\f$, such that

\f[
\hat f = 
\begin{cases}
+\infty & \mbox{if } n = 0 \\
f_1 & \mbox{if } n = 1\\
f_1 & \mbox{if } f_2,f_3,\dots,f_n \geq 0\\
+\infty & \mbox{otherwise}.
\end{cases}
\f]

The user may derive a new combiner that does the combining in some
alternative fashion.

*/
class Generic
{
public:
  
  //! Constructor 
  Generic() {};

  //! Destructor 
  virtual ~Generic() {};
  
  //! Returns the objective value calculated from the vector of function values.
  /*! 

    \param f input vector (may be empty, in which case return value
    should be \f$+\infty\f$)

    \return Combined function value. Be sure to return
    APPSPACK::dne() for \f$f=+\infty\f$. 

  */
  virtual double operator()(const Vector& f) const;

};

}

}

#endif
