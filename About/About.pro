# Base Settings
include($$TOP_SRCDIR/common.pri)

QT += gui widgets
CONFIG += c++11 skip_target_version_ext
DEFINES += ABOUT_LIBRARY
TARGET = About
TEMPLATE = lib
RESOURCES += About.qrc
SOURCES += NAbout.cpp
HEADERS += NAbout.h \
    About.h
DESTDIR=$$LIBRARIES_OUTDIR
INCLUDEPATH += $$TOP_BUILDDIR/Tools \
               ../Definitions \
               ../Tools
LIBS += -L$$DESTDIR \
        -lDefinitions \
        -lTools


