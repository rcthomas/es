#- 
#- File    : Makefile
#- ------------------
#- Created : Fri Dec 11 14:13:10 2009
#- Creator : Rollin C. Thomas (RCT) - rcthomas@lbl.gov
#- 

### REQUIRE_USING_NAMESPACE_STD for APPSPACK

CPP=mpicxx -DREQUIRE_USING_NAMESPACE_STD
CFITSIO_BASE=/opt/local
YAML_BASE=${HOME}/local
APPS_BASE=${HOME}/local

# CPP=CC --no_warnings -fastsse -mp=nonuma --exceptions -Minfo=mp -DMPICH_IGNORE_CXX_SEEK
# CFITSIO_BASE=/project/projectdirs/cmb/modules/franklin/pgi/cmb/2.4.1/cfitsio_3.0.6-2.4.1
# YAML_BASE=${HOME}/local/franklin
# APPS_BASE=${HOME}/local/franklin

CFITSIO_INCL=-I$(CFITSIO_BASE)/include
CFITSIO_LINK=-L$(CFITSIO_BASE)/lib
CFITSIO_LIBS=-lcfitsio

YAML_INCL=-I$(YAML_BASE)/include
YAML_LINK=-L$(YAML_BASE)/lib
YAML_LIBS=-lyaml-cpp

APPS_INCL=-I$(APPS_BASE)/include/appspack
APPS_LINK=-L$(APPS_BASE)/lib
APPS_LIBS=-lappspack -lcdd -llapack -lblas

OBJ =                      \
   ES_Accelerator.o        \
   ES_Blackbody.o          \
   ES_Spectrum.o           \
   ES_LineManager.o        \
   ES_Synow_Grid.o         \
   ES_Synow_Opacity.o      \
   ES_Synow_Source.o       \
   ES_Synow_Spectrum.o     \
   ES_Synow_Setup.o

OBJ_SNPREP=ES_Spectrum.o snprep.o

OBJ_SYNPP=$(OBJ) syn++.o

OBJ_SYNAPPS=$(OBJ)        \
   ES_Synapps_Config.o    \
   ES_Synapps_Evaluator.o \
   synapps.o

all : syn++ synapps snprep

syn++ : $(OBJ_SYNPP)
	$(CPP) $(CFITSIO_LINK) $(YAML_LINK) -o $@ $(OBJ_SYNPP) $(CFITSIO_LIBS) $(YAML_LIBS)

synapps : $(OBJ_SYNAPPS)
	$(CPP) $(CFITSIO_LINK) $(YAML_LINK) $(APPS_LINK) -o $@ $(OBJ_SYNAPPS) $(CFITSIO_LIBS) $(YAML_LIBS) $(APPS_LIBS)

snprep : $(OBJ_SNPREP)
	$(CPP) $(CFITSIO_LINK) -o $@ $(OBJ_SNPREP) $(CFITSIO_LIBS)

.cc.o :
	$(CPP) $(CFITSIO_INCL) $(YAML_INCL) $(APPS_INCL) -c -o $@ $<

clean :
	rm -rf *.o core

realclean : clean
	rm -rf syn++ synapps snprep
