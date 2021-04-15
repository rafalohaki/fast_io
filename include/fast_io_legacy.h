#pragma once
#if !defined(__cplusplus)
#error "You are not using C++ compiler"
#endif

#if __cplusplus<202002L && !defined(_MSC_VER)
#error "fast_io requires at least C++20 standard compiler."
#else
//fast_io_legacy.h deals with legacy C <stdio.h> and C++ <iostream>/<fstream>/<sstream> interface
#if __STDC_HOSTED__==1 && (!defined(_GLIBCXX_HOSTED) || _GLIBCXX_HOSTED==1)
#include<streambuf>
#include<fstream>
#include<sstream>
#include<istream>
#include<ostream>

#include"fast_io.h"
#include"fast_io_legacy_impl/cpp/streambuf_io_observer.h"
#include"fast_io_legacy_impl/cpp/filebuf_file.h"
#endif

#endif