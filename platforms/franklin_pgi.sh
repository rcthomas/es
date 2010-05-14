#!/bin/sh

. /opt/modules/default/init/sh
. /project/projectdirs/cmb/modules/franklin/cmbenv.sh

cmbenv pgi

module load cmb
module unload idl

LAPACK="-L${ACML_DIR}/pgi64/lib -lacml -lacml_mv"
CFITSIO_INC=`cmbconfig -I cfitsio`
CFITSIO_LINK=`cmbconfig -L cfitsio`

./configure CC=cc CXX=CC F77=ftn MPICXX=CC FLIBS="-pgf90libs -pgf77libs" --prefix=${HOME}/es-franklin_pgi --with-cfitsio-cpp="${CFITSIO_INC}" --with-cfitsio-libs="${CFITSIO_LINK}" --with-blas="${LAPACK}" --disable-shared

