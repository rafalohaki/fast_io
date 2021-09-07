#pragma once

#if 0
#include"width.h"
#include"grouping.h"
#endif

#include"boolalpha.h"
#include"integers.h"

#if __cpp_lib_chrono >= 201907L || (__GNUC__  >= 11 || defined(__clang__))
#if defined(_GLIBCXX_CHRONO) || defined(_LIBCPP_CHRONO) || defined(_CHRONO_)
#include"chrono.h"
#endif
#endif

#if 1
#include"lc_time.h"
#endif