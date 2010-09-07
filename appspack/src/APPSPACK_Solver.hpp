// $Id: APPSPACK_Solver.hpp,v 1.31.2.1 2007/01/14 22:09:44 jgriffi Exp $ 
// $Source: /space/CVS-Acro/acro/packages/appspack/appspack/src/APPSPACK_Solver.hpp,v $ 

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
  \file APPSPACK_Solver.hpp
  \brief Class definition for APPSPACK::Solver
*/

#ifndef APPSPACK_SOLVER_HPP
#define APPSPACK_SOLVER_HPP

#include "APPSPACK_Point.hpp"
#include "APPSPACK_Conveyor.hpp"
#include "APPSPACK_Directions.hpp"
#include "APPSPACK_List.hpp"
#include "APPSPACK_Parameter_List.hpp"
#include "APPSPACK_Print.hpp"
#include "APPSPACK_Constraints_Linear.hpp"
#include "APPSPACK_Combiner_Generic.hpp"

namespace APPSPACK
{

//! The solver itself
/*!  <b>Parameters</b>

  These parameters are are stored in the Parameter::List that is
  passed to the Solver constructor. See \ref pageParameters for full details on
  these parameters.

  <ul>
  <li>"Function Tolerance"
  <li>"Maximum Evalutations"
  <li>"Bounds Tolerance"
  <li>"Snap To Boundary"
  <li>"Solution File"
  <li>"Use Random Order"
  </ul>

*/
class Solver
{
  
public:
  
  //! State of the solver
  enum State {
    Continue,			//!< Default state
    StepConverged,		//!< Step length converged
    FunctionConverged,		//!< Function tolerance converged
    EvaluationsExhausted        //!< Number of function evaluations exhausted
  };
  
  //! Constructor 
  Solver(const Parameter::List& params_in, 
	 Executor::Interface& executor_in, 
	 const Constraints::Linear& constraints_in,
	 Combiner::Generic& combiner_in);
  
  //! Constructor 
  Solver(const Parameter::List& params_in, 
	 Executor::Interface& executor_in, 
	 const Constraints::Linear& constraints_in);

  //! Constructor support.
  void setup(const Parameter::List& params_in, 
	     Executor::Interface& executor_in, 
	     const Constraints::Linear& constraints_in);
  
  /*! \brief Called by the Solver constructor to parse the parameter list and
    create the initial best point. */
  static Point* initializeBestPointPtr(Parameter::List& params, 
				       const Constraints::Linear& constraints,
				       Combiner::Generic& combiner);

  //! Destructor 
  ~Solver();


  // --------------------------------------------------------------------------------

  //@{ \name Accessors (for after solve() is called)
  
  //! Return the x-vector corresponding to the best point
  const vector<double>& getBestX() const;

  //! Return the finite function value (if any) associated with the best point
  double getBestF() const;

  //! Returns instance of class Value associated with current best point.
  const vector<double>& getBestVecF() const;
  
  //@}

  // --------------------------------------------------------------------------------

  //@{ \name Manipulators

  //! Find the minimum of the function that was set up in the constructor
  APPSPACK::Solver::State solve();

  //! Do a single APPS iteration 
  APPSPACK::Solver::State iterate();

  //@}

private:

  /*!  

    Process a new best point - Delete the old point (if any) and
    replace it with the new.

    If the function tolerance convergence test is being employed, we
    check for convergence at this point, and return if convergence is
    detected.

    Otherwise, we generate new search directions via a call to
    Directions::computeNewDirections(), re-initialize the
    #step array, and reset various bookkeeping variables (#trueStep
    and #tag).

    All the steps start the same, and they are calculated as follows:
    \f[
    {\rm step}_{\rm new} = \max \{ {\rm step}_{\rm best}, 2 * {\rm stepTolerance} \}
    \f]

   */
  void processNewBestPoint(Point* newBestPointPtr = NULL);

