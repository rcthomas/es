#!/bin/sh

#-
#- Orion is an Ubuntu Linux box with modules for handling ACML.
#- 
#- rthomas@orion $ echo $ACML_INC
#- -I/opt/acml-4.4.0/gfortran64_mp/include
#- rthomas@orion $ echo $ACML_LINK
#- -L/opt/acml-4.4.0/gfortran64_mp/lib -lacml_mv -lacml_mp
#-

. /opt/Modules/3.2.7/init/sh

module load acml_mp

./configure --prefix=${HOME}/local --with-blas="${ACML_LINK}" --with-appspack-cpp="-I${HOME}/local/include" --with-appspack-libs="-L${HOME}/local/lib -lappspack"
