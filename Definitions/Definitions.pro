# Base Settings
include($$TOP_SRCDIR/common.pri)

# Project Settings

QT += gui widgets
CONFIG += c++11 skip_target_version_ext
TEMPLATE = lib
TARGET = Definitions

DEFINES += DEFINITIONS_LIBRARY
DESTDIR=$$LIBRARIES_OUTDIR


SOURCES += NConstants.cpp \
    NMessage.cpp \
    NMessagePool.cpp
HEADERS += Definitions.h \
    NConstants.h \
    NMessage.h \
    NMessagePool.h

