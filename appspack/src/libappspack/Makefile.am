CXX = @MPICXX@

AM_CPPFLAGS = -I$(top_srcdir)/src/cddlib -I$(top_srcdir)/src/libappspack
AM_LDFLAGS = -L$(top_builddir)/src/cddlib -L$(top_builddir)/src/libappspack
AM_LIBS = $(LAPACK_LIBS) $(BLAS_LIBS) -lm $(FLIBS)
EXTRA_CPPFLAGS =
AM_CXXFLAGS =

EXTRA_DIST = APPSPACK_Config.hpp.in

lib_LTLIBRARIES = libappspack.la
libappspack_la_includedir = $(includedir)/APPSPACK
libappspack_la_SOURCES = \
APPSPACK_Cache_Manager.cpp \
APPSPACK_Cache_Point.cpp \
APPSPACK_Combiner_Generic.cpp \
APPSPACK_Constraints_Linear.cpp \
APPSPACK_Conveyor.cpp \
APPSPACK_Counter.cpp \
APPSPACK_Directions.cpp \
APPSPACK_Evaluator_SystemCall.cpp \
APPSPACK_Executor_Serial.cpp \
APPSPACK_Executor_MPI.cpp \
APPSPACK_GCI.cpp \
APPSPACK_List.cpp \
APPSPACK_Parameter_Entry.cpp \
APPSPACK_Parameter_List.cpp \
APPSPACK_Point.cpp \
APPSPACK_Print.cpp \
APPSPACK_Solver.cpp \
APPSPACK_Utils.cpp \
APPSPACK_Vector.cpp \
APPSPACK_Matrix.cpp \
APPSPACK_Float.cpp \
APPSPACK_CDDLIB_Interface.c \
APPSPACK_LAPACK_Wrappers.cpp

nodist_libappspack_la_include_HEADERS = \
APPSPACK/Config.hpp

libappspack_la_include_HEADERS = \
APPSPACK/Cache_Manager.hpp \
APPSPACK/Cache_Point.hpp \
APPSPACK/Cache_SplayTree.hpp \
APPSPACK/Combiner_Generic.hpp \
APPSPACK/Common.hpp \
APPSPACK/Constraints_Linear.hpp \
APPSPACK/Conveyor.hpp \
APPSPACK/Counter.hpp \
APPSPACK/Directions.hpp \
APPSPACK/Evaluator_Interface.hpp \
APPSPACK/Evaluator_SystemCall.hpp \
APPSPACK/Executor_Interface.hpp \
APPSPACK/Executor_Serial.hpp \
APPSPACK/Executor_MPI.hpp \
APPSPACK/GCI.hpp \
APPSPACK/List.hpp \
APPSPACK/Parameter_Entry.hpp \
APPSPACK/Parameter_List.hpp \
APPSPACK/Point.hpp \
APPSPACK/Print.hpp \
APPSPACK/Solver.hpp \
APPSPACK/Utils.hpp \
APPSPACK/Vector.hpp \
APPSPACK/Matrix.hpp \
APPSPACK/Float.hpp \
APPSPACK/CDDLIB_Interface.h \
APPSPACK/LAPACK_Wrappers.hpp \
APPSPACK/Common.h

libappspack_la_LDFLAGS = $(AM_LDFLAGS)
libappspack_la_LIBADD = $(top_builddir)/src/cddlib/libcdd.la
libappspack_la_LIBS = $(AM_LIBS)

