AM_CPPFLAGS = -I$(top_srcdir)/external/yaml -I$(top_srcdir)/external/yaml/yaml-cpp
AM_LDFLAGS = -L$(top_builddir)/external/yaml
AM_LIBS = -lm
EXTRA_CPPFLAGS =
AM_CFLAGS =

nobase_noinst_HEADERS = \
yaml-cpp/conversion.h \
yaml-cpp/emitter.h \
yaml-cpp/emittermanip.h \
yaml-cpp/exceptions.h \
yaml-cpp/iterator.h \
yaml-cpp/mark.h \
yaml-cpp/node.h \
yaml-cpp/nodeimpl.h \
yaml-cpp/nodereadimpl.h \
yaml-cpp/nodeutil.h \
yaml-cpp/noncopyable.h \
yaml-cpp/null.h \
yaml-cpp/ostream.h \
yaml-cpp/parser.h \
yaml-cpp/stlemitter.h \
yaml-cpp/stlnode.h \
yaml-cpp/traits.h \
yaml-cpp/yaml.h

noinst_HEADERS = \
aliascontent.h \
content.h \
emitterstate.h \
emitterutils.h \
exp.h \
indentation.h \
iterpriv.h \
ltnode.h \
map.h \
parserstate.h \
regex.h \
regeximpl.h \
scalar.h \
scanner.h \
scanscalar.h \
scantag.h \
sequence.h \
setting.h \
stream.h \
streamcharsource.h \
stringsource.h \
tag.h \
token.h

noinst_LTLIBRARIES = libyaml-cpp.la
libyaml_cpp_la_SOURCES = \
aliascontent.cpp \
conversion.cpp \
emitter.cpp \
emitterstate.cpp \
emitterutils.cpp \
exp.cpp \
iterator.cpp \
map.cpp \
node.cpp \
null.cpp \
ostream.cpp \
parser.cpp \
parserstate.cpp \
regex.cpp \
scalar.cpp \
scanner.cpp \
scanscalar.cpp \
scantag.cpp \
scantoken.cpp \
sequence.cpp \
simplekey.cpp \
stream.cpp \
tag.cpp
libyaml_cpp_la_LIBADD = $(AM_LIBS)
