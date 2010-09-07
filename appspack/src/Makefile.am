## $Id: Makefile.am,v 1.33 2006/06/30 21:35:10 tgkolda Exp $ 
## $Source: /space/CVS-Acro/acro/packages/appspack/appspack/src/Makefile.am,v $ 
# @HEADER
# ************************************************************************
# 
#          APPSPACK: Asynchronous Parallel Pattern Search
#                 Copyright (2003) Sandia Corporation
# 
# Under terms of Contract DE-AC04-94AL85000, there is a non-exclusive
# license for use of this work by or on behalf of the U.S. Government.
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

# ------------------------------------------------------------------------
# List of all programs
# ------------------------------------------------------------------------

if BUILD_SERIAL

APPSPACK_SERIAL = appspack_serial

endif

if BUILD_MPI

APPSPACK_MPI = appspack_mpi

endif

if BUILD_EXECUTABLES

bin_PROGRAMS = $(APPSPACK_SERIAL) $(APPSPACK_MPI)

endif

# ------------------------------------------------------------------------
# Executables
# ------------------------------------------------------------------------

if BUILD_CDDLIB

CDDINC=-I$(top_srcdir)/cddlib
CDDLIB=$(top_builddir)/cddlib/libcdd.a

endif

appspack_serial_SOURCES = $(srcdir)/APPSPACK_Main_Serial.cpp

appspack_serial_DEPENDENCIES = $(top_builddir)/src/libappspack.a $(CDDLIB)

appspack_serial_LDADD = $(appspack_serial_DEPENDENCIES)

appspack_mpi_SOURCES = $(srcdir)/APPSPACK_Main_MPI.cpp

appspack_mpi_DEPENDENCIES = $(top_builddir)/src/libappspack.a $(CDDLIB)

appspack_mpi_LDADD = $(appspack_mpi_DEPENDENCIES)

# ------------------------------------------------------------------------
# List of all libraries to install in $(libdir)
# ------------------------------------------------------------------------

lib_LIBRARIES = libappspack.a

# ------------------------------------------------------------------------
# Library AR Command
# ------------------------------------------------------------------------

if USE_ALTERNATE_AR

libappspack_a_AR = $(ALTERNATE_AR)

else

libappspack_a_AR = $(AR) cru

endif

# ------------------------------------------------------------------------
# General CPP flags
# ------------------------------------------------------------------------

AM_CPPFLAGS = $(CDDINC)

# ------------------------------------------------------------------------
# Library source
# ------------------------------------------------------------------------

libappspack_a_SOURCES = \
	$(srcdir)/APPSPACK_Cache_Manager.cpp \
	$(srcdir)/APPSPACK_Cache_Point.cpp \
	$(srcdir)/APPSPACK_Combiner_Generic.cpp \
	$(srcdir)/APPSPACK_Constraints_Linear.cpp \
	$(srcdir)/APPSPACK_Conveyor.cpp \
	$(srcdir)/APPSPACK_Counter.cpp \
	$(srcdir)/APPSPACK_Directions.cpp \
	$(srcdir)/APPSPACK_Evaluator_SystemCall.cpp \
	$(srcdir)/APPSPACK_Executor_Serial.cpp \
	$(srcdir)/APPSPACK_Executor_MPI.cpp \
	$(srcdir)/APPSPACK_GCI.cpp \
	$(srcdir)/APPSPACK_List.cpp \
	$(srcdir)/APPSPACK_Parameter_Entry.cpp \
	$(srcdir)/APPSPACK_Parameter_List.cpp \
	$(srcdir)/APPSPACK_Point.cpp \
	$(srcdir)/APPSPACK_Print.cpp \
	$(srcdir)/APPSPACK_Solver.cpp \
	$(srcdir)/APPSPACK_Utils.cpp \
	$(srcdir)/APPSPACK_Vector.cpp \
	$(srcdir)/APPSPACK_Matrix.cpp \
	$(srcdir)/APPSPACK_Float.cpp \
	$(srcdir)/APPSPACK_CDDLIB_Interface.c \
	$(srcdir)/APPSPACK_LAPACK_Wrappers.cpp


# ------------------------------------------------------------------------
# Header files to install
# ------------------------------------------------------------------------

nodist_pkginclude_HEADERS = \
	$(srcdir)/APPSPACK_Config.hpp 

pkginclude_HEADERS = \
	$(srcdir)/APPSPACK_Cache_Manager.hpp \
	$(srcdir)/APPSPACK_Cache_Point.hpp \
	$(srcdir)/APPSPACK_Cache_SplayTree.hpp \
	$(srcdir)/APPSPACK_Combiner_Generic.hpp \
	$(srcdir)/APPSPACK_Common.hpp \
	$(srcdir)/APPSPACK_Constraints_Linear.hpp \
	$(srcdir)/APPSPACK_Conveyor.hpp \
	$(srcdir)/APPSPACK_Counter.hpp \
	$(srcdir)/APPSPACK_Directions.hpp \
	$(srcdir)/APPSPACK_Evaluator_Interface.hpp \
	$(srcdir)/APPSPACK_Evaluator_SystemCall.hpp \
	$(srcdir)/APPSPACK_Executor_Interface.hpp \
	$(srcdir)/APPSPACK_Executor_Serial.hpp \
	$(srcdir)/APPSPACK_Executor_MPI.hpp \
	$(srcdir)/APPSPACK_GCI.hpp \
	$(srcdir)/APPSPACK_List.hpp \
	$(srcdir)/APPSPACK_Parameter_Entry.hpp \
	$(srcdir)/APPSPACK_Parameter_List.hpp \
	$(srcdir)/APPSPACK_Point.hpp \
	$(srcdir)/APPSPACK_Print.hpp \
	$(srcdir)/APPSPACK_Solver.hpp \
	$(srcdir)/APPSPACK_Utils.hpp \
	$(srcdir)/APPSPACK_Vector.hpp \
	$(srcdir)/APPSPACK_Matrix.hpp \
	$(srcdir)/APPSPACK_Float.hpp \
	$(srcdir)/APPSPACK_CDDLIB_Interface.h \
	$(srcdir)/APPSPACK_LAPACK_Wrappers.hpp \
	$(srcdir)/APPSPACK_Common.h

# ------------------------------------------------------------------------
# Extra files to distribute
# ------------------------------------------------------------------------

EXTRA_DIST = 

# ------------------------------------------------------------------------
# Files to be deleted by 'make maintainer-clean'
# ------------------------------------------------------------------------

MAINTAINERCLEANFILES = Makefile.in 

## #######################################################################
## Following are commands that only apply when APPSPACK is a
## subpackage of ACRO.
## #######################################################################
if I_AM_SUBPACKAGE
  TOPINC=../../../include
  TOPLIB=../../../lib
  TOPBIN=${top_srcdir}/../../bin

all-local:
	@if test -d $(TOPLIB) ; then : ; else mkdir $(TOPLIB) ; fi
	@$(TOPBIN)/cp_u libappspack.a $(TOPLIB)

clean-local:
	@rm -f $(TOPLIB)/libappspack.a
	@$(MAKE) clean-headers

copy-headers:
	@if test -d $(TOPINC) ; then : ; else mkdir $(TOPINC) ; fi
	@if test -d $(TOPINC)/appspack ; then : ; else mkdir $(TOPINC)/appspack ; fi
	@$(TOPBIN)/cp_u APPSPACK_Config.hpp $(pkginclude_HEADERS) $(TOPINC)/appspack

clean-headers:
	@rm -rf $(TOPINC)/appspack

endif
