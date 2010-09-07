// $Id: APPSPACK_Constraints_Linear.cpp,v 1.61.2.1 2007/01/14 21:51:27 jgriffi Exp $ 
// $Source: /space/CVS-Acro/acro/packages/appspack/appspack/src/APPSPACK_Constraints_Linear.cpp,v $ 

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
  \file APPSPACK_Constraints_Linear.cpp
  \brief Implementation of non-virtual functions in APPSPACK::Constraints::Linear
*/

#include "APPSPACK_Vector.hpp"
#include "APPSPACK_Constraints_Linear.hpp"
#include "APPSPACK_Print.hpp"
#include "APPSPACK_Utils.hpp"
#include "APPSPACK_Common.hpp"
#include "APPSPACK_Float.hpp"
#include "APPSPACK_LAPACK_Wrappers.hpp"
#include "APPSPACK_CDDLIB_Interface.h"

APPSPACK::Constraints::Linear::Linear(Parameter::List& params) :
  epsMach(params.getParameter("Machine Epsilon", 1.0e-12)),
  displayMatrix(params.getParameter("Display Linear Systems", true))
{
  setup(params);
  errorCheck();
}

APPSPACK::Constraints::Linear::~Linear()
{
}

void APPSPACK::Constraints::Linear::setup(Parameter::List& params)
{  
  setupBounds(params);
  setupScaling(params);
  setupMatrix(params); 
  setupRhs(params);
  setupScaledSystem();
}

void APPSPACK::Constraints::Linear::setupBounds(APPSPACK::Parameter::List& params)
{ 

  vector<bool> isLower;
  vector<bool> isUpper;

  if (params.isParameterVector("Scaling"))
    scaling = params.getVectorParameter("Scaling");
  
  if (params.isParameterVector("Upper"))
    bUpper = params.getVectorParameter("Upper");
  
  if (params.isParameterVector("Lower"))
    bLower = params.getVectorParameter("Lower");
  
  if (params.isParameterVector("Is Lower"))
    params.getVectorParameter("Is Lower").copyToBool(isLower);
  
  if (params.isParameterVector("Is Upper"))
    params.getVectorParameter("Is Upper").copyToBool(isUpper);
  
  // Determine number of variables
  int nvar = max(scaling.size(), bLower.size());
  if (nvar == 0)
    error("setup", "Neither \"Scaling\" nor \"Lower\" is defined in the parameter list");

  // Fill lower (if necessary)
  if (bLower.empty())
  {
    bLower.assign(nvar, dne());
    params.getParameter("Lower", bLower); 
  }

  // Use isLower if it's defined (for backward compatibility)
  if (!isLower.empty())
  {
    if (isLower.size() != nvar)
      error("setupBounds", "\"Is Lower\" is the wrong size");

    for (int i = 0; i < nvar; i ++)
      if (isLower[i])
      {
	if (!exists(bLower[i]))
	  error("setupBounds", "\"Is Lower\" has a true (nonzero) entry but \"Lower\" is not defined");    
      }
      else
      {
	bLower[i] = dne();
      }
  }

  // Fill upper 
  if (bUpper.empty())
  {
    bUpper.assign(nvar, dne());
    params.getParameter("Upper", bUpper);
  }

  // Use isUpper if it's defined (for backward compatibility)
  if (!isUpper.empty())
  {
    if (isUpper.size() != nvar)
      error("setupBounds", "\"Is Upper\" is the wrong size");

    for (int i = 0; i < nvar; i ++)
      if (isUpper[i])
      {
	if (!exists(bUpper[i]))
	  error("setupBounds", "\"Is Upper\" has a true (nonzero) entry but \"Upper\" is not defined");    
      }
      else
      {
	bUpper[i] = dne();
      }
  }

  // Error check
  if (bLower.size() != nvar)
    error("setupBounds", "\"Lower\" vector has incorrect length");
  if (bUpper.size() != nvar)
    error("setupBounds", "\"Upper\" vector has incorrect length");
}

