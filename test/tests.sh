#!/bin/sh
#clear old results
rm results/*
rm log/*
# serial tests
OMP_NUM_THREADS=1
../src/syn++/syn++ yaml/single2001.yaml > results/single2001.dat
../src/syn++/syn++ yaml/double2001-2001.yaml > results/double2001-2001.dat
../src/syn++/syn++ yaml/double2001-1401.yaml > results/double2001-1401.dat
../src/syn++/syn++ yaml/continuum.yaml > results/continuum.dat
# parallel (OMP) tests. This is intentionally set high, and may be larger than the # of processors - this is basically fine; we want to make sure many threads are running
OMP_NUM_THREADS=8
../src/syn++/syn++ yaml/single2001.yaml > results/omp_single2001.dat
../src/syn++/syn++ yaml/double2001-2001.yaml > results/omp_double2001-2001.dat
../src/syn++/syn++ yaml/double2001-1401.yaml > results/omp_double2001-1401.dat
../src/syn++/syn++ yaml/continuum.yaml > results/omp_continuum.dat
# do output comparisons. The log files should be empty if everything works
diff -q data/single2001.dat results/single2001.dat > log/single2001.log
diff -q data/single2001.dat results/omp_single2001.dat > log/omp_single2001.log
diff -q data/double2001-2001.dat results/double2001-2001.dat > log/double2001-2001.log
diff -q data/double2001-2001.dat results/omp_double2001-2001.dat > log/omp_double2001-2001.log
diff -q data/double2001-1401.dat results/double2001-1401.dat > log/double2001-1401.log
diff -q data/double2001-1401.dat results/omp_double2001-1401.dat > log/omp_double2001-1401.log
diff -q data/continuum.dat results/continuum.dat > log/continuum.log
diff -q data/continuum.dat results/omp_continuum.dat > log/omp_continuum.log


