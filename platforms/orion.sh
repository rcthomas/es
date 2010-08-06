#!/bin/sh

. /opt/Modules/3.2.7/init/sh

module load acml_mp

# ./configure --prefix=${HOME}/es-orion --with-blas="${ACML_LINK}"
  ./configure --prefix=${HOME}/es-orion --with-blas="${ACML_LINK}" --with-moatconfig=$HOME/local/bin/moatconfig