void APPSPACK::Constraints::Linear::setupScaling(APPSPACK::Parameter::List& params)
{
  // Fix the scaling (if necessary)
  if (scaling.empty())
  {  
    // Check that all the bounds are specified
    for (int i = 0; i < bLower.size(); i++)
      if ((!exists(bLower[i])) || (!exists(bUpper[i])))
	error("setup", "\"Scaling\" must be specified if upper and lower bounds are not fully specified");
    
    for (int i = 0; i < bLower.size(); i++)
      scaling.push_back(bUpper[i] - bLower[i]);
    
    params.getParameter("Scaling", scaling);
  }
  
  // Error check
  if (scaling.size() != bLower.size())
   error("setupScaling", "\"Scaling\" vector has incorrect length");
}

void APPSPACK::Constraints::Linear::setupMatrix(APPSPACK::Parameter::List& params)
{
  // Get inequality matrix
  if (params.isParameterMatrix("Inequality Matrix"))
  {
    aIneq = params.getMatrixParameter("Inequality Matrix");

    // Error check
    if ((!aIneq.empty()) && (aIneq.getNcols() != scaling.size()))
      error("setupMatrix", "\"Inequality Matrix\" matrix has incorrect size.");
  }

  // Get equality matrix
  if (params.isParameterMatrix("Equality Matrix"))
  {
    aEq = params.getMatrixParameter("Equality Matrix");

    // Error check
    if ((!aEq.empty()) && (aEq.getNcols() != scaling.size()))
	error("setupMatrix", "\"Equality Matrix\" matrix has incorrect size.");
  }
}

void APPSPACK::Constraints::Linear::setupRhs(APPSPACK::Parameter::List& params)
{  
  // Get lower bounds on linear inequality constraints
  if (params.isParameterVector("Inequality Lower"))
    bIneqLower = params.getVectorParameter("Inequality Lower");
  else
    bIneqLower.assign( aIneq.getNrows(), dne());

  // Get upper bounds on linear inequality constraints
  if (params.isParameterVector("Inequality Upper"))
    bIneqUpper = params.getVectorParameter("Inequality Upper");
  else
    bIneqUpper.assign(aIneq.getNrows(), dne());

  // Get upper bounds on linear equality constraints
  if (params.isParameterVector("Equality Bound"))
    bEq = params.getVectorParameter("Equality Bound");

  // Error check
  if (bIneqLower.size() != aIneq.getNrows())
    error("setupRhs", "\"Inequality Lower\" vector has incorrect length");

  // Error check
  if (bIneqUpper.size() != aIneq.getNrows())
    error("setupRhs", "\"Inequality Upper\" vector has incorrect length");

  // Error check
  if (bEq.size() != aEq.getNrows())
    error("setupRhs", "\"Equality Bound\" vector has incorrect length");
}

void APPSPACK::Constraints::Linear::setupScaledSystem()
{  
  // *** Form lHat ***
  for (int j = 0; j < scaling.size(); j++)
  {
    if (exists(bLower[j]))
      lHat.push_back(bLower[j]);
    else
      lHat.push_back(0);
  }

  // *** Form bHatLower and bHatUpper ***

  // Insert components corresponding to lower bounds
  for (int j = 0; j < scaling.size() ; j++)
  {
    if (exists(bLower[j]))
      bHatLower.push_back((bLower[j] - lHat[j]) / scaling[j]);
    else
      bHatLower.push_back(dne());
  }

  // Insert components corresponding to upper bounds
  for (int j = 0; j < scaling.size(); j++)
  {
    if (exists(bUpper[j]))
      bHatUpper.push_back((bUpper[j] - lHat[j]) / scaling[j]);
    else
      bHatUpper.push_back(dne());
  }

  // Insert component corresponding to general linear constraints
  if (!aIneq.empty())
  {
    // Compute ail = aIneq * lhat
    Vector ail(aIneq.getNrows());
    aIneq.multVec(lHat, ail);

    
    for (int j = 0; j < aIneq.getNrows(); j++)
    {
      // Form scaled left-hand side.
      if (exists(bIneqLower[j]))
	bHatLower.push_back(bIneqLower[j] - ail[j]);
      else
	bHatLower.push_back(dne());

      // Form scaled right-hand side.
      if (exists(bIneqUpper[j]))
	bHatUpper.push_back(bIneqUpper[j] - ail[j]);
      else
	bHatUpper.push_back(dne());
    }
  }
  
  // *** Form aHat ***

  // First add identity
  aHat.setToIdentity(scaling.size());
  // Now add in aTildeIneq
  aHat.addMatrix(aIneq, scaling);
  
  // *** Form aTildeEq and bTildeEq ***
  // Nullspace matrix, Z, of aTildeEq is needed to compute distance to
  // inequality constraints within nullspace.
  Matrix ZT;
  if (!aEq.empty())
  {
    // ael = aEq * lhat
    Vector ael(aEq.getNrows());
    aEq.multVec(lHat,ael);

    for (int i = 0; i < aEq.getNrows(); i++)
      bTildeEq.push_back(bEq[i] - ael[i]);

    aTildeEq.scale(aEq, scaling);
    aTildeEq.nullSpace(ZT, epsMach);
  }

  // Error check.
  if (bHatLower.size() != aIneq.getNrows() + scaling.size())
    error("setupScaledSystem", "\"bHatUpper\" vector has incorrect length");
  // Error check.
  if (bHatUpper.size() != aIneq.getNrows() + scaling.size())
    error("setupScaledSystem", "\"bHatUpper\" vector has incorrect length");
  // Error check.
  if ((aHat.getNrows() != aIneq.getNrows() + scaling.size()) || (aHat.getNcols() != scaling.size()))
    error("setupScaledSystem", "\"aHat\" matrix has incorrect size");

  // Store norms of rows for determining distances.
  aHatZNorm.resize(aHat.getNrows());
  aHatNorm.resize(aHat.getNrows());

  // Project matrix into nullspace of scaled equality constraints.
  Matrix AZ = aHat;
  if (!ZT.empty())
    AZ.multMat(ZT, Matrix::Transpose);
  for (int i = 0; i < aHat.getNrows(); i++)
  {
    aHatZNorm[i] = AZ.getRow(i).norm();
    aHatNorm[i] = aHat.getRow(i).norm();
  }
}

void APPSPACK::Constraints::Linear::errorCheck() const
{
  int nvar = scaling.size();

  // Positive scaling values
  for (int i = 0; i < nvar; i ++)
    if (scaling[i] <= 0)
      error("errorCheck", "Negative \"Scaling\" value.");

  // Consistency of bounds
  for (int i = 0; i < nvar; i ++)
    if ((exists(bLower[i])) && (exists(bUpper[i])) && (bLower[i] > bUpper[i]))
      error("errorCheck", "Upper bound is less than lower bound.");

  // Consistency of constraints
  for (int i = 0; i < aIneq.getNrows(); i ++)
    if ((exists(bIneqLower[i])) && (exists(bIneqUpper[i])) && (bIneqLower[i] > bIneqUpper[i]))
      error("errorCheck", "Upper bound is less than lower bound for linear constraint.");

#if !defined(HAVE_LAPACK) && !defined(HAVE_CDDLIB)
    // Error if trying to use general linear constraints without 
    // at least one of LAPACK or CDDLIB
    if (!(aEq.empty() && aIneq.empty()))
    {
      string errorMsg = "General linear constraint capability currently disabled.\n";
      errorMsg += "\nPlease reconfigure with LAPACK and/or CDDLIB.\n";
      error("errorCheck", errorMsg);
    }
#endif
}

double APPSPACK::Constraints::Linear::getEpsMach() const
{
  return epsMach;
}

