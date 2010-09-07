CXX = @MPICXX@

AM_CPPFLAGS = -I$(top_srcdir)/src/cddlib -I$(top_srcdir)/src/libappspack
AM_LDFLAGS = -L$(top_builddir)/src/cddlib -L$(top_builddir)/src/libappspack
AM_LIBS = $(top_builddir)/src/libappspack/libappspack.la $(top_builddir)/src/cddlib/libcdd.la $(LAPACK_LIBS) $(BLAS_LIBS) -lm $(FLIBS)
EXTRA_CPPFLAGS =
AM_CXXFLAGS =

bin_PROGRAMS = appspack_serial

appspack_serial_SOURCES = APPSPACK_Main_Serial.cpp
appspack_serial_LDADD = $(AM_LIBS)



