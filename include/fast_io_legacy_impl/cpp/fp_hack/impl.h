#pragma once
#ifdef __GLIBCXX__
#include"libstdc++.h"
#elif defined(_MSVC_STL_UPDATE)
#include"msvc_stl.h"
#elif defined(__LIBCPP_VERSION)
#include"libc++.h"
#endif