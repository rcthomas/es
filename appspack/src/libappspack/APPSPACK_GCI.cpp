// $Id: APPSPACK_GCI.cpp,v 1.9 2005/08/10 19:13:00 tgkolda Exp $ 
// $Source: /space/CVS-Acro/acro/packages/appspack/appspack/src/APPSPACK_GCI.cpp,v $ 

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
  \file APPSPACK_GCI.cpp
  \brief Implementation of APPSPACK::GCI (an MPI interface)
*/

#include "APPSPACK_GCI.hpp"

using namespace APPSPACK;

// ----------------------------------------------------------------------
// Initialize
// ----------------------------------------------------------------------

#ifdef HAVE_PVM

int GCI::lastBufferId = 0;
struct pvmhostinfo* GCI::pvmHostInfo = NULL;
int GCI::nHosts = 0;
int GCI::nArch = 0;

#elif defined HAVE_MPI

char* GCI::sendBuffer = NULL;
char* GCI::recvBuffer = NULL;
int GCI::sendPosition = 0;
int GCI::recvPosition = 0; 
int GCI::msgSize = 0;
MPI_Comm GCI::APPS_COMM = MPI_COMM_NULL;
MPI_Status GCI::status;

#endif

// ----------------------------------------------------------------------
// Functions
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Initialize & Exit
// ----------------------------------------------------------------------

int GCI::init()
{
  int argc = -1;
  char** argv = NULL;
  return GCI::init(argc, argv);
}

int GCI::init(int& argc, char**& argv) 
{ 

#ifdef HAVE_PVM

  // PVM is initialized by calling any PVM command
  return GCI::getMyTid();

#elif defined HAVE_MPI

  // This routine puts all processes in a communicator called
  // APPS_COMM.  Users who wish to divide the processes into more than
  // one communicator, in order to do multi-level parallelism for
  // instance, should add code to pare down worldgroup to just the set
  // of processes APPS should use. This function returns the process'
  // rank.

  if (argc == -1) 
  {
    cerr << "APPSPACK::GCI::init - must specify argc and argv when using MPI" << endl;
    throw "APPSPACK error";
  }

  MPI_Group worldGroup, appsGroup;
  int e;
  int groupSize;
  int* ranksInOldGroup;

  e = MPI_Init(&argc, &argv);

  if (e != MPI_SUCCESS) 
  {
    cerr << "APPSPACK::GCI::init - error initializing MPI." << endl;
    throw "APPSPACK error";
  }
  
  // Get the set of processes (group, in MPI lingo) associated with 
  // MPI_COMM_WORLD.
  e = MPI_Comm_group(MPI_COMM_WORLD, &worldGroup);
  if (e != MPI_SUCCESS) 
  {
    cerr << "APPSPACK::GCI::init - error calling MPI_Comm_group" << endl;
    throw "APPSPACK error";
  }

  // Find out how many process there are in the group.
  e = MPI_Group_size(worldGroup, &groupSize);
  if (e != MPI_SUCCESS) 
  {
    cerr << "APPSPACK::GCI::init - error calling MPI_Group_size" << endl;
    throw "APPSPACK error";
  }

  // Build an array of ranks to include in the new communicator.  
  // The process with rank ranksInOldGroup[0] will have rank 0 in the 
  // new groupd, ranksInOldGroup[1] will have rank 1, etc.
  // In this case, since we want the new group to be the same as the
  // old, we include all ranks in the original order.
  ranksInOldGroup = new int[groupSize];
  for (int i = 0; i < groupSize; i++)
    ranksInOldGroup[i] = i;

  // Create the new group.
  e = MPI_Group_incl(worldGroup, groupSize, ranksInOldGroup, &appsGroup);
  if (e != MPI_SUCCESS) 
  {
    cerr << "APPSPACK::GCI::init - error calling MPI_Group_incl" << endl;
    throw "APPSPACK error";
  }

  // Create a new communicator which has the processes in group
  // appsGroup, drawn from MPI_COMM_WORLD.
  e = MPI_Comm_create(MPI_COMM_WORLD, appsGroup, &APPS_COMM);
  if (e != MPI_SUCCESS) 
  {
    cerr << "APPSPACK::GCI::init - error calling MPI_Comm_create" << endl;
    throw "APPSPACK error";
  }

  delete[] ranksInOldGroup;

  return GCI::getMyTid();

#else

  cerr << "APPSPACK::GCI::init - requires PVM or MPI" << endl; 
  throw "APPSPACK error";

#endif
}

void GCI::catchOutput(bool flag)
{
  // Set standard output to pipe through this process ONLY IF this
  // process HAS NOT been launched from the PVM console. If flag
  // (default true) is false, then disable output redirection.

#ifdef HAVE_PVM

  if (flag) 
  {
    if (pvm_parent() == PvmParentNotSet) // running from PVM console
      return;
    pvm_catchout(stdout);
    pvm_setopt(PvmShowTids, 0);
  }
  else
    pvm_catchout(0);

#else

  cerr << "Warning! APPSPACK::GCI::catchOutput requires PVM" << endl;

#endif
}

int GCI::spawn(const string name, const string host, char* argv[])
{
  // Spawn a SINGLE task with the given name on a specified host. If
  // the host name is unspecified (""), then the task is spawned on
  // any available machine. Default argv is NULL. Return 0 if the
  // spawn failed. Otherwise, return the taskid.

#ifdef HAVE_PVM

  int taskId;
  int info;

  if (host == "") 
    info = pvm_spawn(const_cast<char*>(name.c_str()), argv, PvmTaskDefault, 
		     NULL, 1, &taskId);
  else 
    info = pvm_spawn(const_cast<char*>(name.c_str()), argv, PvmTaskHost, 
		     const_cast<char*>(host.c_str()), 1, &taskId);

  if (info == PvmBadParam) 
  {
    cerr << "APPSPACK::GCI::spawn - invalid parameter" << endl;
    throw "APPSPACK error";
  }

  if (info == PvmNoHost) 
  {
    cerr << "APPSPACK::GCI::spawn - requested host is not in virtual machine" << endl;
    throw "APPSPACK error";
  }

  if (info == PvmNoFile) 
  {
    cerr << "APPSPACK::GCI::spawn - executable cannot be found" << endl;
    return 0;
  }

  if (info == PvmNoMem) 
  {
    cerr << "APPSPACK::GCI::spawn - not enough memory on host" << endl;
    return 0;
  }

  if (info == PvmSysErr) 
  {
    cerr <<  "APPSPACK::GCI::spawn - pvmd not responding" << endl;
    return 0;
  }

  if (info == PvmOutOfRes) 
  {
    cerr <<  "APPSPACK::GCI::spawn - out of resources" << endl;
    return 0;
  }

  if (info < 0) 
  {
    cerr << "APPSPACK::GCI::spawn - unrecgonized error" << endl;
    throw "APPSPACK error";
  }

  if (info < 1)
    return 0;

  return taskId;

#else

  cerr << "APPSPACK::GCI::spawn requires PVM" << endl;
  throw "APPSPACK error";

#endif
}

