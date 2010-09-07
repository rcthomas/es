// $Id: APPSPACK_Executor_MPI.cpp,v 1.8 2006/06/30 01:42:08 tgkolda Exp $ 
// $Source: /space/CVS-Acro/acro/packages/appspack/appspack/src/APPSPACK_Executor_MPI.cpp,v $ 

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
  \file APPSPACK_Executor_MPI.cpp
  \brief Implemetation of APPSPACK::Executor::MPI
*/

#include "APPSPACK_Executor_MPI.hpp"
#include "APPSPACK_GCI.hpp"

APPSPACK::Executor::MPI::MPI()
{
  int nWorkers = GCI::getNumProcs() - 1;
  workertag.resize(nWorkers);
  for (int i = 0; i < nWorkers; i ++)
    workertag[i] = -1;
}

APPSPACK::Executor::MPI::~MPI()
{
}

bool APPSPACK::Executor::MPI::isWaiting() const
{
  for (int i = 0; i < workertag.size(); i ++)
    if (workertag[i] == -1)
      return true;

  return false;
}

bool APPSPACK::Executor::MPI::spawn(const Vector& x_in, int tag_in)
{
  
  for (int i = 0; i < workertag.size(); i ++)
    if (workertag[i] == -1)
    {
      workertag[i] = tag_in;
      GCI::initSend();
      GCI::pack(tag_in);
      GCI::pack(x_in);
      GCI::send(Feval, i+1);
      return true;
    }

  return false;
}

int APPSPACK::Executor::MPI::recv(int& tag_out, Vector& f_out, string& msg_out)
{
  if (!GCI::probe(Feval))
    return 0;

  int junk;
  int rank;

  GCI::recv(Feval);
  GCI::bufinfo(junk, rank);
  GCI::unpack(tag_out);
  GCI::unpack(f_out);
  GCI::unpack(msg_out);

  // Index into the tag array is the MPI rank minus one.
  int idx = rank - 1;
  workertag[idx] = -1;

  return rank;
}

void APPSPACK::Executor::MPI::print() const
{
  cout << "\n";
  cout << "Using MPI Executor with " << workertag.size() << " workers" << endl;
}
