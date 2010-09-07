// $Id: cex_executor.hpp,v 1.1 2006/06/30 19:14:31 tgkolda Exp $ 
// $Source: /space/CVS-Acro/acro/packages/appspack/appspack/example-custom-executor/cex_executor.hpp,v $ 
/*! 
  \file cex_executor.hpp

  \brief Example of derived custom executor.

  This file creates the custom executor object that allows the user
  the handle the parallelism and distribution of evaluations to
  workers in any way that makes sense..

  \sa \ref pageCustomize_executor 
*/

#include "APPSPACK_Executor_Interface.hpp" // Abstract interface for class

//! Custom executor class derived from APPSPACK::Executor::Interface
/*!
  \sa \ref pageCustomize_executor 
 */
class CustomExecutor : public APPSPACK::Executor::Interface
{

public:

  //! Constuctor can accept whatever arguments are appropriate
  CustomExecutor(int n_in, int nprocs_in);
  //! Destuctor
  ~CustomExecutor();

  //! Returns true if there is a worker free; otherwise, returns false
  bool isWaiting() const;

  //! Spawns a point on a free worker and returns true if successful; otherwise, returns false
  bool spawn(const APPSPACK::Vector& x_in, int tag_in);

  /*!
    Checks to see if there is a result from a worker. Returns 0 if no
    worker has returned a message. Otherwise, it returns the worker
    id (or any value greater than zero) and fills in the
    information. It's critical that the tag_out be filled in so that
    the function value can be matched with the correct
    point. Moreover, the msg_out should contain some sort of message
    even for successful evaluations, e.g., "success".
  */
  int recv(int& tag_out, APPSPACK::Vector& f_out, string& msg_out);

  //! Print information about the executor. 
  /*! Typically called in the intialization of the solver.*/
  void print() const;

private:

  //! Number of parameters
  int n;			

  /*! \brief The vector to be evaluated, stored as a double* which is
   needed for passing it thru MPI */
  double* x;			

  /*! \brief Total number of processors (note that processor zero is
   the master and cannot be used as a worker. */
  int nprocs;			

  /*! \brief The status array for the workers. A zero indicates that
   the worker is NOT busy, and a one indicates that the worker is
   busy. */
 vector<int> status;	

};
