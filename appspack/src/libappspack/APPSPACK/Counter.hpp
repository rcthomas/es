// $Id: APPSPACK_Counter.hpp,v 1.5 2004/11/23 22:26:01 tgkolda Exp $ 
// $Source: /space/CVS-Acro/acro/packages/appspack/appspack/src/APPSPACK_Counter.hpp,v $ 

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
  \file APPSPACK_Counter.hpp
  \brief Class definition for APPSPACK::Counter
*/
#ifndef APPSPACPK_COUNTER_HPP
#define APPSPACPK_COUNTER_HPP

#include "APPSPACK_Common.hpp"

namespace APPSPACK
{

/*! \brief Counts the different types of function evaluations, using
  arbitary codes returned by the user. 

  This object is used by the Conveyor to for counting the number of
  cached points and the number of evaluated points, including on a per
  worker basis. 

  On errors, users can provide a message string. This counter can also
  track how many times each error string is returned, including on a
  per processor basis.
  
  In summary, the counter tracks the following.
  <ul>
  <li> The total number of cached point look-ups.
  <li> The total number of evaluations.
  <li> The total number of each user-defined type of failed evaluation.
  <li> The per-worker numbers of evaluations and each type of failed evaluation.
  </ul>
*/
class Counter
{
public:

  /*! Constructor */
  Counter();

  /*! Destructor */
  ~Counter();


  //! Returns the total number of evaluations
  int getNumEvaluations() const;

  //! Returns a string with the current global counts
  string getCountString() const;


  //! Increment the number of cached function values
  void incrementCached();

  //! Increment the number of evaluations 
  /*!
    \param workerId - The id of the worker that performed the evaluation. 
    \param msg - The user-defined message about the function evaluation.
   */
  void incrementEvaluated(int workerId, const string& msg);

  //! %Print the current counts
  void print() const;

private:

  //! Used the count the number of each type of evaluation
  typedef map<string, int> MsgCnt;

  //! Used to keep per processor counts
  typedef map<int, MsgCnt> WkrMsgCnt;

  //! Iterator for MsgCnt
  typedef MsgCnt::iterator MsgCntIterator;
  //! Iterator for WkrMsgCnt
  typedef WkrMsgCnt::iterator WkrMsgCntIterator;
  //! Const iterator for MsgCnt
  typedef MsgCnt::const_iterator MsgCntConstIterator;
  //! Const iterator for WkrMsgCnt
  typedef WkrMsgCnt::const_iterator WkrMsgCntConstIterator;

  
  //! Increments the number of times that the given msg has appeared in mc.
  void incrementMsgCnt(MsgCnt& mc, const string& msg);

  //! The number of each user-defined type of evaluation
  MsgCnt msgCnt;

  //! The number of each user-defined type of evaluation per worker
  WkrMsgCnt wkrMsgCnt;

  //! The total number of cached function values
  int nCached;

  //! The total number of evaluations
  int nEvaluated;

};

}

#endif
