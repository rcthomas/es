// $Id: APPSPACK_Solver.cpp,v 1.65.2.3 2007/02/03 23:37:05 jgriffi Exp $ 
// $Source: /space/CVS-Acro/acro/packages/appspack/appspack/src/APPSPACK_Solver.cpp,v $ 

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
  \file APPSPACK_Solver.cpp
  \brief Implementation of APPSPACK::Solver
*/

#include <algorithm>
#include "APPSPACK_Solver.hpp"
#include "APPSPACK_Print.hpp"
#include "APPSPACK_Constraints_Linear.hpp"
#include "APPSPACK_Float.hpp"
#include "APPSPACK_LAPACK_Wrappers.hpp"

APPSPACK::Solver::Solver(const Parameter::List& params_in,
			 Executor::Interface& executor_in,
			 const Constraints::Linear& constraints_in,
			 Combiner::Generic& combiner_in) :
  constraints(constraints_in),
  params(params_in),
  print(params),		// modifies params and sets globals
  bestPointPtr(initializeBestPointPtr(params, constraints, combiner_in)), // modifies params
  directions(params, constraints), // modifies params
  conveyor(params, constraints.getScaling(), executor_in), // modifies params
  exchangeList(),
  state(Continue),
  isFunctionTolerance(false),
  isMaxEvaluations(false),
  useSnapTo(params.getParameter("Snap To Boundary",true))
{
  setup(params_in, executor_in, constraints_in);
}

APPSPACK::Solver::Solver(const Parameter::List& params_in,
			 Executor::Interface& executor_in,
			 const Constraints::Linear& constraints_in) :
  constraints(constraints_in),
  params(params_in),
  print(params),		// modifies params and sets globals
  bestPointPtr(initializeBestPointPtr(params, constraints, combiner)), // modifies params
  directions(params, constraints), // modifies params
  conveyor(params, constraints.getScaling(), executor_in), // modifies params
  exchangeList(),
  state(Continue),
  isFunctionTolerance(false),
  isMaxEvaluations(false),
  useSnapTo(params.getParameter("Snap To Boundary",true))
{
  setup(params_in, executor_in, constraints_in);
}

void APPSPACK::Solver::setup(const Parameter::List& params_in,
			     Executor::Interface& executor_in,
			     const Constraints::Linear& constraints_in)
{
  useRandomOrder = params.getParameter("Use Random Order", false);

  cout << "\n"
       << "-----------------------------------------------------\n"
       << "APPSPACK: Asynchronous Parallel Pattern Search\n"
       << "Written by T. G. Kolda et al., Sandia National Labs\n"
       << "For more information visit \n"
       << "http://software.sandia.gov/appspack\n"
       << "-----------------------------------------------------\n"
       << "\n";
  
  // Stopping Criteria
  if (params.isParameterDouble("Function Tolerance"))
  {
    isFunctionTolerance = true;
    functionTolerance = params.getDoubleParameter("Function Tolerance");
  }

  if (params.isParameter("Maximum Evaluations"))
  {
    isMaxEvaluations = true;
    maxEvaluations = params.getParameter("Maximum Evaluations", 0);
  }

  boundsTolerance = params.getParameter("Bounds Tolerance", params.getDoubleParameter("Step Tolerance") / 2.0);
  if (boundsTolerance <= 0)
  {
    cout << "APPSPACK::Solver::Solver - Invalid non-positive value for \"Bounds Tolerance\"." << endl;
    throw "APPSPACK Error";
  }

  //! Print
  if (Print::doPrint(Print::InitialData))
    printInitializationInformation();
  
  processNewBestPoint();
}

APPSPACK::Solver::~Solver()
{
  delete bestPointPtr;
}

const vector<double>& APPSPACK::Solver::getBestX() const
{
  return bestPointPtr->getX().getStlVector();
}

double APPSPACK::Solver::getBestF() const
{
  return bestPointPtr->getF(); 
}

const vector<double>& APPSPACK::Solver::getBestVecF() const
{
  return bestPointPtr->getVecF().getStlVector(); 
}

