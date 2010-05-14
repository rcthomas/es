
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

#include "APPSPACK_CDDLIB_Interface.h"

#ifdef HAVE_CDDLIB

/*!
  \file APPSPACK_CDDLIB_Interface.c
  \brief Implementation of functions declared in APPSPACK_CDDLIB_Interface.h
  \author Robert Michael Lewis, College of William & Mary, 2005
*/

/* Includes from cddlib. */
#include "setoper.h"
#include "cdd.h"

#ifndef  DOXYGEN_SHOULD_SKIP_THIS
#define OK 0
#define OKEY_DOKEY  0
#define ACK        16
#endif

int compute_cone_generators(int *num_pointy,    double ***P,
			    int *num_lineality, double ***L,
			    int n, 
			    int num_equalities,   double **Eq,
			    int num_inequalities, double **Iq,
			    int append)
{
  static dd_PolyhedraPtr poly = NULL;
  static int last_n;

  dd_MatrixPtr A, G;
  dd_ErrorType err;

  int i, j, k;
  int linsize;
  int nl, np;

  int num_constraints;

  /* Some local initializations. */

  *P = NULL;  *num_pointy = 0;
  *L = NULL;  *num_lineality = 0;

  num_constraints = num_equalities + num_inequalities;

  /* Sanity check. */

  if (num_constraints <= 0) {
    printf("Bogus number of constraints specified: %d !!\n", num_constraints);
    return (ACK);
  }

  /* Initialize cddlib. */

  if (poly == NULL) {
    dd_set_global_constants();
  }

  if (append) {
    if (poly == NULL) {
      printf("%s: 'append' makes no sense---this is the first call\n",
             __FILE__);
      return (ACK);
    }
    if (n != last_n) {
      printf("%s: 'append' makes no sense---the dimension has changed\n",
             __FILE__);
      return (ACK);
    }
  }

  /* 
   * If we've called cddlib before and we are starting from scratch with a 
   * new set of constraints, then free the state we have retained from the
   * previous call.
   */

  if (!append && (poly != NULL)) {
    dd_FreePolyhedra(poly);
  }

  /*
   * Slog through the computation of the generators of T(x,r)...
   *
   * cddlib expects the constraints to have the form 
   *    Ax <= b   <=>   b - Ax >= 0,
   * and expects to be given b and -A.  Note the sign reversal of the 
   * constraint coefficients!  Tricky, tricky, tricky!
   *
   * The RHS b goes in A->matrix(:,0) while -A goes in A->matrix(:,1:n+1).
   */

  A = dd_CreateMatrix(num_constraints, n+1);
  A->representation = dd_Inequality;

  for (i = 0; i < num_equalities; i++) {
    dd_set_si(A->matrix[i][0], 0);  /* For our problem, b = 0. */
    for (j = 0; j < n; j++) {
      dd_set_d(A->matrix[i][j+1], -Eq[i][j]);
    }
    set_addelem(A->linset, i+1);
  }
  for (k = 0; i < num_constraints; i++, k++) {
    dd_set_si(A->matrix[i][0], 0);  /* For our problem, b = 0. */
    for (j = 0; j < n; j++) {
      dd_set_d(A->matrix[i][j+1], -Iq[k][j]);
    }
  }

  if (!append) {
    poly = dd_DDMatrix2Poly(A, &err);
  }
  else {
    dd_DDInputAppend(&poly, A, &err);
  }

  if (err != dd_NoError) {
    return (ACK);
  }

  /* Translate the generator representation to a matrix format. */
  G = dd_CopyGenerators(poly);

  if (G == NULL) {
    printf("%s: G == NULL encountered at line %d\n", __FILE__, __LINE__);
    return (ACK);
  }

  if ((G->colsize) != (n+1)) { /* This should never fail... */
    printf("%s: G->colsize != n+1 encountered at line %d\n", 
           __FILE__, __LINE__);
    return (ACK);
  }

  /*
   * Allocate space for the generators.
   */

  /* linsize is the dimension of the lineality space. */

  linsize = set_card(G->linset);

  *num_pointy    = G->rowsize - linsize;
  *num_lineality = linsize;

  /* Watch out!  If the input generates all of R^{n}, then what */
  /* appears in the pointy part is just the vertex at 0!! */
  printf("dim. of lineality space:            %d\n", *num_lineality);
  printf("no. of extreme rays in pointy part: %d\n", *num_pointy);

  if (*num_pointy > 0) {
    *P = (double**) malloc( (*num_pointy) * sizeof(double*) );
    if (*P == NULL) {
      printf("%s: Memory allocation failed! (line %d)\n", __FILE__, __LINE__);
      printf("%s: Requested %lu bytes\n", __FILE__, 
             (*num_pointy) * sizeof(double*));
      return (ACK);
    }
  }

  if (*num_lineality > 0) {
    *L = (double**) malloc( (*num_lineality) * sizeof(double*) );
    if (*L == NULL) {
      printf("%s: Memory allocation failed! (line %d)\n",
             __FILE__, __LINE__);
      printf("%s: Requested %lu bytes\n", __FILE__,
             (*num_lineality) * sizeof(double*));
      return (ACK);
    }
  }

  for (i = 0, np = 0, nl = 0; i < G->rowsize; i++) {
    
    /* 
     * Check whether this generator is part of the lineality space.
     */

    if (set_member(i+1, G->linset)) {
      (*L)[nl] = (double*) malloc(n*sizeof(double));
      if ((*L)[nl] == NULL) {
        printf("%s: Memory allocation failed! (line %d)\n",
               __FILE__, __LINE__);
        printf("%s: Requested %lu bytes\n", __FILE__, n * sizeof(double*));
        return (ACK);
      }
      for (j = 0; j < n; j++) {
        (*L)[nl][j] = dd_get_d(G->matrix[i][j+1]);
      }
      nl++;
    }
    else {
      (*P)[np] = (double*) malloc(n*sizeof(double));
      if ((*P)[np] == NULL) {
        printf("%s: Memory allocation failed! (line %d)\n",
               __FILE__, __LINE__);
        printf("%s: Requested %lu bytes\n", __FILE__, n * sizeof(double*));
        return (ACK);
      }
      for (j = 0; j < n; j++) {
        (*P)[np][j] = dd_get_d(G->matrix[i][j+1]);
      }
      np++;
    }
  }

  /* Clean up! */

  dd_FreeMatrix(A);
  dd_FreeMatrix(G);

  last_n = n;
  return (OKEY_DOKEY);
}


