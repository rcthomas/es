
// $Source: /space/CVS-Acro/acro/packages/appspack/appspack/src/APPSPACK_Directions.hpp,v $ 

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
  \file APPSPACK_Directions.hpp
  \brief Class description for APPSPACK::Directions
*/

#ifndef APPSPACK_DIRECTIONS_HPP
#define APPSPACK_DIRECTIONS_HPP

#include "APPSPACK_Point.hpp"
#include "APPSPACK_Parameter_List.hpp"
#include "APPSPACK_Constraints_Linear.hpp"

namespace APPSPACK
{

//! The search directions and associated information
/*!  
  The job of this object is to generate an appropriate set of search
  directions for given point, and then to track and update the
  associated step lengths.
  
  For each direction, we track the desired step length, the true step
  length (if the desired step length takes us outside the feasible
  region we instead substitute a step length that keeps us feasible),
  and the associated trial point tag. 

  The true step and tag are actually calculated by
  Solver::generateTrialPoints and just stored in this object for
  information purposes.

  <b>Parameters</b>

  These parameters are are stored in the Parameter::List that is
  passed to the Solver constructor. See \ref pageParameters for full details on
  these parameters.

  <ul>
  <li>"Step Tolerance"
  <li>"Minimum Step"
  <li>"Contraction Factor"
  <li>"Epsilon Max"
  <li>"Add Projected Normals"
  <li>"Add Projected Compass"
  </ul>

*/
class Directions
{

public:

  //! Constructor 
  /*!  
    Reads the "Step Tolerance", "Minimum Step", "Contraction Factor", and
    "Epsilon Max" from the parameter list.
  */
  Directions(Parameter::List& params, const Constraints::Linear& constraints_in);

  //! Destructor 
  ~Directions();
  
  //@{ \name Accessors

  //! Returns the indices of directions that are ready for new trial points.
  /*!  
    A direction is ready for a new trial point if its associated
    step length is greater than or equal to the convergence tolerance
    and it doesn't already have an active trial point.
  */
  void  getDirectionIndices(vector<int>& idvector) const;

  //! Returns the i-th search direction
  const Vector& getDirection(int i) const;

  //! Returns the i-th step
  double getStep(int i) const;

  //! Sets the step corresponding to the ith direction = stepTolerance/2.
  void setStepConverged(int i);

  //! Returns true if \e every step length is less than the step convergence tolerance 
  /*!
    The steps are stored in the #step vector.
    The convergence tolerance is stored in #stepTolerance.
  */
  bool isStepConverged() const;

  //! Returns true of #direction is empty, false otherwise.
  bool empty() const;
  //@}


  //@{ \name Manipulators

  /*! \brief Computes a new set of search directions based on a new best point and 
    resets the step information.

    Search directions are computed by one of three private member functions:
    - buildWithNothing(),
    - buildWithLapack(),
    - buildWithCddlib().

    If only bound constraints are present, search direction are computed
    exclusively by buildWithNothing().  
    If general linear constraints are present, a call is first made
   to buildNormalCone().  This method returns the following:
   - either a boolean value of true signifying that search directions for
     the current normal cone have already been generated and is stored
     in private member \e directionCache, or
   - a boolean value of false and the current normal cone.

   If the search directions are already in \e directionCache, computeNewDirections()
   returns these directions.
   If buildNormalCone() returns false, search directions must be computed.

   \b Computing \b new \b search \b directions:

   generateUnconstrained() is called whenever the problem looks locally unconstrained,
   i.e. the normal cone \f$ = \{ 0 \}. \f$
   Otherwise the search directions are computed with buildWithLapack()
   if possible.  If buildWithLapack() is unsuccessful (this can happen if
   either the nearby constraints are degenerate or if APPSPACK was configured
   without LAPACK) buildWithCddlib() is then called.

   Finally, if user parameter "Add Projected Normals" is true, the projected
   normals are also added to the list of search directions.

   On exit the set of newly computed search direction are added to the direction  
   cache and the current search directions are updated.

    computeNewDirections() also resets the step information.
    The initial step (which is the same for all directions) is calculated as
    \f[
    \Delta_{\rm new} = \max \{ \Delta_{\rm old}, \Delta_{\min} \}.
    \f]
    Here, \f$\Delta_{\min}\f$ is #minStep.
  */
  void computeNewDirections(const Point& newPoint);

  //! Adds new directions when point remains the same but step is reduced.
  /*! The purpose of appendNewDirections is to handle the case when the step size
      is reduced.  Because APPSPACK is asynchronous, the step size may be reduced
      for a given direction before the current point is rejected.  In a sense, we
      do not know yet if \f$ x_{k+1} = x_k \f$, signifying a failed iteration, or
      if a new best point will be found.  For failed iterations, the current point
      remains the same and the step size is reduced.  Reducing the step size may alter
      the relevant tangent cone, i.e. fewer constraints will be seen as blocking placing
      increasing the number of relevant search directions.
      Thus, to remain theoretically convergent, trial-points must be appended to the 
      queue corresponding to these new directions in the advent that the current iteration
      fails.  For this reason, appendNewDirections() will be called each time the reduced
      step
      \f$ \hat \Delta_{k}^i \f$ corresponding to the ith direction \f$d_i\f$ satisfies
      \f[
         \hat \Delta_{k}^i < \min_{ 1 \le i \le n_d} \Delta_k^i
      \f]
      where \f$ n_d = \f$ equals the number of search directions.  
      \see computeNewDirections().
  */
  void appendNewDirections();

  //! Set the true step and tag for the trial point corresponding to direction i.
  void setTrueStepAndTag(int i, double trueStep_in, int tag_in);

  //! Reduce the step corresponding to direction i. 
  /*! 
    The new step is calculated as 
    \f[
    \Delta_{\rm new} = 0.5 \Delta_{\rm old}.
    \f]
    Also resets the corresponding trueStep and tag. 
  */
  void reduceStep(int i);

  //@}

  //@{ \name Printing

  //! %Print the directions, preceded by the given label
  void print(const string label) const;

  //! Returns the smallest step size for the direction that have not already converged.
  double getSmallestStep() const;
  //@}

private:

  //@{ \name Cone builders.

  /*! \brief This method build the normal cone generators based on the current
    setting of the state vector #constraintState.

    Let \f$V_p\f$ denote a matrix whose columns are obtained from the outward
    pointing normals of the active inequality constraints which are active
    on a single side.

     Let \f$ V_\ell \f$ denote a matrix
    whose columns are obtained from the normals of the inequality constraints which
    are active at both their upper and lower bounds and the equality constraints.

    Then the normal cone is given by
    \f[
    \mathcal{N}(x,\epsilon) = \{v |\; v = V_p \lambda + V_\ell \alpha, \lambda > 0 \}.
    \f]
    
    \param VpT (output) The transpose of \f$V_p\f$.
    \param VlT (output) The transpose of \f$V_\ell\f$.

    For more on how the active set is determined see updateConstraintState().
  */
  void buildNormalCone(Matrix& VpT, Matrix& VlT) const;

  //! This method builds the tangent cone base upon VpT and VlT. 
  /*! This method assumes the normal cone is given by
    \f[
    \mathcal{N}(x,\epsilon) = \{v |\; v = V_p \lambda + V_\ell \alpha, \lambda > 0 \}.
    \f]
    \param VpT (input) the transpose of \f$ V_p \f$
    \param VlT (input) the transpose of \f$ V_\ell \f$
    \param T (output) generator for the tangent cone.

    If VpT and VlT are empty, the method will simply call generateUnconstrained().  Otherwise
    buildWithLapack() is called.  If buildWithLapack() is unable to compute the tangent
    cone, buildWithCddlib() will next be called.  This can happen if either APPSPACK has
    been intentionally configured without LAPACK, or the normal cone corresponds to degenerate
    constraints.
  */
  void buildTangentCone(const Matrix& VpT, const Matrix& VlT,
			  Matrix& T);
	 
  //! Generate compass search directions for the variable bounds only case.
  /*!
    This method in unique in that it does not require LAPACK or CDDLIB.  Thus APPSPACK
    can reliably run (whenever only variable bounds are present) without either package.
  */
  void buildWithNothing(Matrix& D);
		       
  //! Generates pattern search directions using CDDLIB.
  /*! A positive spanning set for the tangent to the normal cone
    \f[
    \mathcal{N} = \{v |\;  v = V_p \lambda + V_\ell \alpha, \lambda > 0 \}
    \f]
    is generated by a call to compute_cone_generators(),
    where \f$ V_p \f$ and \f$ V_\ell\f$ are inputed in transpose form.
    
    \note Method is called only if general linear constraints exists and
    call to \ref APPSPACK::Directions::buildWithLapack() returns false.
    \param VpT (input) Transpose of \f$ V_p \f$.
    \param VlT (input) Transpose of \f$ V_\ell \f$.
    \param T (output) The tangent cone generators.

    \see For more information on the cddlib library see \ref pageCddlib.
   */
  bool buildWithCddlib(const Matrix& VpT, const Matrix& VlT,
			  Matrix& T);

  //! Generate search directions using LAPACK
  /*!
    The inputs define the \f$\epsilon\f$-normal cone
    \f[
    \mathcal{N} = \{v |\; v =  V_p \lambda + V_\ell \alpha, \lambda \geq 0 \}.
    \f]
    (Note that \f$V_p\f$ and \f$ V_\ell\f$ are provided in transpose
    form.)  The directions we need are the generators of the tangent
    cone \f$ \mathcal{T} = \mathcal{N}^\circ \f$.
   
    Let \f$ Z \f$ be a matrix whose columns form a basis for \f${\rm
    null}(V_\ell)\f$; let \f$ R \f$ be a right inverse for \f$ V_p^T Z
    \f$ (if it exists), i.e., \f$ V_p^T Z R = I\f$; and let \f$ N\f$
    be a matrix whose columns form a basis for \f${\rm null}(V_p^T
    Z)\f$.  

    Then: \f[ \mathcal{T} = \{w |\; w = -ZRu + ZN\xi, u \geq 0
    \}, \f]
    
    \param VpT (input) Transpose of \f$ V_p \f$. 
    \param VlT (input) Transpose of \f$ V_\ell \f$.
    \param T (output) The tangent cone generators.

    \return False if a right inverse for \f$ V_p^T Z \f$ does not
    exist; the search directions cannot be calculated with
    LAPACK. Otherwise, it returns true.
  */
  bool buildWithLapack(const Matrix& VpT, const Matrix& VlT,
			  Matrix& T);

  //@}
  
  //@{ \name Direction generating methods

  //! Generates compass search directions for the "looks locally unconstrained" case.
  void generateUnconstrained(Matrix& D);
 
  /*!\brief This method is called to generate search directions whenever
    general linear constraints are present. 
    
    The method first checks the direction cache, #directionCache, directions for the
    current setting of #constraintState have already been computed.  If the corresponding
    search directions have been found within the cache, input parameter D is updated
    accordingly.  If search directions are not found in the cache, the normal cone
    generators N are formed and the tangent cone T computed (stored by rows).

    If no tangent directions are found, i.e. T is empty, then
    - D = T.
    - D (an empty matrix) is cached with #constraintState.
    - If T is empty, the projected normals are never add to D.

    If nontrivial tangent directions are found, then
    - D = T if #withNormals = true.  
    - D = [T; PN] otherwise. Here PN is used to denote the projected normals, see
    addNormalDirections().
    - Prior to exiting, D is cached with #constraintState.

    \note \li #withNormals is a user-definable parameter via string "Add Projected Normals".
          \li By always returning D empty whenever T is empty we ensure that we never return
    a nontrivial set of infeasible directions.

   */
  void generateForLinear(Matrix& D);
			 
  //! Methods adds in the projected normals of the active linear inequality constraints.
  /*!
    This method assumes the normal cone is given by
    \f[
    \mathcal{N}(x,\epsilon) = \{v |\; v = V_p \lambda + V_\ell \alpha, \lambda > 0 \}.
    \f]
    \param VpT (input) the transpose of \f$ V_p \f$
    \param VlT (input) the transpose of \f$ V_\ell \f$
    \param D (input/output)  on exit, the rows of \f$ V_p^TZZ^T \f$ are added to the bottom
    of matrix D after first being normalized then scaled.
    Here \f$ Z \f$ denotes an orthonormal basis matrix for the null space of \f$ V_\ell^T \f$.

    \warning addNormalDirections() requires LAPACK to perform projection into the null-space
    of \f$ V_\ell^T \f$.  Thus if \f$ V_\ell\f$ is not empty and #withNormals equals true,
    APPSPACK will throw an error and exit.  A subtle point is that \f$ V_\ell \f$ 
    can be non-empty despite the absence of equality constraints. See buildNormalCone().
  */
  void addNormalDirections(const Matrix& VpT, const Matrix& VlT,
			   Matrix& D);

  //! Method adds projected compass search direction to D
  /*!
    \param VlT (input) the transpose of \f$ V_\ell \f$
    \param D (input/output)  on exit, the rows of \f$ \pm ZZ^T\f$ are added to the bottom
    of matrix D after first being normalized then scaled.

    Here \f$ Z \f$ denotes an orthonormal basis matrix for the null space of \f$ V_\ell^T \f$.
    \note If \f$VlT\f$ is empty, then the classical compass search directions
    \f[
    \{\pm e_1, \pm e_2, \ldots, \pm e_n \}
    \f] are added after being scaled.
    \warning addNormalDirections() requires LAPACK to perform projection into the null-space
    of \f$ V_\ell^T \f$.  Thus if \f$ V_\ell\f$ is not empty and #withCompass equals true,
    APPSPACK will throw an error and exit.  A subtle point is that \f$ V_\ell \f$ 
    can be non-empty despite the absence of equality constraints.
  */
  void addCompassDirections(const Matrix& VlT, Matrix& D);
			   
