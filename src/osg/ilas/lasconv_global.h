#ifndef LASCONV_GLOBAL_H
#define LASCONV_GLOBAL_H

#include <QtCore/qglobal.h>

#ifdef LASCONV_LIB
# define LASCONV_EXPORT Q_DECL_EXPORT
#else
# define LASCONV_EXPORT Q_DECL_IMPORT
#endif

#endif // LASCONV_GLOBAL_H
