#- 
#- File    : Makefile
#- ------------------
#- Created : Fri Dec 11 14:13:10 2009
#- Creator : Rollin C. Thomas (RCT) - rcthomas@lbl.gov
#- 

CFITSIO_BASE=/opt/local
YAML_BASE=${HOME}/local

CPP = g++-mp-4.3 -O3 -fopenmp

CFITSIO_INCL=-I$(CFITSIO_BASE)/include
CFITSIO_LINK=-L$(CFITSIO_BASE)/lib
CFITSIO_LIBS=-lcfitsio

YAML_INCL=-I$(YAML_BASE)/include
YAML_LINK=-L$(YAML_BASE)/lib
YAML_LIBS=-lyaml-cpp

OBJ =                      \
   ES_Accelerator.o        \
   ES_Blackbody.o          \
   ES_Spectrum.o           \
   ES_LineManager.o        \
   ES_Synow_Grid.o         \
   ES_Synow_Opacity.o      \
   ES_Synow_Source.o       \
   ES_Synow_Spectrum.o     \
   ES_Synow_Setup.o        \
   syn++.o

all : syn++

syn++ : $(OBJ)
	$(CPP) $(CFITSIO_LINK) $(YAML_LINK) -o $@ $(OBJ) $(CFITSIO_LIBS) $(YAML_LIBS)

.cc.o :
	$(CPP) $(CFITSIO_INCL) $(YAML_INCL) -c -o $@ $<

clean :
	rm -rf *.o

realclean : clean
	rm -rf syn++
