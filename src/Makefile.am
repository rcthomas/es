AM_CPPFLAGS = -I$(top_srcdir)/src $(CFITSIO_CPPFLAGS) $(YAML_CPPFLAGS)
AM_LDFLAGS = -L$(top_builddir)/src
AM_LIBS = $(CFITSIO) $(YAML) -lm
AM_CXXFLAGS =

if HAVE_AM_OPENMP
  AM_CXXFLAGS += $(OPENMP_CFLAGS)
  AM_LIBS += $(OPENMP_CFLAGS)
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
ES_Synapps.hh           \
ES_Synapps_Config.hh    \
ES_Synapps_Evaluator.hh \
ES_Synow.hh             \
ES_Synow_Grid.hh        \
ES_Synow_Opacity.hh     \
ES_Synow_Operator.hh    \
ES_Synow_Setup.hh       \
ES_Synow_Source.hh      \
ES_Synow_Spectrum.hh

noinst_LIBRARIES = libes.a
libes_a_SOURCES =       \
ES_Accelerator.cc       \
ES_Blackbody.cc         \
ES_LineManager.cc       \
ES_Spectrum.cc          \
ES_Synapps_Config.cc    \
ES_Synapps_Evaluator.cc \
ES_Synow_Grid.cc        \
ES_Synow_Opacity.cc     \
ES_Synow_Setup.cc       \
ES_Synow_Source.cc      \
ES_Synow_Spectrum.cc
libes_a_LIBADD = $(AM_LIBS)

bin_PROGRAMS = synpp synapps

synpp_SOURCES  = syn++.cc
synpp_LDFLAGS  = $(AM_LDFLAGS)
synpp_LDADD    = libes.a $(AM_LIBS)

synapps_SOURCES  = synapps.cc
synapps_CPPFLAGS = $(AM_CPPFLAGS) $(APPSPACK_CPPFLAGS)
synapps_LDFLAGS  = $(AM_LDFLAGS)
synapps_LDADD    = libes.a $(APPSPACK) $(LAPACK_LIBS) $(BLAS_LIBS) $(AM_LIBS) $(FLIBS)

