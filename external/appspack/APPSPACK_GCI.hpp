// $Id: APPSPACK_GCI.hpp,v 1.10 2005/06/30 00:44:20 tgkolda Exp $ 
// $Source: /space/CVS-Acro/acro/packages/appspack/appspack/src/APPSPACK_GCI.hpp,v $ 

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
  \file APPSPACK_GCI.hpp
  \brief Class definition for APPSPACK::GCI (an MPI interface)
*/
#ifndef APPSPACK_GCI_H
#define APPSPACK_GCI_H

#include "APPSPACK_Common.hpp"
#include "APPSPACK_Vector.hpp"

#ifdef HAVE_PVM
#include "pvm3.h"
#endif

#ifdef HAVE_MPI
#include "mpi.h"		// for some private variables
#endif

namespace APPSPACK {

/*!
  \brief APPS Communicator - An interface for both PVM and MPI commands.

  \note Although GCI still supports PVM, it is no longer officially
  supported by APPSPACK. If you would like to use PVM, please contact
  \ref pagePrimaryContact.

  \author MPI interface contributed by H. Alton Patrick, Summer 2000.
*/
class GCI {

public:

  //@{ \name Initialization and exit

  //! For PVM, returns PVM taskid. Otherwise, throws an error.
  static int init();

  //! For PVM or MPI, returns process rank. Otherwise, throws an error.
  /* This routine puts all processes in a communicator called
     APPS_COMM. Users who wish to divide the processes into more than
     one communicator, in order to do multi-level parallelism for
     instance, should add code to pare down worldgroup to just the set
     of processes APPS should use. */
  static int init(int& argc, char**& argv);
  
  //! For PVM, set this process to catch all the output. Otherwise, prints a warning.
  /* Set standard output to pipe through this process ONLY IF this
     process HAS NOT been launched from the PVM console. If flag
     (default true) is false, then disable output redirection.*/
  static void catchOutput(bool flag = true);

  //! For PVM, spawn a task. Otherwise, throws an error.
  /* Spawn a SINGLE task with the given name on a specified host. If
     the host name is unspecified (""), then the task is spawned on
     any available machine. Argv is the array of arguments to the
     spawned job. Default argv is NULL. Return 0 if the spawn
     failed. Otherwise, return the PVM taskid. */
  static int spawn(const string name, const string host, char* argv[] = NULL);

  //! For PVM, kills the specified taskid. Otherwise, prints a warning.
  static void kill(int tid);

  //! Exit the communications interface for both PVM and MPI. Otherwise, prints a warning.
  static void exit();

  //@}

  //@{ \name Status and notify

  //! Return PVM taskid or MPI rank. 
  static int getMyTid();

  //! Return the number of processes.
  static int getNumProcs();

  /*! \brief For PVM, returns true is the process was spawned from the
    command line or the PVM console. Otherwise, returns false if it
    was spawned from another PVM process. */
  static bool isOrphan();

  //! For PVM, returns the taskid of the parent. */
  static int parent();

  //! For PVM, returns the hostid of the given task. */
  static int tidToHost(int tid);

  /*! \brief For PVM, sets notify to send a message with the given
    msgtag with the task specified by taskid exits. The body of the
    message will contain the taskid. */
  static void notify(int msgtag, int taskid);

  /*! \brief For PVM, sets notify to send a message with the given
    msgtag whenever any host dies. The body of the message will
    contain the hostid. */
  static void notify(int msgtag);

  //@}

  //@{ \name Send, Recv, and Related Commands

  //! For PVM or MPI, initialize send (must be called before pack and send).
  static void initSend();

  //! For PVM or MPI, send a message with the specified message tag to the specified task.
  static void send(int msgtag, int taskid);

  //! For PVM or MPI, broadcast message in buffer using tag msgtag to
  //! the n tasks listed in the taskid array.
  static void broadcast (int msgtag, const vector<int>& taskid);

  //! For PVM or MPI, blocking receive.
  /*! Blocking receive for a message with the given tag and task id.
     The inputs msgtag and msgtid default to -1 (wildcard).  Returns
     the buffer id of the message (PVM) or 1 (MPI), if any, and 0
     otherwise. */
  static bool recv(int msgtag = -1, int taskid = -1);

