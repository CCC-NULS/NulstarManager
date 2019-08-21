QT += network gui sql widgets
CONFIG += static
DEFINES += CONNECTOR_LIBRARY
TARGET = Connector
TEMPLATE = lib
SOURCES += WCConnector.cpp
HEADERS += WCConnector.h \
            Connector.h
DESTDIR = ../../../Build/WorldcoinManager
FORMS += Connector.ui
RESOURCES += Connector.qrc
INCLUDEPATH += ../Qftp \
               ../Definitions \
               ../Tools
LIBS += -L../../../Build/WorldcoinManager \
        -lDefinitions \
        -lTools
