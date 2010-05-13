// $Id: APPSPACK_Directions.cpp,v 1.90.2.1 2007/01/14 20:45:01 jgriffi Exp $ 
// $Source: /space/CVS-Acro/acro/packages/appspack/appspack/src/APPSPACK_Directions.cpp,v $ 

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
  \file APPSPACK_Directions.cpp
  \brief Implemtation of APPSPACK::Directions
*/

#include "APPSPACK_Directions.hpp"
#include "APPSPACK_Print.hpp"
#include "APPSPACK_Utils.hpp"
#include "APPSPACK_Constraints_Linear.hpp"
#include "APPSPACK_Float.hpp"
#include "APPSPACK_CDDLIB_Interface.h"
#include "APPSPACK_LAPACK_Wrappers.hpp"

APPSPACK::Directions::Directions(Parameter::List& params, const Constraints::Linear& constraints_in) :
  constraints(constraints_in),
  nDimensions(constraints_in.getScaling().size()),
  zero(nDimensions, static_cast<double>(0)),
  stepTolerance(params.getParameter("Step Tolerance", 0.01)),
  minStep(params.getParameter("Minimum Step", 2 * stepTolerance)),
  theta(params.getParameter("Contraction Factor", 0.5)),
  epsilonMax(params.getParameter("Epsilon Max", 2 * stepTolerance)),
  withNormals(params.getParameter("Add Projected Normals", true)),
  withCompass(params.getParameter("Add Projected Compass", false)),
  nDirections(0),
  nCached(0),
  nLapack(0),
  nCddlib(0),
  nMaxDirections(0),
  nAppend(0)
{
  // Check parameters
  if (stepTolerance <= 0)
  {
    cout << "APPSPACK::Directions::Directions - Error: \"Step Tolerance\" cannot be negative." << endl;
    throw "APPSACK Error";
  }

  if (minStep <= stepTolerance)
  {
    cout << "APPSPACK::Directions::Directions - Error: \"Minimum Step\" must be greater than \"Step Tolerance\"." << endl;
    throw "APPSACK Error";
  }

  if ((theta <= 0) || (theta >= 1))
  {
    cout << "APPSPACK::Directions::Directions - Error: \"Contraction Factor\" must be strictly between zero and one." << endl;
    throw "APPSACK Error";
  }

  epsilonMin = epsilonMax;
}

APPSPACK::Directions::~Directions()
{
}

const APPSPACK::Vector& APPSPACK::Directions::getDirection(int i) const
{
  return direction.getRow(i);
}

double APPSPACK::Directions::getStep(int i) const
{
  return step[i];
}

void APPSPACK::Directions::setStepConverged(int i)
{
  step[i] = stepTolerance/2;
}

void APPSPACK::Directions::getDirectionIndices(vector<int>& idvector) const
{
  idvector.resize(0);
  for (int i = 0; i < nDirections; i ++)
    if ((step[i] >= stepTolerance) && (tag[i] == -1))
      idvector.push_back(i);
}

void APPSPACK::Directions::computeNewDirections(const Point& newPoint)
{
  // Grab new point info.
  double newStep = max(newPoint.getStep(), minStep);
  const Vector& x = newPoint.getX();
  
  // Update distance vector for new point.
  constraints.formDistanceVector( x, xDistance );

  do 
  {
    // Update active state vector, member constraintState.
    if (updateConstraintState(newStep))
    {    
      // Generate search directions.
      generateForLinear(direction);
    }

    if (!direction.empty())
    {
      // Update step, trueStep, epsilonMin, and nDirections.
      updateDirectionInfo(newStep);
      break;
    }

    // If directions are empty, we will want a smaller value of newStep.
    newStep = theta * newStep;
  } while (newStep >= stepTolerance);
  
  if (direction.empty())
  {
    cout << "Error in APPSPACK::Directions::computeNewDirections().\n"
	 << "Unable to generate generators for epsilon-tangent cone.\n"
	 << "One of two possible problems may be the culprit:\n"
	 << "(1) Parameter \"Step Tolerance\" is too large \n"
	 << "(2) No feasible search directions exist at current point. \n"
	 << endl;
    throw "APPSPACK Error";
  }
}

