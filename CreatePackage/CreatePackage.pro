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
               ../QuaZIP \
               ../Secp256k1
LIBS += -L$$DESTDIR \
        -lDefinitions \
        -lQuaZIP \
        -lTools \
        -lsecp256k1

# Install Settings

!exists($$DESTDIR/Platform.ndd) {
  OTHER_FILES += Platform.ndd
  COPYFILE1 = \"$$PWD/Platform.ndd\" \"$$DESTDIR\"
  win32: COPYFILE1 ~= s,/,\\,g
  QMAKE_POST_LINK += $$QMAKE_COPY $$COPYFILE1 $$escape_expand(\\n\\t)
}

!exists($$DESTDIR/Software.ndd) {
  OTHER_FILES += Software.ndd
  COPYFILE1 = \"$$PWD/Software.ndd\" \"$$DESTDIR\"
  win32: COPYFILE1 ~= s,/,\\,g
  QMAKE_POST_LINK += $$QMAKE_COPY $$COPYFILE1 $$escape_expand(\\n\\t)
}

!exists($$DESTDIR/PrivateKey.txt) {
  OTHER_FILES += PrivateKey.txt
  COPYFILE1 = \"$$PWD/PrivateKey.txt\" \"$$DESTDIR\"
  win32: COPYFILE1 ~= s,/,\\,g
  QMAKE_POST_LINK += $$QMAKE_COPY $$COPYFILE1 $$escape_expand(\\n\\t)
}
