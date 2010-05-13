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
  \file APPSPACK_LAPACK_Wrappers.cpp
  \brief Implementation of functions declared in APPSPACK_LAPACK_Wrappers.hpp.
*/

#include "APPSPACK_LAPACK_Wrappers.hpp"

#ifndef  HAVE_LAPACK

void APPSPACK::DGELQF_F77(...)
{
  cerr << "Error: DGELQF_F77() should not be called without LAPACK.\n"
       << endl;
  throw "APPSPACK Error";
}

void APPSPACK::DGGLSE_F77(...)
{
  cerr << "Error: DGGLSE_F77() should not be called without LAPACK.\n"
       << endl;
  throw "APPSPACK Error";
}

void APPSPACK::DGESVD_F77(...)
{
  cerr << "Error: DGESVD_F77() should not be called without LAPACK.\n"
       << endl;
  throw "APPSPACK Error";
}

void APPSPACK::DGEMM_F77(...)
{
  cerr << "Error: DGEMM_F77() should not be called without LAPACK.\n"
       << endl;
  throw "APPSPACK Error";
}

void APPSPACK::DGEMV_F77(...)
{
  cerr << "Error: DGEMV_F77() should not be called without LAPACK.\n"
       << endl;
  throw "APPSPACK Error";
}

double APPSPACK::DDOT_F77(...)
{
  cerr << "Error: DDOT_F77() should not be called without LAPACK.\n"
       << endl;
  throw "APPSPACK Error";
}


#else

#endif