  /*! Trial points are generated corresponding to vectors in #directions 
    that are not converged and do not already have an associated trial point.

    For each direction \f$d_i\f$ in #directions, a new trial point \f$ x\f$ is
    computed satisfying
    \f[
    x = x_{\rm best} + \Delta_{\rm max} d_i,
    \f] 
    where \f$\Delta_{\rm max}\f$ denotes
    the maximum scalar in interval \f$[0,\Delta_i]\f$ such that
    \f$ x_{\rm best} + \Delta_{\rm max} d_i,\f$ is feasible.  
    - \f$ \Delta_i\f$ denotes the corresponding step value to \f$ d_i \f$ whose value is
    determined via a call to Directions::getStep().
    - \f$ \Delta_{\rm max} \f$ is determined by a call to
    Constraints::Linear::maxStep().


    If the user definable parameter #useSnapTo = true, a call to
    Constraints::Linear::snapToBoundary() is made to determine if \f$ x \f$
    is near a boundary point.  If a nearby boundary point \f$x_b\f$ is found, 
    \f[
    x = x_b.
    \f]
  
    Trial point information is then saved and \f$ x \f$ is added to the new trial
    point list, #exchangeList.
    
    \note New search directions are created in processNewBestPoint() via a call to 
    Directions::computeNewDirections(). 

    \note A direction is considered converged if its corresponding
    step length is strictly smaller than Directions::stepTolerance.

  */
  void generateTrialPoints();

  /*!

    Process a list of trial points that has been returned from the
    UberEvaluator.

    First, check to see if the best of all these points is better than
    the current best point. If so, replace that best point (see
    processNewBestPoint()) and throw away the rest of the list.

    Otherwise, process each point and delete it.

    Finally, check if all the directions have converged and check if
    we have exhausted the maximum number of function evaluations.

   */
  void processEvaluatedTrialPoints();

  //@{ \name Print Functions

  void printInitializationInformation() const;
  void printBestPoint(const string label = "") const;

  /*! Writes solution to file specified by "Solution File" (see \ref pageParameters).
  */
  void writeSolutionFile(const string filename) const;
  //@}

private:

  //! Constraints
  const Constraints::Linear& constraints;

  //! Parameters
  Parameter::List params;

  //! Print
  Print print;

  //! The search directions 
  Directions directions;
  
  //! Pointer to the best trial point thus far
  Point* bestPointPtr;

  //! Trial Point Evaluator
  Conveyor conveyor;

  //! List of trial points to be processed in some way
  List exchangeList;

  //! The state of the solver
  State state;

  //@{ \name Stopping Criteria

  //! Enforce function value tolerance
  bool isFunctionTolerance;

  //! Function value
  double functionTolerance;

  //! Enforce function evaluation budget
  bool isMaxEvaluations;

  //! Function evaluation budget
  int maxEvaluations;

  //@}

  //! Tolerance for saying whether or not we are on a boundary
  double boundsTolerance;

  //! A temporary vector
  Vector tmpVector;

  //! Determines whether or not Constraints::Linear::snapToBoundary() is called.
  /*! If true, generated trial points will be sent to Constraints::Linear::snapToBoundary()
      prior to being added to the evaluation cue.  If a boundary point is sufficiently near, 
      the respective trial point will be redefined to be this point. \see generateTrialPoints().
   */
  bool useSnapTo;

  //! Default combiner.
  Combiner::Generic combiner;

  //! If true, APPSPACK will randomize the order in which points are generated.
  /*! When points are generated and added to the queue in a fixed
    order, it is possible (when running in asynchronous mode) to
    initially optimize over a strict subspace.  The drawback of this
    is that small-steps must be then taken to the true solution to
    avoid undoing the gains made in the initial subspace.  
  */
  bool useRandomOrder;
};

}

//! Printing an APPSPACK::Solver::State
ostream& operator<<(ostream& stream, APPSPACK::Solver::State state);

#endif
