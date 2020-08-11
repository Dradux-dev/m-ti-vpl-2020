#ifndef BEUTHMATH_GLOBAL_H
#define BEUTHMATH_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(BEUTHMATH_LIBRARY)
#  define BEUTHMATH_EXPORT Q_DECL_EXPORT
#else
#  define BEUTHMATH_EXPORT Q_DECL_IMPORT
#endif

#endif // BEUTHMATH_GLOBAL_H