void GCI::kill(int taskId)
{
  // Kill the process with the given task id.

#ifdef HAVE_PVM

  int info = pvm_kill(taskId);

  if (info == PvmSysErr) 
  {
    cerr << "APPSPACK::GCI::kill -  pvmd not responding" << endl;
    throw "APPSPACK error";
  }
  if (info == PvmBadParam) 
  {
    cerr << "APPSPACK::GCI::kill - invalid taskId" << endl;
    throw "APPSPACK error";
  }
  if (info < 0) 
  {
    cerr << "APPSPACK::GCI::kill - unrecognized PVM error" << endl;
    throw "APPSPACK error";
  }
#else

  cerr << "Warning! APPSPACK::GCI::kill requires PVM" << endl;

#endif
}

void GCI::exit()
{
  // Exit communication interface.

#ifdef HAVE_PVM

  // In PVM it's very important to call this before exiting.

  int info = pvm_exit();

  if (info == PvmSysErr) 
  {
    cerr << "APPSPACK::GCI::exit - pvmd not responding" << endl;
    throw "APPSPACK error";
  }
  if (info < 0) 
  {
    cerr << "APPSPACK::GCI::exit - unrecognized PVM error" << endl;
    throw "APPSPACK error";
  }

#elif defined HAVE_MPI

  delete[] sendBuffer;
  delete[] recvBuffer;

  int e;

  // Wait for all processes to finish.
  e = MPI_Barrier(APPS_COMM);
  if (e != MPI_SUCCESS) 
  {
    cerr << "APPSPACK::GCI::exit - error in MPI barrier" << endl;
    throw "APPSPACK error";
  }
  // End MPI.
  e = MPI_Finalize();
  if (e != MPI_SUCCESS) 
  {
    cerr << "APPSPACK::GCI::exit - error in MPI finalize" << endl;
    throw "APPSPACK error";
  }
#else

  cerr << "Warning! APPSPACK::GCI::exit requires PVM or MPI" << endl;

#endif

}

// ----------------------------------------------------------------------
// Status & Notify
// ----------------------------------------------------------------------

int GCI::getMyTid() 
{
  // Return my taskId (or rank in MPI lingo).

#ifdef HAVE_PVM

  int info = pvm_mytid();

  if (info == PvmSysErr) 
  {
    cerr << "APPSPACK::GCI::getMyTid - pvmd is not running" << endl;
    throw "APPSPACK error";
  }
  if (info < 0) 
  {
    cerr << "APPSPACK::GCI::mytid - unrecognized PVM error" << endl;
    throw "APPSPACK error";
  }
  return info;

#elif defined HAVE_MPI

  int rank;

  int e = MPI_Comm_rank(APPS_COMM, &rank);
  if (e != MPI_SUCCESS) 
  {
    cerr << "APPSPACK::GCI::mytid - error getting MPI rank." << endl;
    throw "APPSPACK error";
  }

  return rank;

#else

  cerr << "APPSPACK::GCI::mytid requires PVM" << endl;
  throw "APPSPACK error";

#endif
}

int GCI::getNumProcs() 
{ 
  // Returns number of processes

#ifdef HAVE_MPI

  int nProcs;

  int e = MPI_Comm_size(APPS_COMM, &nProcs);
  if (e != MPI_SUCCESS) 
  {
    cerr << "APPSPACK::GCI::getNumProcs - error getting number of processes." << endl;
    throw "APPSPACK error";
  }
	
  return nProcs;

#else

   cerr << "APPSPACK::GCI::getNumProcs() requires MPI" << endl;
   throw "APPSPACK error";

#endif
}

bool GCI::isOrphan()
{
#ifdef HAVE_PVM

  // Check if the process is an orphan, i.e., the process was spawned
  // from the command line or the PVM console.

  int info = pvm_parent();

  if (info == PvmSysErr) 
  {
    cerr << "APPSPACK::GCI::isorphan - pvmd not responding" << endl;
    throw "APPSPACK error";
  }
  if ((info < 0) && (info != PvmNoParent) && (info != PvmParentNotSet)) 
  {
    cerr << "APPSPACK::GCI::isorphan - unrecognized PVM error" << endl;
    throw "APPSPACK error";
  }

  return ((info == PvmNoParent) || (info == PvmParentNotSet));

#else

  cerr << "APPSPACK::GCI::isorphan requires PVM" << endl;
  throw "APPSPACK error";

#endif
}

int GCI::parent()
{
#ifdef HAVE_PVM

  // Return the task id for the parent task. This will throw an error
  // if there is not a parent, so call if necessary isOrphan() first
  // to check.

  int info = pvm_parent();

  if (info == PvmNoParent) 
  {
    cerr << "APPSPACK::GCI::parent - no PVM parent" << endl;
    throw "APPSPACK error";
  }
  if (info == PvmSysErr) 
  {
    cerr << "APPSPACK::GCI::parent - pvmd not responding" << endl;
    throw "APPSPACK error";
  }
  if (info < 0) 
  {
    cerr << "APPSPACK::GCI::parent - unrecognized PVM error" << endl;
    throw "APPSPACK error";
  }

  return info;

#else

  cerr << "APPSPACK::GCI::parent requires PVM" << endl;
  throw "APPSPACK error";

#endif
}

