QT += network gui sql widgets
CONFIG += static
DEFINES += ABOUT_LIBRARY
TARGET = About
TEMPLATE = lib
RESOURCES += About.qrc
SOURCES += WCAbout.cpp
HEADERS += WCAbout.h \
    About.h
DESTDIR = ../../../Build/WorldcoinManager
INCLUDEPATH += ../../../Build/WorldcoinManager/Tools \
               ../Definitions \
               ../Tools
LIBS += -L../../../Build/WorldcoinManager \
        -lDefinitions \
        -lTools
