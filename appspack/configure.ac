# ------------------------------------------------------------------------
# Use the "autogen.sh" script in this directory to run autotools.
# ------------------------------------------------------------------------

# @HEADER
# ************************************************************************
# 
#          APPSPACK: Asynchronous Parallel Pattern Search
#                 Copyright (2002) Sandia Corporation
# 
# Under terms of Contract DE-AC04-94AL85000, there is a non-exclusive
# license for use of this work by or on behalf of the U.S. Government.
# 
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.
#   
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#   
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
# 
# Questions? Contact Tammy Kolda (tgkolda@sandia.gov) 
# 
# ************************************************************************
# @HEADER

# ------------------------------------------------------------------------
# Initialization 
# ------------------------------------------------------------------------

# This must be the first line in configure.ac.
# Optional 3rd argument is email address for bugs.

AC_INIT([appspack], [5.0.2], [appspack-developers@software.sandia.gov])

# This is to protect against accidentally specifying the wrong
# directory with --srcdir.

AC_CONFIG_SRCDIR([src/libappspack/APPSPACK_Common.hpp])

# Specify directory for auxillary build tools (e.g., install-sh,
# config.sub, config.guess) and M4 files.

AC_CONFIG_AUX_DIR([config])
AC_CONFIG_MACRO_DIR([config])

# Header file created by configure

AM_CONFIG_HEADER([src/libappspack/APPSPACK_Config.hpp:src/libappspack/APPSPACK_Config.hpp.in])

# AM_MAINTAINER_MODE turns off maintainer-only makefile targets by
# default, and changes configure to understand a
# --enable-maintainer-mode option. --enable-maintainer-mode turns the
# maintainer-only targets back on. The maintainer-only makefile
# targets permit end users to clean automatically-generated files such
# as configure, which means they have to have autoconf and automake
# installed to repair the damage. AM_MAINTAINER_MODE makes it a bit
# harder for users to shoot themselves in the foot.

AM_MAINTAINER_MODE

# Define $build, $host, $target, etc

AC_CANONICAL_TARGET

# Use automake

AM_INIT_AUTOMAKE

# Specify required version of autoconf.

AC_PREREQ([2.64])

# Set default optimization to '-O3' instead of the default '-g -O2'
if test x"${CXXFLAGS}" = x; then
   CXXFLAGS="-O3"
fi

# Checks for programs.
AC_PROG_AWK
AC_PROG_CC
AC_PROG_CXX
AC_PROG_F77
AC_PROG_INSTALL
AC_PROG_LIBTOOL
AC_PROG_LN_S
AC_PROG_MAKE_SET

# Set language
AC_LANG([C++])
AC_CANONICAL_HOST

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADERS([iostream iostream.h])
AC_CHECK_HEADERS([iomanip iomanip.h])
AC_CHECK_HEADERS([cmath math.h])
AC_CHECK_HEADERS([cstdlib stdlib.h])
AC_TYPE_SIZE_T
AC_F77_LIBRARY_LDFLAGS
if test x"${F77}" != x; then
   AC_F77_WRAPPERS
fi

# can we say using std::ostream?
AC_MSG_CHECKING([whether using std ostream is okay])
AC_COMPILE_IFELSE([
  AC_LANG_PROGRAM([[
#ifdef HAVE_IOSTREAM
#include <iostream>
#elif defined(HAVE_IOSTREAM_H)
#include <iostream.h>
#endif
  ]],
  [[using std::ostream;]])
],
[AC_MSG_RESULT([yes])],
[AC_MSG_RESULT([no])
  AC_DEFINE([NO_USING_STD_OSTREAM], [1], [define if using std ostream is not okay])
])

# do we need "using namespace std"?
AC_MSG_CHECKING([whether using namespace std is required])
AC_TRY_COMPILE(
  [#include <string>],
  [string a;],
  [AC_MSG_RESULT([no])],
  [
    AC_MSG_RESULT([yes])
    AC_DEFINE([REQUIRE_USING_NAMESPACE_STD], [1], [define if using namespace std is required])
  ]
)

# Checks for library functions.
AC_CHECK_FUNCS([pow sqrt])

# This is internal, always build
AC_DEFINE([HAVE_CDDLIB], [1], [Always defined since libcdd is built internally.])

# Check for MPI
ACX_MPI([acx_have_mpi="1";AC_DEFINE([HAVE_MPI], [1], [Define if you have the MPI library.])])
AM_CONDITIONAL([HAVE_AM_MPI], [test "$acx_have_mpi" = 1])

# Perform a serial compilation if MPI compilers are not found
if test x"${acx_have_mpi}" = x1; then
   CPPFLAGS="${CPPFLAGS} -DHAVE_MPI"
else
   MPICXX="${CXX}"
fi

# Check for LAPACK/BLAS
ACX_BLAS([], [AC_MSG_ERROR([Could not find a Blas library!])])
ACX_LAPACK([], [AC_MSG_ERROR([Could not find a Lapack library!])])

# Generate Makefiles
AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([doc/Makefile])
AC_CONFIG_FILES([src/Makefile])
AC_CONFIG_FILES([src/cddlib/Makefile])
AC_CONFIG_FILES([src/libappspack/Makefile])
AC_CONFIG_FILES([src/serial/Makefile])
AC_CONFIG_FILES([src/mpi/Makefile])
AC_OUTPUT

# Print out detected build options
AC_MSG_NOTICE([                                           ])
AC_MSG_NOTICE([  ======================== Build Configuration =============================])
AC_MSG_NOTICE([  Serial C Compiler   : $CC])
AC_MSG_NOTICE([  Serial C++ Compiler : $CXX])
if test "$acx_have_mpi" = 1; then
  AC_MSG_NOTICE([  MPI C++ Compiler    : $MPICXX])
else
  AC_MSG_NOTICE([  MPI C++ Compiler    : none - doing serial build])
fi
AC_MSG_NOTICE([  C++ Compile flags   : $CXXFLAGS])
AC_MSG_NOTICE([  BLAS                : $BLAS_LIBS])
AC_MSG_NOTICE([  LAPACK              : $LAPACK_LIBS])
AC_MSG_NOTICE([  ==========================================================================])
AC_MSG_NOTICE([                                           ])

echo
echo "---"
echo "--- Thank you for downloading APPSPACK. Please register your"
echo "--- copy of APPSPACK by subscribing to our announce list at"
echo "--- http://software.sandia.gov/mailman/listinfo/appspack-announce"
echo "---"
echo "--- For more information, visit the APPSPACK web pages at"
echo "--- http://software.sandia.gov/appspack/"
echo
