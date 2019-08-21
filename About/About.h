#ifndef ABOUT_H
#define ABOUT_H

#include <QtCore/qglobal.h>

#if defined(ABOUT_LIBRARY)
#  define ABOUTSHARED_EXPORT Q_DECL_EXPORT
#else
#  define ABOUTSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif
