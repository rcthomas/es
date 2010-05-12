#
# SYNOPSIS
#
#   ACX_YAML([ACTION-IF-FOUND[, ACTION-IF-NOT-FOUND]])
#
# DESCRIPTION
#
#   This macro looks for a version of the YAML library.  The YAML_CPPFLAGS
#   and YAML output variables hold the compile and link flags.
#
#   To link an application with YAML, you should link with:
#
#   	$YAML
#
#   The user may use:
# 
#       --with-yaml-cpp=<flags> --with-yaml-libs=<flags> 
#
#   to manually specify the YAML include and linking flags.
#
#   ACTION-IF-FOUND is a list of shell commands to run if an YAML library is
#   found, and ACTION-IF-NOT-FOUND is a list of commands to run it if it is
#   not found. If ACTION-IF-FOUND is not specified, the default action will
#   define HAVE_YAML and set output variables above.
#
#   This macro requires autoconf 2.50 or later.
#
# LAST MODIFICATION
#
#   2010-05-12
#
# COPYLEFT
#
#   Copyright (c) 2010 Theodore Kisner <tskisner@lbl.gov>
#
#   This program is free software: you can redistribute it and/or modify it
#   under the terms of the GNU General Public License as published by the
#   Free Software Foundation, either version 3 of the License, or (at your
#   option) any later version.
#
#   This program is distributed in the hope that it will be useful, but
#   WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
#   Public License for more details.
#
#   You should have received a copy of the GNU General Public License along
#   with this program. If not, see <http://www.gnu.org/licenses/>.
#
#   As a special exception, the respective Autoconf Macro's copyright owner
#   gives unlimited permission to copy, distribute and modify the configure
#   scripts that are the output of Autoconf when processing the Macro. You
#   need not follow the terms of the GNU General Public License when using
#   or distributing such scripts, even though portions of the text of the
#   Macro appear in them. The GNU General Public License (GPL) does govern
#   all other use of the material that constitutes the Autoconf Macro.
#
#   This special exception to the GPL applies to versions of the Autoconf
#   Macro released by the Autoconf Macro Archive. When you make and
#   distribute a modified version of the Autoconf Macro, you may extend this
#   special exception to the GPL to apply to your modified version as well.

AC_DEFUN([ACX_YAML], [
AC_PREREQ(2.50)
AC_REQUIRE([AX_CXX_CHECK_LIB])

acx_yaml_ok=no
acx_yaml_default="-lyaml-cpp"

YAML_CPPFLAGS=""
YAML=""

AC_ARG_WITH(yaml-cpp, [AC_HELP_STRING([--with-yaml-cpp=<flags>], [use YAML preprocessing flags <flags>.  Set to "no" to disable.])])

AC_ARG_WITH(yaml-libs, [AC_HELP_STRING([--with-yaml-libs=<flags>], [use YAML linking flags <flags>.  Set to "no" to disable.])])

if test x"$with_yaml_cpp" != x; then
   if test x"$with_yaml_cpp" != xno; then
      YAML_CPPFLAGS="$with_yaml_cpp"
   else
      acx_yaml_ok=disable
   fi
fi

if test x"$with_yaml_libs" != x; then
   if test x"$with_yaml_libs" != xno; then
      YAML="$with_yaml_libs"
   else
      acx_yaml_ok=disable
   fi
fi

if test $acx_yaml_ok = disable; then
   echo "**** YAML explicitly disabled by configure."
else

   # Save environment

   acx_yaml_save_CC="$CC"
   acx_yaml_save_CPP="$CPP"
   acx_yaml_save_CPPFLAGS="$CPPFLAGS"
   acx_yaml_save_LIBS="$LIBS"

   # Test serial compile and linking

   CPPFLAGS="$CPPFLAGS $YAML_CPPFLAGS"
   LIBS="$YAML $acx_yaml_save_LIBS -lm"

   AC_CHECK_HEADERS([yaml-cpp/yaml.h])

   AX_CXX_CHECK_LIB([], [YAML::Node], [acx_yaml_ok=yes;AC_DEFINE(HAVE_YAML,1,[Define if you have the YAML library.])])

   if test $acx_yaml_ok = no; then
      YAML="$acx_yaml_default"
      LIBS="$acx_yaml_default $acx_yaml_save_LIBS -lm"
      AX_CXX_CHECK_LIB([], [YAML::Node], [acx_yaml_ok=yes;AC_DEFINE(HAVE_YAML,1,[Define if you have the YAML library.])])
   fi

   # Restore environment

   CC="$acx_yaml_save_CC"
   CPP="$acx_yaml_save_CPP"
   LIBS="$acx_yaml_save_LIBS"
   CPPFLAGS="$acx_yaml_save_CPPFLAGS"

fi

# Define exported variables

AC_SUBST(YAML_CPPFLAGS)
AC_SUBST(YAML)

# Execute ACTION-IF-FOUND/ACTION-IF-NOT-FOUND:
   
if test x"$acx_yaml_ok" = xyes; then
   ifelse([$1],,[echo "**** Enabling support for YAML."],[$1])
else
   ifelse([$2],,[echo "**** YAML not found - disabling support."],[$2])
fi

])



#AC_ARG_WITH([yaml-cpp],
#  [AC_HELP_STRING([--with-yaml-cpp@<:@=DIR@:>@],
#    [using YAML-CPP (DIR = prefix for YAML-CPP installation)])])
#YAML_CPP_CPPFLAGS=''
#YAML_CPP_LDFLAGS=''
#if test "x$with_yaml_cpp" != xno; then
#  if test -n "$with_yaml_cpp"; then
#    YAML_CPP_CPPFLAGS="-I${with_yaml_cpp}/include"
#    YAML_CPP_LDFLAGS="-L${with_yaml_cpp}/lib"
#  fi
#  tmp_CPPFLAGS=$CPPFLAGS
#  tmp_LDFLAGS=$LDFLAGS
#  CPPFLAGS="$CPPFLAGS $YAML_CPP_CPPFLAGS"
#  LDFLAGS="$LDFLAGS $YAML_CPP_LDFLAGS"
#  have_yaml_cpp=no
#  AC_CHECK_HEADER([yaml-cpp/yaml.h],
#    [AX_CXX_CHECK_LIB([yaml-cpp],
#      [YAML::Node],
#      [have_yaml_cpp=yes
#        YAML_CPP_LDFLAGS="$YAML_CPP_LDFLAGS -lyaml-cpp"
#        AC_DEFINE([HAVE_YAML_CPP], 1,
#                  [Define to 1 if we can use YAML_CPP])
#      ]
#    )]
#  )
#  CPPFLAGS=$tmp_CPPFLAGS
#  LDFLAGS=$tmp_LDFLAGS
#fi
#AC_SUBST([YAML_CPP_CPPFLAGS])
#AC_SUBST([YAML_CPP_LDFLAGS])
