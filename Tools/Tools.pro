# Base Settings
include($$TOP_SRCDIR/common.pri)

QT += network gui widgets
CONFIG += c++11 skip_target_version_ext
DEFINES += TOOLS_LIBRARY
TARGET = Tools
TEMPLATE = lib
SOURCES += NValidatorCreator.cpp \
    VAResourceValidator.cpp \
    VAStringValidator.cpp \
    VAHostValidator.cpp \
    VAIntValidator.cpp \
    NSplashScreen.cpp \
    DLProgressBar.cpp \
    DLCheckBox.cpp
HEADERS += Tools.h \
    NValidatorCreator.h \
    VAResourceValidator.h \
    VAStringValidator.h \
    VAHostValidator.h \
    VAIntValidator.h \
    NSplashScreen.h \
    DLProgressBar.h \
    DLCheckBox.h

FORMS += NSplashScreen.ui
DESTDIR=$$LIBRARIES_OUTDIR
INCLUDEPATH += $$TOP_BUILDDIR/$$TARGET \
        ../Definitions

LIBS += -L$$DESTDIR \
        -lDefinitions



