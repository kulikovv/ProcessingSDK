#ifndef TRACKINGCOLOREDOBJECTS_GLOBAL_H
#define TRACKINGCOLOREDOBJECTS_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(TRACKINGCOLOREDOBJECTS_LIBRARY)
#  define TRACKINGCOLOREDOBJECTSSHARED_EXPORT Q_DECL_EXPORT
#else
#  define TRACKINGCOLOREDOBJECTSSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // TRACKINGCOLOREDOBJECTS_GLOBAL_H
