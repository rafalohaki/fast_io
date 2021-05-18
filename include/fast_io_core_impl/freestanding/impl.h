#pragma once

/*

GCC provides more headers in freestanding environment than standard requires because WG21 sucks very badly.
no std::move, std::forward, __builtin_addressof. WTF???

*/

#include"to_address.h"
#include"array.h"
#include"iterator.h"
#include"cstr_len.h"
#include"string_view.h"
#include"algorithm.h"
