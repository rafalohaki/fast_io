#pragma once
#include"native_base.h"
#if defined(__WINNT__) || defined(_MSC_VER)
#include"win32_mapping.h"
#include"win32_transmit.h"
#elif !defined(__NEWLIB__)
#include"posix_mapping.h"
#endif

namespace fast_io
{

#if defined(__WINNT__) || defined(_MSC_VER)
using native_file_map = win32_file_map;
#elif !defined(__NEWLIB__)
using native_file_map = posix_file_map;
#endif

}