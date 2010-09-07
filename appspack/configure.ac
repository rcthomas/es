# ------------------------------------------------------------------------
# Process this file with autoconf to produce a configure script.
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

AC_INIT(appspack, 5.0.1, appspack-developers@software.sandia.gov)

# This is to protect against accidentally specifying the wrong
# directory with --srcdir.

AC_CONFIG_SRCDIR(src/APPSPACK_Common.hpp)

# Specify directory for auxillary build tools (e.g., install-sh,
# config.sub, config.guess) and M4 files.

AC_CONFIG_AUX_DIR(config)

# Header file created by configure

AM_CONFIG_HEADER(src/APPSPACK_Config.hpp:src/APPSPACK_Config.hpp.in)

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

# Use automake, getting name and version from variables defined by
# AC_INIT command.

AM_INIT_AUTOMAKE(${PACKAGE_NAME}, ${PACKAGE_VERSION})

# Specify required version of autoconf.

AC_PREREQ(2.52)

# ------------------------------------------------------------------------
# Check for user overrides (to be used later, maybe)
# ------------------------------------------------------------------------

if test -n "${CFLAGS}"; then
  USERDEF_CFLAGS=yes
fi

if test -n "${CXXFLAGS}"; then
  USERDEF_CXXFLAGS=yes
fi

if test -n "${CPPFLAGS}"; then
  USERDEF_CPPFLAGS=yes
fi

if test -n "${F77}"; then
  USERDEF_F77=yes
fi

if test -n "${FFLAGS}"; then
  USERDEF_FFLAGS=yes
fi

if test -n "${LDFLAGS}"; then
  USERDEF_LDFLAGS=yes
fi

# ------------------------------------------------------------------------
# Checks for extra flags, etc
# ------------------------------------------------------------------------

AC_ARG_WITH(cppflags,
[AC_HELP_STRING([--with-cppflags],[add extra preprocessor flags (e.g., "-Dflag -Idir")])],
[EXTRA_CPPFLAGS=${withval}]
)


AC_ARG_WITH(cxxflags,
[AC_HELP_STRING([--with-cxxflags],[add extra C++ compiler flags])],
[EXTRA_CXXFLAGS=${withval}]
)


AC_ARG_WITH(ldflags,
[AC_HELP_STRING([--with-ldflags],[add extra linker (typically -L) flags])],
[EXTRA_LDFLAGS=${withval}]
)

AC_ARG_WITH(libs,
[AC_HELP_STRING([--with-libs],[add extra library (typically -l) flags])],
[EXTRA_LIBS=${withval}]
)

# ------------------------------------------------------------------------
# Enable CDDLIB
# ------------------------------------------------------------------------

AC_ARG_ENABLE(cddlib,
[AC_HELP_STRING([--enable-cddlib],[Enables CDDLIB])],
[ENABLE_CDDLIB=$enableval],
[ENABLE_CDDLIB=no]
)

if test X${ENABLE_CDDLIB} = Xyes; then
    AC_DEFINE(HAVE_CDDLIB,,[define that cddlib is being used])
fi

AM_CONDITIONAL(BUILD_CDDLIB, test X${ENABLE_CDDLIB} = Xyes)

# ------------------------------------------------------------------------
# Disable building of executables
# ------------------------------------------------------------------------

AC_ARG_ENABLE(executables,
[AC_HELP_STRING([--disable-executables],[Do not build any executables, just the library])],
[ENABLE_EXECUTABLES=$enableval],
[ENABLE_EXECUTABLES=yes]
)

AM_CONDITIONAL(BUILD_EXECUTABLES, test X${ENABLE_EXECUTABLES} = Xyes)

# ------------------------------------------------------------------------
# Disable serial version of APPSPACK
# ------------------------------------------------------------------------

AC_ARG_ENABLE(serial,
[AC_HELP_STRING([--disable-serial],[Do not build serial executable])],
[ENABLE_SERIAL=$enableval],
[ENABLE_SERIAL=yes]
)