const APPSPACK::Vector& APPSPACK::Constraints::Linear::getScaling() const
{
  return scaling;
}

const APPSPACK::Vector& APPSPACK::Constraints::Linear::getLower() const
{
  return bLower;
}

const APPSPACK::Vector& APPSPACK::Constraints::Linear::getUpper() const
{
  return bUpper;
}

const APPSPACK::Vector& APPSPACK::Constraints::Linear::getBhatLower() const
{
  return bHatLower;
}

const APPSPACK::Vector& APPSPACK::Constraints::Linear::getBhatUpper() const
{
  return bHatUpper;
}

const APPSPACK::Vector& APPSPACK::Constraints::Linear::getBtildeEq() const
{
  return bTildeEq;
}

const APPSPACK::Matrix& APPSPACK::Constraints::Linear::getAhat() const
{
  return aHat;
}

const APPSPACK::Matrix& APPSPACK::Constraints::Linear::getAtildeEq() const
{
  return aTildeEq;
}

void APPSPACK::Constraints::Linear::getNominalX(APPSPACK::Vector& x) const
{
  x.resize(scaling.size());
  for (int i = 0; i < scaling.size(); i++)
  {
    if (exists(bUpper[i]) && exists(bLower[i]))
      x[i] = (bUpper[i] + bLower[i]) / 2.0 ;
    else if (exists(bUpper[i]))
      x[i] = bUpper[i];
    else if (exists(bLower[i]))
      x[i] = bLower[i];
    else
      x[i] = 0;
  }
}

void APPSPACK::Constraints::Linear::scale(APPSPACK::Vector& x) const
{ 
  if (x.size() != scaling.size())
    error("scale", "x vector has incorrect length");

  for (int i = 0; i < scaling.size(); i++)
    x[i] = (x[i] - lHat[i]) / scaling[i];
}

void APPSPACK::Constraints::Linear::unscale(APPSPACK::Vector& w) const
{ 
  if (w.size() != scaling.size())
    error("scale", "w vector has incorrect length");

  for (int i = 0; i < scaling.size(); i++)
    w[i] = (scaling[i] * w[i]) + lHat[i];
}

bool APPSPACK::Constraints::Linear::isFeasible(const APPSPACK::Vector& x) const
{  
  int n = scaling.size();

  // Error check
  if (x.size() != n)
    error("isFeasible", "x vector has incorrect length");

  // Create scaled x
  Vector xTilde(x);
  scale(xTilde);

  if (!isInequalityFeasible(xTilde))
    return false;

  if (!isEqualityFeasible(xTilde))
    return false;
  
  return true;
}

void APPSPACK::Constraints::Linear::assertFeasible(const APPSPACK::Vector& x) const
{  
  if (!isFeasible(x))
    error("assertFeasible","Infeasible point");
}

bool APPSPACK::Constraints::Linear::isBoundsOnly() const
{  
  return ((aIneq.empty()) && (aEq.empty()));
}

