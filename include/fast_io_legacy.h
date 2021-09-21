#pragma once
#if !defined(__cplusplus)
#error "You are not using C++ compiler"
#endif

#if defined(__GNUC__) && __GNUC__>=11 && __cplusplus<202002L
#error "fast_io requires at least C++20 standard compiler."
#else
//fast_io_legacy.h deals with legacy C++ <iostream>/<fstream>/<sstream> interface
#if __STDC_HOSTED__==1 && (!defined(_GLIBCXX_HOSTED) || _GLIBCXX_HOSTED==1)
#include<streambuf>
#include<fstream>

#include"fast_io.h"
#include"fast_io_legacy_impl/filebuf/streambuf_io_observer.h"
#include"fast_io_legacy_impl/filebuf/filebuf_file.h"
#endif

#endif