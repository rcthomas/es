#!/bin/sh

. /opt/modules/default/init/sh
. /project/projectdirs/cmb/modules/franklin/cmbenv.sh

cmbenv gnu

module load cmb
module unload idl

LAPACK="-L${ACML_DIR}/gfortran64/lib -lacml -lacml_mv"
CFITSIO_INC=`cmbconfig -I cfitsio`
CFITSIO_LINK=`cmbconfig -L cfitsio`

./configure CC=cc CXX=CC F77=ftn MPICXX=CC --prefix=${HOME}/es-franklin_gnu --with-cfitsio-cpp="${CFITSIO_INC}" --with-cfitsio-libs="${CFITSIO_LINK}" --with-blas="${LAPACK}" --disable-shared