int GCI::tidToHost(int t)
{
  // Find the host id for the given task.

#ifdef HAVE_PVM

  int info = pvm_tidtohost(t);

  if (info == PvmBadParam) 
  {
    cerr << "APPSPACK::GCI::tidtohost - invalid task id" << endl;
    throw "APPSPACK error";
  }
  if (info < 0) 
  {
    cerr << "APPSPACK::GCI::tidtohost - unrecognized PVM error" << endl;
    throw "APPSPACK error";
  }

  return info;

#else

   cerr << "APPSPACK::GCI::tidtohost requires PVM" << endl;
   throw "APPSPACK error";

#endif
}

void GCI::notify(int msgtag, int taskId)
{
  // Set notify for a SINGLE task 

#ifdef HAVE_PVM

  int info = pvm_notify(PvmTaskExit, msgtag, 1, &taskId);

  if (info == PvmSysErr) 
  {
    cerr << "APPSPACK::GCI::notify - pvmd not responding" << endl;
    throw "APPSPACK error";
  }
  if (info == PvmBadParam) 
  {
    cerr << "APPSPACK::GCI::notify - invalid argument to pvm_notify" << endl;
    throw "APPSPACK error";
  }
  if (info < 0) 
  {
    cerr << "APPSPACK::GCI::notify - unrecognized PVM error" << endl;
    throw "APPSPACK error";
  }
#else

   cerr << "APPSPACK::GCI::notify requires PVM" << endl;
   throw "APPSPACK error";

#endif
}

void GCI::notify(int msgtag)
{
  // Set notify for ALL host additions

#ifdef HAVE_PVM

  int info = pvm_notify(PvmHostAdd, msgtag, -1, NULL);

  if (info == PvmSysErr) 
  {
    cerr << "APPSPACK::GCI::notify - pvmd not responding" << endl;
    throw "APPSPACK error";
  }
  if (info == PvmBadParam) 
  {
    cerr << "APPSPACK::GCI::notify - invalid argument to pvm_notify" << endl;
    throw "APPSPACK error";
  }
  if (info < 0) 
  {
    cerr << "APPSPACK::GCI::notify - unrecognized PVM error" << endl;
    throw "APPSPACK error";
  }

#else

   cerr << "APPSPACK::GCI::notify requires PVM" << endl;
   throw "APPSPACK error";

#endif
}

// ----------------------------------------------------------------------
// Send, Recv, and Related Commands
// ----------------------------------------------------------------------

void GCI::initSend()
{
  // Initialize send.

#ifdef HAVE_PVM

  int info = pvm_initsend(PvmDataDefault); // XDR encoding

  if (info == PvmBadParam) 
  {
    cerr << "APPSPACK::GCI::initsend - invalid encoding value" << endl;
    throw "APPSPACK error";
  }
  if (info == PvmNoMem) 
  {
    cerr << "APPSPACK::GCI::initsend - out of memory" << endl;
    throw "APPSPACK error";
  }
  if (info < 0) 
  {
    cerr << "APPSPACK::GCI::initsend - unrecognized PVM error" << endl;
    throw "APPSPACK error";
  }

#elif defined HAVE_MPI

  delete[] sendBuffer;
  sendBuffer = NULL;
  sendPosition = 0;
  sendPosition = 0;

#else

   cerr << "APPSPACK::GCI::initsend requires PVM or MPI" << endl;
   throw "APPSPACK error";

#endif
}

void GCI::send(int msgtag, int taskId)
{
  // Send a message with the specified message tag to the given task
  // id.

#ifdef HAVE_PVM

  int info = pvm_send(taskId, msgtag);

  if (info == PvmSysErr) 
  {
    cerr << "APPSPACK::GCI::send - pvmd not responding" << endl;
    throw "APPSPACK error";
  }
  if (info == PvmBadParam) 
  {
    cerr << "APPSPACK::GCI::send - invalid argument" << endl;
    throw "APPSPACK error";
  }
  if (info == PvmNoBuf) 
  {
    cerr << "APPSPACK::GCI::send - no active send buffer" << endl;
    throw "APPSPACK error";
  }
  if (info < 0) 
  {
    cerr << "APPSPACK::GCI::send - unrecognized PVM error" << endl;
    throw "APPSPACK error";
  }

#elif defined HAVE_MPI

  int e = MPI_Send(sendBuffer, sendPosition, MPI_PACKED, taskId, msgtag, APPS_COMM);
  if (e != MPI_SUCCESS) 
  {
    cerr << "APPSPACK::GCI::send - error in MPI packed send" << endl;
    throw "APPSPACK error";
  }

#else

   cerr << "APPSPACK::GCI::send requires PVM or MPI" << endl;
   throw "APPSPACK error";

#endif
}

void GCI::broadcast (int msgtag, const vector<int>& taskId)
{
  // Broadcast message in buffer using tag msgtag to the n tasks
  // listed in the taskId array.

#ifdef HAVE_PVM

  // MAY WANT TO FIX THIS??

  int n = taskId.size();
  int* taskIdcpy = new int[n];
  for (int i = 0; i < n; i ++)
    taskIdcpy[i] = taskId[i];


  int info = pvm_mcast(taskIdcpy, n, msgtag);

  if (info == PvmSysErr) 
  {
    cerr << "APPSPACK::GCI::broadcast - pvmd not responding" << endl;
    throw "APPSPACK error";
  }
  if (info == PvmBadParam) 
  {
    cerr << "APPSPACK::GCI::broadcast -  msgtag < 0" << endl;
    throw "APPSPACK error";
  }
  if (info == PvmNoBuf) 
  {
    cerr << "APPSPACK::GCI::broadcast - no active send buffer" << endl;
    throw "APPSPACK error";
  }
  if (info < 0) 
  {
    cerr << "APPSPACK::GCI::broadcast - unrecognized PVM error" << endl;
    throw "APPSPACK error";
  }

  delete taskIdcpy;

#else

   cerr << "APPSPACK::GCI::broadcast requires PVM" << endl;
   throw "APPSPACK error";

#endif
}