  //! For PVM or MPI, non-blocking receive
  /*! Non-blocking receive for a message with the given tag and task
      id.  The inputs msgtag and msgtid default to -1 (wildcard).
      Returns the buffer id of the message, if any, and 0 otherwise.*/
  static bool nrecv(int msgtag = -1, int taskid = -1);

  //! For PVM or MPI, non-blocking probe
  /*! Non-blocking probe for a message with the specified message tag
     and task id. Returns the buffer id of the message for PVM or 1
     for MPI, if any, and 0 otherwise. The inputs msgtag and msgtid
     default to -1 (wildcard).*/
  static bool probe(int msgtag = -1, int taskid = -1);

  //! For MPI, blocking probe
  /*! Do a blocking probe for a message with tag msgtag and from
     process msgtid. The default values of msgtag and msgtid are both
     -1. */
  static void bprobe(int msgtag = -1, int taskid = -1); 

  //! For PVM or MPI, determine the msgtag and msgtid for the most
  //! recently probed or received message.
  static void bufinfo(int& msgtag, int& taskid);

  //static void bufinfo(int bufid, int& msgtag, int& taskid);

  //@}

  //@{ \name Pack and Unpack primitives.

  //! Pack an integer
  static void pack(int i);

  //! Pack a char
  static void pack(char i);

  //! Pack a double
  static void pack(double d);

  //! Pack a float
  static void pack(float f);

  //! Pack a bool
  static void pack(bool b);

  //! Pack a string
  static void pack(const string s);

  //! Pack integer vector
  static void pack(const vector<int>& v);

  //! Pack double vector
  static void pack(const Vector& v);

  //! Pack float vector
  static void pack(const vector<float>& v);

  //! Pack a vector of vectors of doubles
  static void pack(const vector< Vector >& v);

  //! Pack a vector of string
  static void pack(const vector<string>& v);

  //! Pack a vector of bools
  static void pack(const vector<bool>& v);

  //! Pack a char array
  static void pack(int length, const char* array);

  //! Pack an integer array
  static void pack(int length, const int* array);

  //! Pack a double array
  static void pack(int length, const double* array);

  //! Pack a float array
  static void pack(int length, const float* array);

  //! Unpack an integer
  static void unpack(int& i);

  //! Unpack a char
  static void unpack(char& c);

  //! Unpack a double
  static void unpack(double& d);

  //! Unpack a float
  static void unpack(float& f);

  //! Unpack a book
  static void unpack(bool& b);

  //! Unpack a string
  static void unpack(string& s);

  //! Unpack a vector of integers
  static void unpack(vector<int>& v);

  //! Unpack a vector of doubles
  static void unpack(Vector& v);

  //! Unpack a vector of a vector of doubles
  static void unpack(vector< Vector >& v);

  //! Unpack a vector of strings
  static void unpack(vector<string>& v);

  //! Unpack a vector of bools
  static void unpack(vector<bool>& v);

  //! Unpack a char array
  static void unpack(int& length, char*& array);

  //! Unpack an integer array
  static void unpack(int& length, int*& array);

  //! Unpack a double array
  static void unpack(int& length, double*& array);

  //! Unpack a float array
  static void unpack(int& length, float*& array);

  //@}

  //@{ \name Host Information

  /*! \brief For PVM, reset host information by asking PVM to update
      hostp - the data structure that contains the host
      information. Return the number of hosts. */
  static int resetHostInfo();

  //! For PVM. get the hostid of the i-th host
  static int getHostTid(int i);
  //! For PVM,get the speed of the i-th host
  static int getHostSpeed(int i);

  //! For PVM, get a pointer to name of the i-th host
  static char* getHostName(int i);
  //@}



private:

#ifndef DOXYGEN_SHOULD_SKIP_THIS

  static void pack(const char* s);
  static void unpack(char*& s);

#ifdef HAVE_PVM
  static void pvmPackCheck(int info);
  static void pvmUnpackCheck(int info);
  static int lastBufferId;
  static struct pvmhostinfo* pvmHostInfo;
  static int nHosts;
  static int nArch;
#endif

#ifdef HAVE_MPI
  static char* sendBuffer;
  static char* recvBuffer;
  static int sendPosition, recvPosition, msgSize;
  static MPI_Comm APPS_COMM;
  static MPI_Status status;
  static int mpiGetMsgSize(MPI_Status localStatus, MPI_Datatype dataType);
  static void mpiStretchSendBuffer(int nBytes);
#endif

#endif

};

}
#endif