void APPSPACK::Directions::appendNewDirections()
{
  double newEps = getSmallestStep();
  // If newEps >= epsilonMin active set remains the same.
  if (newEps >= epsilonMin)
    return;

  // If constraintState remains the same, do nothing.
  if (!updateConstraintState(newEps))
  {
    epsilonMin = newEps;
    return;
  }

  // Compute new directions.  Note that distance vector has not been updated
  // as the current point is assumed the same.
  Matrix newDirection;
  generateForLinear(newDirection);

  // Now add new directions to the current list.
  direction.addUniqueRows(newDirection, constraints.getEpsMach());
  
  // Update step, trueStep, espilonk, and nDirections.
  updateDirectionInfo(newEps, true);
}

void APPSPACK::Directions::setTrueStepAndTag(int i, double trueStep_in, int tag_in)
{
  trueStep[i] = trueStep_in;
  tag[i] = tag_in;
}


void APPSPACK::Directions::print(const string label) const
{
  if (!label.empty())
    cout << label << ":\n";

  for (int i = 0; i < nDirections; i ++)
  {
    cout << setw(4) << i << " : ";

    cout << "d = " << direction.getRow(i) << " ";

    cout<< "step = " << Print::formatPositiveDouble(step[i]) << " ";

    if (tag[i] != -1) 
    {
      cout << "tag = " << setw(6) << tag[i] << " ";
      cout << "trueStep = " <<  Print::formatPositiveDouble(trueStep[i]);
    }

    cout << "\n";
  }
  cout << "Number of times directions calculated by...\n"
       << "  LAPACK: "    << nLapack << endl
       << "  CDDLIB: "    << nCddlib << endl
       << "  Cached: " << nCached  << endl
       << "Max directions in single iteration : " << nMaxDirections << endl
       << "Number of times directions appended: " << nAppend << endl;
}

bool APPSPACK::Directions::isStepConverged() const
{
  for (int i = 0; i < nDirections; i ++)
  {
    if (step[i] >= stepTolerance)
      return false;
  }

  return true;
}

bool APPSPACK::Directions::empty() const
{
  return (direction.empty() == true);
}

void APPSPACK::Directions::reduceStep(int i)
{
  double tmpStep = theta * step[i];

  step[i] = tmpStep;
  trueStep[i] = -1;
  tag[i] = -1;
}

//PRIVATE
void APPSPACK::Directions::buildNormalCone(APPSPACK::Matrix& VpT,
						   APPSPACK::Matrix& VlT) const
{
  // Always insert equality constraints in VlT.
  VlT.addMatrix(constraints.getAtildeEq());
  
  // Insert elements into VpT and VlT from aHat.
  const Matrix& aHat = constraints.getAhat();
  for (int i = 0; i < constraintState.size(); i++)
  {
    if (constraintState[i] == APPSPACK::Constraints::BothActive)
      VlT.addRow(aHat.getRow(i));
    else if (constraintState[i] == APPSPACK::Constraints::LowerActive)
      VpT.addRow(aHat.getRow(i), -1.0);
    else if (constraintState[i] == APPSPACK::Constraints::UpperActive)
      VpT.addRow(aHat.getRow(i));
  }
}