bool GCI::recv(int msgtag, int msgtid)
{
  // Blocking receive for a message with the given tag and task id.
  // The inputs msgtag and msgtid default to -1 (wildcard).  Returns
  // the buffer id of the message (PVM) or 1 (MPI), if any, and 0
  // otherwise.

#ifdef HAVE_PVM

  int bufid = pvm_recv(msgtid, msgtag); 

  if (bufid == PvmBadParam) 
  {
    cerr << "APPSPACK::GCI::recv - invalid tid value or msgtag < -1" << endl;
    throw "APPSPACK error";
  }
  if (bufid == PvmSysErr) 
  {
    cerr << "APPSPACK::GCI::recv - pvmd not responding" << endl;
    throw "APPSPACK error";
  }
  if (bufid < 0) 
  {
    cerr << "APPSPACK::GCI::recv - unrecognized PVM error" << endl;
    throw "APPSPACK error";
  }

  lastBufferId = bufid;
  return (bufid != 0) ? true : false;

#elif defined HAVE_MPI

  if (msgtid == -1) 
    msgtid = MPI_ANY_SOURCE;
  if (msgtag == -1) 
    msgtag = MPI_ANY_TAG;

  // First, do a probe to find out how big to make the receive buffer
  int e = MPI_Probe(msgtid, msgtag, APPS_COMM, &status);
  if (e != MPI_SUCCESS) 
  {
    cerr << "APPSPACK::GCI::recv - error calling probe" << endl;
    throw "APPSPACK error";
  }
  msgSize = mpiGetMsgSize(status, MPI_PACKED);

  // Create the buffer.
  delete[] recvBuffer;
  recvBuffer = new char[msgSize];

  // Receive the message
  MPI_Recv(recvBuffer, msgSize, MPI_PACKED, msgtid, msgtag, APPS_COMM, &status);

  // QUESTION: Should the value of status or some error code be
  // checked here?

  // The value recvPosition is incremented as the message is read via
  // calls to unpack. Set it to 0 so we can start reading at the
  // beginning of the message.
  recvPosition = 0;

  return true;

#else

   cerr << "APPSPACK::GCI::recv requires PVM or MPI" << endl;
   throw "APPSPACK error";

#endif
}

bool GCI::nrecv(int msgtag, int msgtid)
{
  // Non-blocking receive for a message with the given tag and task
  // id.  The inputs msgtag and msgtid default to -1 (wildcard).
  // Returns the buffer id of the message, if any, and 0 otherwise.

#ifdef HAVE_PVM

  int bufid = pvm_nrecv(msgtid, msgtag);

  if (bufid == PvmBadParam) 
  {
    cerr << "APPSPACK::GCI::nrecv - invalid parameter" << endl;
    throw "APPSPACK error";
  }
  if (bufid == PvmSysErr) 
  {
    cerr << "APPSPACK::GCI::nrecv - pvmd not responding" << endl;
    throw "APPSPACK error";
  }
  if (bufid < 0) 
  {
    cerr << "APPSPACK::GCI::nrecv - unrecognized PVM error" << endl;
    throw "APPSPACK error";
  }
 
  lastBufferId = bufid;
  return (bufid != 0) ? true : false;

#else

   cerr << "APPSPACK::GCI::nrecv requires PVM" << endl;
   throw "APPSPACK error";

#endif
}

bool GCI::probe(int msgtag, int msgtid)
{
  // Non-blocking probe for a message with the specified message tag
  // and task id. Returns the buffer id of the message for PVM or 1
  // for MPI, if any, and 0 otherwise. The inputs msgtag and msgtid
  // default to -1 (wildcard).

#ifdef HAVE_PVM

  int bufid = pvm_probe(msgtid, msgtag);

  if (bufid == PvmBadParam) 
  {
    cerr << "APPSPACK::GCI::probe - invalid msgtid or msgtag" << endl;
    throw "APPSPACK error";
  }
  if (bufid == PvmSysErr) 
  {
    cerr << "APPSPACK::GCI::probe - pvmd not responding" << endl;
    throw "APPSPACK error";
  }
  if (bufid < 0) 
  {
    cerr << "APPSPACK::GCI::probe - unrecognized PVM error" << endl;
    throw "APPSPACK error";
  }
  
  lastBufferId = bufid;
  return (bufid != 0) ? true : false;

#elif defined HAVE_MPI

  int iflag;

  if (msgtid == -1) 
    msgtid = MPI_ANY_SOURCE;
  if (msgtag == -1)
    msgtag = MPI_ANY_TAG;

  // MPI sets iflag = 1 if there is a matching message, 0 if not.
  int e = MPI_Iprobe(msgtid, msgtag, APPS_COMM, &iflag, &status);

  if (e != MPI_SUCCESS) 
  {
    cerr << "APPSPACK::GCI::probe - error in MPI iprobe" << endl;
    throw "APPSPACK error";
  }

  return (iflag == 1) ? true : false;

#else

   cerr << "APPSPACK::GCI::probe requires PVM or MPI" << endl;
   throw "APPSPACK error";

#endif
}

void GCI::bprobe(int msgtag, int msgtid) 
{ 
  // Do a blocking probe for a message with tag msgtag and from
  // process msgtid. The default values of msgtag and msgtid are both
  // -1.

#ifdef HAVE_MPI

  if (msgtid == -1) 
    msgtid = MPI_ANY_SOURCE;
  if (msgtag == -1) 
    msgtag = MPI_ANY_TAG;

  int e = MPI_Probe(msgtid, msgtag, APPS_COMM, &status);
  if (e != MPI_SUCCESS) 
  {
    cerr << "APPSPACK::GCI::bprobe - error in MPI probe" << endl;
    throw "APPSPACK error";
  }

#else

   cerr << "APPSPACK::GCI::bprobe requires MPI" << endl;
   throw "APPSPACK error";

#endif
}

