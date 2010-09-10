// $Id: APPSPACK_Constraints_Linear.hpp,v 1.59.2.1 2007/01/14 21:51:27 jgriffi Exp $ 
// $Source: /space/CVS-Acro/acro/packages/appspack/appspack/src/APPSPACK_Constraints_Linear.hpp,v $ 

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
  \file APPSPACK_Constraints_Linear.hpp
  \brief Class definition for APPSPACK::Constraints::Linear
*/


#ifndef APPSPACK_CONSTRAINTS_LINEAR_HPP
#define APPSPACK_CONSTRAINTS_LINEAR_HPP

#include "APPSPACK_Vector.hpp"
#include "APPSPACK_Matrix.hpp"
#include "APPSPACK_Parameter_List.hpp"

namespace APPSPACK
{

//! Namespace for constraint-related objects
namespace Constraints
{

/*! \brief State of an individual (upper or lower, but not both)
  constraint with respect to a given point. 
*/
enum StateType {
  //! Constraint does not exist, i.e., bound is \f$ \pm \infty \f$
  DNE,
  //! Violated - constraint is violated at point 
  Violated,
  //! Active - constraint is active at point (within a tolerance)
  Active,
  //! Inactive - constraint is not active and point is feasible
  Inactive
};

//! Specify upper or lower bound type
enum BoundType {
  //! Upper bound
  UpperBound,
  //! Lower bound
  LowerBound
};


/*! \brief State of an individual constraints with respect to 
both upper and lower bounds and a given point. 

A constraints
is determined active with respect to a given tolerance,
\f$ \epsilon \f$.  Thus, if \f$ \epsilon \f$ is sufficiently
large, it is possible that both upper and lower bounds may be
considered active.
 */
enum ActiveType {
  //! Constraint is not active at given point.
  NeitherActive,
  //! Lower bound on constraint is active at given point.
  LowerActive,
  //! Upper bound on constraint is active at given point.
  UpperActive,
  //! Both upper and lower bound on constraint are active at given point.
  BothActive
};
  

//! Constraint class that implements general linear inequality constraints
/*! 
We consider the following linearly constrained problem
\f[
\begin{array}{rlcrcl}
\displaystyle\mbox{minimize} & \multicolumn{5}{c}{f(x)} \\
x \in R^n \\
\mbox{subject to} & b^{\rm lower} & \le & x & \le & b^{\rm upper}\\
& b^{\rm ineq\_lower} & \le & A^{\rm ineq} x & \le & b^{\rm ineq\_upper} \\
& & & A^{\rm eq} x & = & b^{\rm eq} 
\end{array}
\f]
The bounds in the inequalities are not required to be finite, e.g., it
is allowable that \f$b^{\rm lower} = -\infty\f$.

Because it can dramatically improves performance, variable scaling is
used. The scaling vector is denoted by \f$s\f$.
The scaling can be specified by the user; see \ref pageLinearParameters.
If all variable bounds are finite, a default scaling vector is provided; namely
\f[
s_i = u_i - \ell_i.
\f]
In the case where any element of \f$x\f$ is unbounded,
the user is required
to specify \f$s\f$ within the \ref pageLinearParameters.

We define the variables in the scaled space as follows:
\f[
\begin{array}{lcl}
\tilde{x}                   &=& S^{-1}(x - \hat{\ell}),  \\
\tilde{f(\tilde x)}         &=& f(S \tilde x + \hat{\ell}) = f(x), \\
\tilde{b}^{\rm lower}       &=& S^{-1}(b^{\rm lower} - \hat{\ell}), \\
\tilde{b}^{\rm upper}       &=& S^{-1}(b^{\rm upper} - \hat{\ell}), \\
\tilde{b}^{\rm ineq\_lower} &=& b^{\rm ineq\_lower} - A^{\rm ineq} \hat{\ell}, \\
\tilde{b}^{\rm ineq\_upper} &=& b^{\rm ineq\_upper} - A^{\rm ineq} \hat{\ell}, \\
\tilde{b}^{\rm eq}          &=& b^{\rm eq} - A^{\rm eq} \hat{\ell}, \\
\tilde{A}^{\rm ineq}        &=& A^{\rm ineq} S, \\
\tilde{A}^{\rm eq}          &=& A^{\rm eq} S.
\end{array}
\f]
Here
\f[
S = \left(\begin{array}{ccc}
s_1 \\
& \ddots \\
& &  s_n    
\end{array}\right)
\f]
and 
\f[
\hat{\ell}_ = \left\{ 
\begin{array}{cl}
b^{\rm lower}_i & {\rm if } \; b^{\rm lower}_i > -\infty \\
0 & {\rm otherwise}.
\end{array}
\right.
\f]
The scaled problem is then given by
\f[
\begin{array}{rlcrcl}
\displaystyle\mbox{minimize} & \multicolumn{5}{c}{\tilde f(\tilde x)} \\
\tilde x \in R^n \\
\mbox{subject to} & \tilde b^{\rm lower} & \le & \tilde x & \le & \tilde b^{\rm upper}\\
& \tilde b^{\rm ineq\_lower} & \le & \tilde A^{\rm ineq} \tilde x & \le & \tilde b^{\rm ineq\_upper} \\
& & & \tilde A^{\rm eq} \tilde x & = & \tilde b^{\rm eq} 
\end{array}
\f]

Combining the bound and linear inequality constraints, we get
\f[
\begin{array}{rlcrcl}
\displaystyle\mbox{minimize} & \multicolumn{5}{c}{\tilde f(\tilde x)} \\
\tilde x \in R^n \\
\mbox{subject to} 
& \hat b^{\rm lower} & \le & \hat A^{\rm ineq} \tilde x & \le & \hat b^{\rm upper} \\
& & & \tilde A^{\rm eq} \tilde x & = & \tilde b^{\rm eq} 
\end{array}
\f]
where
\f[
\begin{array}{ccccc}
\hat{b}^{\rm lower} = \left(\begin{array}{l}
\tilde{b}^{\rm lower} \\
\tilde{b}^{\rm ineq\_lower}
\end{array}
\right), 
  & &
\hat{b}^{\rm upper} = \left(\begin{array}{l}
\tilde{b}^{\rm upper} \\
\tilde{b}^{\rm ineq\_upper}
\end{array}
\right), 
&  &
\hat{A} = \left(\begin{array}{l}
I \\
\tilde{A}^{\rm ineq}
\end{array}
\right). 
  \end{array}
\f]

With respect to the scaled system, \ref APPSPACK::Constraints::Linear
stores the following as member variables:
\f$ \hat{A} \f$,  \f$\hat{b}^{\rm lower}\f$, \f$\hat{b}^{\rm upper}\f$,
\f$ \tilde{A}^{\rm eq} \f$, and \f$\tilde{b}^{\rm eq}\f$.

*/
class Linear
{

public:
  //! Constructor.
  /*!  \param params (input/output) See \ref pageLinearParameters for
    details of what it should contain. Defaults are inserted for any
    values that are not defined.
  */
  Linear(Parameter::List& params);

