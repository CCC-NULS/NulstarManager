#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <QtCore/qglobal.h>

#if defined(DEFINITIONS_LIBRARY)
#  define DEFINITIONSSHARED_EXPORT Q_DECL_EXPORT
#else
#  define DEFINITIONSSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // PULZARSERVERDEFINITIONS_GLOBAL_H
