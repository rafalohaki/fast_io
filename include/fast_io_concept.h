#pragma once

//fast_io_concept.h allows you define your device and type without compilation time penalty
#if !defined(__cplusplus)
#error "You are not using C++ compiler"
#endif

#if defined(__GNUC__) && __GNUC__>=11 && __cplusplus<202002L
#error "fast_io requires at least C++20 standard compiler."
#else
#include<concepts>
#if defined(__GLIBCXX__)
#include<bits/move.h>
#else
#include<utility>		//for std::move, std::forward, __builtin_addressof
#endif
#include"fast_io_core_impl/concepts/impl.h"
#endif