#ifndef SOFTWARE_H
#define SOFTWARE_H

#include <QtCore/qglobal.h>

#if defined(SOFTWARE_LIBRARY)
#  define SOFTWARESHARED_EXPORT Q_DECL_EXPORT
#else
#  define SOFTWARESHARED_EXPORT Q_DECL_IMPORT
#endif

#endif //