void GCI::bufinfo(int& msgtag, int& msgtid)
{
  // Determine the msgtag and msgtid for the most recently probed or
  // received message.
  
#ifdef HAVE_PVM

  if (lastBufferId == 0) 
  {
    cerr << "APPSPACK::GCI::bufinfo called without first doing recv or probe" << endl;
    throw "APPSPACK error";
  }

  int bytes;
  int info = pvm_bufinfo(lastBufferId, &bytes, &msgtag, &msgtid);
  
  if (info == PvmNoSuchBuf) 
  {
    cerr << "APPSPACK::GCI::bufinfo - specified PVM buffer does not exist" << endl;
    throw "APPSPACK error";
  }
  if (info == PvmBadParam) 
  {
    cerr << "APPSPACK::GCI::bufinfo - invalid argument to pvm_bufinfo" << endl;
    throw "APPSPACK error";
  }
  if (info < 0) 
  {
    cerr << "APPSPACK::GCI::bufinfo - unrecognized PVM error" << endl;
    throw "APPSPACK error";
  }

#elif defined HAVE_MPI

  msgtag = status.MPI_TAG;
  msgtid = status.MPI_SOURCE;

#else

   cerr << "APPSPACK::GCI::bufinfo requires PVM or MPI" << endl;
   throw "APPSPACK error";

#endif
}

// ----------------------------------------------------------------------
// Pack & Unpack
// ----------------------------------------------------------------------

void GCI::pack(int i)
{
 // Pack a single integer.

#ifdef HAVE_PVM

  pvmPackCheck(pvm_pkint(&i, 1, 1));

#elif defined HAVE_MPI

  int bufsize = sendPosition + sizeof(int);
  mpiStretchSendBuffer(bufsize);
  int e = MPI_Pack(&i, 1, MPI_INT, sendBuffer, bufsize, &sendPosition, APPS_COMM);
  if (e != MPI_SUCCESS) 
  {
    cerr << "APPSPACK::GCI::pack - error in MPI pack" << endl;
    throw "APPSPACK error";
  }

#else

   cerr << "APPSPACK::GCI::pack requires PVM of MPI" << endl;
   throw "APPSPACK error";

#endif
}

void GCI::pack(char i)
{
 // Pack a single chareger.

#ifdef HAVE_PVM

  pvmPackCheck(pvm_pkbyte(&i, 1, 1));

#elif defined HAVE_MPI

  int bufsize = sendPosition + sizeof(char);
  mpiStretchSendBuffer(bufsize);
  int e = MPI_Pack(&i, 1, MPI_CHAR, sendBuffer, bufsize, &sendPosition, APPS_COMM);
  if (e != MPI_SUCCESS) 
  {
    cerr << "APPSPACK::GCI::pack - error in MPI pack" << endl;
    throw "APPSPACK error";
  }

#else

   cerr << "APPSPACK::GCI::pack requires PVM of MPI" << endl;
   throw "APPSPACK error";

#endif
}

void GCI::pack(double d)
{
  // Pack a single double.

#ifdef HAVE_PVM

  pvmPackCheck(pvm_pkdouble(&d, 1, 1));

#elif defined HAVE_MPI

  int bufsize = sendPosition + sizeof(double);
  mpiStretchSendBuffer(bufsize);

  int e = MPI_Pack(&d, 1, MPI_DOUBLE, sendBuffer, bufsize, &sendPosition, APPS_COMM);
  if (e != MPI_SUCCESS) 
  {
    cerr << "APPSPACK::GCI::pack - error in MPI pack" << endl;
    throw "APPSPACK error";
  }

#else
   cerr << "APPSPACK::GCI::pack requires PVM of MPI" << endl;
   throw "APPSPACK error";

#endif
}

