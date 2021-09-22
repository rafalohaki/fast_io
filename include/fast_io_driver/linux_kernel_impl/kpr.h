#pragma once

namespace fast_io::linux
{
extern int linux_kernel_printk(char const* fmt, ...) noexcept asm("printk");

enum class kern
{
emerg,
alert,
crit,
warning,
notice,
info,
debug,
default_value,
cont
};

template<std::integral ch_type>
struct basic_kpr
{
	using char_type = ch_type;
	kern level{kern::default_value};
};

inline constexpr char8_t const* kern_to_fmt(kern k) noexcept
{
	switch(k)
	{
	case kern::emerg:
		return u8"\0010%.*s";
	case kern::alert:
		return u8"\0011%.*s";
	case kern::crit:
		return u8"\0012%.*s";
	case kern::warning:
		return u8"\0013%.*s";
	case kern::notice:
		return u8"\0014%.*s";
	case kern::info:
		return u8"\0015%.*s";
	case kern::debug:
		return u8"\0016%.*s";
	case kern::cont:
		return u8"\001c%.*s";
	break;
	default:
		return u8"%.*s";
	}
}

namespace details
{
#if __has_cpp_attribute(gnu::cold)
[[gnu::cold]]
#endif
inline void linux_kpr_raw_write(::fast_io::linux::kern k,void const* start,void const* last) noexcept
{
	char const* start_char_ptr{reinterpret_cast<char const*>(start)};
	char const* last_char_ptr{reinterpret_cast<char const*>(last)};

	std::size_t diff{static_cast<std::size_t>(last_char_ptr-start_char_ptr)};

	constexpr int int_max{std::numeric_limits<int>::max()};

	char const* fmt{reinterpret_cast<char const*>(kern_to_fmt(k))};

	if constexpr(int_max>std::numeric_limits<std::size_t>::max())
	{
		int diff_int{static_cast<int>(diff)};
		::fast_io::linux::linux_kernel_printk(fmt,diff_int,start_char_ptr);
	}
	else
	{
		std::size_t i{};
		constexpr std::size_t alignment_size{static_cast<std::size_t>(int_max)};
		for(;i+alignment_size<=diff;i+=alignment_size)
			::fast_io::linux::linux_kernel_printk(fmt,alignment_size,start_char_ptr+i);
		std::size_t const remain_size{static_cast<std::size_t>(diff-i)};
		int const iremain_size{static_cast<int>(remain_size)};
		::fast_io::linux::linux_kernel_printk(fmt,iremain_size,start_char_ptr+i);
	}
}

}

template<std::integral char_type,::fast_io::freestanding::contiguous_iterator Iter>
inline void write(basic_kpr<char_type> kpr,Iter first,Iter last) noexcept
{
	details::linux_kpr_raw_write(kpr.level,::fast_io::freestanding::to_address(first),::fast_io::freestanding::to_address(last));
}


template<std::integral ch_type>
inline constexpr basic_kpr<ch_type> io_value_handle(basic_kpr<ch_type> linuxkpr) noexcept
{
	return linuxkpr;
}

using kpr = basic_kpr<char>;
using wkpr = basic_kpr<wchar_t>;
using u8kpr = basic_kpr<char8_t>;
using u16kpr = basic_kpr<char16_t>;
using u32kpr = basic_kpr<char32_t>;

}
