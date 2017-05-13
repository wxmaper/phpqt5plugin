#pragma once

#include <QtGlobal>

#if defined(PHPQT5_LIBRARY)
#  define PHPQT5SHARED_EXPORT Q_DECL_EXPORT
#else
#  define PHPQT5SHARED_EXPORT Q_DECL_IMPORT
#endif
