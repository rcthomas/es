#!/bin/sh

. /usr/common/nsg/opt/Modules/default/init/sh
. /project/projectdirs/cmb/modules/carver/cmbenv.sh

cmbenv pgi

module load cmb
module unload idl
module swap pgi pgi

LAPACK="-L/usr/common/usg/mkl/10.2.2.025/lib/em64t -lmkl_solver_lp64 -lmkl_lapack95_lp64 -lmkl_blas95_lp64 -lmkl_intel_lp64 -lmkl_pgi_thread -lmkl_core -mp -lpthread"
CFITSIO_INC=`cmbconfig -I cfitsio`
CFITSIO_LINK=`cmbconfig -L cfitsio`

./configure CC=pgcc CXX=pgCC F77=pgf77 FLIBS=-pgf77libs --prefix=${HOME}/es-carver_pgi --with-cfitsio-cpp="${CFITSIO_INC}" --with-cfitsio-libs="${CFITSIO_LINK}" --with-blas="${LAPACK}"
