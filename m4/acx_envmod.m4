# ===========================================================================
#                http://autoconf-archive.cryp.to/acx_mpi.html
# ===========================================================================
#
# SYNOPSIS
#
#   MOD_PROG_ENVMOD([ACTION-IF-FOUND], [ACTION-IF-NOT-FOUND]])
#
# DESCRIPTION
#
#   This macro tries to find out if environment modules is installed
#
#   ACTION-IF-FOUND is a list of shell commands to run if an MPI library is
#   found, and ACTION-IF-NOT-FOUND is a list of commands to run if it is not
#   found. If ACTION-IF-FOUND is not specified, the default action will
#   define HAVE_MPI.
#
# LAST MODIFICATION
#
#   2017-10-23
#
# COPYLEFT
#
#   Copyright (c) 2017 Brian W. Mulligan <bwmulligan@astronaos.com>
#


AC_DEFUN([MOD_PROG_ENVMOD],
[

AC_PATH_TOOL([MODCMD], [modulecmd])
AS_IF( [test -n $MODCMD], [AC_SUBST([MODTYPE],["ENVMOD"])], 
	[AS_IF( [test -n $LMOD_CMD], [AC_SUBST([MODTYPE],["LMOD"])], [AC_SUBST([MODTYPE],[""])])])

AM_CONDITIONAL([HAVEMOD],[test -n ${MODTYPE}])
AM_CONDITIONAL([HAVEENVMOD],[test "x$MODTYPE" = "xENVMOD"])
AM_CONDITIONAL([HAVELMOD],[test "x$MODTYPE" = "xLMOD"])



])dnl PKG_PROG_PKG_CONFIG

dnl ENVMOD_INSTALLDIR([DIRECTORY])
dnl -------------------------
dnl Since: 0.27
dnl
dnl Substitutes the variable modfiledir as the location where a module
dnl should install modulefiles. If the user installs to the default installation
dnl directory (i.e. ${prefix} = NONE, or installed to /opt, the module directory
dnl will be set to ${MODULESHOME}/modulefiles. If the user installed to 
dnl any other directory, then the module directory will be set to 
dnl ${HOME}/privatemodules. The default can be changed by passing
dnl DIRECTORY. The user can override through the --with-pkgconfigdir
dnl parameter.




AC_DEFUN([MOD_INSTALLDIR],[
AC_REQUIRE([MOD_PROG_ENVMOD])dnl
AS_IF( [test "x$1" != "x"],
	[AC_SUBST([modfiledefaultdir],[$1])]
,
	[AS_IF( [test -n ${MODTYPE}],
		[AS_IF( [test "x$MODTYPE" = "xENVMOD"],
			[AS_IF( [ test `echo $prefix | grep -c /opt/` -eq 1],
				[AC_SUBST([modfiledefaultdir],[$MODULESHOME/modulefiles/${PACKAGE_NAME}])]
			,
				[AS_IF( [test "x${prefix}" = "xNONE"],
					[AC_SUBST([modfiledefaultdir],[$MODULESHOME/modulefiles/${PACKAGE_NAME}])]
				,
					[AS_IF( [test -e "${HOME}/privatemodules"],
						[AC_SUBST([modfiledefaultdir],[$HOME/privatemodules/${PACKAGE_NAME}])]
					,
						[AC_SUBST([modfiledefaultdir],[""])]
					)]
				)]
			)]
		,
			[AS_IF( [test -e "${HOME}/privatemodules"],
				[AC_SUBST([modfiledefaultdir],[$HOME/privatemodules/${PACKAGE_NAME}])]
			,
				[AC_SUBST([modfiledefaultdir],[""])]
			)]
		)]
	,
		[AC_SUBST([modfiledefaultdir],[""])]
	)]
)

AC_MSG_NOTICE([  prefix      : $prefix])
AC_MSG_NOTICE([  modules type      : $MODTYPE])
AC_MSG_NOTICE([  default module dir      : $modfiledefaultdir])


m4_pushdef([mod_default], [m4_default([$1], ['${modfiledefaultdir}'])])
m4_pushdef([mod_description],
    [modfile installation directory @<:@]mod_default[@:>@])
AC_ARG_WITH([modfiledir],
    [AS_HELP_STRING([--with-modfiledir], mod_description)],,
    [with_modfiledir=]mod_default)
AC_SUBST([modfiledir], [$with_modfiledir])
m4_popdef([mod_default])
m4_popdef([mod_description])

AM_CONDITIONAL([INSTALLMOD],[test "x$modfiledir" != "x"])

# Finally, execute ACTION-IF-FOUND/ACTION-IF-NOT-FOUND:
AS_IF([test "x$modfiledir" != "x"],[$2],[$3])
])dnl ENVMOD_INSTALLDIR