  //! Destructor.
  ~Linear();


  //@{ \name Accessors
  //! Returns machine epsilon value, #epsMach.
  double getEpsMach() const;

  //! Returns scaling vector, #scaling.
  const Vector& getScaling() const;

  //! Return vector of lower bounds, #bLower.
  const Vector& getLower() const;

  //! Return vector of upper bounds, #bUpper.
  const Vector& getUpper() const;
  
  //! Return lower bound on scaled inequality constraints, #bIneqLower. 
  const Vector& getBhatLower() const;

  //! Return upper bound on scaled inequality constraints, #bIneqUpper.
  const Vector& getBhatUpper() const;

  //! Return right-hand side vector of scaled equality constraints, #bEq.
  const Vector& getBtildeEq() const;

  //! Return coefficient matrix for scaled inequality constraints, #aIneq.
  const Matrix& getAhat() const;

  //! Return coefficient matrix for scaled equality constraints, #aEq.
  const Matrix& getAtildeEq() const;

  //! Returns a point (x) that is initially feasible with respect to the bound constraints.
  /*! \note This point may not be feasible with respect to the linear constraints. */
  void getNominalX(Vector& x) const;
  
  //@}

  //@{ \name Projectors.
  //! Maps x to scaled space via affine linear transformation.
  /*! Replaces \f$x\f$ with \f$ S^{-1} (x - \hat{\ell}) \f$. */
  void scale(Vector& x) const;
  
