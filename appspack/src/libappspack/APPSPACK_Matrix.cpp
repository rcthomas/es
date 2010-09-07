// $Id: APPSPACK_Matrix.cpp,v 1.59 2005/10/16 07:18:36 jgriffi Exp $ 
// $Source: /space/CVS-Acro/acro/packages/appspack/appspack/src/APPSPACK_Matrix.cpp,v $ 

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
  \file APPSPACK_Matrix.cpp
  \brief Implementation of APPSPACK::Matrix
*/

#include "APPSPACK_Matrix.hpp"
#include "APPSPACK_Print.hpp"
#include "APPSPACK_Float.hpp"
#include "APPSPACK_LAPACK_Wrappers.hpp"

APPSPACK::Matrix::Matrix() :
  fmatvecSet(false),
  fmatvecTSet(false)
{
}

APPSPACK::Matrix::Matrix(const APPSPACK::Matrix& source, TransposeType ttype) :
  fmatvecSet(false),
  fmatvecTSet(false)
{
  if (ttype == Transpose)
    transpose(source);
  else
    operator=(source);
}

APPSPACK::Matrix::Matrix(const APPSPACK::Matrix& source, const Vector& s, TransposeType ttype) :
  fmatvecSet(false),
  fmatvecTSet(false)
{
  if (ttype == Transpose)
    transpose(source);
  else
    operator=(source);
  scale(s);
}

APPSPACK::Matrix::Matrix(double** Aptr, int nrows, int ncols) :
  fmatvecSet(false),
  fmatvecTSet(false)
{
  resize(nrows, ncols);
  for (int i = 0; i < nrows; i++)
    for (int j = 0; j < ncols; j++)
      matrix[i][j] = Aptr[i][j];
}

APPSPACK::Matrix::~Matrix()
{
}

bool APPSPACK::Matrix::empty() const
{
  return (matrix.size() == 0);
}

int APPSPACK::Matrix::getNrows() const
{
  return matrix.size();
}

int APPSPACK::Matrix::getNcols() const
{
  if (matrix.empty())
    return 0;

  return matrix[0].size();
}

const APPSPACK::Vector& APPSPACK::Matrix::getRow(int i) const
{
  // Make sure this matrix has at least i rows.
  if (getNrows() <= i)
  {
    cerr << "ERROR: APPSPACK::Matrix::getRow - index out of range." << endl;
    throw "APPSPACK Error";
  }

  return matrix[i];
}

void APPSPACK::Matrix::getRowPointers(vector<double *>& Aptr)
{ 
  for (int i = 0; i < getNrows(); i++)
    Aptr.push_back(&matrix[i][0]);
  matrixChanged();
}

void APPSPACK::Matrix::clear()
{
  resize(0,0);
  matrixChanged();;
}

void APPSPACK::Matrix::addRow(const APPSPACK::Vector& r)
{
  // Make sure length of row r is correct.
  if ((!matrix.empty()) && (r.size() != getNcols()))
  {
    cerr << "ERROR: APPSPACK::Matrix::addRow - incorrect length of input row vector." << endl;
    throw "APPSPACK Error";
  }
  
  matrix.push_back(r);
  matrixChanged();
}

void APPSPACK::Matrix::deleteRow(int i)
{
  // Make sure this matrix has at least i rows.
  if (getNrows() <= i)
  {
    cerr << "ERROR: APPSPACK::Matrix::getRow - index out of range." << endl;
    throw "APPSPACK Error";
  }

  matrix.erase(matrix.begin()+i);
  matrixChanged();
}

void APPSPACK::Matrix::addRow(const APPSPACK::Vector& r, double alpha)
{
  addRow(r);
  matrix[matrix.size()-1].scale(alpha);
  matrixChanged();
}

void APPSPACK::Matrix::addMatrix(const APPSPACK::Matrix& B)
{
  for (int i = 0; i < B.getNrows(); i ++)
    addRow(B.getRow(i));
  matrixChanged();
}

void APPSPACK::Matrix::addMatrix(const APPSPACK::Matrix& B, double alpha)
{
  for (int i = 0; i < B.getNrows(); i ++)
    addRow(B.getRow(i), alpha);
  matrixChanged();
}

void APPSPACK::Matrix::addMatrix(const APPSPACK::Matrix& B, const APPSPACK::Vector& s)
{
  for (int i = 0; i < B.getNrows(); i ++)
  {
    addRow(B.getRow(i));
    matrix[matrix.size()-1].scale(s);
  }
  matrixChanged();
}

