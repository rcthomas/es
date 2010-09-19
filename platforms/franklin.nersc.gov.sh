#!/bin/sh

. /opt/modules/default/init/sh
. /project/projectdirs/cmb/modules/franklin/cmbenv.sh

cmbenv pgi

module load cmb
module unload idl

LAPACK="-L${ACML_DIR}/pgi64/lib -lacml -lacml_mv"
CFITSIO_INC=`cmbconfig -I cfitsio`
CFITSIO_LINK=`cmbconfig -L cfitsio`

LOCAL=/project/projectdirs/snfactry/rthomas/local/franklin

CC=cc CXX=CC F77=ftn MPICXX=CC FLIBS=-pgf77libs CXXFLAGS="-O3 -fastsse" CFLAGS="-O3 -fastsse" ./configure --prefix=${LOCAL} --with-cfitsio-cpp="${CFITSIO_INC}" --with-cfitsio-libs="${CFITSIO_LINK}" --with-blas="${LAPACK}" --disable-shared --with-appspack-cpp="-I${LOCAL}/include" --with-appspack-libs="-L${LOCAL}/lib -lappspack"
