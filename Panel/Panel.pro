include($$TOP_SRCDIR/common.pri)

APP_VERSION=1.0.0
APP_VERSION_NAME=Potato Killer

QT += gui network widgets
CONFIG += c++11 skip_target_version_ext
TARGET = NulstarManager
QMAKE_SUBSTITUTES += NulstarManagerVersion.h.in
TEMPLATE = app
SOURCES += Main.cpp \
           NMainWindow.cpp
HEADERS += NMainWindow.h
RESOURCES += Panel.qrc
INCLUDEPATH += $$TOP_BUILDDIR/Tools \
               $$TOP_BUILDDIR/Log \
               $$TOP_BUILDDIR/Platform \
               $$TOP_BUILDDIR/Software \
               ../Definitions \
               ../About \
               ../Client \
               ../Connector \
               ../Log \
               ../QuaZIP \
               ../Tools \
               ../Software \
               ../Platform

LIBS += -L$$DESTDIR \
      -lQuaZIP \
      -lDefinitions \
      -lAbout \
      #-lClient \
      -lLog \
      -lSoftware \
      -lTools \
      -lPlatform





