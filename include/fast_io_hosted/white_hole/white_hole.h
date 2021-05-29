#pragma once
#if defined(_WIN32) || defined(__CYGWIN__)
#include"rtl_gen_random.h"
#endif
#include"posix_dev_urandom.h"

namespace fast_io
{

template<std::integral char_type>
using basic_white_hole =
#ifdef _WIN32
basic_rtl_gen_random<char_type>;
#else
posix_dev_urandom<basic_native_file<char_type>>;
#endif

#if 0
template<std::integral char_type>
using basic_ibuf_white_hole = basic_ibuf<basic_white_hole<char_type>>;
using white_hole = basic_white_hole<char>;
using ibuf_white_hole = basic_ibuf_white_hole<char>;
#endif
}