void APPSPACK::Matrix::addUniqueRows(const APPSPACK::Matrix& B, double epsilon)
{
  // Record number of rows in A now, once we are appending we don't want
  // to end up looping over the columns of B also.
  int nArows = getNrows();
  int nBrows = B.getNrows();
  Vector diff(getNcols());
  for (int i = 0; i < nBrows; i++)
  {
    const Vector& bi = B.getRow(i);
    bool bi_unique = true;
    for (int j = 0; j < nArows; j++)
    {
      diff = getRow(j);          // diff = aj.
      diff -= bi;                // diff = aj - bi.
      if (diff.norm() < epsilon) // Is ||aj - bi|| < epsilon?
      {
	// row is already in A.
	bi_unique = false;      
	break;
      }
    }
    if (bi_unique)
      addRow(bi);
  }
  
  matrixChanged();
}

void APPSPACK::Matrix::operator=(const APPSPACK::Matrix& m)
{
  matrix = m.matrix;
  matrixChanged();
}

void APPSPACK::Matrix::transpose(const Matrix& source)
{
  int nrows = source.getNcols();
  int ncols = source.getNrows();

  resize(nrows,ncols);

  for (int i = 0; i < nrows; i ++)
    for (int j = 0; j < ncols; j ++)
      matrix[i][j] = source.matrix[j][i];
  matrixChanged();
}

void APPSPACK::Matrix::scale(const APPSPACK::Matrix& source, const Vector& scaling)
{
  // Copy source
  matrix = source.matrix; 
  
  // Scale columns
  scale(scaling);
  matrixChanged();
}

void APPSPACK::Matrix::scale(const Vector& scaling)
{
  // Scale columns
  int nrows = getNrows();
  int ncols = getNcols();
  for (int i = 0; i < nrows; i++)
    for (int j = 0; j < ncols; j++)
      matrix[i][j] = matrix[i][j] * scaling[j];
  matrixChanged();
}

void APPSPACK::Matrix::setToIdentity(int n)
{
  resize(n,n);
  for (int i = 0; i < n; i++)
  {
    matrix[i].zero();
    matrix[i][i] = 1.0;
  }
  matrixChanged();
}

void APPSPACK::Matrix::normalize()
{
  for (int i = 0; i < getNrows(); i++)
  {
    double cnorm = matrix[i].norm();
    if (cnorm == 0)
      deleteRow(i);
    else
      matrix[i].scale(1/cnorm);
  }
  matrixChanged();
}

void  APPSPACK::Matrix::multVec(const APPSPACK::Vector& x, APPSPACK::Vector& y,
				TransposeType ttype) const
{
  // Error check.
  if (ttype == Transpose)
  {
    // Verify length of input vector.
    if (x.size() != getNrows())
    {
      cerr << "ERROR: APPSPACK::Matrix::multVec -  Incorrect length of input vector x." << endl;
      throw "APPSPACK Error";
    }
    
    // Verify length of output vector.
    if (y.size() != getNcols())
    {
      cerr << "ERROR: APPSPACK::Matrix::multVec -  Incorrect length of output vector y." << endl;
      throw "APPSPACK Error";
    }
  }
  else
  {
    // Verify length of input vector.
    if (x.size() != getNcols())
    {
      cerr << "ERROR: APPSPACK::Matrix::multVec -  Incorrect length of input vector x." << endl;
      throw "APPSPACK Error";
    }
    
    // Verify length of output vector.
    if (y.size() != getNrows())
    {
      cerr << "ERROR: APPSPACK::Matrix::multVec -  Incorrect length of output vector y." << endl;
      throw "APPSPACK Error";
    }
  }
#ifdef HAVE_LAPACK
  multVecWithBlas(x,y,ttype);
#else
  multVecWithoutBlas(x,y,ttype);
#endif
}

void APPSPACK::Matrix::svd(APPSPACK::Matrix& U, 
			   APPSPACK::Vector &s, 
			   APPSPACK::Matrix& VT) const 
{
  int m = getNrows();
  int n = getNcols();

  // Convert matrix to a FORTRAN vector

  Vector Avec = getMatrixVector();

  // Set up vectors to be filled by the SVD routine

  s.resize(m);			// Store diag(Sigma)
  Vector Uvec(m*m);		// Store U
  Vector VTvec(n*n);		// Store V^T

  // Other info

  int info;
  char jobu  = 'A';
  char jobvt = 'A'; 

  // Note: For call to lapack function dgesvd_, need lwork >=
  // max(3*min(m,n)+max(m,n),5*min(m,n)); Works better if slightly
  // larger--hence, multiplying quantity by 2.

  int lwork = 2 * max( 3 * min(m,n) + max(m,n), 5 * min(m,n) );
  Vector work(lwork);

  DGESVD_F77(&jobu, &jobvt, &m, &n, &Avec[0], &m, &s[0], &Uvec[0], &m,  
	     &VTvec[0], &n, &work[0], &lwork, &info);

  if (info != 0)
  {
    cerr << "ERROR: Problem with call to LAPACK function dgesvd.\n";
    throw "APPSPACK Error";
  }

  U.copyFromFortranVector(Uvec,m,m);
  VT.copyFromFortranVector(VTvec,n,n);
}

