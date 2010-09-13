#!/bin/sh

./configure CXX=g++-mp-4.4 CC=gcc-mp-4.4 F77=gfortran-mp-4.4 --prefix=$HOME/local --with-cfitsio-cpp="-I/opt/local/include" --with-cfitsio-libs="-L/opt/local/lib -lcfitsio" --with-appspack-cpp="-I$HOME/local/include" --with-appspack-libs="-L$HOME/local/lib -lappspack"
