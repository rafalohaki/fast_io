#pragma once

namespace fast_io
{

template<std::integral ch_type>
class basic_rtl_gen_random
{
public:
	using char_type = ch_type;
};

namespace details
{

extern "C" int __stdcall SystemFunction036(void*,std::uint32_t);
inline std::size_t rtl_gen_random_read(void* ptr,std::size_t sz)
{
	if constexpr(sizeof(std::uint32_t)<sizeof(std::size_t))
	{
	if(static_cast<std::size_t>(std::numeric_limits<std::uint32_t>::max())<sz)
		sz=static_cast<std::size_t>(std::numeric_limits<std::uint32_t>::max());
	}
	if(!SystemFunction036(ptr,static_cast<std::uint32_t>(sz)))
		throw_win32_error();
	return sz;
}

}

template<std::integral char_type,std::contiguous_iterator Iter>
inline Iter read(basic_rtl_gen_random<char_type>,Iter bg,Iter ed)
{
	return bg+details::rtl_gen_random_read(std::to_address(bg),(std::to_address(ed)-std::to_address(bg))*sizeof(*bg))/sizeof(*bg);
}

using rtl_gen_random = basic_rtl_gen_random<char>;

template<std::integral char_type>
inline constexpr void require_secure_clear(basic_rtl_gen_random<char_type>){}

}