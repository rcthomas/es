
EXTRA_DIST = AUTHORS ChangeLog COPYING doxygen.txt INSTALL NEWS README platforms autogen.sh python example

SUBDIRS = src

pkgconfig_DATA = es-${API_VERSION}.pc

if INSTALLMOD
if HAVEENVMOD
modfile_DATA = ${PACKAGE_VERSION}
else
modfile_DATA = ${PACKAGE_VERSION}.lua
endif
endif



