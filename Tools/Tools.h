#ifndef TOOLS_H
#define TOOLS_H

#include <QtCore/qglobal.h>

#if defined(TOOLS_LIBRARY)
#  define TOOLSSHARED_EXPORT Q_DECL_EXPORT
#else
#  define TOOLSSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // PULZARTOOLS_H
