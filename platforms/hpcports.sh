#!/bin/sh

# you should call this script with extra options.  At the
# very least, you should specify the install prefix.
#
# For example:
#
# $> ./platforms/hpcports.sh --prefix=/my/install/dir

OPTS=$@

# This is for linking with HPCPorts dependencies

sharedopt="--enable-shared"
if [ "x${HPCP_DYNAMIC}" = "xFALSE" ]; then
    sharedopt="--disable-shared --disable-python"
fi

lapackopt="yes";
if [ "x${lapack_LIBS_CXX}" != "x" ]; then
    lapackopt="${lapack_LIBS_CXX}"
fi

CC="${HPCP_CC}" \
CXX="${HPCP_CXX}" \
FC="${HPCP_FC}" \
F77="${HPCP_F77}" \
CFLAGS="${HPCP_CFLAGS}" \
CXXFLAGS="${HPCP_CXXFLAGS}" \
FCFLAGS="${HPCP_FCFLAGS}" \
FFLAGS="${HPCP_FFLAGS}" \
MPICXX="${HPCP_MPICXX}" \
OPENMP_CXXFLAGS="${HPCP_OMPFLAGS}" \
LDFLAGS="${HPCP_LDFLAGS}" \
LIBS="${HPCP_LIBS}" \
FLIBS="${HPCP_FLIBS}" \
FCLIBS="${HPCP_FCLIBS}" \
MPIFCLIBS="${HPCP_MPIFCLIBS}" \
./configure ${sharedopt} ${OPTS} \
--with-blas="${blas_LIBS_CXX}" \
--with-lapack="${lapackopt}" \
--with-cfitsio_libs="${cfitsio_LIBS_CXX}" \
--with-appspack_libs="${appspack_LIBS_CXX}"