  //! Updates the state vector #constraintState according to distances stored within xDistance. 
  /*! 
    To determine which constraints are active (or near active), 
    an epsilon ball \f$\mathcal{B}(x,\epsilon)\f$ is defined about the current point,
    with x denoting the current point and \f$ \epsilon \f$ defined by
    \f[
    \epsilon = \min(\epsilon_{\rm max}, \Delta_k).
    \f]
    Here \f$\epsilon_{\rm max} = \f$#epsilonMax, and \f$ \Delta_k \f$ is given by 
    input parameter newStep.
    All constraints which pass through this epsilon ball are considered active.
    Essentially, the \f$i_{th}\f$ constraint is considered active if 
    \f[
    \frac{|a_i^T x - b_i|}{\|a_i\|_2} \le \epsilon.
    \f]
    Activity of constraints is always determined in the scaled space.  
    This method updates #constraintState via a call to Constraints::Linear::getActiveIndex(),
    passing in as parameters #xDistance and epsilon.  The parameter #xDistance provides a measure
    of the distance to each inequality constraint from the current point; it is assumed that
    #xDistance has already been updated prior to function call.
    \returns False, if no changes were made to #constraintState, i.e. the active index remained the same
    signifying no updates to the current set of search directions are needed.  
    Otherwise the method returns true.

    \see For more on how active constraints are determined and described by
    #constraintState see Constraints::Linear::getActiveIndex().
*/
  bool updateConstraintState(double newStep);
  
  //! Updates the step, trueStep, and tag information.
  /*!
    \param newStep Step value given to each new directions.
    \param isAppend If false, direction information will
    be updated for all rows in #direction.  Otherwise, direction
    information will be updated only for recently added rows.
    \note When isAppend=true, the first #nDirections rows of #direction
    are considered old; the remaining rows are considered new additions.
  */
  void updateDirectionInfo(double newStep, bool isAppend=false);
  //@}

private:
  
   //! The constraints object
  const Constraints::Linear& constraints;

  //! The number of dimensions
  const int nDimensions;

  //! The vector of all zeros
  const Vector zero;

  //! The step tolerance used to test convergence, etc.
  const double stepTolerance;

  //! Minimum size of the step after a new minimum is found
  const double minStep;

  //! Contraction parameter
  const double theta;

  //! Defines maximum radius of about a current point used to determine active constraints.
  const double epsilonMax;

  //! The current number of search directions
  int nDirections;

  //! The search directions are stored as rows of this matrix
  Matrix direction;

  //! The steps associated with each search direction
  Vector step;

  //! The actual step associated with each search direction
  /*! The step represents the longest <em>feasible</em> step that is
    less than or equal to the corresponding delta.
  */
  Vector trueStep;

  //! The trial point tags corresponding to the search directions
  /*! A value of -1 means that there is no trial point associated with
    the given direction.
  */
  vector<int> tag;

  //! A temporary vector
  Vector tmpVector;

  //! A temporary integer vector
  mutable vector<int> idxVector;

  //! Stores a cache of previously computed search directions.
  /*!
    A vector<\ref Constraints::ActiveType> is paired with a corresponding set of search
    directions (stored as a Matrix) in a map to form a
    direction cache.  Each Matrix stored in \e directionCache consists of
    - generators for the tangent cone, and possibly,
    - projected generators for the normal cone (if parameter
    "Add Projected Normals" is true).

    \see Constraints::Linear::getActiveIndex()
    \note caching is only used if general linear constraints are present.
  */
  map< vector<Constraints::ActiveType>, Matrix>  directionCache;

  //! An iterator to be used in conjunction with the cached directions.
  map< vector<Constraints::ActiveType>, Matrix>::iterator  cacheIter;
  
  //! Stores a unique description of the tangent cone at the current point.
  /*!
    A call to Constraints::Linear::getActiveIndex()
    returns a vector of type \e enum Constraints::ActiveType
    which uniquely identifies the tangent cone at a given point.
 */
  vector<Constraints::ActiveType> constraintState;

  //! Records the last value of epsilonMin used to determine active set.
  double epsilonMin;
  
  //! If true, the projected constraints normals are added to #direction. 
  bool withNormals;  

  //! If true, the projected compass search directions are added to #direction. 
  bool withCompass;  

  //! Records number of time cached directions are used.
  int nCached;
  
  //! Records number of times directions are generated using LAPACK.
  int nLapack;
  
  //! Records number of times directions are generated using cddlib.
  int nCddlib;

  //! Records the maximum number of directions used per iteration.
  int nMaxDirections;

  //! Records the number of times appendNewDirections() is called and nontrivial directions added.
  int nAppend;

  //! Stores distance from current point to each boundary wrt #Constraints::Linear::aHat.
  Vector xDistance;

};

}

#endif