  //! Maps w to unscaled space via affine linear transformation.
  /*! Replaces \f$w\f$ with \f$ Sw + \hat{\ell} \f$. */
  void unscale(Vector& w) const;

  //@}
  
  //@{ \name  Feasibility verifiers.
  //! Return true if feasible, false otherwise.
  /*!  
    We consider the point x to be feasible with respect to a given
    constraint if it is a distance of no more than #epsMach away;
    see getIneqState() and getEqState().
  */
  bool isFeasible(const Vector& x) const;
  
  //! Throws an error if x is infeasible; see isFeasible().
  void assertFeasible(const Vector& x) const;
  
  //@}

  //@{ \name Constraint analysis.

  //! Returns true if only variable bounds exists, false otherwise.
  bool isBoundsOnly() const;

  //! Returns maximum feasible step in interval [0, step] along direction d.  
  /*!
    \param x (input) current point
    \param d (input) search direction
    \param step (input) maximum step

    On exit a nonnegative scalar \f$ \alpha \f$ is returned 
    that gives the maximum feasible step that can be made 
    along direction d.  
    A constraint is said to be a \a blocking \a constraint if
    d points towards its infeasible region.
    A finite variable lower bound is considered to be a blocking
    constraint if \f[ d_i < -scaling[i]*\epsilon. \f]  Similarly,
    a finite variable upper bound is considered to be a blocking
    constraint if \f[ d_i > scaling[i]*\epsilon. \f]
    A general linear inequality \f$ a_i^T x \le b_i \f$
    is considered to be a blocking constraint if
    \f$ a_i^T d > \epsilon \f$. Here \f$\epsilon\f$ equals the
    the user modifiable parameter #epsMach.

    If any blocking inequality constraint is deemed active,
    via a call to \ref APPSPACK::Constraints::Linear::getIneqState, 
    a value of \f$ 0\f$ is returned.  
    Otherwise,
    \f[
    \alpha = {\rm min} \left(\{\Delta\} \cup  
    \left\{\frac{b_i - a_i^T x}{a_i^T d} \Big |\; i \in \mathcal{B}\right\}\right)
    \f]
    is returned, where \f$ \mathcal{B}\f$ denotes the set of blocking inequality constraints.

    \note We require that d lie in the null space (numerically at least)
    with respect to the equality constraints.  d is said to lie in the
    null space if \f$ a_i^T d < \epsilon\f$ for all equality constraints.
    If \f$ d\f$ does not lie in the null space, a value of zero is returned.  
  */
  double maxStep(const Vector& x, const Vector& d, double step) const;

  //! Returns a vector indexing bound type with respect to inequality constraints.
  /*!
    \param xdist Records distance to each inequality constraint from a given point.  It is
    assumed that \em xdist has been
    formed via a call to formDistanceVector().
    \param epsilon The ith inequality constraint is considered active at its lower bound if
    \f[
    {\rm  xdist}(i)  <  {\rm epsilon}.
    \f]
    Similarly, the ith inequality  constraint is consider
    active at its upper bound if 
    \f[
    {\rm xdist}(i + m )  < {\rm epsilon}.
    \f]
    Here \f$ m \f$
    denotes the number of rows in \f$ \hat A^{\rm ineq} \f$.
    \param index On exit index[i] equals one of the enumeration Constraints::ActiveType. 
    - index[i] =  NeitherActive if the \f$i_{ith}\f$ constraint is
    inactive at both its lower and upper bounds. 
    - index[i] = LowerActive if the \f$i_{ith}\f$ constraint is
    active at its lower bound, but not its upper bound. 
    - index[i] =  UpperActive if the \f$i_{ith}\f$ constraint is
    active at its upper bound, but not its lower bound. 
    - index[i] =  BothActive if the \f$i_{ith}\f$ constraint is
    active at both its upper and lower bound. 				
  */
  void getActiveIndex(const Vector& xdist, double epsilon, vector<ActiveType>& index) const;

