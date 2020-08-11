#ifndef BEUTHAI_GLOBAL_H
#define BEUTHAI_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(BEUTHAI_LIBRARY)
#  define BEUTHAI_EXPORT Q_DECL_EXPORT
#else
#  define BEUTHAI_EXPORT Q_DECL_IMPORT
#endif

#endif // BEUTHAI_GLOBAL_H
