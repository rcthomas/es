// $Id: APPSPACK_Common.hpp,v 1.17 2005/09/14 23:51:24 jgriffi Exp $ 
// $Source: /space/CVS-Acro/acro/packages/appspack/appspack/src/APPSPACK_Common.hpp,v $ 

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
  \file APPSPACK_Common.hpp
  \brief All standard C/C++ and STL header files are included via this
  file, and all "using" declarations are declared here.

  The purpose of this file is to handle all the special cases for the
  way the various files are included and also to enforse general
  conventions. For example, on some systems it is vital to declare
  <code>using namespace std;</code>, while on other systems it is just
  the opposite. All these situations are handled in this file.
*/

#ifndef APPSPACK_COMMON_HPP
#define APPSPACK_COMMON_HPP

#include <config.h>

#ifdef HAVE_IOSTREAM
#include <iostream>
#elif defined(HAVE_IOSTREAM_H)
#include <iostream.h>
#else
#include <iostream>
#endif

#ifdef HAVE_IOMANIP
#include <iomanip>
#elif defined(HAVE_IOMANIP_H)
#include <iomanip.h>
#else
#include <iomanip>
#endif

#ifdef HAVE_CMATH
#include <cmath>
#elif defined(HAVE_MATH_H)
#include <math.h>
#else
#include <math.h>
#endif

#ifdef HAVE_CSTDIO
#include <cstdio>	
#elif defined(HAVE_STDIO_H)
#include <stdio.h>
#else		
#include <stdio.h>
#endif

#ifdef HAVE_CSTDLIB
#include <cstdlib>	
#elif defined(HAVE_STDLIB_H)
#include <stdlib.h>
#else		
#include <stdlib.h>
#endif

#ifdef HAVE_CTIME
#include <ctime>	
#elif defined(HAVE_TIME_H)
#include <time.h>
#else		
#include <time.h>
#endif

#ifdef HAVE_CSTRING
#include <cstring>	
#elif defined(HAVE_STRING_H)
#include <string.h>
#else		
#include <string.h>
#endif

#include <unistd.h>		
#include <string>
#include <vector>
#include <list>
#include <fstream>
#include <map>
#include <sstream>
#include <float.h>

#ifdef REQUIRE_USING_NAMESPACE_STD
using namespace std;
#endif

// Special stuff for ASCI Red
#if defined TFLOP || defined TFLOPS || defined COUGAR
#define SNL_TFLOPS_ENV
#endif

using std::vector;

#endif
