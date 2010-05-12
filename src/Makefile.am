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

libes_a_CPPFLAGS = $(CFITSIO_CPPFLAGS) $(YAML_CPP_CPPFLAGS) $(APPSPACK_CPPFLAGS)

bin_PROGRAMS = synpp synapps

synpp_SOURCES  = syn++.cc
synpp_CPPFLAGS = $(YAML_CPP_CPPFLAGS)
synpp_LDADD    = libes.a $(CFITSIO_LDFLAGS) $(YAML_CPP_LDFLAGS)

synapps_SOURCES  = synapps.cc
synapps_CPPFLAGS = $(YAML_CPP_CPPFLAGS) $(APPSPACK_CPPFLAGS)
synapps_LDADD    = libes.a $(CFITSIO_LDFLAGS) $(YAML_CPP_LDFLAGS) $(APPSPACK_LDFLAGS)
