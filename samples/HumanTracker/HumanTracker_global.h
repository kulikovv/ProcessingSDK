#ifndef HUMANTRACKER_GLOBAL_H
#define HUMANTRACKER_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(HUMANTRACKER_LIBRARY)
#  define HUMANTRACKERSHARED_EXPORT Q_DECL_EXPORT
#else
#  define HUMANTRACKERSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // HUMANTRACKER_GLOBAL_H