void APPSPACK::Matrix::multMat(const APPSPACK::Matrix& B,
			       TransposeType ttype)
{
  Matrix C;
  multMat(B,C,ttype);
  matrix = C.matrix;
  matrixChanged();
}

void APPSPACK::Matrix::multMat(const APPSPACK::Matrix& B, APPSPACK::Matrix& C,
			       TransposeType ttype) const
{
  if (ttype == Transpose)
  {
    // Peforming mult of form (m x k)*(n x k)' = (m x n).
    // Make sure B has correct size.
    if (getNcols() != B.getNcols())
    {
      cerr << "ERROR: Input matrix must have " << getNcols() << " columns." << endl;
      throw "APPSPACK Error";
    }
  }
  else
  {
    // Peforming mult of form (m x k)*(k x n) = (m x n).
    // Make sure B has correct size.
    if (getNcols() != B.getNrows())
    {
      cerr << "ERROR: Input matrix must have " << getNrows() << " rows." << endl;
      throw "APPSPACK Error";
    }
  }
  
#ifdef HAVE_LAPACK
    multMatWithBlas(B,C,ttype);
#else
    multMatWithoutBlas(B,C,ttype);
#endif
}

void APPSPACK::Matrix::nullSpace(APPSPACK::Matrix& ZT, double tol) const
{
  // Determine Z such that the columns of Z span the nullspace of this matrix. 

  int m = getNrows();
  int n = getNcols();

  if ((m == 0) || (n == 0))
  {
    cerr << "ERROR: Input matrix must be nonempty." << endl;
    throw "APPSPACK Error";
  }

  Vector s;
  Matrix U;
  Matrix VT;

  // Get SVD decomposition
  svd(U,s,VT);

  // Determine rank
  int rank = s.size();
  for (int i = 0; i < s.size(); i++)
    if (s[i] < tol)
    {
      rank = i;
      break;
    }

  // Form Z' by adding rows of V' that correspond to the zero singular values.
  ZT.copySubMatrix(rank,n-rank, VT);
}

bool APPSPACK::Matrix::getRighInvAndNullBasis(APPSPACK::Matrix& RT, APPSPACK::Matrix& NT,
					    double tol) const
{
  RT.clear();
  NT.clear();

  int m = getNrows();
  int n = getNcols();

  if (m > n)
    return false; // Pseudo-inverse does not exist.

  // First compute the singular value decomposition of A.
  Matrix U;
  Vector s;
  Matrix VT;
  svd(U,s,VT);
  
  for (int i = 0; i < s.size(); i++)
    if (s[i] < tol)
      return false; // Right inverse does not exists.
  
  // First, let's partition V to be V = [Vr N], i.e., the columns that
  // span the range and nullspaces.
  Matrix VrT;
  VrT.copySubMatrix(0, m, VT);
  NT.copySubMatrix(m, n-m, VT);

  // We know that R = Vr*inv(S)*UT => RT = U*inv(S)*VrT.
  for (int i = 0; i < s.size(); i++)
    s[i] = 1 / s[i];
  
  RT = U;
  RT.scale(s);
  RT.multMat(VrT);

  return true;
}

void APPSPACK::Matrix::pruneDependentRows(APPSPACK::Vector& b, double epsilon)
{
  // Perform LQ factorization: A = LQ.
  Vector Avec = getMatrixVector(NoTranspose);
  int info;
  int ncols = getNcols();
  int nrows = getNrows();
  int lwork = ncols*(ncols+2);
  Vector work(lwork, 0.0);
  Vector tau(nrows, 0.0);

  DGELQF_F77(&nrows, &ncols, &Avec[0], &nrows, &tau[0], &work[0], &lwork, &info);
  
  // Delete rows of A and entries of b if necesary.
  // Iterate backwards as matrix size may change, i.e. if we were to iterate
  // foreward, i for Avec will be different for the i for A and b.  Not to mention nrows.
  for (int i = nrows-1; i >= 0; i--)
  {
    if (fabs(Avec[i+i*nrows]) < epsilon)
    {
      deleteRow(i);
      b.erase(i);
    }
  }
  matrixChanged();
}

