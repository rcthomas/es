## $Id: Makefile.am,v 1.5.2.1 2006/06/30 22:04:13 tgkolda Exp $ 
## $Source: /space/CVS-Acro/acro/packages/appspack/appspack/doc/Makefile.am,v $ 
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

## #######################################################################
## Additional files to be included in distribution for 'make dist'
## #######################################################################
EXTRA_DIST = Doxyfile \
	$(srcdir)/images/Evaluator.gif \
	$(srcdir)/images/MPI_Version4.gif \
	$(srcdir)/images/Serial_Version4.gif \
	$(srcdir)/images/appspack.gif \
	$(srcdir)/images/doxygen.gif \
	$(srcdir)/images/gold-rule-500x11.gif \
	$(srcdir)/images/gold-rule-557x11.gif \
	$(srcdir)/images/red-vertical-bar-background.gif \
	$(srcdir)/images/snl-globe-left.gif \
	$(srcdir)/images/snl_globe_right03_mod.gif \
	$(srcdir)/images/spacer-16x1.gif \
	$(srcdir)/images/spacer-6x1.gif \
	$(srcdir)/includes/footer.html \
	$(srcdir)/includes/header.html \
	$(srcdir)/includes/stylesheet.css \
	$(srcdir)/input/bugs.dox \
	$(srcdir)/input/cddlib.dox \
	$(srcdir)/input/configure.dox \
	$(srcdir)/input/configureoptions.dox \
	$(srcdir)/input/customize.dox \
	$(srcdir)/input/developers.dox \
	$(srcdir)/input/downloads.dox \
	$(srcdir)/input/doxygen.dox \
	$(srcdir)/input/executables.dox \
	$(srcdir)/input/faq.dox \
	$(srcdir)/input/license.dox \
	$(srcdir)/input/linearparams.dox \
	$(srcdir)/input/mailinglists.dox \
	$(srcdir)/input/mainpage.dox \
	$(srcdir)/input/params.dox \
	$(srcdir)/input/primarycontact.dox \
	$(srcdir)/input/pubs.dox \
	$(srcdir)/input/users.dox

# ------------------------------------------------------------------------
# Files to be deleted by 'make maintainer-clean'
# ------------------------------------------------------------------------

MAINTAINERCLEANFILES = Makefile.in 

docs:
	@(doxygen Doxyfile > doxygen.out) 2>&1
