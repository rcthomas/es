// $Id: APPSPACK_Executor_Interface.hpp,v 1.11 2006/06/30 01:42:08 tgkolda Exp $ 
// $Source: /space/CVS-Acro/acro/packages/appspack/appspack/src/APPSPACK_Executor_Interface.hpp,v $ 

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
  \file APPSPACK_Executor_Interface.hpp
  \brief Class description for APPSPACK::Executor::Interface
*/
#ifndef APPSPACK_EXECUTOR_INTERFACE_HPP
#define APPSPACK_EXECUTOR_INTERFACE_HPP

#include "APPSPACK_Vector.hpp"

namespace APPSPACK 
{

//! Namespace for objects that coordinate the execution for function evaluations
namespace Executor
{

//! Generic interface for an object that coordinates the execution of function evaluations
class Interface
{
public:

  //! Constructor 
  Interface() {};

  //! Destructor 
  virtual ~Interface() {};

  //! Return true if the executor is waiting for more points to be spawned
  virtual bool isWaiting() const = 0;

  //! Start a function evaluation for the given point with the given integer tag.
  /*! This function should not be called without checking isNotFull()
    first. However, if it is called but the executor is already full,
    this method will return false. */
  virtual bool spawn(const Vector& x_in, int tag_in) = 0;

  /*!  
    \brief Check to see if any function evaluations have finished and
    return the relevant information
  
    If a function evaluation has completed, fill in the references
    that are passed to the function as follows.

    \param tag_out - The tag corresponding to the x-vector that was evaluated
    \param f_out - Vector of function values. An empty vector means
    that there is no value. Or use dne() as a vector entry to say that
    a particular value does not exist.
    \param msg_out - Message about function evaluation

    \retval Returns 0 (zero) if no function value is
    received. Otherwise, returns the worker id (any \e positive
    integer).
   */
  virtual int recv(int& tag_out, Vector& f_out, string& msg_out) = 0;

  //! Optional print function to print out information about the executor.
  virtual void print() const {};

};

}

}

#endif
