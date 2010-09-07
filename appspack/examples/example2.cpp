/* $Id: example2.cpp,v 1.4 2006/06/30 01:07:26 tgkolda Exp $ */
/* $Source: /space/CVS-Acro/acro/packages/appspack/appspack/examples/example2.cpp,v $ */

/* @HEADER */
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
/* @HEADER */

/*! \file example2.cpp

  \brief Example with bound constraints and a nonlinear constraint 
  (handled using an extreme barrier approach).

  A simple example of a function evaluation that takes its input from
  a text file, and it writes a single objective value or an error
  message to an output file.


  In this case, the two-dimensional example function is:
  \f[
  f(x) = x_1^2 + 2 x_2^2
  \f]

  And there is also a constraint:
  \f[
  c(x) \equiv x_1^2 + x_2^2 -1 \geq 0
  \f]

  If the constraint is violated, then an error message is printed to
  the output file rather than the function value.

  The input file example2.apps, to be used with example2.cpp, includes
  the following lower and upper bounds and initial point:

  \f[
  \ell = \begin{bmatrix}-1\\-1\end{bmatrix}, \quad
  u = \begin{bmatrix}3\\3\end{bmatrix}, \quad
  x^{(0)} = \begin{bmatrix}0.2\\0.3\end{bmatrix}
  \f]

  \sa example2a.cpp is a variation on this example that writes both
  its function value and its constraint value to the output file.

*/

#include <iostream>
#include <fstream>
#include <vector>

using std::vector;
using std::ifstream;
using std::ofstream;
using std::cout;
using std::endl;

//! Function Evaluation 
double feval(const vector<double>& x)
{
  double f = 0;

  for (int i = 0; i < x.size(); i ++)
    f += (i + 1) * x[i] * x[i];

  return(f);

} // end feval 

//! Nonlinear Constraint 
bool constraint(vector<double>& x)
{
  double tmp = 0;

  for (int i = 0; i < x.size(); i ++)
    tmp += x[i] * x[i];

  return (tmp >= 1.0);

} // end constraint 

//! Main 
int main(int argc, char* argv[]) 
{

  int i;			// counter 
  vector<double> x;		// x-vector 
  double y;			// solution of f(x) 
  ifstream fin;			// input file
  ofstream fout;		// output file

  // Check input arguments 

  if (argc < 2) 
  {
    cout << "usage: " << argv[0] << " <input file> <output file>" << endl;
    return -1;
  }

  // Open input file 

  fin.open(argv[1]);
  if (!fin)
  {
    cout << argv[0] << " - Error opening input file " << argv[1] << endl;
    return -1;
  }

  // Read size of x 

  int n;
  fin >> n;

  // Allocate memory for x 

  x.resize(n);

  // Read x 

  for (i = 0; i < n; i ++)
    fin >> x[i];
  
  // Close input file 

  fin.close();

  // Open output file 

  fout.open(argv[2]);
  if (!fout)
  {
    cout << argv[0] << " - Error opening output file " << argv[2] << endl;
    return -1;
  }

  // Check constraints
  if (constraint(x))
  {

    // Evaluate function at x 
    y = feval(x);

    // Write function value to output file 
    fout << y;
  }
  else
  {

    // Write message to output file
    fout << "Constraint Violation";
  }

  // Close output file 

  fout.close();

  // Exit 

  return 0;
}

