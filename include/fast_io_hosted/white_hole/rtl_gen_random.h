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
inline std::size_t scatter_read(basic_rtl_gen_random<char_type>,io_scatters_t sp)
{
	std::size_t total_bytes{};
	for(std::size_t i{};i!=sp.len;++i)
	{
		win32::details::rtl_gen_random_read(const_cast<void*>(sp.base[i].base),sp.base[i].len);
		total_bytes+=sp.base[i].len;
	}
	return total_bytes;
}
using rtl_gen_random = basic_rtl_gen_random<char>;

template<std::integral char_type>
inline constexpr void require_secure_clear(basic_rtl_gen_random<char_type>){}

}