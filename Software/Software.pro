include($$TOP_SRCDIR/common.pri)

QT += gui widgets
CONFIG += c++11 skip_target_version_ext
DEFINES += SOFTWARE_LIBRARY
TARGET = Software
TEMPLATE = lib
SOURCES += NSoftware.cpp \
           NSoftwareRecord.cpp
HEADERS += NSoftware.h \
           NSoftwareRecord.h \
           Software.h
DESTDIR = $$LIBRARIES_OUTDIR
FORMS += NSoftware.ui \
         NSoftwareRecord.ui
RESOURCES += NSoftware.qrc
INCLUDEPATH += $$TOP_BUILDDIR/$$TARGET \
               ../Definitions \
               ../Tools
LIBS += -L$$DESTDIR \
        -lDefinitions \
        -lTools
