#pragma once
#include"native_base.h"
#if defined(__WINNT__) || defined(_MSC_VER)
#include"win32_mapping.h"
#include"win32_transmit.h"
#elif !defined(__NEWLIB__)
#include"posix_mapping.h"
#endif
#ifndef __NEWLIB__
#include"omap.h"
#endif
