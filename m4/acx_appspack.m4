#
# SYNOPSIS
#
#   ACX_APPSPACK([ACTION-IF-FOUND[, ACTION-IF-NOT-FOUND]])
#
# DESCRIPTION
#
#   This macro looks for a version of the APPSPACK library.  The APPSPACK_CPPFLAGS
#   and APPSPACK output variables hold the compile and link flags.
#
#   To link an application with APPSPACK, you should link with:
#
#   	$APPSPACK
#
#   The user may use:
# 
#       --with-appspack-cpp=<flags> --with-appspack-libs=<flags> 
#
#   to manually specify the APPSPACK include and linking flags.
#
#   ACTION-IF-FOUND is a list of shell commands to run if an APPSPACK library is
#   found, and ACTION-IF-NOT-FOUND is a list of commands to run it if it is
#   not found. If ACTION-IF-FOUND is not specified, the default action will
#   define HAVE_APPSPACK and set output variables above.
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

AC_DEFUN([ACX_APPSPACK], [
AC_PREREQ(2.50)
AC_REQUIRE([ACX_MPI])
AC_REQUIRE([AX_CXX_CHECK_LIB])
AC_REQUIRE([ACX_LAPACK])

acx_appspack_ok=no
acx_appspack_cdd_ok=no
acx_appspack_default="-lappspack -lcdd"

APPSPACK_CPPFLAGS=""
APPSPACK=""

AC_ARG_WITH(appspack-cpp, [AC_HELP_STRING([--with-appspack-cpp=<flags>], [use APPSPACK preprocessing flags <flags>.  Set to "no" to disable.])])

AC_ARG_WITH(appspack-libs, [AC_HELP_STRING([--with-appspack-libs=<flags>], [use APPSPACK linking flags <flags>.  Set to "no" to disable.])])

if test x"$with_appspack_cpp" != x; then
   if test x"$with_appspack_cpp" != xno; then
      APPSPACK_CPPFLAGS="$with_appspack_cpp"
   else
      acx_appspack_ok=disable
   fi
fi

if test x"$with_appspack_libs" != x; then
   if test x"$with_appspack_libs" != xno; then
      APPSPACK="$with_appspack_libs"
   else
      acx_appspack_ok=disable
   fi
fi

if test $acx_appspack_ok = disable; then
   echo "**** APPSPACK explicitly disabled by configure."
else

   # Save environment

   acx_appspack_save_CXX="$CXX"
   acx_appspack_save_CPPFLAGS="$CPPFLAGS"
   acx_appspack_save_LIBS="$LIBS"

   # Test MPI compile and linking

   CXX="$MPICXX"
   CPPFLAGS="$CPPFLAGS $APPSPACK_CPPFLAGS"
   LIBS="$APPSPACK $LAPACK_LIBS $BLAS_LIBS $acx_appspack_save_LIBS $FLIBS -lm"

#   AC_CHECK_HEADERS([APPSPACK_Executor_MPI.hpp])

   AX_CXX_CHECK_LIB([cdd], [dd_InitializeArow], [acx_appspack_cdd_ok=yes])

   if test $acx_appspack_cdd_ok = yes; then
      AX_CXX_CHECK_LIB([appspack], [APPSPACK::Solver::getBestF () const], [acx_appspack_ok=yes;AC_DEFINE(HAVE_APPSPACK,1,[Define if you have the APPSPACK library.])])
   else
      APPSPACK="$acx_appspack_default"
      LIBS="$acx_appspack_default $LAPACK_LIBS $BLAS_LIBS $acx_appspack_save_LIBS $FLIBS -lm"
      AX_CXX_CHECK_LIB([cdd], [dd_InitializeArow], [acx_appspack_cdd_ok=yes])
      if test $acx_appspack_cdd_ok = no; then
      	 AC_MSG_WARN([Cannot link to Appspack libcdd library- did you build appspack with --enable-cddlib?])
      else
         AX_CXX_CHECK_LIB([appspack], [APPSPACK::Solver::getBestF () const], [acx_appspack_ok=yes;AC_DEFINE(HAVE_APPSPACK,1,[Define if you have the APPSPACK library.])])
      fi
    fi

   # Restore environment

   CXX="$acx_appspack_save_CXX"
   LIBS="$acx_appspack_save_LIBS"
   CPPFLAGS="$acx_appspack_save_CPPFLAGS"

fi

# Define exported variables

AC_SUBST(APPSPACK_CPPFLAGS)
AC_SUBST(APPSPACK)

# Execute ACTION-IF-FOUND/ACTION-IF-NOT-FOUND:
   
if test x"$acx_appspack_ok" = xyes; then
   ifelse([$1],,[echo "**** Enabling support for APPSPACK."],[$1])
else
   ifelse([$2],,[echo "**** APPSPACK not found - disabling support."],[$2])
fi

])

