AM_CPPFLAGS = -I$(top_srcdir)/external/yaml -I$(top_srcdir)/external/yaml/yaml-cpp -I$(top_srcdir)/external/yaml-cpp/boost
AM_LDFLAGS = -L$(top_builddir)/external/yaml
AM_LIBS = -lm
EXTRA_CPPFLAGS =
AM_CFLAGS =

nobase_noinst_HEADERS = \
yaml-cpp/anchor.h \
yaml-cpp/binary.h \
yaml-cpp/dll.h \
yaml-cpp/emitfromevents.h \
yaml-cpp/emitter.h \
yaml-cpp/emitterdef.h \
yaml-cpp/emittermanip.h \
yaml-cpp/eventhandler.h \
yaml-cpp/exceptions.h \
yaml-cpp/mark.h \
yaml-cpp/noncopyable.h \
yaml-cpp/null.h \
yaml-cpp/ostream_wrapper.h \
yaml-cpp/parser.h \
yaml-cpp/stlemitter.h \
yaml-cpp/traits.h \
yaml-cpp/yaml.h

noinst_HEADERS = \
collectionstack.h \
directives.h \
emitterstate.h \
emitterutils.h \
exp.h \
indentation.h \
nodebuilder.h \
nodeevents.h \
ptr_stack.h \
ptr_vector.h \
regex.h \
regeximpl.h \
scanner.h \
scanscalar.h \
scantag.h \
setting.h \
singledocparser.h \
stream.h \
streamcharsource.h \
stringsource.h \
tag.h \
token.h

noinst_LTLIBRARIES = libyaml-cpp.la
libyaml_cpp_la_SOURCES = \
binary.cpp \
convert.cpp \
directives.cpp \
emit.cpp \
emitfromevents.cpp \
emitter.cpp \
emitterstate.cpp \
emitterutils.cpp \
exp.cpp \
memory.cpp \
node.cpp \
node_data.cpp \
nodebuilder.cpp \
nodeevents.cpp \
null.cpp \
ostream_wrapper.cpp \
parse.cpp \
parser.cpp \
regex.cpp \
scanner.cpp \
scanscalar.cpp \
scantag.cpp \
scantoken.cpp \
simplekey.cpp \
singledocparser.cpp \
stream.cpp \
tag.cpp
libyaml_cpp_la_LIBADD = $(AM_LIBS)

