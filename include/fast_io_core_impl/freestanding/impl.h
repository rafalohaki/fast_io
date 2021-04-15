#pragma once

/*

GCC provides more headers in freestanding environment than standard requires because WG21 sucks very badly.
no std::move, std::forward, std::addressof. WTF???

*/

#if defined(__GLIBCXX__)
#include"libstdc++.h"
#else
#include<utility>		//for std::move, std::forward, std::addressof
#endif
#include"to_address.h"
#include"array.h"
#include"iterator.h"
#include"cstr_len.h"
#include"string_view.h"
#include"algorithm.h"