void APPSPACK::Matrix::constrainedLSQR(APPSPACK::Vector& x, const APPSPACK::Vector& b) const
{  
  if (empty())
    return;

  int info;
  int ncols = getNcols();
  int nrows = getNrows();
  int lwork = ncols*(ncols+2);
  Vector work(lwork);
  Vector newx(ncols);
  Vector bcopy = b;

  Vector Avec = getMatrixVector();

  Vector identity(ncols*ncols, 0.0);
  for(int i = 0; i < ncols; i++)
    identity[i+i*ncols] = 1;
  
  DGGLSE_F77(&ncols, &ncols, &nrows, &identity[0], &ncols, &Avec[0], &nrows,
	     &x[0], &bcopy[0], &newx[0], &work[0], &lwork, &info);

  if (info == 0)
    x = newx;
}

ostream& APPSPACK::Matrix::leftshift(ostream& stream) const
{
  stream << endl;
  for (int i = 0; i < getNrows(); i++) 
  {
    for (int j = 0; j < getNcols(); j++) 
    {
      stream <<  APPSPACK::Print::formatDouble(matrix[i][j]) << " ";
    }
    stream << endl;
  }
  return stream;
}

void APPSPACK::Matrix::matrixChanged()
{
  fmatvecTSet = false;
  fmatvecSet = false;
}

const APPSPACK::Vector& APPSPACK::Matrix::getMatrixVector(TransposeType ttype) const
{
  if (ttype == Transpose)
  {  
    if (!fmatvecTSet) // Vectorized matrix not current.  Update.
    {
      copyToFortranVector(fmatvecT, ttype);
      fmatvecTSet = true;
    }
    return fmatvecT;
  }
  else
  {
    if (!fmatvecSet) // Vectorized matrix not current.  Update.
    {
      copyToFortranVector(fmatvec, ttype);
      fmatvecSet = true;
    }
    return fmatvec;
  }
}

void APPSPACK::Matrix::copyToFortranVector(APPSPACK::Vector& Avec, TransposeType ttype) const
{
  // Fortran stores its matrices stored columnwise.
  int nrows = getNrows();
  int ncols = getNcols();

  Avec.resize(0);
  Avec.reserve(nrows * ncols);
  
  if (ttype == Transpose)
  {  
    // Store transpose of A. Elements can be added an entire row at a time.
    for (int i = 0; i < nrows; i++)
      Avec.append(matrix[i]);
  }
  else
  {
    // Store A. Elements must be added individually.
    for (int j = 0; j < ncols; j++)
      for (int i = 0; i < nrows; i ++)
	Avec.push_back(matrix[i][j]);
  }
  
}

void APPSPACK::Matrix::copyFromFortranVector(const APPSPACK::Vector& Avec, 
					     int nrows, int ncols,
					     TransposeType ttype)
{
  // Fortran stores its matrices stored columnwise.

  resize(nrows,ncols);
  
  if (ttype == Transpose)
  {
    // Avec contains a transposed version of A. Unpack row-by-row.
    int k = 0;
    for (int i = 0; i < nrows; i++)
      for (int j = 0; j < ncols; j++)
	matrix[i][j] = Avec[k++];
  }
  else
  {
    // Avec contains A. Unpack column-by-column.
    int k = 0;
    for (int j = 0; j < ncols; j++)
      for (int i = 0; i < nrows; i++)
	matrix[i][j] = Avec[k++];
  }
  
  matrixChanged();
}

// Private
void APPSPACK::Matrix::resize(int nrows, int ncols)
{
  matrix.resize(nrows);
  for (int i = 0; i < nrows; i++)
    matrix[i].resize(ncols);
  matrixChanged();
}

// Private
void APPSPACK::Matrix::copySubMatrix(int istart, int iextent, 
				     const APPSPACK::Matrix& B)
{
  if ((istart + iextent) > B.getNrows()) 
  {
    cerr << "ERROR: Requested submatrix does not exist.";
    throw "APPSPACK Error";
  }

  clear();
  for (int i = 0; i < iextent; i++)
    addRow(B.getRow(i+istart));
  matrixChanged();
}

