/* $Id: example1.c,v 1.4 2006/06/30 01:07:26 tgkolda Exp $ */
/* $Source: /space/CVS-Acro/acro/packages/appspack/appspack/examples/example1.c,v $ */

/* @HEADER */
/*
************************************************************************

         APPSPACK: Asynchronous Parallel Pattern Search
                Copyright (2003) Sandia Corporation

Under terms of Contract DE-AC04-94AL85000, there is a non-exclusive
license for use of this work by or on behalf of the U.S. Government.

This library is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as
published by the Free Software Foundation; either version 2.1 of the
License, or (at your option) any later version.
 
This library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.
                                                                                
You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
USA.                                                                           .

Questions? Contact Tammy Kolda (tgkolda@sandia.gov) 

************************************************************************
*/
/* @HEADER */

/*! \file example1.c 

\brief Example with bound constraints.

  A simple example of a function evaluation that takes its input from
  a text file, and it writes its single objective value to an output
  file.

  In this case, the two-dimensional example function is:
  \f[
  f(x) = x_1^2 + 2 x_2^2
  \f]

  The input file example1.apps, to be used with example1.c, includes
  the following lower and upper bounds and initial point:

  \f[
  \ell = \begin{bmatrix}-1\\-1\end{bmatrix}, \quad
  u = \begin{bmatrix}1\\1\end{bmatrix}, \quad
  x^{(0)} = \begin{bmatrix}0.2\\0.3\end{bmatrix}
  \f]
*/

#include <stdio.h>
#include <stdlib.h>

/*! Function evaluation */
double example1_feval(int n, double *x) 
{
  int i;
  double f = 0;

  for (i = 0; i < n; i ++)
    f += (i + 1) * x[i] * x[i];

  return(f);

} /* end feval */

/*! Main */
int main(int argc, char* argv[]) {

  int i;			/* counter */
  int n;			/* number of dimensions */
  double* x;			/* n-dimensional vector */
  double y;			/* solution of f(x) */
  FILE* fp;			/* file pointer */


  /* Check input arguments */
  if (argc < 2) {
    fprintf (stderr, "usage: %s <input file> <output file>\n", argv[0]);
    return -1;
  }

  /* Open input file */

  if ((fp = fopen(argv[1], "r")) == NULL) {
    fprintf(stderr, "%s - Error opening input file %s.\n", argv[0], argv[1]);
    return -1;
  }

  /* Read size of x */

  if ((fscanf(fp, "%d", &n)) != 1) {
    fprintf(stderr, "%s - Error reading n.\n", argv[0]);
    return -1;
  }

  /* Allocate memory for x */

  if ((x = malloc(n * sizeof(double))) == NULL) {
    fprintf(stderr, "%s - Error allocating space for x.\n", argv[0]);
    return -1;
  }

  /* Read x */

  for (i = 0; i < n; i ++)
    if ((fscanf(fp, "%le", &x[i])) != 1) {
      fprintf(stderr, "%s - Error reading x[%d].\n", argv[0], i);
      return -1;
    }
  
  /* Close input file */

  fclose(fp);

  /* Evaluate function at x */

  y = example1_feval(n, x);

  /* Open output file */

  if ((fp = fopen(argv[2], "w")) == NULL) {
    fprintf(stderr, "%s - Error opening output file.\n", argv[0]);
    return -1;
  }

  /* Write function value to output file */
  
  fprintf(fp, "%e\n", y);

  /* Close output file */

  fclose(fp);

  /* Release memory */

  free(x);

  /* Exit */

  return 0;
}

