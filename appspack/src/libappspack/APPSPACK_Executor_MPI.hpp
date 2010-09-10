// $Id: APPSPACK_Executor_MPI.hpp,v 1.11 2006/06/30 01:42:08 tgkolda Exp $ 
// $Source: /space/CVS-Acro/acro/packages/appspack/appspack/src/APPSPACK_Executor_MPI.hpp,v $ 

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
  \file APPSPACK_Executor_MPI.hpp
  \brief Class definition for APPSPACK::Executor::MPI
*/
#ifndef APPSPACK_EXECUTOR_MPI_HPP
#define APPSPACK_EXECUTOR_MPI_HPP

#include "APPSPACK_Executor_Interface.hpp"

namespace APPSPACK 
{

namespace Executor
{

//! MPI Executor
/*!  MPI implementation of the Executor::Interface. Coordinates the
  workers. This includes keeping track of each worker's status (free
  or busy), sending messages to the workers with the points to be
  evaluated, and receiving messages with completed function
  evaluations from the workers.

  \note Only works if MPI is enabled at configure time; see \ref
  pageConfigureOptions_MPI.
*/
class MPI : public Interface
{
public:

  //! Message codes exchanged with worker
  enum MsgTags {
    Init, 			//!< Initialization
    Feval,			//!< Function Evaluation
    Terminate			//!< Terminate
  };

  //! constructor 
  MPI();

  //! Destructor 
  virtual ~MPI();

  // derived
  virtual bool isWaiting() const;

  // derived
  virtual bool spawn(const Vector& x_in, int tag_in);

  // derived
  virtual int recv(int& tag_out, Vector& f_out, string& msg_out);

  // Prints out how many workers are being used.
  virtual void print() const;

private:

  //! Tag of current point, if any
  /*! A tag of -1 indicates that the worker is free. */
  vector<int> workertag;

};

}

}

#endif
