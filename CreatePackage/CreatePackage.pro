include($$TOP_SRCDIR/common.pri)

QT += gui widgets
CONFIG += c++11 skip_target_version_ext
DEFINES += CREATEPACKAGE_LIBRARY
TARGET = CreatePackage
TEMPLATE = lib
SOURCES += NCreatePackage.cpp
HEADERS += NCreatePackage.h \
           CreatePackage.h
DESTDIR = $$LIBRARIES_OUTDIR
FORMS += NCreatePackage.ui
RESOURCES += NCreatePackage.qrc
INCLUDEPATH += $$TOP_BUILDDIR/$$TARGET \
               ../Definitions \
               ../Tools \
               ../QuaZIP
LIBS += -L$$DESTDIR \
        -lDefinitions \
        -lQuaZIP \
        -lTools