AM_CONDITIONAL(BUILD_SERIAL, test X${ENABLE_SERIAL} = Xyes)

# ------------------------------------------------------------------------
# MPI, Part 1 of 4: configure options
# ------------------------------------------------------------------------

AC_ARG_ENABLE(mpi,
[AC_HELP_STRING([--enable-mpi],[Enable MPI support])],
[ENABLE_MPI=$enableval],
[ENABLE_MPI=no]
)

AC_ARG_WITH(mpi-compilers,
[AC_HELP_STRING([--with-mpi-compilers=DIR],
[use MPI compilers mpicc, mpif77, and mpicxx (or mpiCC) in the specified path or in the default path if no path is specified. Automatically enables MPI])],
[
  ENABLE_MPI=yes
  if test X${withval} = Xyes; then
    # Check for mpicxx, if it does not exist, use mpiCC instead.
    AC_CHECK_PROG(MPI_CXX, mpicxx, mpicxx, mpiCC)
    MPI_CC=mpicc
    MPI_F77=mpif77
  else
    MPI_TEMP_CXX=${withval}/mpicxx
    if test -f ${MPI_TEMP_CXX}; then
      MPI_CXX=${MPI_TEMP_CXX}
    else
      MPI_CXX=${withval}/mpiCC
    fi
    MPI_CC=${withval}/mpicc
    MPI_F77=${withval}/mpif77
  fi
]
)

AC_ARG_WITH(mpi,
[AC_HELP_STRING([--with-mpi=MPIROOT],[use MPI root directory. Automatically enables MPI.])],
[
  # This if statement is necessary (for now) to make this compatible with DAKOTA
  if test X${withval} != X; then
     ENABLE_MPI=yes
     MPI_DIR=${withval}
     AC_MSG_CHECKING(MPI directory)
     AC_MSG_RESULT([${MPI_DIR}])
  fi
]
)


AC_ARG_WITH(mpi-libs,
[AC_HELP_STRING([--with-mpi-libs="LIBS"],[MPI libraries @<:@"-lmpi"@:>@])],
[
  MPI_LIBS=${withval}
  AC_MSG_CHECKING(user-defined MPI libraries)
  AC_MSG_RESULT([${MPI_LIBS}])
]
)


AC_ARG_WITH(mpi-incdir,
[AC_HELP_STRING([--with-mpi-incdir=DIR],[MPI include directory @<:@MPIROOT/include@:>@])],
[
  MPI_INC=${withval}
  AC_MSG_CHECKING(user-defined MPI includes)
  AC_MSG_RESULT([${MPI_INC}])
]
)


AC_ARG_WITH(mpi-libdir,
[AC_HELP_STRING([--with-mpi-libdir=DIR],[MPI library directory @<:@MPIROOT/lib@:>@])],
[
  MPI_LIBDIR=${withval}
  AC_MSG_CHECKING(user-defined MPI library directory)
  AC_MSG_RESULT([${MPI_LIBDIR}])
]
)

AC_MSG_CHECKING(whether we are using MPI)
AC_MSG_RESULT([${ENABLE_MPI}])

# ------------------------------------------------------------------------
# MPI, Part 2 of 4: Check for MPI compilers (must be done *before* AC_PROG_CXX)
# ------------------------------------------------------------------------

if test -n "${MPI_CXX}"; then

  # Test if the file exists (first the easy way, then the hard way)
  if test -f ${MPI_CXX}; then   
    MPI_CXX_EXISTS=yes
  else
    AC_CHECK_PROG(MPI_CXX_EXISTS, ${MPI_CXX}, yes, no)
  fi

  if test "X${MPI_CXX_EXISTS}" = "Xyes"; then
    CXX=${MPI_CXX}
  else
    echo "-----"
    echo "Cannot find MPI C++ compiler ${MPI_CXX}."
    echo "Specify a path to all mpi compilers with --with-mpi-compilers=PATH"
    echo "or specify a C++ compiler using CXX=<compiler>"
    echo "Do not use --with-mpi-compilers if using CXX=<compiler>"
    echo "-----"
    AC_MSG_ERROR([MPI C++ compiler (${MPI_CXX}) not found.])
  fi

