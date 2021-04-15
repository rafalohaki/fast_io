#pragma once

//fast_io_concept.h allows you define your device and type without compilation time penalty
#if !defined(__cplusplus)
#error "You are not using C++ compiler"
#endif

#if __cplusplus<202002L && !defined(_MSC_VER)
#error "fast_io requires at least C++20 standard compiler."
#else
#include"fast_io_core_impl/concepts/impl.h"
#endif