#
# SYNOPSIS
#
#   ACX_MOAT([ACTION-IF-FOUND[, ACTION-IF-NOT-FOUND]])
#
# DESCRIPTION
#
#   This macro looks for a version of the Machine Optimized Analysis Tools
#   MOAT library.  By default, it looks for the "moat-config.sh" executable
#   in the default path.  The MOAT_CPPFLAGS, MOAT, and MOATMPI output variables 
#   hold the compile and serial/MPI link flags.
#
#   To link an application with MOAT, you should link with:
#
#   	$MOAT (serial) 
#          *OR*
#       $MOATMPI (MPI)
#
#   Additionally, the variable $MOATOCL is set to "1" if OpenCL support is
#   enabled in MOAT and set to "0" otherwise.
#
#   The user may use:
# 
#       --with-moat-config=<full path to moat-config.sh>
#
#   to manually specify the location of the moat-config.sh script.
#
#   ACTION-IF-FOUND is a list of shell commands to run if the MOAT library is
#   found, and ACTION-IF-NOT-FOUND is a list of commands to run it if it is
#   not found. If ACTION-IF-FOUND is not specified, the default action will
#   define HAVE_MOAT and set output variables above.
#
#   This macro requires autoconf 2.50 or later.
#
# LAST MODIFICATION
#
#   2009-04-15
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

AC_DEFUN([ACX_MOAT], [
AC_PREREQ(2.50)
AC_REQUIRE([ACX_MPI])

acx_moat_ok=no
acx_moat_serial=no
acx_moat_mpi=no

MOAT_CPPFLAGS=""
MOAT=""
MOATMPI=""
MOATOCL="0"

AC_ARG_WITH(moatconfig, [AC_HELP_STRING([--with-moatconfig=<PATH>], [use the moatconfig specified by <PATH>.])])

if test x"$with_moatconfig" != x; then
   acx_moat_config="$with_moatconfig"
else
   AC_PATH_PROG([acx_moat_config], moatconfig)
fi

if test x"$acx_moat_config" != x; then

   # Get flags from moat-config

   MOAT_CPPFLAGS=`$acx_moat_config --cflags`
   MOAT=`$acx_moat_config --libs`
   MOATMPI=`$acx_moat_config --mpilibs`
   MOATOCL=`$acx_moat_config --opencl`

   # Check for headers and libraries

   acx_moat_save_CC="$CC"
   acx_moat_save_CPP="$CPP"
   acx_moat_save_CPPFLAGS="$CPPFLAGS"
   acx_moat_save_LIBS="$LIBS"
   
   CPPFLAGS="$CPPFLAGS $MOAT_CPPFLAGS"
   LIBS="$MOAT $LIBS"
   acx_moat_serial=no
   AC_CHECK_HEADERS([moat.h])
   AC_MSG_CHECKING([for moat_mem_double_alloc in libmoat])
   AC_TRY_LINK_FUNC(moat_mem_double_alloc, [acx_moat_ok=yes;acx_moat_serial=yes;AC_DEFINE(HAVE_MOAT,1,[Define if you have the MOAT library.])], [])
   AC_MSG_RESULT($acx_moat_serial)

   CC="$MPICC"
   CPP="$MPICC -E"
   LIBS="$MOATMPI $acx_moat_save_LIBS"
   acx_moat_mpi=no
   AC_CHECK_HEADERS([moat_mpi.h])
   AC_MSG_CHECKING([for moatmpi_io_stat in libmoatmpi])
   AC_TRY_LINK_FUNC(moatmpi_io_stat, [acx_moat_ok=yes;acx_moat_mpi=yes;AC_DEFINE(HAVE_MOAT_MPI,1,[Define if you have the MPI MOAT library.])], [])
   AC_MSG_RESULT($acx_moat_mpi)

   CC="$acx_moat_save_CC"
   CPP="$acx_moat_save_CPP"
   LIBS="$acx_moat_save_LIBS"
   CPPFLAGS="$acx_moat_save_CPPFLAGS"

fi

# Define exported variables

AC_SUBST(MOAT_CPPFLAGS)
AC_SUBST(MOAT)
AC_SUBST(MOATMPI)
AC_SUBST(MOATOCL)

# Execute ACTION-IF-FOUND/ACTION-IF-NOT-FOUND:

if test x"$acx_moat_ok" = xyes; then
   ifelse([$1],,[echo "**** Enabling MOAT support."],[$1])
else
   ifelse([$2],,[echo "**** MOAT not found - disabling support."],[$2])
fi

])