fi

if test -n "${MPI_CC}"; then
  if test -f ${MPI_CC}; then
    MPI_CC_EXISTS=yes
  else
    AC_CHECK_PROG(MPI_CC_EXISTS, ${MPI_CC}, yes, no)
  fi

  if test "X${MPI_CC_EXISTS}" = "Xyes"; then
    CC=${MPI_CC}
  else
    echo "-----"
    echo "Cannot find MPI C compiler ${MPI_CC}."
    echo "Specify a path to all mpi compilers with --with-mpi-compilers=PATH"
    echo "or specify a C compiler using CC=<compiler>"
    echo "Do not use --with-mpi-compilers if using CC=<compiler>"
    echo "-----"
    AC_MSG_ERROR([MPI C compiler (${MPI_CC}) not found.])
  fi
fi

if test -n "${MPI_F77}"; then
  if test -f ${MPI_F77}; then
    MPI_F77_EXISTS=yes
  else
    AC_CHECK_PROG(MPI_F77_EXISTS, ${MPI_F77}, yes, no)
  fi

  if test "X${MPI_F77_EXISTS}" = "Xyes"; then
    F77=${MPI_F77}
  else
    echo "-----"
    echo "Cannot find MPI Fortran compiler ${MPI_F77}."
    echo "Specify a path to all mpi compilers with --with-mpi-compilers=PATH"
    echo "or specify a Fortran 77 compiler using F77=<compiler>"
    echo "Do not use --with-mpi-compilers if using F77=<compiler>"
    echo "-----"
    AC_MSG_ERROR([MPI Fortran 77 compiler (${MPI_F77}) not found.])
  fi
fi

# ------------------------------------------------------------------------
# Checks for programs
# ------------------------------------------------------------------------


c_compilers="cc gcc"
cxx_compilers="CC cxx xlC c++ g++ gcc"
# Revise compiler list for Cygwin ... which get's confused about 'CC'
echo "TARGET $target"
case $target in
  *cygwin) cxx_compilers="g++ gcc"; c_compilers="gcc";;
esac
echo "COMPILERS $target"
AC_PROG_CC(${c_compilers})
AC_PROG_CXX(${cxx_compilers})

AC_PROG_CXXCPP
AC_PROG_F77(f77 g77 f90 xlf90 ifort)
AC_PROG_RANLIB
AC_CXX_COMPILER_VENDOR([CXX_VENDOR])
AC_CXX_CPPFLAGS_STD_LANG([CXXFLAGS])

# ------------------------------------------------------------------------
# Checks for special compiler options
# ------------------------------------------------------------------------


case $target in

  # OSF test
  *osf*)

    case $CXX in
       cxx) SPECIAL_CXXFLAGS="-distinguish_nested_enums -tlocal";;
    esac

  ;;

  # IBM test
  rs6000-ibm-aix3.2.* | rs6000-ibm-aix4.* | powerpc-ibm-aix4.*)

    case $CXX in
      xlC* ) SPECIAL_CXXFLAGS="-qnolm -qrtti";;
    esac

  ;;

  # SGI/IRIX test
  mips-sgi-irix* ) 

    if test "${CXX}" = "CC"; then
     # SPECIAL_CXXFLAGS="-LANG:std -LANG:ansi-for-init-scope=ON -64 -DSGI64"
     # FFLAGS_MISC="-64"
     SPECIAL_CXXFLAGS="-LANG:std -LANG:ansi-for-init-scope=ON"
     FFLAGS_MISC=""
     F77_MISC=f77
     SPECIAL_AR="CC -ar -W,l -W,s -o"    
    fi

  ;;

  sparc-sun-solaris* )

    if test "${CXX}" = "CC"; then
       SPECIAL_AR="CC -xar -o"
    fi

  ;;

