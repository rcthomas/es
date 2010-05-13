// $Id: APPSPACK_Point.hpp,v 1.15 2006/06/30 01:42:08 tgkolda Exp $ 
// $Source: /space/CVS-Acro/acro/packages/appspack/appspack/src/APPSPACK_Point.hpp,v $ 

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
  \file APPSPACK_Point.hpp
  \brief Class definition for APPSPACK::Point
*/
#ifndef APPSPACK_POINT_HPP
#define APPSPACK_POINT_HPP

#include "APPSPACK_Common.hpp"
#include "APPSPACK_Vector.hpp"
#include "APPSPACK_Combiner_Generic.hpp"

namespace APPSPACK
{
//! A trial point
/*!

  A trial point contains an x-vector either waiting to be evaluated or
  that is already evaluated. Each Point is unique and has an
  associated unique tag. Copying is disallowed.

*/
class Point 
{
public:

  //! The evaluation state of the trial point
  enum State
    {
      //! Trial point violates constraints
      Infeasible,

      //! Trial point has not yet been evaluated 
      Unevaluated,

      /*! \brief Trial point function value has been retrieved from
	the cache, but there is insufficient decrease */
      CachedInsufficientDecrease,
      
      //! Trial point function value has been retrieved from the cache
      Cached,
      
      /*! \brief Trial point has been evaluated successfully, but
	there is insufficient decrease */
      EvaluatedInsufficientDecrease,

      /*! \brief Trial point has been evaluated successfully, and
	there is sufficient decrease */
      Evaluated
    };

   //@{ \name Constructors

  /*!
    Construct a trial point with no parent. This constructor is only
    used for the initial guess.

    \param x_in - The x-vector.
    \param f_in - The function value (which may be unknown).
    \param step_in - The step length.
    \param alpha_in - The value of #alpha to be used by all points.
    \param combiner_in - Object which combines the components stored in
                         f_in to create a scalar objective value.
  */
  Point(const Vector& x_in, const Vector& f_in, 
	double step_in, double alpha_in,
	Combiner::Generic& combiner_in);

  /*!  
    Construct a trial point with a parent. 

    \param x_in - The x-vector to be evaluated.

    \param step_in - The step length.  This is saved as #step and also
    used in the calculation of #rho.

    \param parent - The parent of this trial point. The tag of this
    point is saved as #parentTag for later use in processing the
    resulting function evaluation. Also, the function value is saved
    as #parentValue to save for the sufficient decrease comparison.
    Finally, #alpha is copied.

    \param idx_in - The index of direction used to generate this trial
    point, saved as #idx. This is used in processing the resulting
    function value.
  */
  Point(const Vector& x_in, double step_in, const Point& parent, int idx_in);

  //@}

  //@{ \name Destructors
  /*! Destructor */
  ~Point();
  //@}

  //@{ \name Accessors

  /*! Return x. Needed for extracting points to send to user function evaluator. */
  const Vector& getX() const;

  /*! Return single function value */
  double getF() const;

  /*! Return vector of function values */
  const Vector& getVecF() const;

  /*! Get the tag of this trial point. */
  int getTag() const;

  /*! Get parent's tag. */
  int getParentTag() const;

  /*! Get index of direction the generated this trial point. */
  int getIndex() const;
  
  /*! Return the step associated with this trial point. */
  double getStep() const;

  //@}

  //@{ \name Manipulators

  /*! Store the cached function value in #f. Automatically checks for
    sufficient decrease and sets #state appropriately.
  */
  void setCachedFunctionValue(const Vector& f_in, const string& msg_in);

  /*! Store the evaluated function value in #f. Automatically checks for
    sufficient decrease and sets #state appropriately.
  */
  void setEvaluatedFunctionValue(const Vector& f_in, const string& msg_in);

  /*!  The trial point is infeasible. Set #state appropriately.
  */
  void setInfeasible();

  //@}

  //@{ \name Comparisons

  /*! 
     Basically, the evaluation is based on comparing the two function
     values. To complicate matters, however, we may not have to valid
     function values, which leads to all sorts of special
     cases. Further, if two different points have equal function
     values, we break ties based on the tag.

     We apply the following logic, in order, to determine if this
     point is less than the other.
    
    <ul>
    
    <li> <b>Case I:</b> Tags are equal (i.e., comparing this point to
    itself. Return false.
    
    <li> <b>Case II:</b> This point does not have a valid function
    value. Return false.
    
    <li> <b>Case III:</b> The other point does not have a valid function
    value. Return true.
    
    <li> <b>Case IV:</b> This function value is less than the
    other. Return true.
    
    <li> <b>Case V:</b> The other function value is less than this
    one. Return false.
    
    <li> <b>Case VI:</b> This tag is less than the other tag. Return true.
    
    <li> <b>Case VII:</b> Default. Return false.
    
    </ul>
  */
  bool operator<(const Point& other) const;

  //! Returns true if combiner(f) < combiner(other).
  bool lessThan(const Vector& value) const;

  //! Returns trure if combiner(f) > combiner(other).
  bool greaterThan(const Vector& value) const;
  //@}

  //@{ \name Printing
 
  //! %Print out the point to the given stream. 
  ostream& leftshift(ostream& stream) const;


  //@}

private:

  //! Disallowed
  void operator=(const Point& source);

  //! Disallow copy constructor
  Point(const Point& source);

  //! Check for sufficient decrease as compared to the parent's function value
  bool isSufficientDecrease() const;
  
private:

  //! Static counter used to generate unique tags
  static int staticCount;

  //! Combines vector values stored in class Values.
  Combiner::Generic& combiner;

  //! The x vector
  const Vector x;

  //! The function value (which may be a vector of values), f(x)
  Vector f;

  //! Integer tag value
  const int tag;

  //! The step that was used to create this point  
  const double step;

  //! Message string
  string msg;

  //! Integer tag of parent 
  /*! \note If there is no parent, then the tag is -1 */
  const int parentTag;

  //! Index of corresponding direction
  const int idx;

  //! Parent function value
  const double parentF;


  //! Value of the sufficient decrease parameter
  /*! This is used in the calculation of #rho. 

    This value is set in the Point constructors. A user specified
    value may be provided by specifying the the "Sufficient Decrease
    Factor" parameter in the Parameter::List passed to
    APPSPACK::Solver::Solver(); see \ref pageParameters.
  */
  const double alpha;

  //! Value of \f$\rho\f$ used in sufficient decrease calculation.
  /*! The amount of decrease required is given by,
    \f[
    \rho = \alpha \Delta^2
    \f]
    Here \f$\alpha\f$ is #alpha, and
    \f$\Delta\f$ is #step.
  */
  const double rho;

  //! Trial point state
  State state;

};
}

//! Printing an APPSPACK::Point
ostream& operator<<(ostream& stream, const APPSPACK::Point& point);

//! Printing an APPSPACK::Point::State
ostream& operator<<(ostream& stream, APPSPACK::Point::State state);

#endif
