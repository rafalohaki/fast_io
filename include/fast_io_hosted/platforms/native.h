#pragma once
#include"native_base.h"
#if defined(__WINNT__) || defined(_MSC_VER)
#include"win32_mapping.h"
#include"win32_transmit.h"
#include"omap.h"
#elif !defined(__NEWLIB__) && !defined(__MSDOS__)
#include"posix_mapping.h"
#include"omap.h"
#endif