//PRIVATE
void APPSPACK::Directions::buildTangentCone(const APPSPACK::Matrix& VpT, 
					      const APPSPACK::Matrix& VlT,
					      APPSPACK::Matrix& T)
{
#if !(defined HAVE_LAPACK) && !(defined HAVE_CDDLIB)
  buildWithNothing(T);
  return;
#endif
  
  if ((VpT.empty()) && (VlT.empty()))
  {
    // Generate all compass directions. Problem looks locally unconstrained.
    generateUnconstrained(T);
    return;
  }
  
  if (buildWithLapack(VpT, VlT, T))
  {
    nLapack++;
    return;
  }
  
  // Either constraints degenerate or LAPACK unavailable.  Try cddlib.
  if (buildWithCddlib(VpT, VlT, T))
  {
    nCddlib++;
    return;
  }
  
  cout << "Error in APPSPACK::Directions::buildTangentCone().\n"
       << "Unable to generate generators for epsilon-tangent cone.\n"
       << "One of four possible problems may be the culprit:\n"
       << "(1) APPSPACK was configured without LAPACK\n"
       << "(2) APPSPACK was configured without CDDLIB and the constraints are degenerate\n"
       << "(3) Either CDDLIB or LAPACK capability disabled in .apps files\n"
       << "(4) CDDLIB failed to find generators (please submit bug report!)\n"
       << endl;
  throw "APPSPACK Error";
}

//PRIVATE
void APPSPACK::Directions::buildWithNothing(APPSPACK::Matrix& D)					     
{
  D.clear();
  const Vector& scaling = constraints.getScaling();
  for (int i = 0; i < constraintState.size(); i++)
  {
    tmpVector = zero;
    if (constraintState[i] == APPSPACK::Constraints::NeitherActive)
    {
      // Add +e_i
      tmpVector[i] = scaling[i];
      D.addRow(tmpVector);
      // Add -e_i
      tmpVector[i] = -1 * scaling[i];
      D.addRow(tmpVector);
    }
    else if (constraintState[i] == APPSPACK::Constraints::LowerActive)
    {
      // Add +e_i
      tmpVector[i] = scaling[i];
      D.addRow(tmpVector);
    }
    else if (constraintState[i] == APPSPACK::Constraints::UpperActive)
    {
      // Add -e_i
      tmpVector[i] = -1 * scaling[i];
      D.addRow(tmpVector);
    }
  }
}

//PRIVATE
bool APPSPACK::Directions::buildWithCddlib( const APPSPACK::Matrix& VpT,
					       const APPSPACK::Matrix& VlT,
					       APPSPACK::Matrix& Tcone)
{
#ifndef HAVE_CDDLIB
  return false;
#endif
  Tcone.clear();  
  const Vector& scaling = constraints.getScaling(); 
  int nvar = scaling.size();

  // Get row pointers needed by call to compute_cone_generators.
  vector< double *> VpTptr;
  const_cast<Matrix &>(VpT).getRowPointers(VpTptr);

  vector< double *> VlTptr;
  const_cast<Matrix &>(VlT).getRowPointers(VlTptr);
  
  int num_pointy = 0;
  int num_lineality = 0;
  double **P;
  double **L;

  compute_cone_generators( &num_pointy, &P, &num_lineality, &L, nvar,
			   VlT.getNrows(), &VlTptr[0], VpT.getNrows(), &VpTptr[0], 0);

  // Add in lineality directions.
  Matrix Lmat(L, num_lineality, nvar);
  Lmat.normalize();
  Lmat.scale(scaling);
  Tcone.addMatrix(Lmat);
  Tcone.addMatrix(Lmat, -1.0);

  // Add in pointy directions.
  Matrix Pmat(P, num_pointy, nvar);
  Pmat.normalize();
  Pmat.scale(scaling);
  Tcone.addMatrix(Pmat);
  
  // Deallocate.
  for (int i = 0; i < num_pointy; i++)
    free(P[i]);
  free(P);
  for (int i = 0; i < num_lineality; i++)
    free(L[i]);
  free(L);
  
  return true;
}

