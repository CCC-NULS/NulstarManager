QT += gui network widgets sql
TARGET = WorldcoinManager
TEMPLATE = app
SOURCES += Main.cpp \
           WCMainWindow.cpp
HEADERS += WCMainWindow.h
DESTDIR = ../../../Applications/WorldcoinManager
RESOURCES += Panel.qrc
INCLUDEPATH += ../Definitions \
               ../About \
               ../Client \
               ../Connector \
               ../Log \
               ../Tools \
               ../Software \
               ../SystemObject \
               ../../../Libraries/Quazip/include \
               ../../../Build/WorldcoinManager/Tools \
               ../../../Build/WorldcoinManager/Client \
               ../../../Build/WorldcoinManager/Connector \
               ../../../Build/WorldcoinManager/Log \
               ../../../Build/WorldcoinManager/Software \
               ../../../Build/WorldcoinManager/SystemObject
LIBS += -L../../../Build/WorldcoinManager \
      -L../../../Data/Development/Libraries/Quazip/lib \
      -lquazip \
      -lDefinitions \
      -lAbout \
      -lClient \
      -lConnector \
      -lLog \
      -lSoftware \
      -lTools \
      -lSystemObject