  //! Returns (nullspace projected) distance from x to each inequality constraints.
  /*!
    \param x (input) A given point.
    \param xdist (output) Gives the distance from x to each inequality constraints
    when moving within the nullspace of the scaled equality constraints.

    Distance is always determined in terms of the scaled system (for
    a detailed description see Linear):
    \f[
    \hat b^{\rm lower} \le  \hat A^{\rm ineq} \tilde x  \le  \hat b^{\rm upper}.
    \f]
    In order to differentiate between lower bounds and upper bounds, the inequality
    constraints are implicitly stacked as follows:
    \f[
    \left(\begin{array}{c} -\hat A^{\rm ineq} \\ \hat A^{\rm ineq} \end{array}\right)
    \tilde x \le 
    \left(\begin{array}{c} -\hat b^{\rm lower} \\ \hat b^{\rm upper}\end{array}\right).
    \f]
    Thus on exit, the length of xdist is necessarily twice the number of rows in
    \f$ \hat A^{\rm ineq} \f$.
    If we let
    \f[
    C = \left(\begin{array}{c} -\hat A^{\rm ineq} \\ \hat A^{\rm ineq} \end{array}\right),
    \; {\rm and } \; 
    d = \left(\begin{array}{c} -\hat b^{\rm lower} \\ \hat b^{\rm upper}\end{array}\right),
    \f]
    and let \f$\varepsilon\f$ denotes #epsMach and \f$Z\f$ denotes a matrix whose
    columns form an orthonormal basis for \f${\rm null}(\tilde A^{eq})\f$, we can
    define xdist as follows:

    If \f$|d_i| < \infty \f$the entries of xdist are given by
    \f[
    {\rm xdist}(i) = \left\{
    \begin{array}{ll}
    | c_i^T \tilde x - d_i | / \|Z^T c_i\|_2 & {\rm if } \; \|Z^T c_i\| > \epsilon, \\
    0 & {\rm if }  \; \|Z^T c_i \| < \epsilon, 
    \;{\rm and }\; |c_i^T \tilde x - d_i| < \epsilon, \\
    \infty &  {\rm if } \; \|Z^T c_i \| 
    < \epsilon \;{\rm and }\; |c_i^T \tilde x - d_i| > \epsilon. \\
    \end{array}
    \right.
    \f]
    If \f$|d_i| = \infty\f$ then \f${\rm xdist}(i) = \infty\f$.  

    \note We are computing distance constrained to movement within the
    nullspace of the equality constraints.  In exact arithmetic, the
    distance \f$d(x,a,b)\f$ from a point \f$x\f$ to the constraint
    \f$a^T y \le b\f$ within the space spanned by the orthonormal
    matrix \f$Z\f$ is given by
    \f[d(x,a,b) = \left\{
    \begin{array}{ll}
    | a^T x - b | / \|Z^T a\|_2 & {\rm if }\;  Z^T a \ne 0, \\
    0 & {\rm if } \;  Z^T a = 0 \; {\rm and } \; |a^T x - b| = 0, \\
    \infty & {\rm if }\; Z^T a  = 0 \; {\rm and } \; |a^T x - b| > 0. \\
    \end{array}
    \right.
    \f]
  */
  void formDistanceVector(const Vector& x, Vector& xdist) const;

