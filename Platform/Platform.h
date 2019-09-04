#ifndef PLATFORM_H
#define PLATFORM_H

#include <QtCore/qglobal.h>

#if defined(PLATFORM_LIBRARY)
#  define PLATFORMSHARED_EXPORT Q_DECL_EXPORT
#else
#  define PLATFORMSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif //
