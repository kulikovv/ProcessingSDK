#ifndef %ProjectName:u%_GLOBAL_H
#define %ProjectName:u%_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(%ProjectName:u%_LIBRARY)
#  define %ProjectName:u%SHARED_EXPORT Q_DECL_EXPORT
#else
#  define %ProjectName:u%SHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // %ProjectName:u%_GLOBAL_H
