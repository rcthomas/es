// $Id: APPSPACK_Counter.cpp,v 1.7 2006/06/30 01:42:08 tgkolda Exp $ 
// $Source: /space/CVS-Acro/acro/packages/appspack/appspack/src/APPSPACK_Counter.cpp,v $ 

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
  \file APPSPACK_Counter.cpp
  \brief Implements APPSPACK::Counter
*/

#include "APPSPACK_Counter.hpp"

APPSPACK::Counter::Counter() :
  nCached(0),
  nEvaluated(0)
{
}

APPSPACK::Counter::~Counter()
{
}

int APPSPACK::Counter::getNumEvaluations() const
{
  return nEvaluated;
}

string APPSPACK::Counter::getCountString() const
{
  ostringstream s;
  for (MsgCntConstIterator i = msgCnt.begin(); i != msgCnt.end(); i ++)
    s << " " << (*i).first << ": " << (*i).second;
  return s.str();
}

void APPSPACK::Counter::incrementCached()
{
  nCached++;
}

void APPSPACK::Counter::incrementEvaluated(int workerId, const string& msg)
{
  nEvaluated++;
  incrementMsgCnt(msgCnt, msg);
  incrementMsgCnt(wkrMsgCnt[workerId], msg);
}

// PRIVATE
void APPSPACK::Counter::incrementMsgCnt(MsgCnt& mc, const string& msg)
{
  MsgCntIterator i = mc.find(msg);
  if (i == mc.end())
    mc[msg] = 1;
  else
    (*i).second ++;
}

void APPSPACK::Counter::print() const
{
  cout << "\n";
  cout << "Number of Evaluations       : " << nEvaluated << "\n";
  cout << "Number of Cached Evaluations: " << nCached << "\n";

  cout << "Evaluation Breakdown by Message Type...\n";
  for (MsgCntConstIterator i = msgCnt.begin(); i != msgCnt.end(); i ++)
  {
    cout << "  " << (*i).first << ": " << (*i).second << "\n";
  }

  cout << "Evaluation Breakdown by Processor and Message Type...\n";
  for (WkrMsgCntConstIterator j = wkrMsgCnt.begin(); j != wkrMsgCnt.end(); j++)
  {
    cout << "  Worker #" << (*j).first << "\n";
    for (MsgCntConstIterator i = (*j).second.begin(); i != (*j).second.end(); i ++)
    {
      cout << "    " << (*i).first << ": " << (*i).second << "\n";
    }
  }

}


