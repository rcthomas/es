#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.64])
AC_INIT([ES], [0.95], [rcthomas@lbl.gov])
AC_CONFIG_SRCDIR([src/ES_Accelerator.cc])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])
#AM_INIT_AUTOMAKE([-Wall -Werror])
AM_INIT_AUTOMAKE

# Set default optimization to '-O3' instead of the default '-g -O2'
if test x"${CXXFLAGS}" = x; then
   CXXFLAGS="-O3"
fi

# Set language
AC_LANG(C++)
AC_CANONICAL_HOST

# Checks for programs.
AC_PROG_AWK
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_LIBTOOL
AC_PROG_LN_S
AC_PROG_MAKE_SET

# Doxygen options
DX_PDF_FEATURE(ON)
DX_HTML_FEATURE(ON)
DX_INIT_DOXYGEN(ES, doxygen.txt, doc)

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_TYPE_SIZE_T
AC_F77_LIBRARY_LDFLAGS
if test x"${F77}" != x; then
   AC_F77_WRAPPERS
fi

# Checks for library functions.
AC_CHECK_FUNCS([pow sqrt])

# Check for arch flags
ACX_CXX_ARCHFLAG([no], [], [])

# Check for OpenMP
AX_OPENMP([ax_have_openmp=yes;AC_DEFINE(HAVE_OPENMP,1,[Define if OpenMP is enabled])])
AM_CONDITIONAL([HAVE_AM_OPENMP], [test $ax_have_openmp = yes])

# Check for MPI
ACX_MPI
AM_CONDITIONAL([HAVE_AM_MPI], [test "$acx_have_mpi" = 1])

# Check for CFITSIO
ACX_CFITSIO([], [AC_MSG_ERROR([Could not find the CFITSIO library!])])

# Check for YAML-CPP

AC_ARG_WITH([yaml-cpp],
  [AC_HELP_STRING([--with-yaml-cpp@<:@=DIR@:>@],
    [using YAML-CPP (DIR = prefix for YAML-CPP installation)])])
YAML_CPP_CPPFLAGS=''
YAML_CPP_LDFLAGS=''
if test "x$with_yaml_cpp" != xno; then
  if test -n "$with_yaml_cpp"; then
    YAML_CPP_CPPFLAGS="-I${with_yaml_cpp}/include"
    YAML_CPP_LDFLAGS="-L${with_yaml_cpp}/lib"
  fi
  tmp_CPPFLAGS=$CPPFLAGS
  tmp_LDFLAGS=$LDFLAGS
  CPPFLAGS="$CPPFLAGS $YAML_CPP_CPPFLAGS"
  LDFLAGS="$LDFLAGS $YAML_CPP_LDFLAGS"
  have_yaml_cpp=no
  AC_CHECK_HEADER([yaml-cpp/yaml.h],
    [AX_CXX_CHECK_LIB([yaml-cpp],
      [YAML::Node],
      [have_yaml_cpp=yes
        YAML_CPP_LDFLAGS="$YAML_CPP_LDFLAGS -lyaml-cpp"
        AC_DEFINE([HAVE_YAML_CPP], 1,
                  [Define to 1 if we can use YAML_CPP])
      ]
    )]
  )
  CPPFLAGS=$tmp_CPPFLAGS
  LDFLAGS=$tmp_LDFLAGS
fi
AC_SUBST([YAML_CPP_CPPFLAGS])
AC_SUBST([YAML_CPP_LDFLAGS])


# Generate Makefiles
AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([src/Makefile])
AC_OUTPUT

# Print out detected build options
AC_MSG_NOTICE([                                           ])
AC_MSG_NOTICE([=========== Build Configuration ===========])
AC_MSG_NOTICE([  C++ Compiler     : $CXX])
AC_MSG_NOTICE([  MPI C++ Compiler : $MPICXX])
AC_MSG_NOTICE([  C Compile flags  : $CXXFLAGS])
if test x"${ax_have_openmp}" = xyes; then
  AC_MSG_NOTICE([  OpenMP           : Detected ($OPENMP_CXXFLAGS)])
else
  AC_MSG_NOTICE([  OpenMP           : Disabled])
fi
AC_MSG_NOTICE([===========================================])
AC_MSG_NOTICE([                                           ])

