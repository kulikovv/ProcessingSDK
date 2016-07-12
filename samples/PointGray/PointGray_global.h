#ifndef POINTGRAY_GLOBAL_H
#define POINTGRAY_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(POINTGRAY_LIBRARY)
#  define POINTGRAYSHARED_EXPORT Q_DECL_EXPORT
#else
#  define POINTGRAYSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // POINTGRAY_GLOBAL_H