APPSPACK::Point* APPSPACK::Solver::initializeBestPointPtr(Parameter::List& params, 
							  const Constraints::Linear& constraints,
							  Combiner::Generic& combiner)
{
  // Set up the initial point
  Vector nominalX;
  if (!params.isParameter("Initial X"))
    constraints.getNominalX(nominalX);
  
  Vector initialX = params.getParameter("Initial X", nominalX);
  
  // Check that the initial point is the right size
  if (initialX.size() != constraints.getScaling().size())
  {
    cerr << "Error: Size mismatch\n";
    cerr << "The size of the initial X is not the same as the lower bounds.\n";
    throw "APPSPACK Error";
  }  

  // Try making feasible wrt to simple bounds.
  if (!constraints.isFeasible(initialX))
    constraints.makeBoundsFeasible(initialX);
  
#ifdef HAVE_LAPACK
  double epsilonSnap = params.getParameter("Bounds Tolerance", params.getDoubleParameter("Step Tolerance") / 2.0);
  if (epsilonSnap <= 0)
  {
    cout << "APPSPACK::Solver::Solver - Invalid non-positive value for \"Bounds Tolerance\"." << endl;
    throw "APPSPACK Error";
  }

  // Try snapping to
  if (!constraints.isFeasible(initialX)) 
    constraints.snapToBoundary(initialX,epsilonSnap);
#endif

  // Check that x is feasible
  constraints.assertFeasible(initialX);

  // Initial F
  Vector initialF;
  if (params.isParameterDouble("Initial F"))
  {
    initialF.assign(1,params.getDoubleParameter("Initial F"));
    params.setParameter("Initial F",initialF); // reset to be a vector!
  }
  else if (params.isParameterVector("Initial F"))
  {
    initialF = params.getVectorParameter("Initial F");
  }

  // Initial step
  double initialStep = params.getParameter("Initial Step", 1.0);
  
  // Point
  double alpha = params.getParameter("Sufficient Decrease Factor", 0.01);
  
  // Create first best point
  return new Point(initialX, initialF, initialStep, alpha, combiner);
}

APPSPACK::Solver::State APPSPACK::Solver::solve()
{
  while (state == Continue)
  {
    iterate();
  }

  // Print the final solution
  if (Print::doPrint(Print::FinalSolution))
  {
    cout << "\nFinal State: " << state << "\n";
    printBestPoint("Final Min");
    conveyor.getCounter().print();
    directions.print("Final Directions");
    cout << "\n"
	 << "-----------------------------------------------------\n"
	 << "Thank you!\n"
	 << "APPSPACK: Asynchronous Parallel Pattern Search\n"
	 << "Written by T. G. Kolda et al., Sandia National Labs\n"
	 << "For more information visit \n"
	 << "http://software.sandia.gov/appspack\n"
	 << "-----------------------------------------------------\n"
	 << "\n";
  }


  
  // Print solution file if specified by user.
  string solfile;
  solfile = params.getParameter("Solution File", solfile);
  if (!solfile.empty())
    writeSolutionFile(solfile);

  return state;
}

APPSPACK::Solver::State APPSPACK::Solver::iterate()
{
    generateTrialPoints();
    
    conveyor.exchange(exchangeList);

    processEvaluatedTrialPoints();
    
    return state;
}

// PRIVATE
void APPSPACK::Solver::processNewBestPoint(Point* newBestPointPtr)
{
  // Update the best point
  if (newBestPointPtr != NULL)
  {
    delete bestPointPtr;
    bestPointPtr = newBestPointPtr;
  }

  // Print
  if (Print::doPrint(Print::NewBestPoint))
    printBestPoint("New Min");
  
  // Check for convergence based on the function value
  if ((isFunctionTolerance) && (bestPointPtr->getF() < functionTolerance))
  {
    state = FunctionConverged;
    return;
  }

  // Update the (scaled) search directions
  directions.computeNewDirections(*bestPointPtr);

  // Print
  if (Print::doPrint(Print::NewBestDirections))
    directions.print("New Directions");
}

