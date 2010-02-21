#!/bin/sh

date
aprun -n 1 -N 1 -d 4 ./syn_s++ $1 > s_out.dat

date
aprun -n 1 -N 1 -d 4 ./syn_v++ $1 > v_out.dat

date

#xmgrace -geometry 1152x900 s_out.dat v_out.dat
