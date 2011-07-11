#!/bin/sh
#PBS -N 1x090
#PBS -q regular
#PBS -l mppwidth=90
#PBS -l mppnppn=4
#PBS -l mppdepth=1
#PBS -l walltime=02:30:00
#PBS -A m106
#PBS -j eo

prefix=1x090

cd $PBS_O_WORKDIR

rm -rf $prefix.cache

log=$prefix.log
rm -f $log
touch $log

yaml=$prefix.yaml
rm -f $yaml

cat << EOF > $yaml
---
grid : 
    bin_width   : 0.30
    v_size      : 100
    v_outer_max : 30.00
opacity : 
    line_dir    : /project/projectdirs/snfactry/rthomas/local/share/es/lines
    ref_file    : /project/projectdirs/snfactry/rthomas/local/share/es/refs.dat
    form        : exp
    v_ref       : 10.00
    log_tau_min : -2.00
source : 
    mu_size     : 10
spectrum : 
    p_size      : 60
    flatten     : No
evaluator : 
    target_file : ../../data/SNF20080514-002_M000075_restframe.dat
    vector_norm :  2.00
    regions     : 
        apply   : [   Yes,   Yes,   Yes ]
        weight  : [  0.00,  1.00,  1.00 ]
        lower   : [     0,  3200,  7400 ]
        upper   : [ 10000,  6400,  8800 ]
config : 
    fit_file    : SNF20080514-002_M000075_restframe.fit
    cache_file  : SNF20080514-002_M000075_restframe.cache

    a0          : { fixed:  No, start:   1.00, lower:   0.00, upper:  10.00, scale:   1.00 }
    a1          : { fixed:  No, start:  -1.04, lower: -10.00, upper:  10.00, scale:   1.00 }
    a2          : { fixed:  No, start:   1.07, lower: -10.00, upper:  10.00, scale:   1.00 }
    v_phot      : { fixed:  No, start:  11.50, lower:   5.00, upper:  15.00, scale:   1.00 }
    v_outer     : { fixed: Yes, start:  30.00, lower:  15.00, upper:  30.00, scale:   1.00 }
    t_phot      : { fixed:  No, start:   8.90, lower:   5.00, upper:  10.00, scale:   1.00 }

    ions        : [   601,   800,  1100,  1201,  1401,  1402,  1601,  2001,  2601,  2602,  2701,  2801 ]
    active      : [   Yes,   Yes,   Yes,   Yes,   Yes,   Yes,   Yes,   Yes,   Yes,   Yes,   Yes,   Yes ]
    detach      : [    No,    No,    No,    No,    No,    No,    No,    No,    No,    No,    No,    No ]
                           
    log_tau     :          
        fixed   : [    No,    No,    No,    No,    No,    No,    No,    No,    No,    No,    No,    No ]
        start   : [  0.73,  0.94, -0.57,  0.45,  2.38,  0.98,  2.00,  4.00, -2.96,  1.88,  0.34,  0.06 ]
        lower   : [ -3.00, -3.00, -3.00, -3.00, -3.00, -3.00, -3.00, -3.00, -3.00, -3.00, -3.00, -3.00 ]
        upper   : [  4.00,  4.00,  4.00,  4.00,  4.00,  4.00,  4.00,  4.00,  4.00,  4.00,  4.00,  4.00 ]
        scale   : [  1.00,  1.00,  1.00,  1.00,  1.00,  1.00,  1.00,  1.00,  1.00,  1.00,  1.00,  1.00 ]
    v_min       :          
        fixed   : [    No,    No,    No,    No,    No,    No,    No,    No,    No,    No,    No,    No ]
        start   : [ 11.50, 11.50, 11.50, 11.50, 11.50, 11.50, 11.50, 11.50, 11.50, 11.50, 11.50, 11.50 ]
        lower   : [  5.00,  5.00,  5.00,  5.00,  5.00,  5.00,  5.00,  5.00,  5.00,  5.00,  5.00,  5.00 ]
        upper   : [ 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00 ]
        scale   : [  1.00,  1.00,  1.00,  1.00,  1.00,  1.00,  1.00,  1.00,  1.00,  1.00,  1.00,  1.00 ]
    v_max       :          
        fixed   : [   Yes,   Yes,   Yes,   Yes,   Yes,   Yes,   Yes,   Yes,   Yes,   Yes,   Yes,   Yes ]
        start   : [ 30.00, 30.00, 30.00, 30.00, 30.00, 30.00, 30.00, 30.00, 30.00, 30.00, 30.00, 30.00 ]
        lower   : [ 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00, 15.00 ]
        upper   : [ 30.00, 30.00, 30.00, 30.00, 30.00, 30.00, 30.00, 30.00, 30.00, 30.00, 30.00, 30.00 ]
        scale   : [  1.00,  1.00,  1.00,  1.00,  1.00,  1.00,  1.00,  1.00,  1.00,  1.00,  1.00,  1.00 ]
    aux         :          
        fixed   : [    No,    No,    No,    No,    No,    No,    No,    No,    No,    No,    No,    No ]
        start   : [  0.37,  1.91,  6.86,  1.51,  0.91,  0.80,  0.55,  0.63,  0.34,  0.53,  1.08,  0.76 ]
        lower   : [  0.10,  0.10,  0.10,  0.10,  0.10,  0.10,  0.10,  0.10,  0.10,  0.10,  0.10,  0.10 ]
        upper   : [ 10.00, 10.00, 10.00, 10.00, 10.00, 10.00, 10.00, 10.00, 10.00, 10.00, 10.00, 10.00 ]
        scale   : [  1.00,  1.00,  1.00,  1.00,  1.00,  1.00,  1.00,  1.00,  1.00,  1.00,  1.00,  1.00 ]
    temp        :          
        fixed   : [    No,    No,    No,    No,    No,    No,    No,    No,    No,    No,    No,    No ]
        start   : [ 12.50, 12.90, 10.00, 22.40,  7.10,  5.00,  5.00, 11.30, 10.00, 13.20, 10.20, 13.70 ]
        lower   : [  5.00,  5.00,  5.00,  5.00,  5.00,  5.00,  5.00,  5.00,  5.00,  5.00,  5.00,  5.00 ]
        upper   : [ 25.00, 25.00, 25.00, 25.00, 25.00, 25.00, 25.00, 25.00, 25.00, 25.00, 25.00, 25.00 ]
        scale   : [  1.00,  1.00,  1.00,  1.00,  1.00,  1.00,  1.00,  1.00,  1.00,  1.00,  1.00,  1.00 ]
EOF

export OMP_NUM_THREADS=1
aprun -n 90 -N 4 -d 1 synapps $yaml | tee -a $log
