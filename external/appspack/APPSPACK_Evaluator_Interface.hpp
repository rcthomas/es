// $Id: APPSPACK_Evaluator_Interface.hpp,v 1.11 2006/06/30 01:42:08 tgkolda Exp $ 
// $Source: /space/CVS-Acro/acro/packages/appspack/appspack/src/APPSPACK_Evaluator_Interface.hpp,v $ 

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
  \file APPSPACK_Evaluator_Interface.hpp
  \brief Class description for APPSPACK::Evaluator::Interface
*/

#ifndef APPSPACK_EVALUATOR_INTERFACE
#define APPSPACK_EVALUATOR_INTERFACE

#include "APPSPACK_Common.hpp"
#include "APPSPACK_Vector.hpp"

namespace APPSPACK
{

/*! \brief Namespace for objects that do (serial) function
  evaluations */
namespace Evaluator 
{

//! Abstract interface for a single function evaluation
/*!

Abstract interface for the object that handles the evaluation of a
single function evaluation. The SystemCall implementation is used by
the default executables that are provided with APPSPACK. 

See also \ref pageCustomize_evaluator.

Note that the Evaluator handles only a single function evaluation, and
the Executor coordinates the parallel execution of the function
evaluations.

*/

class Interface
{
public:
  
  //! Constructor 
  Interface() {};

  //! Destructor 
  virtual ~Interface() {};
  

  //! Do the function evaluation for the point x_in, and fill int he results.
  /*!
    \param tag_in - Contains a unique tag which can be used to name files, etc.
    \param x_in - The point to be evaluated
    \param f_out - On output, this should contain the function
    value. An empty vector indicates that the function value did not
    exist. Or, set individual entries of the function to
    APPSPACK::dne();
    \param msg_out - On output, this should contain a message about the function evaluation
   */
  virtual void operator()(int tag_in, const Vector& x_in, Vector& f_out, string& msg_out) = 0;

  //! Optional - prints information about the evaluator object
  virtual void print() const {};

};

}

}

#endif
