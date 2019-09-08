#ifndef CREATEPACKAGE_H
#define CREATEPACKAGE_H

#include <QtCore/qglobal.h>

#if defined(CREATEPACKAGE_LIBRARY)
#  define CREATEPACKAGESHARED_EXPORT Q_DECL_EXPORT
#else
#  define CREATEPACKAGESHARED_EXPORT Q_DECL_IMPORT
#endif

#endif //
