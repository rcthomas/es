#!/usr/bin/env python

"""
Parse synapps scaling test output.
"""

import sys
import os

for run in sys.argv[ 1 : ] :
    total_evals  = None
    cached_evals = None
    for line in open( run + "/" + run + ".log" ) :
        if line.find( "Number of Evaluations" ) >= 0 :
            total_evals = int( line.rstrip().split()[ -1 ] )
            continue
        if line.find( "Number of Cached Evaluations" ) >= 0 :
            cached_evals = int( line.rstrip().split()[ -1 ] )
            continue
    try :
        start_time = os.path.getmtime( run + "/" + run + ".yaml" )
    except OSError :
        start_time = None
    try :
        stop_time = os.path.getmtime( run + "/" + run + ".log" )
    except OSError :
        stop_time = None
    if not total_evals :
        total_evals = 0
        cached_evals = 0
        for line in open( run + "/SNF20080514-002_M000075_restframe.cache" ) :
            total_evals += 1
    if not ( start_time and stop_time ) :
        print run, "missing data"
        continue
    total_evals += cached_evals
    duration = stop_time - start_time
    print run, total_evals / duration


