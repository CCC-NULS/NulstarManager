include($$TOP_SRCDIR/common.pri)

TEMPLATE = lib
CONFIG += qt warn_on
QT -= gui
TARGET = QuaZIP

CONFIG += c++11 skip_target_version_ext

# This one handles dllimport/dllexport directives.
DEFINES += QUAZIP_BUILD
DEFINES+=QT_NO_CAST_FROM_ASCII
DEFINES+=QT_NO_CAST_TO_ASCII
# You'll need to define this one manually if using a build system other
# than qmake or using QuaZIP sources directly in your project.
#CONFIG(staticlib): DEFINES += QUAZIP_STATIC

# Input
include(quazip.pri)


#CONFIG(debug, debug|release) {
 #    mac: TARGET = $$join(TARGET,,,_debug) 
#     win32: TARGET = $$join(TARGET,,,d)
#}

#unix:!symbian {
   # headers.path=$$PREFIX/include/quazip
    #headers.files=$$HEADERS
    #target.path=$$PREFIX/lib/$${LIB_ARCH}
   # QMAKE_PKGCONFIG_DESTDIR = pkgconfig
   # INSTALLS += headers target

#	OBJECTS_DIR=.obj
#	MOC_DIR=.moc
	
#}

win32 {
   # headers.path=$$PREFIX/include/quazip
   # headers.files=$$HEADERS
   # INSTALLS += headers target
   # CONFIG(staticlib){
     #   target.path=$$PREFIX/lib
      #  QMAKE_PKGCONFIG_LIBDIR = $$PREFIX/lib/
   # } else {
    #    target.path=$$PREFIX/bin
     #   QMAKE_PKGCONFIG_LIBDIR = $$PREFIX/bin/
   # }

    # workaround for qdatetime.h macro bug
    DEFINES += NOMINMAX
}

