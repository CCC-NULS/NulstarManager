include($$TOP_SRCDIR/common.pri)

QT += gui widgets
CONFIG += c++11 skip_target_version_ext
DEFINES += LOG_LIBRARY
TARGET = Log
TEMPLATE = lib
SOURCES += NLog.cpp
HEADERS += NLog.h \
    Log.h
DESTDIR = $$LIBRARIES_OUTDIR
FORMS += Log.ui
INCLUDEPATH += $$TOP_BUILDDIR/$$TARGET \
               ../Definitions \
               ../Tools
message($$INCLUDEPATH)
LIBS += -L$$DESTDIR \
        -lDefinitions \
        -lTools


RESOURCES += \
    Log.qrc