//PRIVATE
bool APPSPACK::Directions::buildWithLapack(const APPSPACK::Matrix& VpT, 
					      const APPSPACK::Matrix& VlT,
					      APPSPACK::Matrix& Tcone)
{ 
#ifndef HAVE_LAPACK
  return false;
#endif
  Tcone.clear();
  // Get orthonormal basis matrix Z for nullspace of VlT if Vlt is non-empty.
  // Otherwise Z becomes the idenity.
  Matrix ZT;
  if (!VlT.empty())
  {
    VlT.nullSpace(ZT, constraints.getEpsMach());
    if (ZT.empty())
      return true;
  }
  else
  {
    // The are no equality constraints.  null(VlT) = I (trivially).
    ZT.setToIdentity(VpT.getNcols());
  }

  // Only equality constraints are present, so +/- Z generates space.
  if (VpT.empty()) 
  {
    // ZTscaled = Z^T * S
    Matrix ZTscaled(ZT, constraints.getScaling());
    Tcone.addMatrix(ZTscaled);
    Tcone.addMatrix(ZTscaled, -1.0);
    return true;
  }
  
  // Compute product VpTZ = VpT * Z.
  Matrix VpTZ;
  VpT.multMat(ZT, VpTZ, Matrix::Transpose);
  
  // Compute matrices R and N such that VpTZ*R = I and VptZ*N = 0;
  Matrix RT, NT;
  if (!VpTZ.getRighInvAndNullBasis(RT, NT, constraints.getEpsMach()))
    return false;

  // Now form matrix Z*R.  Since we are working with transposes, we really form RT*ZT.
  Matrix ZRT(RT);
  ZRT.multMat(ZT);
  // Columns of Z*R are not necessarily unit length.
  ZRT.normalize();
  // Now scale columns and add to direction.
  ZRT.scale(constraints.getScaling());
  Tcone.addMatrix(ZRT, -1.0);

  // Now form Z*N.  Since we are working with transposes, we really form NT*ZT.
  // N may be empty if Z'*Vp is a square system, implying null(Z'*Vp) = {0}.
  if (!NT.empty())
  {
    Matrix ZNT(NT);
    ZNT.multMat(ZT); 
    // Normalizing unnecessary, N and Z both have orthonormal columns.
    // Now scale columns and add to direction.
    ZNT.scale(constraints.getScaling());
    Tcone.addMatrix(ZNT);
    Tcone.addMatrix(ZNT, -1.0);
  }
  
  return true;
}

//PRIVATE
void APPSPACK::Directions::generateUnconstrained(APPSPACK::Matrix& D)						     
{
  D.clear();
  const Vector& scaling = constraints.getScaling();

  for (int i = 0; i < nDimensions; i ++)
  {
    tmpVector = zero;    
    // Add +e_i
    tmpVector[i] = scaling[i];
    D.addRow(tmpVector);
    // Add -e_i
    tmpVector[i] = -1 * scaling[i];
    D.addRow(tmpVector);
  }
}

//PRIVATE
void APPSPACK::Directions::generateForLinear(APPSPACK::Matrix& D)
{
  D.clear();

  // Check if directions are cached.
  cacheIter = directionCache.find(constraintState);
  if (cacheIter != directionCache.end())
  {
    nCached++;
    D = cacheIter->second;
    return;
  }
  
  // Directions are not cached.  Form the normal cone.
  Matrix VpT;
  Matrix VlT;
  buildNormalCone(VpT, VlT);
  buildTangentCone(VpT, VlT, D);
  
  // If there are no tangent direction, Deltak must be reduced.
  if (D.empty())
  {
    // We still want to cache direction here--the work to figure out D is 
    // empty may be nontrivial.
    directionCache[constraintState] = D;
    return;
  }
  
  // Add normals.
  if (withNormals)
    addNormalDirections(VpT, VlT, D);

  if (withCompass)
    addCompassDirections(VlT, D);
  
  // Cache newly computed directions.
  directionCache[constraintState] = D;
}

