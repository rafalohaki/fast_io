﻿#pragma once
#if !(defined(__linux__) && defined(__NR_getrandom)) && __has_include(<sys/random.h>)
#include<sys/random.h>
#endif

namespace fast_io
{

template<std::integral ch_type>
class basic_linux_getrandom
{
public:
	using char_type = ch_type;
	unsigned flags{};
	constexpr void close() noexcept{}
};

template<std::integral char_type>
inline constexpr basic_linux_getrandom<char_type> io_value_handle(basic_linux_getrandom<char_type> brg) noexcept
{
	return brg;	
}

namespace linux
{

namespace details
{
inline std::size_t linux_getrandom_read(void* ptr,std::size_t sz,unsigned flags)
{
#if defined(__linux__) && defined(__NR_getrandom)
	auto ret{system_call<__NR_getrandom,std::ptrdiff_t>(ptr,sz,flags)};
	system_call_throw_error(ret);
#else
	auto ret{noexcept_call(getrandom,ptr,sz,flags)};
	if(ret<0)
		throw_posix_error();
#endif
	return static_cast<std::size_t>(ret);
}
}
}

template<std::integral char_type,::fast_io::freestanding::contiguous_iterator Iter>
inline Iter read(basic_linux_getrandom<char_type> g,Iter bg,Iter ed)
{
	return bg+::fast_io::linux::details::linux_getrandom_read(::fast_io::freestanding::to_address(bg),(ed-bg)*sizeof(*bg),g.flags)/sizeof(*bg);
}

using linux_getrandom = basic_linux_getrandom<char>;

template<std::integral char_type>
inline constexpr void require_secure_clear(basic_linux_getrandom<char_type>){}

}
