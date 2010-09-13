#!/bin/sh

. /opt/modules/default/init/sh
. /project/projectdirs/cmb/modules/franklin/cmbenv.sh

cmbenv pgi

module load cmb
module unload idl

LAPACK="-L${ACML_DIR}/pgi64/lib -lacml -lacml_mv"
CFITSIO_INC=`cmbconfig -I cfitsio`
CFITSIO_LINK=`cmbconfig -L cfitsio`

./configure CC=cc CXX=CC F77=ftn MPICXX=CC FLIBS=-pgf77libs --prefix=${HOME}/local/franklin --with-cfitsio-cpp="${CFITSIO_INC}" --with-cfitsio-libs="${CFITSIO_LINK}" --with-blas="${LAPACK}" --disable-shared --with-appspack-cpp="-I${HOME}/local/franklin/include" --with-appspack-libs="-L${HOME}/local/franklin/lib -lappspack"