double  APPSPACK::Constraints::Linear::maxStep(const APPSPACK::Vector& x,
					       const APPSPACK::Vector& d,
					       double step) const
{
  double maxStep = step;
  int n = scaling.size();

  Vector xTilde(x);
  scale(xTilde);

  // Determine max step for bounds component

  for (int i = 0; i < scaling.size(); i++)
  {
    if (d[i] < -scaling[i] * epsMach) // Points to lower bound
      switch (getIneqState(i, LowerBound, xTilde))
      {
      case Active:	     // Can't move if the constraint is active
	return 0;
      case Inactive:		
	maxStep = min(maxStep, (bLower[i] - x[i]) / d[i]);
      }
    else if (d[i] > scaling[i] * epsMach) // Points to upper bound
      switch (getIneqState(i, UpperBound, xTilde))
      {
      case Active:	     // Can't move if the constraint is active
	return 0;
      case Inactive:
	maxStep = min(maxStep, (bUpper[i] - x[i]) / d[i]);
      }
  }

  // Determine max step for inequality component.

  if (!aIneq.empty())
  {
    int p = aIneq.getNrows();

    // aix = aIneq * x
    Vector aix(p);
    aIneq.multVec(x, aix);

    // aid = aIneq * d
    Vector aid(p);
    aIneq.multVec(d, aid);

    for (int j = 0; j < p; j++)
    {
      if (aid[j] < -epsMach) // Points to lower bound
	switch (getIneqState(n + j, LowerBound, xTilde))
	{
	case Active:	     // Can't move if the constraint is active
	  return 0;
	case Inactive:
	  maxStep = min(maxStep, (bIneqLower[j] - aix[j]) / aid[j]);
	}
      else if (aid[j] > epsMach)  // Points to upper bound
	switch (getIneqState(n + j, UpperBound, xTilde))
	{
	case Active:	     // Can't move if the constraint is active
	  return 0;
	case Inactive:
	  maxStep = min(maxStep, (bIneqUpper[j] - aix[j]) / aid[j]);
	}
    }
  }

  // Process equality constraints.

  if (!aEq.empty())
  {
    int p = aEq.getNrows();

    // z = aEq * d
    Vector z(p);
    aEq.multVec(d,z);

    // Check that direction stays on hyperplane defined by the
    // equality constraints
    for (int i = 0; i < p; i++)
      if (fabs(z[i]) > epsMach)
	return 0;
  }

  return maxStep;
}

void APPSPACK::Constraints::Linear::getActiveIndex(const APPSPACK::Vector& xdist, 
						   double epsilon,
						   vector<ActiveType>& index) const

{
  int nrows = aHat.getNrows();
  // Size the index
  index.resize(nrows);

  // Determine which bounds are near.
  for (int i = 0; i < nrows; i++)
  {
    // Check if upper or lower bounds are near.
    bool nearlow = (xdist[i] < epsilon);
    bool nearupp = (xdist[i+nrows] < epsilon);
    
    // Update index.
    if (nearlow && nearupp)
      index[i] = BothActive;
    else if (nearlow)		// and not nearupp
      index[i] = LowerActive;
    else if (nearupp) 		// and not nearlow
      index[i] = UpperActive;
    else
      index[i] = NeitherActive;
  }
}

void APPSPACK::Constraints::Linear::formDistanceVector(const Vector& x, Vector& xdist) const
{
  // Transform x to scaled space..
  Vector xTilde = x;
  scale(xTilde);
  
  int nrows = aHat.getNrows();

  // z = aHat*xTilde.
  Vector z(nrows);
  aHat.multVec(xTilde, z);
  
  // xdist must be twice the size of z to hold upper and lower bound info.
  xdist.resize(2*nrows);
  for (int i = 0; i < nrows; i++)
  {
    // Process lower bound.
    if (exists(bHatLower[i]))
    {
      if (aHatZNorm[i] > epsMach)
	xdist[i] = fabs( z[i]  - bHatLower[i] ) / aHatZNorm[i];
      else if (fabs( z[i] - bHatLower[i] ) < epsMach)
	xdist[i] = 0;
      else
	xdist[i] = dne();
    }
    else
      xdist[i] = dne();
    
    // Process upper bound.
    if (exists(bHatUpper[i]))
    {
      if (aHatZNorm[i] > epsMach)	
	xdist[i+nrows] = fabs( z[i]  - bHatUpper[i] ) / aHatZNorm[i];
      else if (fabs( z[i] - bHatUpper[i] ) < epsMach)
	xdist[i+nrows] = 0;
      else
	xdist[i+nrows] = dne();
    }
    else
      xdist[i+nrows] = dne();
  }
}

