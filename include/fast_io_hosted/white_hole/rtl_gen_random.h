#pragma once

namespace fast_io
{

template<std::integral ch_type>
class basic_rtl_gen_random
{
public:
	using char_type = ch_type;
	constexpr void close() noexcept{}
};

namespace win32
{

namespace details
{
inline void rtl_gen_random_read(void* ptr,std::size_t sz)
{
	if constexpr(sizeof(std::uint32_t)<sizeof(std::size_t))
	{
		for(;sz;)
		{
			constexpr std::size_t uint32_max{static_cast<std::size_t>(UINT32_MAX)};
			std::size_t mn{sz};
			if(uint32_max<sz)
				mn=uint32_max;
			if(!SystemFunction036(ptr,static_cast<std::uint32_t>(mn)))
				throw_win32_error();
			sz-=mn;
		}
	}
	else
	{
		if(!SystemFunction036(ptr,static_cast<std::uint32_t>(sz)))
			throw_win32_error();
	}
}
}
}

template<std::integral char_type,::fast_io::freestanding::contiguous_iterator Iter>
inline Iter read(basic_rtl_gen_random<char_type>,Iter bg,Iter ed)
{
	win32::details::rtl_gen_random_read(::fast_io::freestanding::to_address(bg),(ed-bg)*sizeof(*bg));
	return ed;
}

template<std::integral char_type>
inline constexpr basic_rtl_gen_random<char_type> io_value_handle(basic_rtl_gen_random<char_type> brg) noexcept
{
	return brg;	
}

using rtl_gen_random = basic_rtl_gen_random<char>;

template<std::integral char_type>
inline constexpr void require_secure_clear(basic_rtl_gen_random<char_type>){}

}