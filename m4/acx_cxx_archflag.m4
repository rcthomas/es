dnl @synopsis ACX_ARCHFLAG([PORTABLE?], [ACTION-SUCCESS], [ACTION-FAILURE])
dnl
dnl @summary find target architecture name for platform specific compiler optimization
dnl
dnl This macro tries to guess the "native" arch corresponding to the
dnl target architecture for use with the compilers optimization
dnl flags. If found, the cache variable $acx_cv_archflag is set to
dnl this flag and ACTION-SUCCESS is executed; otherwise
dnl $acx_cv_archflag is is set to "unknown" and ACTION-FAILURE is
dnl executed. The default ACTION-SUCCESS is to add $acx_cv_archflag
dnl to the end of $CFLAGS.
dnl
dnl PORTABLE? should be either [yes] (default) or [no]. In the former
dnl case, the flag is set to -mtune (or equivalent) so that the
dnl architecture is only used for tuning, but the instruction set used
dnl is still portable. In the latter case, the flag is set to -march
dnl (or equivalent) so that architecture-specific instructions are
dnl enabled.
dnl
dnl The user can specify --with-arch=<arch> in order to override
dnl the macro's choice of architecture, or --without-arch to
dnl disable this.
dnl
dnl When cross-compiling, or if $CXX is not recognized, then ACTION-FAILURE is
dnl called unless the user specified --with-arch manually.
dnl
dnl Requires macros: AX_CHECK_COMPILER_FLAGS, ACX_X86_CPUID
dnl
dnl (The main emphasis here is on recent CPUs, on the principle that
dnl doing high-performance computing on old hardware is uncommon.)
dnl
dnl @category Misc
dnl @author Steven G. Johnson <stevenj@alum.mit.edu> and Matteo Frigo.
dnl @version 2006-01-04
dnl @license GPLWithACException
dnl
dnl Modified by Theodore Kisner <tskisner@lbl.gov>
dnl 2008-12-10
dnl Support as many compilers as possible (not just GCC).
dnl