void APPSPACK::Matrix::multMatWithBlas(const APPSPACK::Matrix& B, APPSPACK::Matrix& C,
				       TransposeType ttype) const
{  
  int n;  // n denotes number of columns of C (ultimately).
          // n is set equal to the number of columns in B if ttype = NoTranspose,
          // otherwise, n equals the number of rows in B.
 
  int ldb; // Leading dimension of B in Fortran lingo.  Essentially,
           // ldb equals the number of actual columns in B.  So
           // if ttype = NoTranspose, then ldb = n, since B is a
           // a (k x n) matrix.  If ttype = Transpose, then ldb = k,
           // since in this case B is is a (n x k) matrix.
  int m = getNrows();
  int k = getNcols();
  char transb;
  if (ttype == Transpose)
  {  
    // Peforming mult of form (m x k)*(n x k)' = (m x n).
    n      = B.getNrows();
    ldb    = k;
    transb = 'N';
  }
  else
  {  
    // Peforming mult of form (m x k)*(k x n) = (m x n).
    n      = B.getNcols();
    ldb    = n;
    transb = 'T';
  }
  
  // Create memory to hold A*B in vector form.
  Vector Cvec(m*n);


  // We're actually going to compute C = (A')' * (B')' because it's
  // more efficient to convert A' and B' to vectors.

  Vector& ATvec = const_cast<Vector&>(getMatrixVector(Transpose));   // Store A' as a vector
  Vector& BTvec = const_cast<Vector&>(B.getMatrixVector(Transpose)); // Store B' as a vector
  
  char transa = 'T';
  double one  = 1;
  double zero = 0;

  DGEMM_F77(&transa, &transb, &m, &n, &k, &one, &ATvec[0], &k,
	    &BTvec[0], &ldb , &zero, &Cvec[0], &m);  

  C.copyFromFortranVector(Cvec, m, n);
}

void APPSPACK::Matrix::multMatWithoutBlas(const APPSPACK::Matrix& B, APPSPACK::Matrix& C,
					  TransposeType ttype) const
{
  int m = getNrows();
  int k = getNcols();
  if (ttype == Transpose)
  {
    // Peforming mult of form (m x k)*(n x k)' = (m x n).
    int n = B.getNrows();
    C.resize(m,n);
    // C(i,j) = sum_{l=1}^k A(i,l)*B(j,l)
    for( int i = 0; i < m; i++)
      for( int j = 0; j < n; j++)
      {
	C.matrix[i][j] = 0;
	for( int l = 0; l < k; l++)
	  C.matrix[i][j] += matrix[i][l] * B.matrix[j][l];
      }
  }
  else
  {
    // Peforming mult of form (m x k)*(k x n) = (m x n).
    int n = B.getNcols();
    C.resize(m,n);
    // C(i,j) = sum_{l=1}^k A(i,l)*B(l,j)
    for( int i = 0; i < m; i++)
      for( int j = 0; j < n; j++)
      {
	C.matrix[i][j] = 0;
	for( int l = 0; l < k; l++)
	  C.matrix[i][j] += matrix[i][l] * B.matrix[l][j];
      }
  }
}

void  APPSPACK::Matrix::multVecWithoutBlas(const APPSPACK::Vector& x, APPSPACK::Vector& y,
					   TransposeType ttype) const
{
  if (ttype == Transpose)
  {
    // Compute y = A' * x, i.e. y_j = sum_i A_{ij} x_i
    y.zero();
    for (int i = 0; i < getNrows(); i++)
      for (int j = 0; j < getNcols(); j++)
	y[j] += matrix[i][j] * x[i];
  }
  else
  {
    // Compute y = A * x, i.e., y_i = sum_j A_{ij} x_j
    y.zero();
    for (int i = 0; i < getNrows(); i++)
      for (int j = 0; j < getNcols(); j++)
	y[i] += matrix[i][j] * x[j];
  } 
}

void  APPSPACK::Matrix::multVecWithBlas(const APPSPACK::Vector& x, APPSPACK::Vector& y,
					TransposeType ttype) const
{
  char trans;
  if (ttype == Transpose)
    trans='T'; // Performing tranpose mult.
  else
    trans='N'; // Performing normal mult.
  int m = getNrows();
  int n = getNcols();

  int lda = m;  // Leading dimension.
  int incx = 1; // Vectors increment by one.
  int incy = 1; // Vectors increment by one.
  double beta = 0.0;
  double alpha = 1.0;  

  Vector& mcopy = const_cast<Vector&>(getMatrixVector());
  Vector& xcopy = const_cast<Vector&>(x);
  // Returns y = alpha * mcopy + beta * xcopy.
  DGEMV_F77(&trans, &m, &n, &alpha, &mcopy[0], &lda, &xcopy[0], &incx, &beta, &y[0], &incy);  
}


ostream& operator<<(ostream& stream, const APPSPACK::Matrix& x)
{
  stream << "[";
  x.leftshift(stream);
  stream << "]";
  return stream;
}
