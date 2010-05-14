AM_CPPFLAGS = -I$(top_srcdir)/src/libes -I$(top_srcdir)/src/synpp -I$(top_srcdir)/external/yaml $(CFITSIO_CPPFLAGS)
AM_LDFLAGS = -L$(top_builddir)/src/libes -L$(top_builddir)/src/synpp -L$(top_builddir)/external/yaml
AM_LIBS = $(top_builddir)/external/yaml/libyaml-cpp.la $(CFITSIO) -lm
AM_CXXFLAGS =

EXTRA_DIST = syn++.yaml

if HAVE_AM_OPENMP
  AM_CXXFLAGS += $(OPENMP_CXXFLAGS)
  AM_LIBS += $(OPENMP_CXXFLAGS)
endif

bin_PROGRAMS = synpp

synpp_SOURCES  = syn++.cc
synpp_LDFLAGS  = $(AM_LDFLAGS)
synpp_LDADD    = $(top_builddir)/src/libes/libes.la $(AM_LIBS)

synppyamldir = $(datadir)/es
synppyaml_DATA = syn++.yaml
