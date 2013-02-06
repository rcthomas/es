AM_CPPFLAGS = -I$(top_srcdir)/src/libes -I$(top_srcdir)/src/syn++ -I$(top_srcdir)/external/yaml $(CFITSIO_CPPFLAGS) $(gtkmm_CFLAGS) $(python_CFLAGS)
AM_LDFLAGS = -L$(top_builddir)/src/libes -L$(top_builddir)/src/syn++ -L$(top_builddir)/external/yaml $(gtkmm_LIBS) $(python_LIBS)
AM_LIBS = $(top_builddir)/external/yaml/libyaml-cpp.la $(CFITSIO) -lm
AM_CXXFLAGS =

EXTRA_DIST = syn++.yaml

if HAVE_AM_OPENMP
  AM_CXXFLAGS += $(OPENMP_CXXFLAGS)
  AM_LIBS += $(OPENMP_CXXFLAGS)
endif

bin_PROGRAMS = syn++

syn___SOURCES  = syn++.cc
syn___LDFLAGS  = $(AM_LDFLAGS)
syn___LDADD    = $(top_builddir)/src/libes/libes.la $(AM_LIBS)

syn__yamldir = $(datadir)/es
syn__yaml_DATA = syn++.yaml
