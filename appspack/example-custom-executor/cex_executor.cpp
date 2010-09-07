/*! 
  \file cex_executor.cpp 
  \brief Implementation of example custom executor.
  \sa \ref pageCustomize_executor 
*/

#include "mpi.h"
#include "msgtags.hpp"
#include "executor.hpp"

CustomExecutor::CustomExecutor(int n_in, int nprocs_in) : 
  n(n_in), nprocs(nprocs_in), status(nprocs,0)
{
  // Allocate memory for x
  x = new double(n);

  // First processor is always busy because it's me!
  status[0] = 1;
}

CustomExecutor::~CustomExecutor()
{
  delete x;
}

bool CustomExecutor::isWaiting() const
{
  for (int i = 1; i < nprocs; i ++)
    if (status[i] == 0)
      return true;

  return false;
}

bool CustomExecutor::spawn(const APPSPACK::Vector& x_in, int tag_in)
{
  // Find a free worker (we assume that there must be one because
  // isWaiting should have already been called).
  int idx = -1;
  for (int i = 1; i < nprocs; i ++)
    if (status[i] == 0)
    {
      idx = i;
      break;
    }

  if (idx == -1)
    return false;

  // Copy x
  for (int i = 0; i < n; i ++)
    x[i] = x_in[i];

  // Send tag and x to processor idx
  MPI_Send(&tag_in, 1, MPI_INT, idx, XTAG, MPI_COMM_WORLD);
  MPI_Send(x, n, MPI_DOUBLE, idx, XVEC, MPI_COMM_WORLD);

  // Mark the worker as busy
  status[idx] = 1;

  return true;
}

int CustomExecutor::recv(int& tag_out, 
			 APPSPACK::Vector& f_out, 
			 string& msg_out)
{
  // Check to see if a new message is waiting
  MPI_Status mpiStatus;
  int flag;

  // Non-block probe to check for incoming message
  // NOTE: Cannot use a blocking probe here
  MPI_Iprobe(MPI_ANY_SOURCE, XTAG, MPI_COMM_WORLD, &flag, &mpiStatus);

  // Return zero if there is no message
  if (flag == 0)
    return 0;

  // Get the source of the message
  int source = mpiStatus.MPI_SOURCE;

  // Unpack the message
  int tag;
  int code;
  double f;
  MPI_Recv(&tag, 1, MPI_INT, source, XTAG, MPI_COMM_WORLD, &mpiStatus);
  MPI_Recv(&code, 1, MPI_INT, source, CODE, MPI_COMM_WORLD, &mpiStatus);
  MPI_Recv(&f, 1, MPI_DOUBLE, source, FVAL, MPI_COMM_WORLD, &mpiStatus);

  // Interpret the result
  if (code == 0)
  {
    tag_out = tag;
    f_out.resize(0);
    msg_out = "constraint violation";
  }
  else
  {
    tag_out = tag;
    f_out.resize(1);
    f_out[0] = f;
    msg_out = "success";
  }

  // Mark the worker as free
  status[source] = 0;

  // Important to return a non-zero to indicate that the recv was successful
  return source;
}

void CustomExecutor::print() const
{
  cout << "Custom MPI executor with " << (nprocs - 1) << " workers" << endl;
}