esac

CXXFLAGS="${CXXFLAGS} ${SPECIAL_CXXFLAGS}"
FFLAGS="${FFLAGS} ${SPECIAL_FFLAGS}"

AC_MSG_CHECKING([for C++ flags])

if test "X${USERDEF_CXXFLAGS}" = "Xyes"; then

  AC_MSG_RESULT([${CXXFLAGS} (user-specified via CXXFLAGS)])

else

  if test -n "${CXXFLAGS_MISC}"; then
    CXXFLAGS="${CXXFLAGS} ${CXXFLAGS_MISC}"
  fi			  

  AC_MSG_RESULT([${CXXFLAGS}])

fi

if test "X${USERDEF_F77}" != "Xyes"; then
  if test -n "${F77_MISC}"; then
    F77="${F77_MISC}"
  fi			  
fi

AC_MSG_CHECKING([for F77 flags])

if test "X${USERDEF_FFLAGS}" = "Xyes"; then

  AC_MSG_RESULT([${FFLAGS} (user-specified via FFLAGS)])

else

  if test -n "${FFLAGS_MISC}"; then
    FFLAGS="${FFLAGS} ${FFLAGS_MISC}"
  fi			  

  AC_MSG_RESULT([${FFLAGS}])

fi

# ------------------------------------------------------------------------
# Checks for extra flags
# ------------------------------------------------------------------------

AC_MSG_CHECKING([for extra C++ compiler flags])
if test -z "${EXTRA_CXXFLAGS}"; then
  AC_MSG_RESULT(none)
else
  AC_MSG_RESULT([${EXTRA_CXXFLAGS}])
  CXXFLAGS="${CXXFLAGS} ${EXTRA_CXXFLAGS}"
fi

AC_MSG_CHECKING([for extra preprocessor flags])
if test -z "${EXTRA_CPPFLAGS}"; then
  AC_MSG_RESULT(none)
else
  AC_MSG_RESULT([${EXTRA_CPPFLAGS}])
  CPPFLAGS="${CPPFLAGS} ${EXTRA_CPPFLAGS}"
fi

AC_MSG_CHECKING([for extra linker flags])
if test -z "${EXTRA_LDFLAGS}"; then
  AC_MSG_RESULT(none)
else
  AC_MSG_RESULT([${EXTRA_LDFLAGS}])
  LDFLAGS="${LDFLAGS} ${EXTRA_LDFLAGS}"
fi

AC_MSG_CHECKING([for extra library flags])
if test -z "${EXTRA_LIBS}"; then
  AC_MSG_RESULT(none)
else
  AC_MSG_RESULT([${EXTRA_LIBS}])
  LIBS="${LIBS} ${EXTRA_LIBS}"
fi

# ------------------------------------------------------------------------
# Alternate archiver
# ------------------------------------------------------------------------

AC_ARG_WITH(ar,
[AC_HELP_STRING([--with-ar],[specify archiver command @<:@"ar cru"@:>@])],
[ AC_MSG_CHECKING(user-defined archiver)
  AC_MSG_RESULT([${withval}])
  ALTERNATE_AR="${withval}"
])

if test -n "${SPECIAL_AR}" && test -z "${ALTERNATE_AR}" ; then
  ALTERNATE_AR="${SPECIAL_AR}"
fi

AC_MSG_CHECKING(for special archiver command)
if test -n "${ALTERNATE_AR}"; then
   AC_MSG_RESULT([${ALTERNATE_AR}])
   AM_CONDITIONAL(USE_ALTERNATE_AR, true)
else
   AC_MSG_RESULT([none])
   AM_CONDITIONAL(USE_ALTERNATE_AR, false)
