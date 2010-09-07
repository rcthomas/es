// $Id: APPSPACK_Point.cpp,v 1.13 2006/06/30 01:42:08 tgkolda Exp $ 
// $Source: /space/CVS-Acro/acro/packages/appspack/appspack/src/APPSPACK_Point.cpp,v $ 

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
  \file APPSPACK_Point.cpp
  \brief Implementation of APPSPACK::Point
*/

#include "APPSPACK_Point.hpp"
#include "APPSPACK_Float.hpp"
#include "APPSPACK_Print.hpp"

int APPSPACK::Point::staticCount = 0;

APPSPACK::Point::Point(const Vector& x_in, const Vector& f_in, 
		       double step_in, double alpha_in,
		       Combiner::Generic& combiner_in) :
  combiner(combiner_in),
  x(x_in),
  f(f_in),
  tag(staticCount),
  step(step_in),
  msg("(Initial Point)"),
  parentTag(-1),
  idx(-1),
  parentF(APPSPACK::dne()),
  alpha(alpha_in),
  rho(0),
  state(Evaluated)
{
  staticCount ++;
}

APPSPACK::Point::Point(const Vector& x_in, double step_in,
		       const Point& parent, int idx_in) :
  combiner(parent.combiner),
  x(x_in),
  f(),
  tag(staticCount),
  step(step_in),
  msg(),
  parentTag(parent.tag),
  idx(idx_in),
  parentF(combiner(parent.f)),
  alpha(parent.alpha),
  rho(alpha * step * step),
  state(Unevaluated)
{
  staticCount ++;
}

APPSPACK::Point::~Point()
{
}

const APPSPACK::Vector& APPSPACK::Point::getX() const
{
  return x;
}

double APPSPACK::Point::getF() const
{
  return combiner(f);
}

const APPSPACK::Vector& APPSPACK::Point::getVecF() const
{
  return f;
}

int APPSPACK::Point::getTag() const
{
  return tag;
}

int APPSPACK::Point::getParentTag() const
{
  return parentTag;
}

int APPSPACK::Point::getIndex() const
{
  return idx;
}

double APPSPACK::Point::getStep() const
{
  return step;
}

void APPSPACK::Point::setCachedFunctionValue(const Vector& f_in, const string& msg_in)
{
  f = f_in;
  msg = msg_in;
  state = (isSufficientDecrease()) ? Cached : CachedInsufficientDecrease;
}

void APPSPACK::Point::setEvaluatedFunctionValue(const Vector& f_in, const string& msg_in)
{
  f = f_in;
  msg = msg_in;
  state = (isSufficientDecrease()) ? Evaluated : EvaluatedInsufficientDecrease;
}

void APPSPACK::Point::setInfeasible()
{
  state = Infeasible;
}

bool APPSPACK::Point::operator<(const Point& other) const
{
  // Case I: Equal tags
  if (tag == other.tag)
    return false;

  // Case II: This point does not have a valid function value
  if ((state != Cached) && (state != Evaluated))
    return false;

  // Case III: The other point does not have a valid function value
  if ((other.state != Cached) && (other.state != Evaluated))
    return true;

  // Case IV: f(x) < f(y)
  if (lessThan(other.f))		
    return true;

  // Case III: f(x) > f(y)
  if (greaterThan(other.f))
    return false;

  // Case IV: f(x) = f(y)
  return (tag < other.tag);
}

bool APPSPACK::Point::lessThan(const Vector& other) const
{
  double fx = combiner(f);
  double fy = combiner(other);

  // Case I: f(x) = infty
  if (!exists(fx))
    return false;
  
  // Case II: f(y) = infty and f(x) is finite
  if (!exists(fy))
    return true;
  
  // Case III: f(x) and f(y) are finite
  return (fx < fy);
}

bool APPSPACK::Point::greaterThan(const Vector& other) const
{
  double fx = combiner(f);
  double fy = combiner(other);

  // Case I: f(y) = infty
  if (!exists(fy))
    return false;
  
  // Case II: f(x) = infty and f(y) is finite
  if (!exists(fx))
    return true;
  
  // Case III: f(x) and f(y) are finite
  return (fx > fy);
}

// PRIVATE
bool APPSPACK::Point::isSufficientDecrease() const
{
  // No sufficient decrease criteria imposed
  if (rho <= 0)
    return true;

  double fx = combiner(f);

  // Case I: f(x) = infty
  if (!exists(fx))
    return false;
  
  // Case II: f(x) is finite and f(y) = infty
  if (!exists(parentF))
    return true;
  
  // Case III: f(x) and f(y) are finite
  return (fx < (parentF - rho));
}


ostream& APPSPACK::Point::leftshift(ostream& stream) const
{
  // Print point to the given stream. 

  stream << "f=" << APPSPACK::Print::formatPositiveDouble(combiner(f));
  stream << " x=" << x;
  stream << " step=" << APPSPACK::Print::formatPositiveDouble(step);
  stream << " tag=" << tag;
  stream << " state=" << state;
  stream << " " << msg;
  return stream;
}

ostream& operator<<(ostream& stream, const APPSPACK::Point& point)
{
  return point.leftshift(stream);
}

ostream& operator<<(ostream& stream, APPSPACK::Point::State state)
{
  switch(state)
  {
  case APPSPACK::Point::Unevaluated:
    stream << "Unevaluated";
    break; 
  case APPSPACK::Point::Infeasible:
    stream << "Infeasible";
    break;
  case APPSPACK::Point::CachedInsufficientDecrease:
    stream << "CachedInsufficientDecrease";
    break;
  case APPSPACK::Point::Cached:
    stream << "Cached";
    break;
  case APPSPACK::Point::EvaluatedInsufficientDecrease:
    stream << "EvaluatedInsufficientDecrease";
    break;
  case APPSPACK::Point::Evaluated:
    stream << "Evaluated";
    break;
  }
  
  return stream;
}