  //! Attempts to find the closest point to x that satisfies all constraints within distance esnap.
  /*! 
     \param x Current point.
     \param esnap Used to define epsilon ball about x to determine which constraints are
                  nearby.

     The primary purpose of this method is to avoid scenarios where 1) the 
     true solution lies on the boundary, and 2) the boundary is
     is asymptotically being approached but never reached.
     In order to avoid this "half-stepping" to the boundary, snapToBoundary()
     attempts to generate the closest point to the current point x that satisfies all
     constraints lying within the given distance esnap.
     The method begins by calling findNearbyConstraints() to determine all constraint 
     within esnap of the current point x.    
     A matrix \f$A_{\rm esnap}\f$ is then formed that consists of the rows
     \f$\tilde A^{Eq} \f$ followed by the rows of \f$\hat A \f$ that correspond to
     nearby constraints.
     An LQ factorization of \f$A_{\rm esnap}\f$ is then formed to determine
     a linearly independent subset of \f$A_{\rm esnap}\f$.
        \f$A_{\rm esnap}\f$ is then redefined (if necessary) so that 
     \f$A_{\rm esnap}\f$ has full row rank.   The corresponding 
     right-hand side vector \f$ b_{\rm esnap}\f$ is at this time also formed. 
    The solution \f$ y^\ast \f$ to the below equality-constrained least-squares
    problem is then computed:
    \f[
    \begin{array}{cc}
   minimize & \|y - x\| \\
   subject  & A_{\rm esnap} y = b_{\rm esnap}.
   \end{array}
    \f]
    On exit, x is set equal to \f$y^\ast \f$.

    Method requires that APPSPACK be configured with LAPACK.
  */
  void snapToBoundary(Vector& x, double esnap) const;

  //! Makes x feasible with respect to variable bounds.
  /*!
    On exit,  
    \f[x_i = \left \{ 
    \begin{array}{cc}
    \ell_i &  {\rm if } \; x_i < \ell_i \\
    u_i     &  {\rm if } \; x_i > u_i \\
    x_i     &  {\rm otherwise}.
    \end{array}
    \right. \f]
    Here \f$ \ell \f$ and \f$u\f$ are used to denote the lower and upper bounds
    of x, respectively.
  */
  void makeBoundsFeasible(Vector& x) const;

  //! Forms system Asnap, and bsnap.  On exit Asnap has full row rank.
  /*! Add more comments */
  void formSnapSystem(const Vector& xtilde, double esnap,
		      Matrix& Asnap, Vector& bsnap) const;

  //@}

  //! Prints the bounds, scaling, and general linear constraint information.
  void print() const;

private:

  //@{ \name Constructor assistance.

  //! Used by constructor to fill the parameter list with appropriate defaults and return the scaling.
  void setup(Parameter::List& params);

  //! Sets up lower and upper bounds.
  void setupBounds(Parameter::List& params);

  //! Sets up scaling vector.
  void setupScaling(Parameter::List& params);
  
  //! Form the corresponding right-hand side vector of the coefficient matrix.
  void setupRhs(Parameter::List& params);
  
  //! Form the coefficient matrix wrt bound and linear constraints.
  void setupMatrix(Parameter::List& params);

  //! Form the corresponding scaled right-hand side vector of the coefficient matrix.
  void setupScaledSystem();  

  //@}

  //@{ \name Constraint categorizers.

  //! Get state of inequality constraint i with respect to the given point.
  /*!  Computes the state of inequality constraint i with respect to
    point \f$ \tilde x \f$. (It is assumed the point \f$ \tilde x \f$
    is scaled.)

   Constraint i is deemed active if the (scaled) point is within a
    distance of \f$\varepsilon\f$ of the (scaled)
    constraint. Mathematically speaking, for constraint i with respect
    to the lower bound, that means

    \f[
    \frac{ | \hat a_i^T \tilde x - \hat b_i^{\rm lower} | } { \| \hat a_i \| } \leq \varepsilon
    \f]
    
    \param i (input) Constraint index.  Let \f$n\f$ denote the number
             of variables, i.e., the size of \f$ \tilde x \f$. Let
             \f$m\f$ denote the number of linear inequality
             constraints, i.e., the number of rows in \f$A^{\rm
             ineq}\f$. Let \f$p = n + m\f$.  %Constraints 0 thru
             \f$n-1\f$ correspond to the bound constraints, and
             constraints \f$n\f$ thru \f$p-1\f$ correspond to the
             linear inequality constraints.
		    
   \param bType (input) Specify upper or lower bound; see ::BoundType.

   \param xTilde (input) Vector in scaled space.

   \param epsilon (optional input) A point is considered active if it
                  is less than this distance from the constraint (in
                  the scaled space). Defaults to #epsMach.

  */
  StateType getIneqState(int i, BoundType bType, const Vector& xTilde, double epsilon = -1) const;

