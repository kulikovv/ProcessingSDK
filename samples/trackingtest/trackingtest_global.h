#ifndef TRACKINGTEST_GLOBAL_H
#define TRACKINGTEST_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(TRACKINGTEST_LIBRARY)
#  define TRACKINGTESTSHARED_EXPORT Q_DECL_EXPORT
#else
#  define TRACKINGTESTSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // TRACKINGTEST_GLOBAL_H
