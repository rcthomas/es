#
# SYNOPSIS
#
#   ACX_CFITSIO([ACTION-IF-FOUND[, ACTION-IF-NOT-FOUND]])
#
# DESCRIPTION
#
#   This macro looks for a version of the CFITSIO library.  The CFITSIO_CPPFLAGS
#   and CFITSIO output variables hold the compile and link flags.
#
#   To link an application with CFITSIO, you should link with:
#
#   	$CFITSIO
#
#   The user may use:
# 
#       --with-cfitsio-cpp=<flags> --with-cfitsio-libs=<flags> 
#
#   to manually specify the CFITSIO include and linking flags.
#
#   ACTION-IF-FOUND is a list of shell commands to run if an CFITSIO library is
#   found, and ACTION-IF-NOT-FOUND is a list of commands to run it if it is
#   not found. If ACTION-IF-FOUND is not specified, the default action will
#   define HAVE_CFITSIO and set output variables above.
#
#   This macro requires autoconf 2.50 or later.
#
# LAST MODIFICATION
#
#   2009-07-08
#
# COPYLEFT
#
#   Copyright (c) 2009 Theodore Kisner <tskisner@lbl.gov>
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

AC_DEFUN([ACX_CFITSIO], [
AC_PREREQ(2.50)

acx_cfitsio_ok=no
acx_cfitsio_default="-lcfitsio"
acx_fitsio_inc=no
acx_cfitsio_inc=no

CFITSIO_CPPFLAGS=""
CFITSIO=""

AC_ARG_WITH(cfitsio-cpp, [AC_HELP_STRING([--with-cfitsio-cpp=<flags>], [use serial CFITSIO preprocessing flags <flags>.  Set to "no" to disable.])])

AC_ARG_WITH(cfitsio-libs, [AC_HELP_STRING([--with-cfitsio-libs=<flags>], [use serial CFITSIO linking flags <flags>.  Set to "no" to disable.])])

if test x"$with_cfitsio_cpp" != x; then
   if test x"$with_cfitsio_cpp" != xno; then
      CFITSIO_CPPFLAGS="$with_cfitsio_cpp"
   else
      acx_cfitsio_ok=disable
   fi
fi

if test x"$with_cfitsio_libs" != x; then
   if test x"$with_cfitsio_libs" != xno; then
      CFITSIO="$with_cfitsio_libs"
   else
      acx_cfitsio_ok=disable
   fi
fi

if test $acx_cfitsio_ok = disable; then
   echo "**** CFITSIO explicitly disabled by configure."
else

   # Save environment

   acx_cfitsio_save_CC="$CC"
   acx_cfitsio_save_CPP="$CPP"
   acx_cfitsio_save_CPPFLAGS="$CPPFLAGS"
   acx_cfitsio_save_LIBS="$LIBS"

   # Test serial compile and linking

   CPPFLAGS="$CPPFLAGS $CFITSIO_CPPFLAGS"
   LIBS="$CFITSIO $acx_cfitsio_save_LIBS -lm"

   AC_CHECK_HEADERS([fitsio.h],[acx_fitsio_inc="yes"],[])

   if test $acx_fitsio_inc = no; then
      AC_CHECK_HEADERS([cfitsio/fitsio.h],[acx_cfitsio_inc="yes"],[])
   fi

   AC_MSG_CHECKING([for ffopen in user specified location])
   AC_TRY_LINK_FUNC(ffopen, [acx_cfitsio_ok=yes;AC_DEFINE(HAVE_CFITSIO,1,[Define if you have the CFITSIO library.])], [])
   AC_MSG_RESULT($acx_cfitsio_ok)

   if test $acx_cfitsio_ok = no; then
      CFITSIO="$acx_cfitsio_default"
      LIBS="$acx_cfitsio_default $acx_cfitsio_save_LIBS -lm"
      AC_MSG_CHECKING([for ffopen in default location])
      AC_TRY_LINK_FUNC(ffopen, [acx_cfitsio_ok=yes;AC_DEFINE(HAVE_CFITSIO,1,[Define if you have the CFITSIO library.])], [])
      AC_MSG_RESULT($acx_cfitsio_ok)
   fi

   # Restore environment

   CC="$acx_cfitsio_save_CC"
   CPP="$acx_cfitsio_save_CPP"
   LIBS="$acx_cfitsio_save_LIBS"
   CPPFLAGS="$acx_cfitsio_save_CPPFLAGS"


fi

# Define exported variables
if test $acx_cfitsio_inc = yes; then
  echo "setting CFITSIO define"
  CFITSIO_CPPFLAGS="-DFITSIO_CFITSIO=1"
fi

AC_SUBST(CFITSIO_CPPFLAGS)
AC_SUBST(CFITSIO)


# Execute ACTION-IF-FOUND/ACTION-IF-NOT-FOUND:
   
if test x"$acx_cfitsio_ok" = xyes; then
   ifelse([$1],,[echo "**** Enabling support for CFITSIO."],[$1])
else
   ifelse([$2],,[echo "**** CFITSIO not found - disabling support."],[$2])
fi

])
