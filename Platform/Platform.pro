include($$TOP_SRCDIR/common.pri)

QT += gui widgets
CONFIG += c++11 skip_target_version_ext
DEFINES += PLATFORM_LIBRARY
TARGET = Platform
TEMPLATE = lib
SOURCES += NPlatform.cpp \
           NPlatformRecord.cpp
HEADERS += NPlatform.h \
           NPlatformRecord.h \
           Platform.h
DESTDIR = $$LIBRARIES_OUTDIR
FORMS += NPlatform.ui \
         NPlatformRecord.ui
RESOURCES += NPlatform.qrc
INCLUDEPATH += $$TOP_BUILDDIR/$$TARGET \
               ../Definitions \
               ../Tools
LIBS += -L$$DESTDIR \
        -lDefinitions \
        -lTools
