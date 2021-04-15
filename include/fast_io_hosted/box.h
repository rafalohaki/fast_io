#pragma once

#if defined(_MSC_VER)
#pragma comment(lib,"User32.lib")
#endif

namespace fast_io
{

template<std::integral ch_type>
struct basic_win32_box_t
{
	using char_type = ch_type;
	explicit constexpr basic_win32_box_t() noexcept = default;
};

using win32_box_t = basic_win32_box_t<char>;
using wwin32_box_t = basic_win32_box_t<wchar_t>;
using u8win32_box_t = basic_win32_box_t<char8_t>;
using u16win32_box_t = basic_win32_box_t<char16_t>;
using u32win32_box_t = basic_win32_box_t<char32_t>;

namespace details
{

inline void win32_box_write_impl(char16_t* first,char16_t* last)
{
	*std::remove(first,last,0)=0;
	using wchar_t_may_alias_ptr
#if __has_cpp_attribute(gnu::may_alias)
	[[gnu::may_alias]]
#endif
	= wchar_t const*;
	if(!::fast_io::win32::MessageBoxW(nullptr,
		reinterpret_cast<wchar_t_may_alias_ptr>(first),
		reinterpret_cast<wchar_t_may_alias_ptr>(u"fast_io"),
		0x00000040L /*MB_ICONINFORMATION*/))
		throw_win32_error();
}

inline void win32_box_raw_path_impl(char16_t const* first,char16_t const* last)
{
	std::size_t const array_size{static_cast<std::size_t>(last-first)};
	local_operator_new_array_ptr<char16_t> ptr(intrinsics::add_or_overflow_die(array_size,1));//+1 overflow can never happen intrinsics::add_or_overflow_die(array_size,1));
	non_overlapped_copy(first,last,ptr.ptr);
	win32_box_write_impl(ptr.ptr,ptr.ptr+array_size);
}

template<std::integral char_type>
inline void win32_box_dealer_path_impl(char_type const* first,char_type const* last)
{
	win32_path_dealer dealer(first,static_cast<std::size_t>(last-first));
	win32_box_write_impl(dealer.buffer_data,dealer.buffer_data_end);
}

}

template<std::integral char_type,::fast_io::freestanding::contiguous_iterator Iter>
requires std::same_as<char_type,::fast_io::freestanding::iter_value_t<Iter>>
inline void write(basic_win32_box_t<char_type>,Iter first,Iter last)
{
	if constexpr(std::same_as<char_type,wchar_t>)
	{
		using char16_t_may_alias_ptr
#if __has_cpp_attribute(gnu::may_alias)
		[[gnu::may_alias]]
#endif
		= char16_t const*;
		details::win32_box_raw_path_impl(
			reinterpret_cast<char16_t_may_alias_ptr>(::fast_io::freestanding::to_address(first)),
			reinterpret_cast<char16_t_may_alias_ptr>(::fast_io::freestanding::to_address(last)));
	}
	else if constexpr(std::same_as<char_type,char16_t>)
	{
		details::win32_box_raw_path_impl(::fast_io::freestanding::to_address(first),::fast_io::freestanding::to_address(last));
	}
	else
		details::win32_box_dealer_path_impl(::fast_io::freestanding::to_address(first),::fast_io::freestanding::to_address(last));
}

}