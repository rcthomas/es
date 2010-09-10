// $Id: APPSPACK_LAPACK_Wrappers.hpp,v 1.12 2006/01/26 21:53:09 jgriffi Exp $ 
// $Source: /space/CVS-Acro/acro/packages/appspack/appspack/src/APPSPACK_LAPACK_Wrappers.hpp,v $ 

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
  \file APPSPACK_LAPACK_Wrappers.hpp
  \brief No classes - Declare wrappers for the LAPACK functions
*/

#include "APPSPACK_Common.hpp"
#ifndef APPSPACK_LAPACK_WRAPPERS_HPP
#define APPSPACK_LAPACK_WRAPPERS_HPP

namespace APPSPACK
{
  
#ifdef  HAVE_LAPACK

//! Name of LAPACK dqelfq function
#define DGELQF_F77 F77_FUNC(dgelqf,DGELQF)

//! Name of LAPACK dgglse function
#define DGGLSE_F77 F77_FUNC(dgglse,DGGLSE)

//! Name of LAPACK dgesvd function
#define DGESVD_F77 F77_FUNC(dgesvd,DGESVD)

//! Name of LAPACK dgemm function
#define DGEMM_F77 F77_FUNC(dgemm,DGEMM)

//! Name of BLAS dgemm function
#define DGEMM_F77 F77_FUNC(dgemm,DGEMM)

//! Name of BLAS dgemm function
#define DGEMV_F77 F77_FUNC(dgemv,DGEMV)

//! Name of BLAS ddot function
#define DDOT_F77 F77_FUNC(ddot,DDOT)

#ifdef __cplusplus
extern "C" {
#endif

  /*!
    
  \brief DGELQF computes an LQ factorization of a real M-by-N matrix A:
  A = L * Q.
  
  
  \param M (input) INTEGER The number of rows of the matrix A.  M >= 0.
  
  \param N (input) INTEGER The number of columns of the matrix A.  N
  >= 0.
  
  \param A (input/output) DOUBLE PRECISION array, dimension (LDA,N) On
  entry, the M-by-N matrix A.  On exit, the elements on and
  below the diagonal of the array contain the m-by-min(m,n)
  lower trapezoidal matrix L (L is lower triangular if m <=
  n); the elements above the diagonal, with the array TAU,
  represent the orthogonal matrix Q as a product of elementary
  reflectors (see Further Details).
  
  \param LDA (input) INTEGER The leading dimension of the array A.  LDA
  >= max(1,M).
  
  \param TAU (output) DOUBLE PRECISION array, dimension (min(M,N)) The
  scalar factors of the elementary reflectors (see Further
  Details).
  
  \param WORK (workspace/output) DOUBLE PRECISION array, dimension
  (LWORK) On exit, if INFO = 0, WORK(1) returns the optimal
  LWORK.
  
  \param LWORK (input) INTEGER The dimension of the array WORK.  LWORK
  >= max(1,M).  For optimum performance LWORK >= M*NB, where
  NB is the optimal blocksize.
  If LWORK = -1, then a workspace query is assumed; the routine
  only calculates the optimal size of the WORK array, returns
  this value as the first entry of the WORK array, and no error
  message related to LWORK is issued by XERBLA.
  
  \param INFO (output) INTEGER = 0: successful exit < 0: if INFO = -i,
  the i-th argument had an illegal value
  
  <b> Further Details</b>
  
  The matrix Q is represented as a product of elementary reflectors 
  
  Q = H(k) . . . H(2) H(1), where k = min(m,n).
  
  Each H(i) has the form
  
  H(i) = I - tau * v * v'
  
  where tau is a real scalar, and v is a real vector with
  v(1:i-1) = 0 and v(i) = 1; v(i+1:n) is stored on exit in A(i,i+1:n),
  and tau in TAU(i).
*/

  void DGELQF_F77(int* M, int* N, double A[], int* LDA, double TAU[], 
		  double WORK[], int* LWORK, int* INFO );
  
  /*!
    
  \brief Solves the linear equality-constrained least squares problem min || c - Ax || subject to Bx = d.
  
  DGGLSE solves the linear equality-constrained least squares (LSE)
  problem:
  
  \f[
  \mbox{minimize } \| c - Ax \|_2   \mbox{ subject to }  Bx = d
  \f]
  
  where A is an M-by-N matrix, B is a P-by-N matrix, c is a given
  M-vector, and d is a given P-vector. It is assumed that P <= N <= M+P,
  and
  \f[
  \mbox{rank}(B) = P \mbox{ and } \mbox{rank}
  \left( \begin{array}{c} A \\ B \end{array} \right)
  = N.
  \f]
  These conditions ensure that the LSE problem has a unique solution,
  which is obtained using a GRQ factorization of the matrices B and A.
  
  
  \param M (input) INTEGER The number of rows of the matrix A.  M >= 0.
  
  \param N (input) INTEGER The number of columns of the matrices A and
  B. N >= 0.
  
  \param P (input) INTEGER The number of rows of the matrix B. 0 <= P <=
  N <= M+P.
  
  \param A (input/output) DOUBLE PRECISION array, dimension (LDA,N) On
  entry, the M-by-N matrix A.  On exit, A is destroyed.
  
  \param LDA (input) INTEGER The leading dimension of the array A. LDA
  >= max(1,M).
  
  \param B (input/output) DOUBLE PRECISION array, dimension (LDB,N) On
  entry, the P-by-N matrix B.  On exit, B is destroyed.
  
  \param LDB (input) INTEGER The leading dimension of the array B. LDB
  >= max(1,P).
  
  \param C (input/output) DOUBLE PRECISION array, dimension (M) On
  entry, C contains the right hand side vector for the least
  squares part of the LSE problem.  On exit, the residual sum
  of squares for the solution is given by the sum of squares
  of elements N-P+1 to M of vector C.
  
  \param D (input/output) DOUBLE PRECISION array, dimension (P) On
  entry, D contains the right hand side vector for the
  constrained equation.  On exit, D is destroyed.
  
  \param X (output) DOUBLE PRECISION array, dimension (N) On exit, X is
  the solution of the LSE problem.
  
  \param WORK (workspace/output) DOUBLE PRECISION array, dimension
  (LWORK) On exit, if INFO = 0, WORK(1) returns the optimal
  LWORK.
  
  \param LWORK (input) INTEGER The dimension of the array WORK. LWORK >=
  max(1,M+N+P).  For optimum performance LWORK >=
  P+min(M,N)+max(M,N)*NB, where NB is an upper bound for the
  optimal blocksizes for DGEQRF, SGERQF, DORMQR and SORMRQ.
  If LWORK = -1, then a workspace query is assumed; the routine
  only calculates the optimal size of the WORK array, returns
  this value as the first entry of the WORK array, and no error
  message related to LWORK is issued by XERBLA.
  
  \param INFO (output) INTEGER = 0: successful exit.  < 0: if INFO = -i,
  the i-th argument had an illegal value.
  

*/
  
  void DGGLSE_F77(int* M, int* N, int* P, double B[], int* LDB, double A[], 
		  int* LDA, double C[], double D[], double X[], double WORK[], 
		  int* LWORK, int* INFO);


/*!    
  \brief DGESVD computes the singular value decomposition (SVD) of a real
  M-by-N matrix A, optionally computing the left and/or right singular
  vectors. The SVD is written
\f[
       A = U  \Sigma  V^T
\f]
  where \f$\Sigma\f$ is an M-by-N matrix which is zero except for its
  min(m,n) diagonal elements, U is an M-by-M orthogonal matrix, and
  V is an N-by-N orthogonal matrix.  The diagonal elements of \f$\Sigma\f$
  are the singular values of A; they are real and non-negative, and
  are returned in descending order.  The first min(m,n) columns of
  U and V are the left and right singular vectors of A.

  Note that the routine returns \f$V^T\f$, not \f$V\f$.


\param JOBU (input) CHARACTER*1 Specifies options for computing all or part of the matrix U:
-         = 'A':  all M columns of U are returned in array U:
-         = 'S':  the first min(m,n) columns of U (the left singular
                 vectors) are returned in the array U;
-         = 'O':  the first min(m,n) columns of U (the left singular
                 vectors) are overwritten on the array A;
-         = 'N':  no columns of U (no left singular vectors) are
                 computed.
\param JOBVT   (input) CHARACTER*1 Specifies options for computing all or part of the matrix
         V^T:
-         = 'A':  all N rows of V^T are returned in the array VT;
-         = 'S':  the first min(m,n) rows of V^T (the right singular
                 vectors) are returned in the array VT;
-         = 'O':  the first min(m,n) rows of V^T (the right singular
                 vectors) are overwritten on the array A;
-         = 'N':  no rows of V^T (no right singular vectors) are
                 computed.
         JOBVT and JOBU cannot both be 'O'.
\param M  (input) INTEGER The number of rows of the input matrix A.  M >= 0.
\param N  (input) INTEGER The number of columns of the input matrix A.  N >= 0.
\param A  (input/output) DOUBLE PRECISION array, dimension (LDA,N). On entry, the M-by-N matrix A.
         On exit,
-         if JOBU = 'O',  A is overwritten with the first min(m,n)
                         columns of U (the left singular vectors,
                         stored columnwise);
-         if JOBVT = 'O', A is overwritten with the first min(m,n)
                         rows of V^T (the right singular vectors,
                         stored rowwise);
-         if JOBU .ne. 'O' and JOBVT .ne. 'O', the contents of A
                         are destroyed.
\param LDA (input) INTEGER The leading dimension of the array A.  LDA >= max(1,M).
\param S  (output) DOUBLE PRECISION array, dimension (min(M,N)).
         The singular values of A, sorted so that S(i) >= S(i+1).
\param U (output) DOUBLE PRECISION array, dimension (LDU,UCOL)
         - (LDU,M) if JOBU = 'A' or,
         - (LDU,min(M,N)) if JOBU = 'S'.
         - If JOBU = 'A', U contains the M-by-M orthogonal matrix U;
         - if JOBU = 'S', U contains the first min(m,n) columns of U
         (the left singular vectors, stored columnwise);
         - if JOBU = 'N' or 'O', U is not referenced.
\param  LDU (input) INTEGER
         The leading dimension of the array U.  LDU >= 1; if
         JOBU = 'S' or 'A', LDU >= M.
\param VT  (output) DOUBLE PRECISION array, dimension (LDVT,N)
         - If JOBVT = 'A', VT contains the N-by-N orthogonal matrix
         \f$V^T\f$;
         - if JOBVT = 'S', VT contains the first min(m,n) rows of
         V^T (the right singular vectors, stored rowwise);
         - if JOBVT = 'N' or 'O', VT is not referenced.
\param LDVT (input) INTEGER
         The leading dimension of the array VT.  LDVT >= 1; 
         - if JOBVT = 'A', LDVT >= N; 
         - if JOBVT = 'S', LDVT >= min(M,N).
\param WORK  (workspace/output) DOUBLE PRECISION array, dimension (LWORK)
         On exit, if INFO = 0, WORK(1) returns the optimal LWORK;
         if INFO > 0, WORK(2:MIN(M,N)) contains the unconverged
         superdiagonal elements of an upper bidiagonal matrix B
         whose diagonal is in S (not necessarily sorted). B
         satisfies A = U B VT, so it has the same singular values
         as A, and singular vectors related by U and VT.
\param LWORK   (input) INTEGER
         The dimension of the array WORK. LWORK >= 1.
         LWORK >= MAX(3*MIN(M,N)+MAX(M,N),5*MIN(M,N)).
         For good performance, LWORK should generally be larger.
         If LWORK = -1, then a workspace query is assumed; the routine
         only calculates the optimal size of the WORK array, returns
         this value as the first entry of the WORK array, and no error
         message related to LWORK is issued by XERBLA.
\param INFO    (output) INTEGER
-         = 0:  successful exit.
-         < 0:  if INFO = -i, the i-th argument had an illegal value.
-         > 0:  if DBDSQR did not converge, INFO specifies how many
               superdiagonals of an intermediate bidiagonal form B
               did not converge to zero. See the description of WORK
               above for details.
*/  
  void DGESVD_F77(char* JOBU, char* JOBVT, int* M, int* N, double A[], int* LDA,
		  double S[], double U[], int* LDU, double VT[], int* LDVT,
		  double WORK[], int* LWORK, int* INFO);

  /*! \brief DGEMM  performs one of the matrix-matrix operations
    \f[ C := \alpha *op( A ) *op( B ) + \beta* C, \f]
  where  \f$ op( X )\f$ is one of
   \f$ op( X ) = X\f$   or   \f$ op( X ) = X^T \f$,
  and \f$\alpha\f$ and \f$ \beta\f$ are scalars, and \f$A\f$, \f$B\f$ and \f$C\f$ 
  are matrices, with \f$ op(A)\f$
  an \f$ m \times k\f$ matrix,  \f$ op(B)\f$  a  \f$ k \times n\f$
  matrix and  \f$ C \f$ an \f$m \times n\f$ matrix.

\param TRANSA - CHARACTER1*.
           On entry, TRANSA specifies the form of op(A) to be used in
           the matrix multiplication as follows:
-              TRANSA = 'N' or 'n',  \f$op(A) = A\f$.
-              TRANSA = 'T' or 't',  \f$op(A) = A^T\f$.
-              TRANSA = 'C' or 'c',  \f$op(A) = A^T\f$.
-           Unchanged on exit.
\param  TRANSB - CHARACTER*1.
           On entry, TRANSB specifies the form of op(B) to be used in
           the matrix multiplication as follows:
-              TRANSB = 'N' or 'n',  \f$op(B) = B\f$.
-              TRANSB = 'T' or 't',  \f$op(B) = B^T\f$.
-             TRANSB = 'C' or 'c',   \f$op(B) = B^T\f$.
-           Unchanged on exit.
\param  M  - INTEGER.
           On entry,  M  specifies  the number  of rows  of the  matrix
           op(A)  and of the  matrix  C.  M  must  be at least  zero.
           Unchanged on exit.
\param  N      - INTEGER.
           On entry,  N  specifies the number  of columns of the matrix
           op(B) and the number of columns of the matrix C. N must be
           at least zero.
           Unchanged on exit.
\param  K      - INTEGER.
           On entry,  K  specifies  the number of columns of the matrix
           op(A) and the number of rows of the matrix op(B). K must
           be at least  zero.
           Unchanged on exit.
\param ALPHA  - DOUBLE PRECISION.
           On entry, ALPHA specifies the scalar alpha.
           Unchanged on exit.
\param  A      - DOUBLE PRECISION array of DIMENSION (LDA, ka), where ka is
           k  when  TRANSA = 'N' or 'n',  and is  m  otherwise.
           Before entry with  TRANSA = 'N' or 'n',  the leading  \f$m\times k\f$
           part of the array  A  must contain the matrix  A,  otherwise
           the leading  \f$ k \times m\f$  part of the array  A  must contain  the
           matrix A.
           Unchanged on exit.
\param  LDA    - INTEGER.
           On entry, LDA specifies the first dimension of A as declared
           in the calling (sub) program. When  TRANSA = 'N' or 'n' then
           LDA must be at least  \f$\max(1,m)\f$, otherwise  LDA must be at
           least  \f$\max(1,k)\f$.
           Unchanged on exit.
\param  B      - DOUBLE PRECISION array of DIMENSION (LDB, kb), where kb is
           n  when  TRANSB = 'N' or 'n',  and is  k  otherwise.
           Before entry with  TRANSB = 'N' or 'n',  the leading  \f$k \times n \f$
           part of the array  B  must contain the matrix  B,  otherwise
           the leading  \f$ n \times k \f$  part of the array  B  must contain  the
           matrix B.
           Unchanged on exit.
\param  LDB    - INTEGER.
           On entry, LDB specifies the first dimension of B as declared
           in the calling (sub) program. When  TRANSB = 'N' or 'n' then
           LDB must be at least  \f$\max(1,k)\f$, otherwise  LDB must be at
           least  \f$\max(1,n)\f$.
           Unchanged on exit.
\param  BETA   - DOUBLE PRECISION.
           On entry,  BETA  specifies the scalar  beta.  When  BETA  is
           supplied as zero then C need not be set on input.
           Unchanged on exit.
\param  C      - DOUBLE PRECISION array of DIMENSION (LDC, n).
           Before entry, the leading  \f$m\times n\f$  part of the array  C must
           contain the matrix  C,  except when  beta  is zero, in which
           case C need not be set on entry.
           On exit, the array  C  is overwritten by the  \f$m\times n\f$  matrix
           \f$ \left(\alpha * op(A) * op(B) + \beta*C \right)\f$.
\param  LDC    - INTEGER.
           On entry, LDC specifies the first dimension of C as declared
           in  the  calling  (sub)  program.   LDC  must  be  at  least
           \f$\max(1,m)\f$.
           Unchanged on exit.

DGEMM is a level 3 Blas routine.
*/
  void DGEMM_F77(char* TRANSA, char* TRANSB, int* M, int* N, int* K, double* ALPHA, 
		 double A[], int* LDA, double B[], int* LDB, double* BETA, double C[], 
		 int* LDC);

  /*!\brief DGEMV  performs one of the matrix-vector operations
    \f[
    y := \alpha A x + \beta y,   \;{\rm or }\;   y := \alpha A' x + \beta y,
    \f]
   where \f$\alpha\f$ and \f$beta\f$ are scalars, \f$x\f$ and \f$y\f$ are vectors 
   and \f$A\f$ is an \f$m\f$ by \f$n\f$ matrix.
   \param TRANS  CHARACTER*1.
   On entry, TRANS specifies the operation to be performed as
   follows:
   -  TRANS = 'N' or 'n'  \f$  y := \alpha A x + \beta y \f$
   -  TRANS = 'T' or 't'  \f$  y := \alpha A' x + \beta y, \f$
   -  TRANS = 'C' or 'c'  \f$ y := \alpha A' x + \beta y, \f$
   Unchanged on exit.
   \param M  INTEGER. On entry, M specifies the number of rows of the matrix A.  M 
   must be at least zero. Unchanged on exit.
   \param N INTEGER. On entry, N specifies the number of columns of the matrix A.
   N must be at least zero. Unchanged on exit.
   \param ALPHA DOUBLE PRECISION. On entry, ALPHA specifies the scalar alpha.
   Unchanged on exit.
   \param A DOUBLE PRECISION array of DIMENSION ( LDA, n ).  Before entry, the 
   leading m by n part of the array A must contain the matrix of coefficients.
   Unchanged on exit.
   \param LDA INTEGER. On entry, LDA specifies the first dimension of A as declared
   in the calling (sub) program. LDA must be at least max( 1, m ). Unchanged on exit.
   \param X DOUBLE PRECISION array of DIMENSION at least
   - ( 1 + ( n - 1 )*abs( INCX ) ) when TRANS = 'N' or 'n' and at least
   - ( 1 + ( m - 1 )*abs( INCX ) ) otherwise.
   Before entry, the incremented array X must contain the vector x. Unchanged on exit.
   \param INCX INTEGER.  On entry, INCX specifies the increment for the elements of  
   X. INCX must not be zero.  Unchanged on exit.
   \param BETA DOUBLE PRECISION.   On entry, BETA specifies the scalar beta. When BETA is
   supplied as zero then Y need not be set on input.  Unchanged on exit. 
   \param Y DOUBLE PRECISION array of DIMENSION at least
   - ( 1 + ( m - 1 )*abs( INCY ) ) when TRANS = 'N' or 'n' and at least
   - ( 1 + ( n - 1 )*abs( INCY ) ) otherwise.
   Before entry with BETA non-zero, the incremented array Y
   must contain the vector y. On exit, Y is overwritten by the updated vector y.
   \param INCY INTEGER.  On entry, INCY specifies the increment for the elements of
    Y. INCY must not be zero.  Unchanged on exit.

   Level 2 Blas routine.

   -- Written on 22-October-1986.
   \li   Jack Dongarra, Argonne National Lab.
   \li   Jeremy Du Croz, Nag Central Office.
   \li   Sven Hammarling, Nag Central Office.
   \li   Richard Hanson, Sandia National Labs.
   */  
  void DGEMV_F77(char* TRANS, int* M, int* N, double* ALPHA, double A[], int* LDA, 
		 double X[], int* INCX, double* BETA, double Y[], int* INCY);

  /*! \brief Forms the dot product of two vectors. Uses unrolled loops for 
    increments equal to one.
    \param n Length of dx and dy.
    \param dx Input vector.
    \param incx Increments to move along dx by.
    \param dy Input vector.
    \param incy Increments to move along dy by.
    \li  jack dongarra, linpack, 3/11/78.
    \li  modified 12/3/93, array(1) declarations changed to array(*)
  */
  double DDOT_F77(int* n, double dx[], int* incx, double dy[], int* incy);


#ifdef __cplusplus
}
#endif

#else
  
//! Declaration of disabled LAPACK function.
void DGELQF_F77(...);

//! Declaration of disabled LAPACK function.
void DGGLSE_F77(...);

//! Declaration of disabled LAPACK function.
void DGESVD_F77(...);

//! Declaration of disabled BLAS function.
void DGEMM_F77(...);

//! Declaration of disabled BLAS function.
void DGEMV_F77(...);
  
//! Declaration of disabled BLAS function.
double DDOT_F77(...);

#endif

}

#endif

