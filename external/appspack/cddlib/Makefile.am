AM_CPPFLAGS = -I$(top_srcdir)/external/appspack/cddlib
AM_LDFLAGS = -L$(top_builddir)/external/appspack/cddlib
AM_LIBS = -lm
EXTRA_CPPFLAGS =
AM_CFLAGS =

noinst_HEADERS = \
cdd.h \
cddmp.h \
cddtypes.h \
setoper.h

noinst_LTLIBRARIES = libcdd.la
libcdd_la_SOURCES = \
cddcore.c \
cddio.c \
cddlib.c \
cddlp.c \
cddmp.c \
cddproj.c \
setoper.c
libcdd_la_LIBADD = $(AM_LIBS)

