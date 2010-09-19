#!/bin/sh

. /usr/common/nsg/opt/Modules/default/init/sh
. /project/projectdirs/cmb/modules/carver/cmbenv.sh

cmbenv gnu

module load cmb
module unload idl

LAPACK="-L/usr/common/usg/mkl/10.2.2.025/lib/em64t -lmkl_solver_lp64 -lmkl_lapack95_lp64 -lmkl_blas95_lp64 -lmkl_intel_lp64 -lmkl_gnu_thread -lmkl_core -fopenmp -lpthread"
CFITSIO_INC=`cmbconfig -I cfitsio`
CFITSIO_LINK=`cmbconfig -L cfitsio`

LOCAL=/project/projectdirs/snfactry/rthomas/local/carver

CC=gcc CXX=g++ F77=gfortran CXXFLAGS="-ffast-math -O3" CFLAGS="-ffast-math -O3" ./configure --prefix=${LOCAL} --with-cfitsio-cpp="${CFITSIO_INC}" --with-cfitsio-libs="${CFITSIO_LINK}" --with-blas="${LAPACK}" --with-appspack-libs="-L${LOCAL}/lib -lappspack" --with-appspack-cpp="-I${LOCAL}/include"
