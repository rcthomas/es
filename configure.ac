#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.64])
AC_INIT([ES], [0.97], [rcthomas@lbl.gov])
AC_CONFIG_SRCDIR([src/libes/ES_Accelerator.cc])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])
#AM_INIT_AUTOMAKE([-Wall -Werror])
AM_INIT_AUTOMAKE

# Set default optimization to '-O3' instead of the default '-g -O2'
if test x"${CXXFLAGS}" = x; then
   CXXFLAGS="-O3"
fi

# Checks for programs.
AC_PROG_AWK
AC_PROG_CC
AC_PROG_F77
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_LIBTOOL
AC_PROG_LN_S
AC_PROG_MAKE_SET

# Doxygen options
DX_PDF_FEATURE([ON])
DX_HTML_FEATURE([ON])
DX_INIT_DOXYGEN([ES], [doxygen.txt], [doc])

# Set language
AC_LANG([C++])
AC_CANONICAL_HOST

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADERS([iostream iostream.h])
AC_CHECK_HEADERS([iomanip iomanip.h])
AC_CHECK_HEADERS([cmath math.h])
AC_CHECK_HEADERS([cstdlib stdlib.h])
AC_HEADER_STDBOOL
AC_TYPE_SIZE_T
AC_F77_LIBRARY_LDFLAGS
if test x"${F77}" != x; then
   AC_F77_WRAPPERS
fi

# can we say using std::ostream?
AC_MSG_CHECKING([whether "using std::ostream" is okay])
AC_COMPILE_IFELSE(
[
  AC_LANG_PROGRAM(
  [[
#ifdef HAVE_IOSTREAM
#include <iostream>
#elif defined(HAVE_IOSTREAM_H)
#include <iostream.h>
#endif
  ]],
  [[using std::ostream;]])
],
[AC_MSG_RESULT(yes)],
[
  AC_MSG_RESULT(no)
  AC_DEFINE(NO_USING_STD_OSTREAM,1,[define if "using std::ostream" is not okay])
])

# do we need "using namespace std"?
AC_MSG_CHECKING([whether "using namespace std" is required])
AC_TRY_COMPILE(
  [#include<string>],
  [string a;],
  [AC_MSG_RESULT([no])],
  [
    AC_MSG_RESULT([yes])
    AC_DEFINE([REQUIRE_USING_NAMESPACE_STD], [1], [define if "using namespace std" is required])
  ]
)

# Checks for library functions.
AC_CHECK_FUNCS([pow sqrt])

# Check for OpenMP
AX_OPENMP([ax_have_openmp=yes;AC_DEFINE([HAVE_OPENMP], [1], [Define if OpenMP is enabled])])
AM_CONDITIONAL([HAVE_AM_OPENMP], [test $ax_have_openmp = yes])

# Check for CFITSIO
ACX_CFITSIO([], [AC_MSG_ERROR([Could not find the CFITSIO library!])])

# Check for LAPACK/BLAS
acx_have_lapack=no
ACX_BLAS
ACX_LAPACK([acx_have_lapack=yes;AC_DEFINE([HAVE_LAPACK], [1], [Define if you have the Lapack library.])], [AC_MSG_WARN([Could not find a Lapack library- synapps disabled])])
AM_CONDITIONAL([HAVE_AM_LAPACK], [test $acx_have_lapack = yes])

# Check for MPI
acx_have_mpi=no
ACX_MPI([acx_have_mpi=yes;AC_DEFINE([HAVE_MPI], [1],[Define if you have the MPI library.])])
AM_CONDITIONAL([HAVE_AM_MPI], [test $acx_have_mpi = yes])

# Check for APPSPACK
acx_have_appspack=no
ACX_APPSPACK([acx_have_appspack=yes;AC_DEFINE([HAVE_APPSPACK], [1], [Define if you have the Appspack library.])], [AC_MSG_WARN([Could not find the Appspack tools- synapps disabled])])
AM_CONDITIONAL([HAVE_AM_APPSPACK], [test $acx_have_appspack = yes])

# Generate Makefiles
AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([src/Makefile])
AC_CONFIG_FILES([src/libes/Makefile])
AC_CONFIG_FILES([src/syn++/Makefile])
AC_CONFIG_FILES([src/synapps/Makefile])
AC_CONFIG_FILES([external/Makefile])
AC_CONFIG_FILES([external/yaml/Makefile])
AC_OUTPUT

# Print out detected build options
AC_MSG_NOTICE([                                           ])
AC_MSG_NOTICE([  ======================== Build Configuration =============================])
AC_MSG_NOTICE([  C++ Compiler      : $CXX])
AC_MSG_NOTICE([  MPI C++ Compiler  : $MPICXX])
AC_MSG_NOTICE([  C++ Compile flags : $CXXFLAGS])
if test x"${ax_have_openmp}" = xyes; then
  AC_MSG_NOTICE([  OpenMP            : Detected ($OPENMP_CXXFLAGS)])
else
  AC_MSG_NOTICE([  OpenMP            : Disabled])
fi
AC_MSG_NOTICE([  CFITSIO           : $CFITSIO])
AC_MSG_NOTICE([  BLAS              : $BLAS_LIBS])
AC_MSG_NOTICE([  LAPACK            : $LAPACK_LIBS])
if test x"${acx_have_appspack}" = xyes; then
  AC_MSG_NOTICE([  APPSPACK          : $APPSPACK])
else
  AC_MSG_NOTICE([  APPSPACK          : Disabled (synapps not built)])
fi
AC_MSG_NOTICE([  ==========================================================================])
AC_MSG_NOTICE([                                           ])

