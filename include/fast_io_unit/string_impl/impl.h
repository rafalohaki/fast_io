#pragma once

#if defined(_LIBCPP_VERSION)
#include"libc++.h"
#elif defined(__GLIBCXX__)
#include"libstdc++.h"
#elif defined(_MSVC_STL_UPDATE)
#include"msvc_stl.h"
#else
#error "Anyone knows how to hack brutal std::basic_string internal on this platform?"
#endif

#include"ostring_ref.h"
#include"string_context.h"
#include"concat.h"

namespace fast_io
{
template<std::integral char_type>
inline constexpr basic_io_scatter_t<char_type> print_alias_define(io_alias_t,std::basic_string<char_type> const& str)
{
	return {str.data(),str.size()};
}

}