void GCI::pack(float d)
{
  // Pack a single float.

#ifdef HAVE_PVM

  pvmPackCheck(pvm_pkfloat(&d, 1, 1));

#elif defined HAVE_MPI

  int bufsize = sendPosition + sizeof(double);
  mpiStretchSendBuffer(bufsize);

  int e = MPI_Pack(&d, 1, MPI_FLOAT, sendBuffer, bufsize, &sendPosition, APPS_COMM);
  if (e != MPI_SUCCESS) 
  {
    cerr << "APPSPACK::GCI::pack - error in MPI pack" << endl;
    throw "APPSPACK error";
  }

#else
   cerr << "APPSPACK::GCI::pack requires PVM of MPI" << endl;
   throw "APPSPACK error";

#endif
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
void GCI::pack(const char* s)
{
  // Pack character array WITH its length first

#ifdef HAVE_PVM

  pack((int)strlen(s));
  pvmPackCheck(pvm_pkstr(const_cast<char*>(s)));

#elif defined HAVE_MPI

  // Calculate length of string including terminator '\O'
  int n = strlen(s) + 1;

  // Pack string length
  pack(n);

  // Pack the string itself
  int bufsize = sendPosition + sizeof(char) * n; 
  mpiStretchSendBuffer(bufsize);

  int e = MPI_Pack(const_cast<char*>(s), n, MPI_CHAR, sendBuffer, bufsize, &sendPosition, APPS_COMM);
  if (e != MPI_SUCCESS) 
  {
    cerr << "APPSPACK::GCI::pack - error in MPI pack" << endl;
    throw "APPSPACK error";
  }

#else

   cerr << "APPSPACK::GCI::pack requires PVM of MPI" << endl;
   throw "APPSPACK error";

#endif
}
#endif

void GCI::pack(bool b)
{
  // Pack a single bool. Bools are packed as ints because neither PVM
  // nor MPI can handle the bool datatype.

  int fakebool = (int) b;

#ifdef HAVE_PVM

  pvmPackCheck(pvm_pkint(&fakebool, 1, 1));

#elif defined HAVE_MPI

  pack(fakebool);

#else

   cerr << "APPSPACK::GCI::pack requires PVM of MPI" << endl;
   throw "APPSPACK error";

#endif
}

void GCI::pack(const string s)
{

  // Pack a string.

#if defined HAVE_PVM || defined HAVE_MPI

  pack(s.c_str());

#else

   cerr << "APPSPACK::GCI::pack requires PVM or MPI" << endl;
   throw "APPSPACK error";

#endif
}

void GCI::pack(const vector<int>& v)
{
  int n = v.size();
  pack(n);
  for (int i = 0; i < n; i ++)
    pack(v[i]);
}

void GCI::pack(const Vector& v)
{
  int n = v.size();
  pack(n);
  for (int i = 0; i < n; i ++)
    pack(v[i]);
}

void GCI::pack(const vector< Vector >& v)
{
  int n = v.size();
  pack(n);
  for (int i = 0; i < n; i ++)
    pack(v[i]);
}

void GCI::pack(const vector<string>& v)
{
  int n = v.size();
  pack(n);
  for (int i = 0; i < n; i ++)
    pack(v[i]);
}

void GCI::pack(const vector<bool>& v)
{
  // Bool vectors must be handled specially because they are stored is
  // a compacted format.
  int n = v.size();
  pack(n);
  int tmp;
  for (int i = 0; i < n; i ++) 
  {
    tmp = (int) v[i];
    pack(tmp);
  }
}    

void GCI::pack(int length, const char* array)
{
  // Pack array WITH its length first

#ifdef HAVE_PVM

  pack(length);
  pvmPackCheck(pvm_pkbyte(const_cast<char*>(array), length, 1));

#elif defined HAVE_MPI

  // Pack array length
  pack(length);

  // Pack the array itself
  int bufsize = sendPosition + sizeof(char) * length; 
  mpiStretchSendBuffer(bufsize);

  int e = MPI_Pack(const_cast<char*>(array), length, MPI_CHAR, sendBuffer, bufsize, &sendPosition, APPS_COMM);
  if (e != MPI_SUCCESS) 
  {
    cerr << "APPSPACK::GCI::pack - error in MPI pack" << endl;
    throw "APPSPACK error";
  }

#else

   cerr << "APPSPACK::GCI::pack requires PVM of MPI" << endl;
   throw "APPSPACK error";

#endif
}

void GCI::pack(int length, const int* array)
{
  // Pack array WITH its length first

#ifdef HAVE_PVM

  pack(length);
  pvmPackCheck(pvm_pkint(const_cast<int*>(array), length, 1));

#elif defined HAVE_MPI

  // Pack array length
  pack(length);

  // Pack the array itself
  int bufsize = sendPosition + sizeof(int) * length; 
  mpiStretchSendBuffer(bufsize);

  int e = MPI_Pack((int*)array, length, MPI_INT, sendBuffer, bufsize, &sendPosition, APPS_COMM);
  if (e != MPI_SUCCESS) 
  {
    cerr << "APPSPACK::GCI::pack - error in MPI pack" << endl;
    throw "APPSPACK error";
  }

#else

   cerr << "APPSPACK::GCI::pack requires PVM of MPI" << endl;
   throw "APPSPACK error";

#endif
}

void GCI::pack(int length, const double* array)
{
  // Pack array WITH its length first

#ifdef HAVE_PVM

  pack(length);
  pvmPackCheck(pvm_pkdouble((double*) array, length, 1));

#elif defined HAVE_MPI

  // Pack array length
  pack(length);

  // Pack the array itself
  int bufsize = sendPosition + sizeof(double) * length; 
  mpiStretchSendBuffer(bufsize);

  int e = MPI_Pack((double*)array, length, MPI_DOUBLE, sendBuffer, bufsize, &sendPosition, APPS_COMM);
  if (e != MPI_SUCCESS) 
  {
    cerr << "APPSPACK::GCI::pack - error in MPI pack" << endl;
    throw "APPSPACK error";
  }

#else

   cerr << "APPSPACK::GCI::pack requires PVM of MPI" << endl;
   throw "APPSPACK error";

#endif
}


void GCI::pack(int length, const float* array)
{
  // Pack array WITH its length first

#ifdef HAVE_PVM

  pack(length);
  pvmPackCheck(pvm_pkfloat((float*) array, length, 1));

#elif defined HAVE_MPI

  // Pack array length
  pack(length);

  // Pack the array itself
  int bufsize = sendPosition + sizeof(float) * length; 
  mpiStretchSendBuffer(bufsize);

  int e = MPI_Pack((float*)array, length, MPI_FLOAT, sendBuffer, bufsize, &sendPosition, APPS_COMM);
  if (e != MPI_SUCCESS) 
  {
    cerr << "APPSPACK::GCI::pack - error in MPI pack" << endl;
    throw "APPSPACK error";
  }

#else

   cerr << "APPSPACK::GCI::pack requires PVM of MPI" << endl;
   throw "APPSPACK error";

#endif
}

void GCI::unpack(int& i)
{
  // Unpack a single integer

#ifdef HAVE_PVM

  pvmUnpackCheck(pvm_upkint(&i, 1, 1));

#elif defined HAVE_MPI

  int e = MPI_Unpack(recvBuffer, msgSize, &recvPosition, &i, 1, MPI_INT, APPS_COMM);
  if (e != MPI_SUCCESS) 
  {
    cerr << "APPSPACK::GCI::unpack - error in MPI unpack" << endl;
    throw "APPSPACK error";
  }

#else

   cerr << "APPSPACK::GCI::unpack requires PVM of MPI" << endl;
   throw "APPSPACK error";

#endif
}

void GCI::unpack(char& i)
{
  // Unpack a single integer

#ifdef HAVE_PVM

  pvmUnpackCheck(pvm_upkbyte(&i, 1, 1));

#elif defined HAVE_MPI

  int e = MPI_Unpack(recvBuffer, msgSize, &recvPosition, &i, 1, MPI_CHAR, APPS_COMM);
  if (e != MPI_SUCCESS) 
  {
    cerr << "APPSPACK::GCI::unpack - error in MPI unpack" << endl;
    throw "APPSPACK error";
  }

#else

   cerr << "APPSPACK::GCI::unpack requires PVM of MPI" << endl;
   throw "APPSPACK error";

#endif
}

void GCI::unpack(double& d)
{
  // Unpack a single double

#ifdef HAVE_PVM

  pvmUnpackCheck(pvm_upkdouble(&d, 1, 1));

#elif defined HAVE_MPI

  int e = MPI_Unpack(recvBuffer, msgSize, &recvPosition, &d, 1, MPI_DOUBLE, APPS_COMM);
  if (e != MPI_SUCCESS) 
  {
    cerr << "APPSPACK::GCI::unpack - error in MPI unpack" << endl;
    throw "APPSPACK error";
  }

#else

   cerr << "APPSPACK::GCI::unpack requires PVM of MPI" << endl;
   throw "APPSPACK error";

#endif
}

void GCI::unpack(float& d)
{
  // Unpack a single float

#ifdef HAVE_PVM

  pvmUnpackCheck(pvm_upkfloat(&d, 1, 1));

#elif defined HAVE_MPI

  int e = MPI_Unpack(recvBuffer, msgSize, &recvPosition, &d, 1, MPI_FLOAT, APPS_COMM);
  if (e != MPI_SUCCESS) 
  {
    cerr << "APPSPACK::GCI::unpack - error in MPI unpack" << endl;
    throw "APPSPACK error";
  }

#else

   cerr << "APPSPACK::GCI::unpack requires PVM of MPI" << endl;
   throw "APPSPACK error";

#endif
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
void GCI::unpack(char*& s)
{
  // Unpack char array s. If s is NULL, memory is allocated for it. If
  // not, the length of s is checked to be sure it's large enough.

  int len;
  unpack(len);

  if (!s) 
    s = new char[len];
  else if (((int)strlen(s)) < len) 
  {
    cerr << "APPSPACK::GCI::unpack - string is too small for unpack" << endl;
    throw "APPSPACK error";
  }

#ifdef HAVE_PVM

  pvmUnpackCheck(pvm_upkstr(s));

#elif defined HAVE_MPI

  int e = MPI_Unpack(recvBuffer, msgSize, &recvPosition, s, len, MPI_CHAR, APPS_COMM);
  if (e != MPI_SUCCESS) 
  {
    cerr << "APPSPACK::GCI::unpack - error in MPI unpack" << endl;
    throw "APPSPACK error";
  }

#else

   cerr << "APPSPACK::GCI::unpack requires PVM of MPI" << endl;
   throw "APPSPACK error";

#endif
}
#endif

void GCI::unpack(bool& b)
{
  // Unpack a single boolean.

#ifdef HAVE_PVM

  int bfake;
  pvmUnpackCheck(pvm_upkint(&bfake, 1, 1));
  b = (bool) bfake;

#elif defined HAVE_MPI

  int bfake;
  int e = MPI_Unpack(recvBuffer, msgSize, &recvPosition, &bfake, 1, MPI_INT, APPS_COMM);
  if (e != MPI_SUCCESS) 
  {
    cerr << "APPSPACK::GCI::unpack - error in MPI unpack" << endl;
    throw "APPSPACK error";
  }

  b = (bool) bfake;

#else

   cerr << "APPSPACK::GCI::unpack requires PVM of MPI" << endl;
   throw "APPSPACK error";

#endif
}

void GCI::unpack(string& s)
{

#if defined HAVE_PVM || defined HAVE_MPI

#ifdef DEBUG
  cout << "Unpacking a string" << endl;
#endif

  char* tmp = NULL;
  unpack(tmp);

#ifdef DEBUG
  cout << "Unpacking a string=" << tmp << endl;
#endif

  s = tmp;

#ifdef DEBUG
  cout << "Unpacking a string=" << s << endl;
#endif


  try
  {
    delete [] tmp;
  }
  catch(...)
  {
  }
  
#ifdef DEBUG
  cout << "Unpacking a string=" << s << endl;
#endif

#else

   cerr << "APPSPACK::GCI::unpack requires PVM of MPI" << endl;
   throw "APPSPACK error";

#endif
}

void GCI::unpack(vector<int>& v)
{
  int n;
  unpack(n);
  v.resize(n);
  for (int i = 0; i < n; i ++)
    unpack(v[i]);
}

void GCI::unpack(Vector& v)
{
  int n;
  unpack(n);
  v.resize(n);
  for (int i = 0; i < n; i ++)
    unpack(v[i]);
}

void GCI::unpack(vector< Vector >& v)
{
  int n;
  unpack(n);
  v.resize(n);
  for (int i = 0; i < n; i ++)
    unpack(v[i]);
}

void GCI::unpack(vector<string>& v)
{
  int n;
  unpack(n);
  v.resize(n);
  for (int i = 0; i < n; i ++)
    unpack(v[i]);
}


void GCI::unpack(vector<bool>& v)
{
  // Bool vectors must be handled specially because they are stored is
  // a compacted format.
  int n;
  unpack(n);
  v.resize(n);
  int tmp;
  for (int i = 0; i < n; i ++) 
  {
    unpack(tmp);
    v[i] = (bool) tmp;
  }
}    

void GCI::unpack(int& length, char*& array)
{
  // Unpack array s. If s is NULL, memory is allocated for it. If
  // not, the length of s is checked to be sure it's large enough.

  unpack(length);
  array = new char[length];

#ifdef HAVE_PVM

  pvmUnpackCheck(pvm_upkbyte(array, length, 1));

#elif defined HAVE_MPI

  int e = MPI_Unpack(recvBuffer, msgSize, &recvPosition, array, length, MPI_CHAR, APPS_COMM);
  if (e != MPI_SUCCESS) 
  {
    cerr << "APPSPACK::GCI::unpack - error in MPI unpack" << endl;
    throw "APPSPACK error";
  }

#else

   cerr << "APPSPACK::GCI::unpack requires PVM of MPI" << endl;
   throw "APPSPACK error";

#endif
}

void GCI::unpack(int& length, int*& array)
{
  // Unpack array s. If s is NULL, memory is allocated for it. If
  // not, the length of s is checked to be sure it's large enough.

  unpack(length);
  array = new int[length];

#ifdef HAVE_PVM

  pvmUnpackCheck(pvm_upkint(array, length, 1));

#elif defined HAVE_MPI

  int e = MPI_Unpack(recvBuffer, msgSize, &recvPosition, array, length, MPI_INT, APPS_COMM);
  if (e != MPI_SUCCESS) 
  {
    cerr << "APPSPACK::GCI::unpack - error in MPI unpack" << endl;
    throw "APPSPACK error";
  }

#else

   cerr << "APPSPACK::GCI::unpack requires PVM of MPI" << endl;
   throw "APPSPACK error";

#endif
}

void GCI::unpack(int& length, double*& array)
{
  // Unpack array s. If s is NULL, memory is allocated for it. If
  // not, the length of s is checked to be sure it's large enough.

  unpack(length);
  array = new double[length];

#ifdef HAVE_PVM

  pvmUnpackCheck(pvm_upkdouble(array, length, 1));

#elif defined HAVE_MPI

  int e = MPI_Unpack(recvBuffer, msgSize, &recvPosition, array, length, MPI_DOUBLE, APPS_COMM);
  if (e != MPI_SUCCESS) 
  {
    cerr << "APPSPACK::GCI::unpack - error in MPI unpack" << endl;
    throw "APPSPACK error";
  }

#else

   cerr << "APPSPACK::GCI::unpack requires PVM of MPI" << endl;
   throw "APPSPACK error";

#endif
}

void GCI::unpack(int& length, float*& array)
{
  // Unpack array s. If s is NULL, memory is allocated for it. If
  // not, the length of s is checked to be sure it's large enough.

  unpack(length);
  array = new float[length];

#ifdef HAVE_PVM

  pvmUnpackCheck(pvm_upkfloat(array, length, 1));

#elif defined HAVE_MPI

  int e = MPI_Unpack(recvBuffer, msgSize, &recvPosition, array, length, MPI_FLOAT, APPS_COMM);
  if (e != MPI_SUCCESS) 
  {
    cerr << "APPSPACK::GCI::unpack - error in MPI unpack" << endl;
    throw "APPSPACK error";
  }

#else

   cerr << "APPSPACK::GCI::unpack requires PVM of MPI" << endl;
   throw "APPSPACK error";

#endif
}

// ----------------------------------------------------------------------
// Host Info
// ----------------------------------------------------------------------

int GCI::resetHostInfo() 
{
  // Reset host information by asking PVM to update pvmHostInfo - the data
  // structure that contains the host information. Return the number
  // of hosts.

#ifdef HAVE_PVM

  pvmHostInfo = NULL;

  if ((pvm_config(&nHosts, &nArch, &pvmHostInfo)) < 0) 
  {
    cerr << "APPSPACK::GCI::resetHostInfo - error from pvm_config" << endl;
    throw "APPSPACK error";
  }

  return nHosts;

#else

   cerr << "APPSPACK::GCI::resetHostInfo requires PVM" << endl;
   throw "APPSPACK error";

#endif
}

int GCI::getHostTid(int i) 
{
  // Get the host id of the ith host.

#ifdef HAVE_PVM

  return pvmHostInfo[i].hi_tid;

#else

   cerr << "APPSPACK::GCI::getHostTid requires PVM" << endl;
   throw "APPSPACK error";

#endif
}

int GCI::getHostSpeed(int i) 
{
  // Get the speed of the ith host.

#ifdef HAVE_PVM

  return pvmHostInfo[i].hi_speed;

#else

   cerr << "APPSPACK::GCI::getHostSpeed requires PVM" << endl;
   throw "APPSPACK error";

#endif
}

char* GCI::getHostName(int i) 
{
  // Get the name of the ith host.

#ifdef HAVE_PVM

  return pvmHostInfo[i].hi_name;

#else

   cerr << "APPSPACK::GCI::getHostName requires PVM" << endl;
   throw "APPSPACK error";

#endif
}

// ----------------------------------------------------------------------
// Helper functions for PVM
// ----------------------------------------------------------------------

#ifdef HAVE_PVM

void GCI::pvmPackCheck(int info)	// PRIVATE
{
  // Check return code from pvm_pack

  if (info == PvmNoMem) 
  {
    cerr << "APPSPACK::GCI::pack - out of memory" << endl;
    throw "APPSPACK error";
  }
  if (info == PvmNoBuf) 
  {
    cerr << "APPSPACK::GCI::pack - no active send buffer" << endl;
    throw "APPSPACK error";
  }
  if (info < 0) 
  {
    cerr << "APPSPACK::GCI::pack - unrecognized PVM error" << endl;
    throw "APPSPACK error";
  }
}

void GCI::pvmUnpackCheck(int info) // PRIVATE
{
  // Check return code form pvm_unpack

  if (info == PvmNoData) 
  {
    cerr << "APPSPACK::GCI::unpack - reading past end of buffer" << endl;
    throw "APPSPACK error";
  }
  if (info == PvmBadMsg) 
  {
    cerr << "APPSPACK::GCI::unpack - incompatiable encoding" << endl;
    throw "APPSPACK error";
  }
  if (info == PvmNoBuf) 
  {
    cerr << "APPSPACK::GCI::unpack - no active receive buffer" << endl;
    throw "APPSPACK error";
  }
  if (info < 0) 
  {
    cerr << "APPSPACK::GCI::unpack - unrecognized PVM error" << endl;
    throw "APPSPACK error";
  }
}

#endif


// ----------------------------------------------------------------------
// Helper functions for MPI
// ----------------------------------------------------------------------

#ifdef HAVE_MPI

int GCI::mpiGetMsgSize(MPI_Status localstatus, MPI_Datatype datatype)
{
  // Returns the size of the message localstatus is associated with.
  // "localstatus" is so named to separate it from the global variable
  // "status."

  int count;

  // This MPI call sets count = the number of items of type datatype
  // in the message. 
  int e = MPI_Get_count(&localstatus, datatype, &count);
  if (e != MPI_SUCCESS) 
  {
    cerr << "APPSPACK::GCI::mpiGetMsgSize - error" << endl;
    throw "APPSPACK error";
  }

  return(count);
}

void GCI::mpiStretchSendBuffer(int bufsize)
{
  // This is a helper function for the pack methods.  It makes the
  // size of the send buffer bufsize.

  // Create new buffer
  char* newbuffer = new char[bufsize];
  
  // Copy old buffer into new
  for(int i = 0; i < sendPosition; i++)
    newbuffer[i] = sendBuffer[i];

  // Delete old buffer and replace with new
  delete[] sendBuffer;
  sendBuffer = newbuffer;
}

#endif

