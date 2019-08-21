QT += gui widgets
CONFIG += static
DEFINES += LOG_LIBRARY
TARGET = Log
TEMPLATE = lib
SOURCES += WCLog.cpp
HEADERS += WCLog.h \
    Log.h
DESTDIR = ../../../Build/WorldcoinManager
FORMS += Log.ui
INCLUDEPATH += ../Definitions
LIBS += -L../../../Build/WorldcoinManager \
        -lDefinitions

RESOURCES += \
    Log.qrc
