AM_CPPFLAGS = -I$(top_srcdir)/src/libes -I$(top_srcdir)/external/yaml $(CFITSIO_CPPFLAGS)
AM_LDFLAGS = -L$(top_builddir)/src/libes -L$(top_builddir)/external/yaml
AM_LIBS = $(top_builddir)/external/yaml/libyaml-cpp.la $(CFITSIO) -lm
AM_CXXFLAGS =

if HAVE_AM_OPENMP
  AM_CXXFLAGS += $(OPENMP_CXXFLAGS)
  AM_LIBS += $(OPENMP_CXXFLAGS)
endif

noinst_HEADERS =        \
ES_Accelerator.hh       \
ES_Blackbody.hh         \
ES_Exception.hh         \
ES_Generic_Grid.hh      \
ES_Generic_Operator.hh  \
ES_Line.hh              \
ES_LineManager.hh       \
ES_Spectrum.hh          \
ES_Synow.hh             \
ES_Synow_Grid.hh        \
ES_Synow_Opacity.hh     \
ES_Synow_Operator.hh    \
ES_Synow_Setup.hh       \
ES_Synow_Source.hh      \
ES_Synow_Spectrum.hh

noinst_LTLIBRARIES = libes.la
libes_la_SOURCES =      \
ES_Accelerator.cc       \
ES_Blackbody.cc         \
ES_LineManager.cc       \
ES_Spectrum.cc          \
ES_Synow_Grid.cc        \
ES_Synow_Opacity.cc     \
ES_Synow_Setup.cc       \
ES_Synow_Source.cc      \
ES_Synow_Spectrum.cc
libes_la_LIBADD = $(AM_LIBS)

if HAVE_AM_MOAT
  SUFFIXES = .cl .ocl

  MOATCC = $(MOATBUILD)
  MOATCCFLAGS =

  .cl.ocl:
	$(MOATCC) $(MOATCCFLAGS) $(AM_MOATCCFLAGS) $<

  noinst_HEADERS += ES_Synow_MOATSource.hh
  libes_la_SOURCES += ES_Synow_MOATSource.cc
  AM_CPPFLAGS += $(MOAT_CPPFLAGS)
  AM_LIBS += $(MOAT)
  libes.la : ES_Synow_MOATSource.ocl

  CLEANFILES = ES_Synow_MOATSource.ocl
endif

bin_PROGRAMS = snprep

snprep_SOURCES  = snprep.cc
snprep_LDFLAGS  = $(AM_LDFLAGS)
snprep_LDADD    = libes.la $(AM_LIBS)

