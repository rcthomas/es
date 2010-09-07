// $Id: cex_msgtags.hpp,v 1.1.2.1 2007/02/15 18:54:29 jgriffi Exp $ 
// $Source: /space/CVS-Acro/acro/packages/appspack/appspack/example-custom-executor/cex_msgtags.hpp,v $ 

/*!
  \file cex_msgtags.hpp
  \brief File containing the message tags for the custom executor.
  \sa \ref pageCustomize_executor 
*/

//! MPI message tag for sending quit command to workers.
#define QUIT 0
//! MPI message tag for sending/receiving size of a point.
#define SIZE 1
//! MPI message tag for sending/receiving a point x.
#define XVEC 2
//! MPI message tag for sending/receiving a corresponding point tag.
#define XTAG 3
//! MPI message tag for sending/receiving constraint violation code.
#define CODE 4
//! MPI message tag for sending/receiving a corresponding function value.
#define FVAL 5
