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
               ../Definitions \
               ../About \
               ../Client \
               ../Connector \
               ../Log \
               ../QuaZIP \
               ../Tools \
               ../Software \
               ../SystemObject #\
            #   ../../../Libraries/Quazip/include \
            #   ../../../Build/NulstarManager/Tools \
            #   ../../../Build/NulstarManager/Client \
            #   ../../../Build/NulstarManager/Connector \
            #   ../../../Build/NulstarManager/Log \
            #   ../../../Build/NulstarManager/Software \
            #   ../../../Build/NulstarManager/SystemObject
LIBS += -L$$DESTDIR \
#-L../../../Build/NulstarManager \
      #-L../../../Data/Development/Libraries/Quazip/lib \
      -lQuaZIP \
      -lDefinitions \
      -lAbout \
      #-lClient \
      #-lConnector \
      #-lLog \
      #-lSoftware \
      -lTools #\
     # -lSystemObject





