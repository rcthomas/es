// $Id: APPSPACK_Conveyor.hpp,v 1.13 2005/10/10 16:47:58 jgriffi Exp $ 
// $Source: /space/CVS-Acro/acro/packages/appspack/appspack/src/APPSPACK_Conveyor.hpp,v $ 

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
  \file APPSPACK_Conveyor.hpp
  \brief Class definition for APPSPACK::Conveyor
*/
#ifndef APPSPACPK_CONVEYOR_HPP
#define APPSPACPK_CONVEYOR_HPP

#include "APPSPACK_List.hpp"
#include "APPSPACK_Cache_Manager.hpp"
#include "APPSPACK_Executor_Interface.hpp"
#include "APPSPACK_Parameter_List.hpp"
#include "APPSPACK_Counter.hpp"

namespace APPSPACK
{

//! Conveys trial points through the process of being queued, looked
//! up in the cache, and then evaluated.

/*!  The conveyor's main interface is the exchange() function. It
  takes a (possibly empty) list of unevaluated trial points and
  returns a (non-empty) list of evaluated trial points with function
  values.

  The conveyor owns an APPSPACK::List for the queue of trial points to
  be evaluated; see #queueList.

  The conveyor owns a APPSPACK::Cache::Manager to manage the cache;
  see #cache.

  The conveyor owns a user-provided APPSPACK::Executor object for the
  function evaluations; see #executor. The points that have been passed
  to the executor but have not yet finished are stored an
  APPSPACK::List; see #pendingList.

*/
class Conveyor
{
public:


  //! Constructor 
  Conveyor(Parameter::List& params, const Vector& scaling_in, Executor::Interface& executor_in);


  //! Destructor 
  ~Conveyor();


  /*! 

  \brief Exchange a list of unevaluated trial points for a list of
    evaluated trial points.

  On input, remove all the trial points from the list.
  On output, fill with trial points that have been evaluated.

  The process is as follows.

  <ul>

  <li> Remove all the points (if any) from the input list
       (exchangeList) and insert them into the queue (#queueList).

  <li> Collect points to be returned (in exchangeList). So long as the
       return list is smaller than the minimum return size
       (#minReturn) and there are still points to be evaluated (i.e.,
       either the queue is non-empty and/or the pending list is
       non-empty):

       <ul>
       <li> While the queue is non-empty and the executor is not full:
  
            <ul>
	    <li> Pop the next trial point off the queue.

	    <li> Check to see if that trial point is cached. If it is
	    cached, increment the cached value count, fill in the
	    function value, and add this trial point to the return
	    list.
	    
	    <li> Otherwise, spawn an evaluation of the trial point
	    using the executor and push the trial point onto the
	    pending list.
	    </ul>

       <li> While the executor has a function value to return and the
            exchange list is not full:

	    <ul> 
	    <li> Increment the number of function evaluations.

	    <li> Pop the appropriate point off the pending list and fill in the function value.
	    
	    <li> Insert the new information into the queue for future looks.

	    <li> Add the trial point onto the return list.

	    </ul>
       </ul>
    </ul>
  */
  void exchange(List& exchangeList);

  //! Prune the queue to the size specified by #maxQueue.
  void prune();

  //! Returns counts of all the function evaluations, etc.
  const Counter& getCounter() const;

  //! Calls the print function of the #executor
  void print() const;

private:

  //! Object that is used to evaluate the function
  /*! This object is passed in to the constructor */
  Executor::Interface& executor;

  //! The function value cache
  Cache::Manager cache;

  //! Trial points that are queued to be processed
  List queueList;

  //! Trial points that are in the processed of being evaluated
  List pendingList;

  /*! \brief Be synchronous (set according to "Synchronous") */
  bool doSync;

  //! How big the queue is allowed to be after a prune (set according to "Max Queue Size" parameter)
  int maxQueue;

  /*! \brief The minimum number of items that should be returned on a
    call to exchange() (set according to "Minimum Exchange Return") */
  int minReturn;

  /*! \brief The maximum number of items that should be returned on a
    call to exchange() (set according to "Maximum Exchange Return") */
  int maxReturn;

  //! Counter for the number of evaluations, etc.
  Counter counter;

};

}

#endif
