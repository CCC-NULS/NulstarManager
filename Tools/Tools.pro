QT += gui network widgets
CONFIG += static
DEFINES += TOOLS_LIBRARY
TARGET = Tools
TEMPLATE = lib
SOURCES += WCValidatorCreator.cpp \
    VAResourceValidator.cpp \
    VAStringValidator.cpp \
    VAHostValidator.cpp \
    VAIntValidator.cpp \
    WCSplashScreen.cpp \
    DLProgressBar.cpp \
    DLCheckBox.cpp
HEADERS += Tools.h \
    WCValidatorCreator.h \
    VAResourceValidator.h \
    VAStringValidator.h \
    VAHostValidator.h \
    VAIntValidator.h \
    WCSplashScreen.h \
    DLProgressBar.h \
    DLCheckBox.h
FORMS += WCSplashScreen.ui
DESTDIR = ../../../Build/WorldcoinManager
INCLUDEPATH += ../Definitions
LIBS += -L../../../Build/WorldcoinManager \
        -lDefinitions