AC_DEFUN([ACX_ARCHFLAG],
[AC_REQUIRE([AC_PROG_CXX])
AC_REQUIRE([AC_CANONICAL_HOST])

AC_ARG_WITH(arch, [AC_HELP_STRING([--with-arch=<arch>], [use architecture-specific optimizations <arch>, instead of guessing])], acx_arch=$withval, acx_arch=yes)

AC_MSG_CHECKING([for compiler optimization flags])
AC_MSG_RESULT([])
AC_CACHE_VAL(acx_cv_archflag,
[
acx_cv_archflag="unknown"

if test "x$acx_arch" = xyes; then
acx_arch=""
if test "$cross_compiling" = no; then
case $host_cpu in
  i[[3456]]86*|x86_64*) # use cpuid codes, in part from x86info-1.7 by D. Jones
     ACX_X86_CPUID(0)
     ACX_X86_CPUID(1)
     case $acx_cv_x86_cpuid_0 in
       *:756e6547:*:*) # Intel
          case $acx_cv_x86_cpuid_1 in
	    *5[[48]]?:*:*:*) acx_arch="pentium-mmx pentium" ;;
	    *5??:*:*:*) acx_arch=pentium ;;
	    *6[[3456]]?:*:*:*) acx_arch="pentium2 pentiumpro" ;;
	    *6a?:*[[01]]:*:*) acx_arch="pentium2 pentiumpro" ;;
	    *6a?:*[[234]]:*:*) acx_arch="pentium3 pentiumpro" ;;
	    *6[[9d]]?:*:*:*) acx_arch="pentium-m pentium3 pentiumpro" ;;
	    *6[[78b]]?:*:*:*) acx_arch="pentium3 pentiumpro" ;;
	    *6??:*:*:*) acx_arch=pentiumpro ;;
            *f3[[347]]:*:*:*|*f4[1347]:*:*:*)
		case $host_cpu in
                  x86_64*) acx_arch="nocona pentium4 pentiumpro" ;;
                  *) acx_arch="prescott pentium4 pentiumpro" ;;
                esac ;;
            *f??:*:*:*) acx_arch="pentium4 pentiumpro";;
          esac ;;
       *:68747541:*:*) # AMD
          case $acx_cv_x86_cpuid_1 in
	    *5[[67]]?:*:*:*) acx_arch=k6 ;;
	    *5[[8d]]?:*:*:*) acx_arch="k6-2 k6" ;;
	    *5[[9]]?:*:*:*) acx_arch="k6-3 k6" ;;
	    *60?:*:*:*) acx_arch=k7 ;;
	    *6[[12]]?:*:*:*) acx_arch="athlon k7" ;;
	    *6[[34]]?:*:*:*) acx_arch="athlon-tbird k7" ;;
	    *67?:*:*:*) acx_arch="athlon-4 athlon k7" ;;
	    *6[[68a]]?:*:*:*)
	       ACX_X86_CPUID(0x80000006) # L2 cache size
	       case $acx_cv_x86_cpuid_0x80000006 in
                 *:*:*[[1-9a-f]]??????:*) # (L2 = ecx >> 16) >= 256
			acx_arch="athlon-xp athlon-4 athlon k7" ;;
                 *) acx_arch="athlon-4 athlon k7" ;;
	       esac ;;
	    *f[[4cef8b]]?:*:*:*) acx_arch="athlon64 k8" ;;
	    *f5?:*:*:*) acx_arch="opteron k8" ;;
	    *f7?:*:*:*) acx_arch="athlon-fx opteron k8" ;;
	    *f??:*:*:*) acx_arch="k8" ;;
          esac ;;
	*:746e6543:*:*) # IDT
	   case $acx_cv_x86_cpuid_1 in
	     *54?:*:*:*) acx_arch=winchip-c6 ;;
	     *58?:*:*:*) acx_arch=winchip2 ;;
	     *6[[78]]?:*:*:*) acx_arch=c3 ;;
	     *69?:*:*:*) acx_arch="c3-2 c3" ;;
	   esac ;;
     esac
     if test x"$acx_arch" = x; then # fallback
	case $host_cpu in
	  i586*) acx_arch=pentium ;;
	  i686*) acx_arch=pentiumpro ;;
        esac
     fi
     ;;

  sparc*)
     AC_PATH_PROG([PRTDIAG], [prtdiag], [prtdiag], [$PATH:/usr/platform/`uname -i`/sbin/:/usr/platform/`uname -m`/sbin/])
     cputype=`(((grep cpu /proc/cpuinfo | cut -d: -f2) ; ($PRTDIAG -v |grep -i sparc) ; grep -i cpu /var/run/dmesg.boot ) | head -n 1) 2> /dev/null`
     cputype=`echo "$cputype" | tr -d ' -' |tr $as_cr_LETTERS $as_cr_letters`
     case $cputype in
         *ultrasparciv*) acx_arch="ultrasparc4 ultrasparc3 ultrasparc v9" ;;
         *ultrasparciii*) acx_arch="ultrasparc3 ultrasparc v9" ;;
         *ultrasparc*) acx_arch="ultrasparc v9" ;;
         *supersparc*|*tms390z5[[05]]*) acx_arch="supersparc v8" ;;
         *hypersparc*|*rt62[[056]]*) acx_arch="hypersparc v8" ;;
         *cypress*) acx_arch=cypress ;;
     esac ;;

  alphaev5) acx_arch=ev5 ;;
  alphaev56) acx_arch=ev56 ;;
  alphapca56) acx_arch="pca56 ev56" ;;
  alphapca57) acx_arch="pca57 pca56 ev56" ;;
  alphaev6) acx_arch=ev6 ;;
  alphaev67) acx_arch=ev67 ;;
  alphaev68) acx_arch="ev68 ev67" ;;
  alphaev69) acx_arch="ev69 ev68 ev67" ;;
  alphaev7) acx_arch="ev7 ev69 ev68 ev67" ;;
  alphaev79) acx_arch="ev79 ev7 ev69 ev68 ev67" ;;

  powerpc*)
     cputype=`((grep cpu /proc/cpuinfo | head -n 1 | cut -d: -f2 | cut -d, -f1 | sed 's/ //g') ; /usr/bin/machine ; /bin/machine; grep CPU /var/run/dmesg.boot | head -n 1 | cut -d" " -f2) 2> /dev/null`
     cputype=`echo $cputype | sed -e 's/ppc//g;s/ *//g'`
     case $cputype in
       *750*) acx_arch="750 G3" ;;
       *740[[0-9]]*) acx_arch="$cputype 7400 G4" ;;
       *74[[4-5]][[0-9]]*) acx_arch="$cputype 7450 G4" ;;
       *74[[0-9]][[0-9]]*) acx_arch="$cputype G4" ;;
       *970*) acx_arch="970 G5 power4";;
       *POWER4*|*power4*|*gq*) acx_arch="power4 970";;
       *POWER5*|*power5*|*gr*|*gs*) acx_arch="power5 power4 970";;
       603ev|8240) acx_arch="$cputype 603e 603";;
       *) acx_arch=$cputype ;;
     esac
     acx_arch="$acx_arch powerpc"
     ;;
esac
fi # not cross-compiling
fi # guess arch

if test "x$acx_arch" != x -a "x$acx_arch" != xno; then
for arch in $acx_arch; do
  if test "x[]m4_default([$1],yes)" = xyes; then # if we require portable code
    flags="-mtune=$arch"
    # -mcpu=$arch and m$arch generate nonportable code on every arch except
    # x86.  And some other arches (e.g. Alpha) don't accept -mtune.  Grrr.
    case $host_cpu in i*86|x86_64*) flags="$flags -mcpu=$arch -m$arch";; esac
  else
    flags="-march=$arch -mcpu=$arch -m$arch"
  fi
  for flag in $flags; do
    AX_CHECK_COMPILER_FLAGS($flag, [acx_cv_archflag=$flag; break])
  done
  test "x$acx_cv_archflag" = xunknown || break
done
fi

])
AC_MSG_CHECKING([for compiler optimization flags])
AC_MSG_RESULT($acx_cv_archflag)
if test "x$acx_cv_archflag" = xunknown; then
  m4_default([$3],:)
else
  m4_default([$2], [CXXFLAGS="$CXXFLAGS $acx_cv_archflag"])
fi
])