// PRIVATE
void APPSPACK::Solver::generateTrialPoints()
{
  // Local references
  const Vector& parentX = bestPointPtr->getX();
  
  Vector& x = tmpVector;
  int n = parentX.size();
  x.resize(n);

  vector<int> indices;
  directions.getDirectionIndices(indices);

  if (useRandomOrder == true)
  {
    // Randomize access order.    
    random_shuffle(indices.begin(), indices.end());
  }
  
  for (int i = 0; i < indices.size(); i ++)
  {
    int idx = indices[i];
    const Vector& direction = directions.getDirection(idx);
    const double step = directions.getStep(idx);
    double maxStep = constraints.maxStep(parentX, direction, step);
  
    // Only add nontrivial points to the exchange list.
    if (maxStep <= 0)
    {
      // Cannot move along this direction and remain feasible.
      directions.setStepConverged(idx);
    }
    else
    {
      for (int j = 0; j < n; j++)
	x[j] = parentX[j] + maxStep*direction[j];
      
#ifdef HAVE_LAPACK
      if (useSnapTo)
      {
	Vector xsnap = x;
	constraints.snapToBoundary(xsnap,boundsTolerance);
	if (constraints.isFeasible(xsnap))
	  x = xsnap;
      }
#endif
      
      constraints.assertFeasible(x);
      
      // Create a new trial point
      Point* newPointPtr = new Point(x, step, *bestPointPtr, idx);
      
      // Save off trial point information
      directions.setTrueStepAndTag(idx, maxStep, newPointPtr->getTag());
      
      // Push this trial point onto the new trial point list.
      // Ownership of the pointer transfers to the list.
      exchangeList.push(newPointPtr);
    }
  }
  
  if (Print::doPrint(Print::Directions))
    directions.print("Directions After Trial Point Generation");

  if (Print::doPrint(Print::UnevaluatedPoints))
    exchangeList.print("Trial Points Before Conveyor");

}

void APPSPACK::Solver::processEvaluatedTrialPoints()
{
  // Print
  if (Print::doPrint(Print::EvaluatedPoints))
    exchangeList.print("Trial Points After Conveyor");

  // Check for a new best point
  if (exchangeList.best() < *bestPointPtr)
  {
    processNewBestPoint(exchangeList.popBest());
    exchangeList.prune();
    conveyor.prune();
  }
  else 
  {
    // Otherwise, just process the list
    Point* ptr;
    bool stepReduced = false;
    while (!exchangeList.isEmpty())
    {
      ptr = exchangeList.pop();
      if (ptr->getParentTag() == bestPointPtr->getTag())
      {
	directions.reduceStep(ptr->getIndex());
	stepReduced = true;
      }
      delete ptr;
    }
    // Check for step length convergence.  If not converged, append new directions to list.
    if (directions.isStepConverged())
      state = StepConverged;
    else if (stepReduced)
      directions.appendNewDirections();
  }

  // Check for number of evaluations
  if ((state == Continue) && 
      (isMaxEvaluations) && 
      (conveyor.getCounter().getNumEvaluations() >= maxEvaluations))
    state = EvaluationsExhausted; 
}

void APPSPACK::Solver::printInitializationInformation() const
{
  cout << "\n";
  cout << "###########################################" << "\n";
  cout << "###   APPSPACK Initialization Results   ###" << endl;
  cout << "\n";
  
  // Paramters
  cout << "*** Parameter List ***" << "\n";
  params.print();

  // Constraints
  cout << "\n" << "*** Constraints ***" << "\n";
  constraints.print();
  
  // Conveyor
  cout << "\n" << "*** Conveyor ***" << "\n";
  conveyor.print();

  cout << "\n";
  cout << "### End APPSPACK Initialization Results ###" << endl;
  cout << "###########################################" << "\n";
}

void APPSPACK::Solver::printBestPoint(const string label) const
{
  cout << "\n" << label << ": " << *bestPointPtr << endl;
}

ostream& operator<<(ostream& stream, APPSPACK::Solver::State state) 
{
  switch (state)
  {
  case APPSPACK::Solver::Continue:
    stream << "Continue";
    break;
  case APPSPACK::Solver::StepConverged:
    stream << "Step Converged";
    break;
  case APPSPACK::Solver::FunctionConverged:
    stream << "Function Converged";
    break;
  case APPSPACK::Solver::EvaluationsExhausted:
    stream << "Evaluations Exhausted";
    break;
  }

  return stream;
}

void APPSPACK::Solver::writeSolutionFile(const string filename) const
{
  // Determine output precision.
  int precision = params.getParameter("Solution File Precision", 14);
    
  // Open file for append
  ofstream fout;
  fout.open(filename.c_str(), std::ios::out);
  if (!fout)
  {
    cerr << "APPSPACK::Solver::writeSolutionFile() - Unable to open solution file" << endl;
    return;
  }

  // Write best value to outname using cache file output style.
  fout << "f=[ ";
  (bestPointPtr->getVecF()).leftshift(fout,precision);
  fout << " ]";
  
  fout << " x=[ ";
  (bestPointPtr->getX()).leftshift(fout,precision);
  fout << " ]" << endl;
}
