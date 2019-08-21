#ifndef LOG_H
#define LOG_H

#include <QtCore/qglobal.h>

#if defined(LOG_LIBRARY)
#  define LOGSHARED_EXPORT Q_DECL_EXPORT
#else
#  define LOGSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // PULZARCONNECTOR_H