void APPSPACK::Constraints::Linear::snapToBoundary(APPSPACK::Vector& x, 
						   double esnap) const
{
  // Form scaled x.
  Vector xTilde = x;
  scale(xTilde);
  // Form snap-to system.
  Matrix Asnap;
  Vector bsnap;
  formSnapSystem(xTilde, esnap, Asnap, bsnap);
  // Find closest point satisfying snap constraints.
  Asnap.constrainedLSQR(xTilde,bsnap);
  unscale(xTilde);
  // Update x.
  x = xTilde;
}

void APPSPACK::Constraints::Linear::makeBoundsFeasible(APPSPACK::Vector& x) const
{
  for (int i = 0; i < x.size(); i++)
  {
    if (exists(bLower[i]) && x[i] < bLower[i])
      x[i] = bLower[i];
    if (exists(bUpper[i]) && x[i] > bUpper[i])
      x[i] = bUpper[i];
  }
}

void APPSPACK::Constraints::Linear::formSnapSystem(const APPSPACK::Vector& xTilde, double esnap,
						   APPSPACK::Matrix& Asnap,
						   APPSPACK::Vector& bsnap) const
{
  Asnap.clear();
  bsnap.resize(0);
  
  if (aHat.empty())
  {
    if (!isEqualityFeasible(xTilde))
    {
      Asnap = aTildeEq;
      bsnap = bTildeEq;
    }
    return;
  }

  int nrow = aHat.getNrows();
  int nvar = aHat.getNcols();
  // Form z = Ahat*xTilde;
  Vector z(nrow);
  aHat.multVec(xTilde, z);
  
  // Measure and sort distance in terms of scaled space.
  multimap<double,int> dmap;
  multimap<double,int>::iterator iter;
  for (int i = 0; i < nrow; i++)
  { 
    if (exists(bHatLower[i]))
    { 
      double distlow = fabs( z[i] - bHatLower[i] ) / aHatNorm[i];
     dmap.insert(std::pair<const double, int>(distlow, i));
    }
    if (exists(bHatUpper[i]))
    { // Indices for upper bounds in dmap denoted by i+nrow.  Symbolically stacking matrix.
      double distupp = fabs( z[i] - bHatUpper[i] ) / aHatNorm[i];
      dmap.insert(std::pair<const double, int>(distupp, i+nrow));
    }
  }

  // Erase constraints which are outside the epsilon ball.
  dmap.erase(dmap.upper_bound(esnap),dmap.end());

  if ((dmap.lower_bound(epsMach) == dmap.end()) && isEqualityFeasible(xTilde))
    return; // There are no constraints within distance esnap which are inactive.

  // Equality constraints are always added.
  Asnap = aTildeEq;
  bsnap = bTildeEq;

  // Now add inequality constraints within distance esnap.
  // Recall that we have symbolically stacked aHat, so we must mod out by nrow.
  for (iter=dmap.begin(); iter != dmap.end(); iter++)
  {
    if (bsnap.size() >= nvar)
      break;
    int irow = iter->second;
    if (irow < nrow)
    { // Snap to lower bound.
      Asnap.addRow(aHat.getRow(irow));
      bsnap.push_back(bHatLower[irow]);
    }
    else
    { // Snap to upper bound.
      Asnap.addRow(aHat.getRow(irow % nrow));
      bsnap.push_back(bHatUpper[irow % nrow]);
    }
  }

  // Now ensure that rank(Asnap) = Asnap.getNrows().
  Asnap.pruneDependentRows(bsnap,epsMach);
}

void APPSPACK::Constraints::Linear::error(const string& fname, const string& msg) const
{
  cout << "APPSPACK::Constraints::" << fname << " - " << msg << endl;
  throw "APPSPACK Error";
}

void APPSPACK::Constraints::Linear::print() const
{
  cout << "\n";
  cout << "Bound Constraints " << endl;
  cout << "lower = " << bLower << endl;
  cout << "upper = " << bUpper << endl;
  cout << "scaling = " << scaling << endl;
  
  if (!aIneq.empty() && displayMatrix)
  {
    cout << endl << "Inequality Constraints" << endl;
    cout << "Inequality matrix = " << aIneq << endl;
    cout << "Inequality lower = " << bIneqLower << endl;
    cout << "Inequality upper = " << bIneqUpper << endl;
  }

  if (!aEq.empty() && displayMatrix)
  {
    cout << endl << "Equality Constraints" << endl;
    cout << "Equality matrix = " << aEq << endl;
    cout << "Equality bound = " << bEq << endl;
  }

  // Determine total number of finite variable bounds.
  int varlow = 0;
  int varupp = 0;
  for (int i = 0; i < bLower.size(); i++)
  {
    if (exists(bLower[i]))
      varlow++;
    if (exists(bUpper[i]))
      varupp++;
  }

  // Determine total number of (finite) inequality bounds.
  int genlow = 0;
  int genupp = 0;
  for (int i = 0; i < bIneqLower.size(); i++)
  {
    if (exists(bIneqLower[i]))
      genlow++;
    if (exists(bIneqUpper[i]))
      genupp++;
  }

  cout << "Problem size summary:" << endl;
  cout << "Number of variables: " << scaling.size() << endl;
  cout << "Number of variable bounds: " << varlow + varupp 
       << " (" << varlow << " lower, " << varupp << " upper)" << endl;
  cout << "Number of inequality constraints: " << genlow + genupp
       << " (" << genlow << " lower, " << genupp << " upper)" << endl;
  cout << "Number of equality constraints: " << bEq.size() << endl;

}

// PRIVATE
APPSPACK::Constraints::StateType 
APPSPACK::Constraints::Linear::getIneqState(int i, BoundType bType, const Vector& xTilde, double epsilon) const
{
  if (epsilon == -1)
    epsilon = epsMach;

  // a = constraint row
  const Vector& a = aHat.getRow(i);
  double anorm = aHatNorm[i];

  // b = lhs
  double b = (bType == LowerBound) ? bHatLower[i] : bHatUpper[i];

  // Check finiteness of the bound
  if (!exists(b))
    return DNE;

  // z = a' * xTilde
  double z;
  z = xTilde.dot(a);

  // Check if the constraint is epsilon-active
  if ( fabs(z - b) < (epsilon * anorm) )
    return Active;

  // Check if the constraint is otherwise satisfied
  if (((bType == LowerBound) && ( b <= z )) ||
      ((bType == UpperBound) && ( z <= b )))
    return Inactive;

  // Otherwise, it must be violated.
  return Violated;
}

// PRIVATE
APPSPACK::Constraints::StateType 
APPSPACK::Constraints::Linear::getEqState(int i, const APPSPACK::Vector& xTilde, double epsilon) const
{
  if (epsilon == -1)
    epsilon = epsMach;

  // a = constraint row
  const Vector& a = aTildeEq.getRow(i);
  double anorm = a.norm();

  // b = rhs
  double b = bTildeEq[i];

  // z = a' * xTilde
  double z;
  z = xTilde.dot(a);

  // Check if the constraint is epsilon-active
  if ( fabs(z - b) < (epsilon * anorm) )
    return Active;

  // Otherwise, it must be violated.
  return Violated;
}

// PRIVATE
bool APPSPACK::Constraints::Linear::isEqualityFeasible(const APPSPACK::Vector& xTilde) const
{  
  // Check equality constraints
  for (int i = 0; i < aTildeEq.getNrows(); i ++)
  {
    if (Violated == getEqState(i, xTilde))
      return false;
  }
  
  return true;
}

// PRIVATE
bool APPSPACK::Constraints::Linear::isInequalityFeasible(const APPSPACK::Vector& xTilde) const
{  
  // Check inequality constraints
  for (int i = 0; i < aHat.getNrows(); i ++)
  {
    if (Violated == getIneqState(i, UpperBound, xTilde))
      return false;
    if (Violated == getIneqState(i, LowerBound, xTilde))
      return false;
  }
  
  return true;
}