fi
AC_SUBST(ALTERNATE_AR)

# ------------------------------------------------------------------------
# MPI, Part 3 of 4: compile check
# ------------------------------------------------------------------------

if test "X${ENABLE_MPI}" = "Xyes"; then

  if test -n "${MPI_DIR}" && test -z "${MPI_INC}"; then
    MPI_INC="${MPI_DIR}/include"
  fi

  if test -n "${MPI_INC}"; then
    CPPFLAGS="${CPPFLAGS} -I${MPI_INC}"
  fi

  AC_LANG([C++]) 
  AC_MSG_CHECKING(for mpi.h)
  AC_PREPROC_IFELSE(
  [AC_LANG_SOURCE([[#include "mpi.h"]])],
  [ 
    AC_MSG_RESULT(yes)
    AC_DEFINE(HAVE_MPI,,[define that mpi is being used])
  ],[
    AC_MSG_RESULT(no)  
    echo "---"
    echo "Cannot find header file mpi.h."
    echo "Either compile without mpi (no --enable-mpi or --with-mpi directives)"
    echo "or specify --with-mpi-incdir=<dir>."
    echo "---"
    AC_MSG_ERROR(cannot find mpi.h)
  ])

  #Define Automake conditional for mpi builds: HAVE_MPI
  AM_CONDITIONAL(BUILD_MPI, true)

else

  AM_CONDITIONAL(BUILD_MPI, false)

fi

# ------------------------------------------------------------------------
# Checks for header files
# ------------------------------------------------------------------------

#iostream
AC_LANG([C++]) 
AC_CHECK_HEADERS(
[iostream iostream.h], 
break, 
AC_MSG_NOTICE([You do not appear to have iostream nor iostream.h!!]))

#iomanip
AC_LANG([C++]) 
AC_CHECK_HEADERS(
[iomanip iomanip.h],
break,
AC_MSG_NOTICE([You do not appear to have iomanip nor iomanip.h!!]))

#cmath
AC_LANG([C++]) 
AC_CHECK_HEADERS(
[cmath math.h],
break,
AC_MSG_NOTICE([You do not appear to have cmath nor math.h!!]))

#cstdlib
AC_LANG([C++]) 
AC_CHECK_HEADERS(
[cstdlib stdlib.h],
break,
AC_MSG_NOTICE([You do not appear to have cstdlib nor stdlib.h!!]))

# can we say using std::ostream?
AC_LANG([C++]) 
AC_MSG_CHECKING(whether "using std::ostream" is okay)
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

# ------------------------------------------------------------------------
# Checks for C++ anomalies
# ------------------------------------------------------------------------

# do we need "using namespace std"?
AC_LANG_CPLUSPLUS 
AC_MSG_CHECKING(whether "using namespace std" is required)
AC_TRY_COMPILE(
[#include<string>],
[string a;],
[AC_MSG_RESULT(no)], 
[
  AC_MSG_RESULT(yes)  
  AC_DEFINE(REQUIRE_USING_NAMESPACE_STD,,[define if "using namespace std" is required])
]
)

# ------------------------------------------------------------------------
# Checks for libraries
# ------------------------------------------------------------------------

if test X${ENABLE_EXECUTABLES} = Xyes; then

  AC_SEARCH_LIBS(pow,[m],,AC_MSG_ERROR(Cannot find math library))
  AC_SEARCH_LIBS(sqrt,[m],,AC_MSG_ERROR(Cannot find math library))

  # Checks for libraries.
  AC_F77_LIBRARY_LDFLAGS
  LIBS=" ${FLIBS} ${LIBS}"

  # Find valid BLAS
  ACX_BLAS(,[AC_MSG_WARN([No BLAS --- either because it is disabled or could not be found])])
  LIBS=" ${BLAS_LIBS} ${LIBS}"

  # Find valid LAPACK (defines HAVE_LAPACK if LAPACK is available)
  ACX_LAPACK(,[AC_MSG_WARN([No LAPACK --- either because it is disabled or could not be found])])
  LIBS=" ${LAPACK_LIBS} ${LIBS}"

  # Define F77_FUNC - used for correctly munging Fortran function calls
  AC_F77_WRAPPERS
fi

# ------------------------------------------------------------------------
# MPI, Part 4 of 4: Linking
# ------------------------------------------------------------------------

if test X${ENABLE_EXECUTABLES} = Xyes && test "X${ENABLE_MPI}" = "Xyes"; then

  if test -n "${MPI_DIR}" && test -z "${MPI_LIBDIR}"; then
    MPI_LIBDIR="${MPI_DIR}/lib"
  fi

  if test -n "${MPI_LIBDIR}"; then
    LDFLAGS="${LDFLAGS} -L${MPI_LIBDIR}"
  fi

  if test -z "${MPI_LIBS}" && test -n "${MPI_LIBDIR}"; then
    MPI_LIBS="-lmpi"
  fi

  if test -n "${MPI_LIBS}"; then
    LIBS="${MPI_LIBS} ${LIBS}"
  fi

  AC_LANG([C++]) 
  AC_MSG_CHECKING(whether MPI will link using C++ compiler)
  AC_LINK_IFELSE(
  [AC_LANG_PROGRAM([[#include <mpi.h>]], [[int c; char** v; MPI_Init(&c,&v);]])],
  [AC_MSG_RESULT(yes)],
  [AC_MSG_RESULT(no)  
   echo "-----"
   echo "Cannot link simple MPI program."
   echo "Try --with-mpi-cxx to specify MPI C++ compile script."
   echo "Or try --with-mpi-libs, --with-mpi-incdir, --with-mpi-libdir"
   echo "to specify all the specific MPI compile options."
   echo "-----"
   AC_MSG_ERROR(MPI cannot link)
  ])

fi

# ------------------------------------------------------------------------
# Checks for linker characteristics
# ------------------------------------------------------------------------

# -- DEC CXX --
case $target in
  alpha*)
    if test $CXX = cxx; then
      CXXREPOS="cxx_repository/*.o"
    fi
  ;;
esac

AC_MSG_CHECKING([for C++ repository files])
if test X$CXXREPOS = X; then
AC_MSG_RESULT(none)
else
AC_MSG_RESULT($CXXREPOS)
fi

AC_SUBST(CXXREPOS)


# ------------------------------------------------------------------------
# Checks for library functions
# ------------------------------------------------------------------------

# ------------------------------------------------------------------------
# Checks for system services
# ------------------------------------------------------------------------

# ------------------------------------------------------------------------
# Perform substitutions in output files
# ------------------------------------------------------------------------

#
###############################################################
# Is appspack being built as part of acro?  If so we will
# copy our library to acro/lib when done.  If not, we'll
# copy it to top_builddir/lib.
# TODO - is there a better way for appspack to know it's a subpackage?
# TODO - is there a variable containing the path that was used
#   to run configure?  If so we could check for a file in the
#   build directory.
#

if test -d ../../packages ; then
  AM_CONDITIONAL(I_AM_SUBPACKAGE, true)
else
  AM_CONDITIONAL(I_AM_SUBPACKAGE, false)
fi

# ------------------------------------------------------------------------
# Output files
# ------------------------------------------------------------------------

AC_CONFIG_FILES([
		Makefile 
		src/Makefile
		examples/Makefile
		doc/Makefile
		cddlib/Makefile
		])

AC_OUTPUT()

echo
echo "---"
echo "--- Thank you for downloading APPSPACK. Please register your"
echo "--- copy of APPSPACK by subscribing to our announce list at"
echo "--- http://software.sandia.gov/mailman/listinfo/appspack-announce"
echo "---"
echo "--- For more information, visit the APPSPACK web pages at"
echo "--- http://software.sandia.gov/appspack/"
echo