/*****************************************************************************
 * A test driver, included at no extra charge!!
 */

#if defined(TEST_MAIN)
#include "math.h"

int main (int argc, char **argv)
{
  int i, j, k;

  int retcd;
  int num_pointy, num_lineality;
  double **P;
  double **L;
  double **Eq, **Iq;
  sp47opts opts = {true, true, false, 1.0e-08};

  int
    num_equalities,
    num_inequalities;
  int n;

  num_equalities   = 3;
  num_inequalities = 0;
  n = 7;

  Eq = (double**) malloc(num_equalities*sizeof(double*));
  for (i = 0; i < num_equalities; i++) {
    Eq[i] = (double*) malloc(n*sizeof(double)); 
    for (j = 0; j < n; j++) {
      Eq[i][j] = drand48();
    }
  }
  Iq = NULL;

  for (k = 0; k < 100; k++) {
    compute_generators_new (&num_pointy, &P, &num_lineality, &L, 
                        n, num_equalities, Eq, num_inequalities, Iq, &opts);
#if defined(VERBOSE)
    printf("Pointy directions:\n");
    for (i = 0; i < num_pointy; i++) {
      for (j = 0; j < n; j++) {
        printf("% f ", P[i][j]);
      }
      printf("\n");
    }

    printf("Lineality space:\n");
    for (i = 0; i < num_lineality; i++) {
      for (j = 0; j < n; j++) {
        printf("% f ", L[i][j]);
      }
      printf("\n");
    }
#endif

    for (i = 0; i < num_pointy; i++) {
      free(P[i]);
    }
    free(P);

    for (i = 0; i < num_lineality; i++) {
      free(L[i]);
    }
    if (L != NULL) {
      free(L);
    }
  }

  for (i = 0; i < num_equalities; i++) {
    free(Eq[i]);
  }
  free(Eq);

  /***************************************************************
   * Test on a pointy cone.
   */

  num_equalities   = 0;
  num_inequalities = 20;
  n = 30;

  Eq = NULL;
  Iq = (double**) malloc(num_inequalities*sizeof(double*));
  for (i = 0; i < num_inequalities; i++) {
    Iq[i] = (double*) malloc(n*sizeof(double)); 
    for (j = 0; j < n; j++) {
      Iq[i][j] = drand48();
    }
  }

  for (k = 0; k < 500; k++) {
    compute_generators_new (&num_pointy, &P, &num_lineality, &L, 
                        n, num_equalities, Eq, num_inequalities, Iq, &opts);

#if defined(VERBOSE)
    printf("Pointy directions:\n");
    for (i = 0; i < num_pointy; i++) {
      for (j = 0; j < n; j++) {
        printf("% f ", P[i][j]);
      }
      printf("\n");
    }

    printf("Lineality space:\n");
    for (i = 0; i < num_lineality; i++) {
      for (j = 0; j < n; j++) {
        printf("% f ", L[i][j]);
      }
      printf("\n");
    }
#endif

    for (i = 0; i < num_pointy; i++) {
      free(P[i]);
    }
    free(P);

    for (i = 0; i < num_lineality; i++) {
      free(L[i]);
    }
    if (L != NULL) {
      free(L);
    }
  }

  for (i = 0; i < num_inequalities; i++) {
    free(Iq[i]);
  }
  free(Iq);
}
#endif

#else

int compute_cone_generators(int* n,...)
{
  printf("Error: compute_cone_generators() should not be called without CDDLIB.\n\n");
  printf("\nAPPSPACK Error\n");
  exit(1);
}

#endif
