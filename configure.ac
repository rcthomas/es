#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.64])
AC_INIT([ES], [0.95], [rcthomas@lbl.gov])
AC_CONFIG_SRCDIR([src/ES_Accelerator.cc])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([-Wall -Werror])
AC_LANG(C++)

# Checks for programs.
AC_PROG_AWK
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_RANLIB

# Check for CFITSIO

AC_ARG_WITH([cfitsio],
  [AC_HELP_STRING([--with-cfitsio@<:@=DIR@:>@],
    [using CFITSIO (DIR = prefix for CFITSIO installation)])])
CFITSIO_CPPFLAGS=''
CFITSIO_LDFLAGS=''
if test "x$with_cfitsio" != xno; then
  if test -n "$with_cfitsio"; then
    CFITSIO_CPPFLAGS="-I${with_cfitsio}/include"
    CFITSIO_LDFLAGS="-L${with_cfitsio}/lib"
  fi
  tmp_CPPFLAGS=$CPPFLAGS
  tmp_LDFLAGS=$LDFLAGS
  CPPFLAGS="$CPPFLAGS $CFITSIO_CPPFLAGS"
  LDFLAGS="$LDFLAGS $CFITSIO_LDFLAGS"
  have_cfitsio=no
  AC_CHECK_HEADER([fitsio.h],
    [AC_CHECK_LIB([cfitsio],
      [main],
      [have_cfitsio=yes
        CFITSIO_LDFLAGS="$CFITSIO_LDFLAGS -lcfitsio"
        AC_DEFINE([HAVE_CFITSIO], 1,
                  [Define to 1 if we can use CFITSIO])
      ]
    )]
  )
  CPPFLAGS=$tmp_CPPFLAGS
  LDFLAGS=$tmp_LDFLAGS
fi
AC_SUBST([CFITSIO_CPPFLAGS])
AC_SUBST([CFITSIO_LDFLAGS])

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

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_TYPE_SIZE_T

# Checks for library functions.
AC_CHECK_FUNCS([pow sqrt])

AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT
