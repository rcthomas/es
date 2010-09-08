CXX = @MPICXX@

AM_CPPFLAGS = -I$(top_srcdir)/src/cddlib -I$(top_srcdir)/src/libappspack
AM_LDFLAGS = -L$(top_builddir)/src/cddlib -L$(top_builddir)/src/libappspack
AM_LIBS = $(LAPACK_LIBS) $(BLAS_LIBS) -lm $(FLIBS)
EXTRA_CPPFLAGS =
AM_CXXFLAGS =

noinst_HEADERS = \
APPSPACK_Config.hpp

include_HEADERS = \
APPSPACK_Cache_Manager.hpp \
APPSPACK_Cache_Point.hpp \
APPSPACK_Cache_SplayTree.hpp \
APPSPACK_Combiner_Generic.hpp \
APPSPACK_Common.hpp \
APPSPACK_Constraints_Linear.hpp \
APPSPACK_Conveyor.hpp \
APPSPACK_Counter.hpp \
APPSPACK_Directions.hpp \
APPSPACK_Evaluator_Interface.hpp \
APPSPACK_Evaluator_SystemCall.hpp \
APPSPACK_Executor_Interface.hpp \
APPSPACK_Executor_Serial.hpp \
APPSPACK_Executor_MPI.hpp \
APPSPACK_GCI.hpp \
APPSPACK_List.hpp \
APPSPACK_Parameter_Entry.hpp \
APPSPACK_Parameter_List.hpp \
APPSPACK_Point.hpp \
APPSPACK_Print.hpp \
APPSPACK_Solver.hpp \
APPSPACK_Utils.hpp \
APPSPACK_Vector.hpp \
APPSPACK_Matrix.hpp \
APPSPACK_Float.hpp \
APPSPACK_CDDLIB_Interface.h \
APPSPACK_LAPACK_Wrappers.hpp \
APPSPACK_Common.h

lib_LTLIBRARIES = libappspack.la
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
libappspack_la_LDFLAGS = $(AM_LDFLAGS)
libappspack_la_LIBADD = $(top_builddir)/src/cddlib/libcdd.la
libappspack_la_LIBS = $(AM_LIBS)

