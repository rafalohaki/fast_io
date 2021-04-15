#pragma once
/*
https://en.cppreference.com/w/cpp/freestanding
There are two kinds of implementations defined by the C++ standard:
hosted and freestanding implementations.
For hosted implementations the set of standard library headers required by the C++ standard is much larger than for freestanding ones.
*/
//fast_io_hosted defines what we could use in a hosted environment.

#if !defined(__cplusplus)
#error "You are not using C++ compiler"
#endif

#if __cplusplus<202002L && !defined(_MSC_VER)
#error "fast_io requires at least C++20 standard compiler."
#else
#include"fast_io_freestanding.h"
#if __STDC_HOSTED__==1 && (!defined(_GLIBCXX_HOSTED) || _GLIBCXX_HOSTED==1)
#include<string>
#include"fast_io_hosted/string/impl.h"
#include"fast_io_hosted/concat.h"
#include"fast_io_hosted/posix_error.h"
#include"fast_io_hosted/posix_error_scatter.h"
#ifdef __MSDOS__
#undef __STRICT_ANSI__
#endif

#include"fast_io_hosted/io_io_file.h"
#include"fast_io_hosted/cstring_view.h"

#include"fast_io_hosted/mmap.h"
#include"fast_io_hosted/chrono.h"
#include"fast_io_hosted/posix_status.h"
#include"fast_io_hosted/platforms/native.h"
#include"fast_io_hosted/file_loaders/impl.h"

#if __cpp_lib_jthread >= 201703L
#include"fast_io_hosted/iomutex.h"
#endif
#include"fast_io_hosted/wrapper.h"
#include"fast_io_hosted/filesystem/native.h"

#ifdef FAST_IO_PROCESS_REVAMP
#include"fast_io_hosted/process_revamp/native.h"
#endif

#include"fast_io_hosted/white_hole/white_hole.h"
//#include<memory_resource>
//#include"fast_io_hosted/pmr_resource.h"
#include<complex>
#include"fast_io_hosted/complex.h"

#include"fast_io_hosted/time.h"
#if defined(_WIN32) || defined(__CYGWIN__)
#include"fast_io_hosted/box.h"
#endif


namespace fast_io
{

template<std::integral char_type>
using basic_iobuf_io_io_observer = basic_iobuf<basic_io_io_observer<char_type>>;
template<std::integral char_type>
using basic_iobuf_io_io_handle = basic_iobuf<basic_io_io_handle<char_type>>;
template<std::integral char_type>
using basic_iobuf_io_file = basic_iobuf<basic_io_file<char_type>>;
#ifndef _MSC_VER
template<std::integral char_type>
using basic_io_files = basic_ovector<basic_io_file<char_type>>;
using io_files = basic_io_files<char>;
#endif

using iobuf_io_io_observer = basic_iobuf_io_io_observer<char>;
using iobuf_io_io_handle = basic_iobuf_io_io_handle<char>;
using iobuf_io_file = basic_iobuf_io_file<char>;

using wiobuf_io_io_observer = basic_iobuf_io_io_observer<wchar_t>;
using wiobuf_io_io_handle = basic_iobuf_io_io_handle<wchar_t>;
using wiobuf_io_file = basic_iobuf_io_file<wchar_t>;

using u8iobuf_io_io_observer = basic_iobuf_io_io_observer<char8_t>;
using u8iobuf_io_io_handle = basic_iobuf_io_io_handle<char8_t>;
using u8iobuf_io_file = basic_iobuf_io_file<char8_t>;

using u16iobuf_io_io_observer = basic_iobuf_io_io_observer<char16_t>;
using u16iobuf_io_io_handle = basic_iobuf_io_io_handle<char16_t>;
using u16iobuf_io_file = basic_iobuf_io_file<char16_t>;

using u32iobuf_io_io_observer = basic_iobuf_io_io_observer<char32_t>;
using u32iobuf_io_io_handle = basic_iobuf_io_io_handle<char32_t>;
using u32iobuf_io_file = basic_iobuf_io_file<char32_t>;
}

#endif

#endif