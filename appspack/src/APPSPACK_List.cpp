// $Id: APPSPACK_List.cpp,v 1.8 2004/04/12 17:43:55 tgkolda Exp $ 
// $Source: /space/CVS-Acro/acro/packages/appspack/appspack/src/APPSPACK_List.cpp,v $ 

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
  \file APPSPACK_List.cpp
  \brief Implementation of APPSPACK::List
*/

#include "APPSPACK_List.hpp"
#include "APPSPACK_Print.hpp"

APPSPACK::List::List() :
  isValidBest(false)
{

}

APPSPACK::List::~List()
{
  prune();
}

bool APPSPACK::List::isEmpty() const
{
  return tpl.empty();
}

bool APPSPACK::List::isNotEmpty() const
{
  return !(tpl.empty());
}

int APPSPACK::List::size() const
{
  return tpl.size();
}

void APPSPACK::List::push(Point* pushed)
{
  isValidBest = false;
  tpl.push_front(pushed);
}

void APPSPACK::List::insertList(List& source)
{
  isValidBest = false;
  tpl.insert(tpl.begin(), source.tpl.begin(), source.tpl.end());
  source.tpl.clear();
}

void APPSPACK::List::prune(int n)
{
  if (n <= 0)
  {
    for (TPL::iterator tpi = tpl.begin(); tpi != tpl.end(); tpi ++)
      delete *tpi;
    
    tpl.clear();
  }
  else
  {
    int p = size() - n;
    Point* popped;
    for (int i = 0; i < p; i ++)
    {
      popped = pop();
      delete popped;
    }
  }
}

APPSPACK::Point* APPSPACK::List::pop()
{
  if (tpl.empty())
    return NULL;

  isValidBest = false;
  Point* popped = tpl.back();
  tpl.pop_back();
  return popped;
}

APPSPACK::Point* APPSPACK::List::pop(int tag)
{
  if (tpl.empty())
    return NULL;

  TPL::iterator tpi = tpl.begin();
  for (; tpi != tpl.end(); tpi ++)
    if (((*tpi)->getTag()) == tag)
      break;

  if (tpi == tpl.end())
    return NULL;

  isValidBest = false;
  Point* popped = *tpi;
  tpl.erase(tpi);

  return popped;
}

void APPSPACK::List::moveBestToEndOfList() 
{
  if (isValidBest) 
    return;

  if (tpl.empty())
  {
    cerr << "APPSPACK::List::moveBestToEndOfList - "
	 << "Trying to find the best in an empty list!" << endl;
    throw "APPSPACK Error";
  }

  if (tpl.size() == 1)
    return;

  // Find the index of the best point
  TPL::iterator bestIterator = tpl.begin();
  TPL::iterator tpi = tpl.begin();

  for (tpi ++; tpi != tpl.end(); tpi ++)
    if (**tpi < **bestIterator)
      bestIterator = tpi;

  // Swap the best point to the end of the list
  Point* tmp = *bestIterator;
  *bestIterator = tpl.back();
  tpl.back() = tmp;

  isValidBest = true;
}

const APPSPACK::Point& APPSPACK::List::best() 
{
  moveBestToEndOfList();
  return *tpl.back();
}

APPSPACK::Point* APPSPACK::List::popBest()
{
  moveBestToEndOfList();
  return pop();
}

void APPSPACK::List::print(const string label) const
{
  cout << "\n" << label << ":\n";

  if (tpl.empty())
  {
    cout << "<empty>" << endl;
    return;
  }

  for (TPL::const_reverse_iterator tpi = tpl.rbegin(); tpi != tpl.rend(); tpi ++)
    cout << *(*tpi) << endl;
}
