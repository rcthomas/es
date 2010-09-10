## $Id: Makefile.am,v 1.11 2006/06/30 01:07:26 tgkolda Exp $ 
## $Source: /space/CVS-Acro/acro/packages/appspack/appspack/examples/Makefile.am,v $ 
# @HEADER
# ************************************************************************
# 
#          APPSPACK: Asynchronous Parallel Pattern Search
#                 Copyright (2003) Sandia Corporation
# 
# Under terms of Contract DE-AC04-94AL85000, there is a non-exclusive
# license for use of this work by or on exbehalf of the U.S. Government.
# 
# This library is free software; you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as
# published by the Free Software Foundation; either version 2.1 of the
# License, or (at your option) any later version.
#  
# This library is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#                                                                                 
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
# USA.                                                                           .
# 
# Questions? Contact Tammy Kolda (tgkolda@sandia.gov) 
# 
# ************************************************************************
# @HEADER

CXX = @MPICXX@

AM_CPPFLAGS = -I$(top_srcdir)/src/cddlib -I$(top_srcdir)/src/libappspack
AM_LDFLAGS = -L$(top_builddir)/src/cddlib -L$(top_builddir)/src/libappspack
AM_LIBS = $(top_builddir)/src/libappspack/libappspack.la $(LAPACK_LIBS) $(BLAS_LIBS) -lm $(FLIBS)
EXTRA_CPPFLAGS =
AM_CXXFLAGS =

noinst_PROGRAMS = example1 example2 example2a example3 example4 example5

example1_SOURCES = example1.c
example1_LDADD = $(AM_LIBS)

example2_SOURCES = example2.cpp
example2_LDADD = $(AM_LIBS)

example2a_SOURCES = example2a.cpp
example2a_LDADD = $(AM_LIBS)

example3_SOURCES = example3.cpp
example3_LDADD = $(AM_LIBS)

example4_SOURCES = example4.cpp
example4_LDADD = $(AM_LIBS)

example5_SOURCES = example5.cpp
example5_LDADD = $(AM_LIBS)


## #######################################################################
## Extra files to be distributed
## #######################################################################

EXTRA_DIST = README \
	example1.apps example2.apps example2a.apps \
	example3.apps example4.apps \
	example5.apps \
	example1.out example2.out example2a.out\
	example3.out example4.out \
	example5.out	

## #######################################################################
## Files to be deleted by 'make maintainer-clean'
## #######################################################################

MAINTAINERCLEANFILES = Makefile.in 

