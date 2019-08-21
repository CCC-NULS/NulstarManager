QT += gui
CONFIG += static
TARGET = Definitions
TEMPLATE = lib
DEFINES += DEFINITIONS_LIBRARY
SOURCES += WCVersion.cpp \
    WCConstants.cpp \
    WCMessage.cpp \
    WCMessagePool.cpp
HEADERS += Definitions.h \
           WCVersion.h \
    WCConstants.h \
    WCMessage.h \
    WCMessagePool.h
DESTDIR = ../../../Build/WorldcoinManager
