// $Id: APPSPACK_Executor_Serial.cpp,v 1.8 2006/06/30 01:42:08 tgkolda Exp $ 
// $Source: /space/CVS-Acro/acro/packages/appspack/appspack/src/APPSPACK_Executor_Serial.cpp,v $ 

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
  \file APPSPACK_Executor_Serial.cpp
  \brief Implemetation of APPSPACK::Executor::Serial
*/

#include "APPSPACK_Executor_Serial.hpp"

APPSPACK::Executor::Serial::Serial(Evaluator::Interface& evaluator_in) :
  evaluator(evaluator_in),
  isFree(true)
{
}

bool APPSPACK::Executor::Serial::isWaiting() const
{
  return isFree;
}

bool APPSPACK::Executor::Serial::spawn(const Vector& x_in, int tag_in)
{
  if (!isFree)
    return false;

  tag = tag_in;
  evaluator(tag, x_in, f, msg);
  isFree = false;

  return true;
}

int APPSPACK::Executor::Serial::recv(int& tag_out, Vector& f_out, string& msg_out)
{
  if (isFree)
    return 0;

  tag_out = tag;
  f_out = f;
  msg_out = msg;
  isFree = true;

  return 1;
}

void APPSPACK::Executor::Serial::print() const
{
  cout << "\n";
  cout << "Using Serial Executor " << endl;
  evaluator.print();
}