  //! Get state of equality constraint i with respect to the given point.
  /*!  Computes the state of equality constraint i with respect to
    point \f$ \tilde x \f$. (It is assumed the point \f$ \tilde x \f$
    is scaled.)

    Constraint i is deemed satisfied if the (scaled) point is within a
    distance of \f$\varepsilon\f$ of the (scaled)
    constraint. Mathematically speaking, for constraint i with respect
    to the lower bound, that means

    \f[
    \frac{ | (\tilde a_i^{\rm eq})^T \tilde x - \tilde b_i^{\rm eq} | } { \| \tilde a_i^{\rm eq} \| } \leq \varepsilon
    \f]
    
    \param i (input) Constraint index.  Let \f$n\f$ denote the number
             of variables, i.e., the size of \f$ \tilde x \f$. Let
             \f$m\f$ denote the number of linear inequality
             constraints, i.e., the number of rows in \f$A^{\rm
             ineq}\f$. Let \f$p = n + m\f$.  %Constraints 0 thru
             \f$n-1\f$ correspond to the bound constraints, and
             constraints \f$n\f$ thru \f$p-1\f$ correspond to the
             linear inequality constraints.
		    
   \param xTilde (input) Vector in scaled space.

   \param epsilon (optional input) A point is considered satisfied if it
                  is less than this distance from the constraint (in
                  the scaled space). Defaults to #epsMach.

  */
  StateType getEqState(int i, const Vector& xTilde, double epsilon = -1) const;

  //! Returns true if equality constraints are feasible.
  /*! see getEqState() */
  bool isEqualityFeasible(const Vector& xtilde) const;

  //! Returns true if inequality constraints are feasible.
  /*!  see getIneqState() */
  bool isInequalityFeasible(const Vector& xtilde) const;

  //@}

  //! %Print an error message and throw an exception.
  void error(const string& fname, const string& msg) const;
  
  //! Error checks on input
  void errorCheck() const;

private:
  
  //! Machine epsilon
  const double epsMach;

  /*! If true, equality and inequality constraint information will be printed explicitly
    for certain debug levels.
  */
  bool displayMatrix;

  //! Scaling
  Vector scaling;
  
  //! Unscaled lower bounds.
  Vector bLower;
  
  //! Unscaled upper bounds.
  Vector bUpper;

//@{ \name Unscaled-sytem members.
  //! The coefficient matrix of the unscaled general linear inequality constraints.
  Matrix aIneq;

  //! The coefficient matrix of unscaled linear equality constraints.
  Matrix aEq;

  //! The unscaled lower bounds on general linear inequality constraints.
  Vector bIneqLower;

  //! The unscaled upper bounds on general linear inequality constraints.
  Vector bIneqUpper;

  //! The right-hand side of unscaled equality constraints.
  Vector bEq;
//@}

//@{ \name Scaled-sytem members.
  //! The scaled coefficient matrix of the linear (bound and general) inequality constraints.
  Matrix aHat;

  //! Holds two norms of rows of aHat projected into the nullspace of the scaled equality constraints.
  Vector aHatZNorm;

  //! Holds two norms of rows of aHat.
  Vector aHatNorm;
 
  //! The scaled lower bounds on the linear (bound and general) inequality constraints.
  Vector bHatLower;

  //! The scaled upper bounds on the linear (bound and general) inequality constraints.
  Vector bHatUpper;

  //! The scaled coefficient matrix of linear equality constraints.
  Matrix aTildeEq;

  //! The scaled right-hand side of equality coefficient matrix.
  Vector bTildeEq;

  //! Used in transforming x to scaled space.
  /*! Scaled w is related to unscaled x via the affine linear transformation
      x = Sw+lHat. See APPSPACK_Constraints_Linear.hpp */ 
  Vector lHat;
//@}

};

}
}

#endif 
