/* $Id: example5.cpp,v 1.2 2006/06/30 01:07:26 tgkolda Exp $ */
/* $Source: /space/CVS-Acro/acro/packages/appspack/appspack/examples/example5.cpp,v $ */

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

/*! \file example5.cpp

   \brief Example with linear equality and inequality constraints.

   Example 5 function evaluation progam. This function evaluation
   expects its input to come from a text file, and it writes its
   output to to a text file. 

   The test problem is:
   
   \f[
   \begin{array}{ll}
   {\rm min} & (x_1 - 10)^2 + (x_2 - 10)^2 + (x_3 - 10)^2 + (x_4 - 10)^2\\
   \mbox{s.t.} & -10 \le -x_1 - x_2 - x_3 -x_4 \\
   & x_1 - x_2 + x_3 - x_4 \le -1 \\
   & 2x_1 + 2x_3            \le  9 \\
   & 2x_1 + 2x_3 - 7x_4  = 3 \\
   & -10 \le x_1 \\
   & -10 \le x_2,x_4 \le 10 \\
   & x_3 \le 10 \\
   \end{array}
   \f]
   
   All of the linear inequality, linear equality, and bound
   constraints are defined in the file example4.apps, where we also
   define the scaling and initial point as:
   \f[
   s = \begin{bmatrix}1\\1\\1\\1 \end{bmatrix}, \quad
   x^{(0)} = \begin{bmatrix}-1\\1\\-1\\-1 \end{bmatrix}
   \f]
   

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
  double f1 = (x[0] - 10)*(x[0] - 10) + (x[1] - 10)*(x[1] - 10) +(x[2] - 10)*(x[2] - 10)
    +(x[3] - 10)*(x[3] - 10);

  return(f1);

} // end feval 

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

  // Evaluate function at x 

  y = feval(x);

  // Write function value to output file 

  fout << y;

  // Close output file 

  fout.close();

  // Exit 

  return 0;
}

