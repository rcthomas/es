// $Id: APPSPACK_Conveyor.cpp,v 1.13 2006/06/30 01:42:08 tgkolda Exp $ 
// $Source: /space/CVS-Acro/acro/packages/appspack/appspack/src/APPSPACK_Conveyor.cpp,v $ 

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
  \file APPSPACK_Conveyor.cpp
  \brief Implements APPSPACK::Conveyor
*/

#include "APPSPACK_Conveyor.hpp"

APPSPACK::Conveyor::Conveyor(Parameter::List& params, 
			     const Vector& scaling_in,
			     Executor::Interface& executor_in) :
			     
  executor(executor_in),
  cache(params, scaling_in),
  doSync(params.getParameter("Synchronous", false)),
  maxQueue(params.getParameter("Max Queue Size", 0)),
  minReturn(params.getParameter("Minimum Exchange Return", 1)),
  maxReturn(params.getParameter("Maximum Exchange Return", max(minReturn, 1000) ))
{
}

APPSPACK::Conveyor::~Conveyor()
{
  queueList.prune();
  pendingList.prune();
}

const APPSPACK::Counter& APPSPACK::Conveyor::getCounter() const
{
  return counter;
}

void APPSPACK::Conveyor::exchange(List& exchangeList)
{
  Point* ptr;
  int tag;
  bool isf;
  Vector f;
  string msg;

  // Add the new points to the queue.  Has the side effect of emptying
  // the exchangeList.
  queueList.insertList(exchangeList);

  while (((queueList.size() > 0) || (pendingList.size() > 0)) && ((doSync) || (exchangeList.size() < minReturn)))
  {
    // Try to submit as many items as possible to the executor
    while ((queueList.isNotEmpty()) && (executor.isWaiting()))
    {
      ptr = queueList.pop();
      if ( cache.isCached( ptr->getX(), f) )
      {
	counter.incrementCached();
	ptr->setCachedFunctionValue(f, counter.getCountString());
	exchangeList.push(ptr);
      }
      else
      {
	executor.spawn( ptr->getX(), ptr->getTag() );
	pendingList.push(ptr);
      }
    }
    
    // Process items coming out of the executor
    int id;
    while (((id = executor.recv(tag,f,msg)) != 0) && ((doSync) || (exchangeList.size() < maxReturn)))
    {
      counter.incrementEvaluated(id, msg);
      ptr = pendingList.pop(tag);
      ptr->setEvaluatedFunctionValue(f, counter.getCountString());
      cache.insert(ptr->getX(),f);
      exchangeList.push(ptr);
    }

  }
}

void APPSPACK::Conveyor::prune()
{
  queueList.prune(maxQueue);
}

void APPSPACK::Conveyor::print() const
{
  executor.print();
}
