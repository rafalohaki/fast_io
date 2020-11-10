#pragma once

#if !defined(__MSDOS__) && !defined(__NEWLIB__)

#if defined(_WIN32)
#include"nt.h"
#else
#include"posix.h"
#endif
#endif

