#!/bin/sh

./configure --prefix=${HOME}/es-orion --with-cfitsio-cpp="-I/opt/local/include" --with-cfitsio-libs="-L/opt/local/lib -lcfitsio"
