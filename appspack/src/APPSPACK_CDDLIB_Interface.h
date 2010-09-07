/*
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
*/

/*! 
  \file APPSPACK_CDDLIB_Interface.h

  \brief Prototype for compute_cone_generators() function.

  \author Robert Michael Lewis, College of William & Mary, 2005
*/

#ifndef APPSPACK_CDDLIB_INTERFACE_H
#define APPSPACK_CDDLIB_INTERFACE_H
#include "APPSPACK_Common.h"


#ifdef __cplusplus
extern "C" {
#endif

#ifdef HAVE_CDDLIB

/*!
  \brief Interface function to call CDDLIB

  \param num_pointy (output) The number of generators for the "pointy
  part" of the tangent cone.

  \param P (output) On exit, *P points to a (num_pointy x n) matrix
   stored in double** format whose rows form a positive spanning set
   for the "pointy part" of the tangent cone.  That is, *P[i] points
   to a double array of length n.  If we define \f$p_i\f$ to be the
   mathematical vector stored in *P[i], then \f$\{p_1,\ldots,p_{\rm
   num\_pointy}\}\f$ generates the pointy part of the space.

  \param num_lineality (output) The number of vectors in a basis for
  the lineality space of the tangent cone.

  \param L (output) On exit, *L points to a (num_lineality x n) matrix
  stored in double** format whose rows form a basis for th lineality
  space of the tangent cone.  That is, *L[i] points to a double array
  of length n.  If we define \f$\ell_i\f$ to be the mathematical
  vector store in *L[i], then \f$\{\ell_1,\ldots,\ell_{\rm
  num\_lineality}\}\f$ forms a basis for the lineality space.

  \param n (input) The array length, a.k.a. the length of array Eq[i]
  and Iq[i], a.k.a, the number of columns in Eq and Iq.

  \param num_equalities (input) Number of rows in Eq.  

  \param Eq (input) A (num_equalities x n) matrix stored in double** format whose
  rows span the lineality space of the normal cone.

  \param num_inequalities (input) Number of rows in Iq.

  \param Iq (input) A (num_inequalities x n) matrix stored in double** format whose
  rows generate the "pointy part" of the normal cone. 

  \param append (input) Used as a logical operator to signal whether or not
  P and L are already partially formed and being appended to.

  Caution:  Memory for arguments P and L is allocated internally 
  within compute_cone_generators()
  "c-style", and must therefore be deallocated "c-style" externally.

  \author Robert Michael Lewis, College of William & Mary, 2005
*/
int compute_cone_generators(int *num_pointy,    double ***P,
			    int *num_lineality, double ***L,
			    int n, 
			    int num_equalities, double **Eq,
			    int num_inequalities, double **Iq,
			    int append);

#else

/*! Declaration of disabled CDDLIB function. */
int compute_cone_generators(int*,...);

#endif

#ifdef __cplusplus
}
#endif

#endif