//PRIVATE
void APPSPACK::Directions::addNormalDirections(const APPSPACK::Matrix& VpT, 
					       const APPSPACK::Matrix& VlT,
					       APPSPACK::Matrix& D)
{
  // Add in normal directions, projecting with Z*ZT if necessary.
  if (VpT.empty())
    return;
  
  Matrix ND(VpT);
  if (!VlT.empty())
  {    
#ifndef HAVE_LAPACK
    {
      cerr << "APPSPACK::Directions::addNormalDirections - Error: "
	   << "Cannot add projected normals without LAPACK." << endl;
      throw "APPSACK Error";
    }
#endif
    Matrix ZT;
    VlT.nullSpace(ZT, constraints.getEpsMach());
    if (ZT.empty()) // null(ZT) is empty, projecting to empty set.
      return;
    ND.multMat(ZT, Matrix::Transpose);
    ND.multMat(ZT);
  }
  
  ND.normalize();
  ND.scale(constraints.getScaling());
  D.addMatrix(ND);
}

//PRIVATE
void APPSPACK::Directions::addCompassDirections(const APPSPACK::Matrix& VlT,
						APPSPACK::Matrix& D)
{
  //First form compass search directions.
  Matrix PCD;
  generateUnconstrained(D);
  //No project if necessary
  if (!VlT.empty())
  {    
#ifndef HAVE_LAPACK
    {
      cerr << "APPSPACK::Directions::addNormalDirections - Error: "
	   << "Cannot add projected normals without LAPACK." << endl;
      throw "APPSACK Error";
    }
#endif
    Matrix ZT;
    VlT.nullSpace(ZT, constraints.getEpsMach());
    if (ZT.empty()) // null(ZT) is empty, projecting to empty set.
      return;
    PCD.multMat(ZT, Matrix::Transpose);
    PCD.multMat(ZT);
  }
  
  PCD.normalize();
  PCD.scale(constraints.getScaling());
  D.addUniqueRows(PCD, constraints.getEpsMach());
}

//PRIVATE
bool APPSPACK::Directions::updateConstraintState(double newStep)					     
{
  
  // Ensure epsilon < epsilonMax.
  double epsilon = min(epsilonMax, newStep);
  // Determine indices of near active constraints wrt epsilon.
  vector<Constraints::ActiveType> newState;
  constraints.getActiveIndex(xDistance, epsilon, newState);

  // If constraintState remains the same and is not updated return false.
  if (newState == constraintState)
    return false;
  
  // Otherwise update constraintState to newState and return true.
  constraintState = newState;
  return true;
}

//PRIVATE
void APPSPACK::Directions::updateDirectionInfo(double newStep, bool isAppend)
{
  //Note: newStep \ge minStep if 1) isAppend = false, and 2) the corresponding
  //tangent cone is empty.  newStep is bumped up to minStep initially inside
  //computeNewDirections.  An empty tangent cone corresponds to a failed iteration
  //and hence we do not want to bumb up to minStep in this case.
  if (isAppend)
  {
    // Only update new directions.
    int numnew = direction.getNrows() - nDirections;
    if (numnew > 0)
      nAppend++;
    nDirections = direction.getNrows();
    step.append(numnew, newStep);
    trueStep.append(numnew, -1);
    tag.insert(tag.end(), numnew, -1);
  }
  else
  {
    // Update all directions.
    nDirections = direction.getNrows();
    step.assign(nDirections, newStep);
    trueStep.assign(nDirections, -1);
    tag.assign(nDirections, -1);
  }
  nMaxDirections=max(nMaxDirections, nDirections);
  // epsilonMin records smallest value of epsilon used so far.
  epsilonMin = min(epsilonMax, getSmallestStep());
}

double APPSPACK::Directions::getSmallestStep() const
{
  double sstep = step.max();
  for (int i=0; i < step.size(); i++)
  {
    if (step[i] >= stepTolerance)
      sstep = min(sstep, step[i]);
  }
  return sstep;
}


