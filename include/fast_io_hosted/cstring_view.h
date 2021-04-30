#pragma once

namespace fast_io
{
struct null_terminated_t
{
explicit inline constexpr null_terminated_t() noexcept = default;
};
inline constexpr null_terminated_t null_terminated{};

template<typename ch_type,typename tr_type = std::char_traits<ch_type>>
class basic_cstring_view:public ::fast_io::freestanding::basic_string_view<ch_type,tr_type>
{
public:
	using string_view_type = ::fast_io::freestanding::basic_string_view<ch_type,tr_type>;
	using typename ::fast_io::freestanding::basic_string_view<ch_type,tr_type>::traits_type;
	using typename ::fast_io::freestanding::basic_string_view<ch_type,tr_type>::value_type;
	using typename ::fast_io::freestanding::basic_string_view<ch_type,tr_type>::pointer;
	using typename ::fast_io::freestanding::basic_string_view<ch_type,tr_type>::const_pointer;
	using typename ::fast_io::freestanding::basic_string_view<ch_type,tr_type>::reference;
	using typename ::fast_io::freestanding::basic_string_view<ch_type,tr_type>::const_reference;
	using typename ::fast_io::freestanding::basic_string_view<ch_type,tr_type>::const_iterator;
	using typename ::fast_io::freestanding::basic_string_view<ch_type,tr_type>::iterator;
	using typename ::fast_io::freestanding::basic_string_view<ch_type,tr_type>::const_reverse_iterator;
	using typename ::fast_io::freestanding::basic_string_view<ch_type,tr_type>::reverse_iterator;
	using typename ::fast_io::freestanding::basic_string_view<ch_type,tr_type>::size_type;
	using typename ::fast_io::freestanding::basic_string_view<ch_type,tr_type>::difference_type;
	constexpr basic_cstring_view() noexcept=default;
	template<std::size_t len>
	requires (len!=0)
	constexpr basic_cstring_view(ch_type const (&cstr)[len]) noexcept:string_view_type(cstr,len-1)
	{
	}
	template<typename T>
	requires (std::convertible_to<T,const_pointer>&&!std::is_array_v<T>)
	constexpr basic_cstring_view(T const& cstr) noexcept:string_view_type(cstr)
	{
	}
	constexpr basic_cstring_view(null_terminated_t, const_pointer c, size_type len) noexcept:string_view_type(c,len){}
	constexpr basic_cstring_view(null_terminated_t, string_view_type sv) noexcept:string_view_type(sv){}
#if  !defined(_LIBCPP_VERSION)
	template<std::ranges::contiguous_range crg>
	requires (std::same_as<std::ranges::range_value_t<crg>, ch_type>&&requires(crg cont)
	{
		{cont.c_str()}->std::same_as<const_pointer>;
	})
	constexpr basic_cstring_view(crg const& cont):string_view_type(std::ranges::data(cont),std::ranges::size(cont)){}
#endif

      template<::fast_io::freestanding::contiguous_iterator It, std::sized_sentinel_for<It> End>
	requires std::same_as<::fast_io::freestanding::iter_value_t<It>, ch_type>
	  && (!std::convertible_to<End, size_type>)
	constexpr basic_cstring_view(null_terminated_t, It first, End last): string_view_type(first,static_cast<std::size_t>(last-first)){}

	constexpr string_view_type substr(size_type pos, size_type n=::fast_io::freestanding::basic_string_view<ch_type,tr_type>::npos) const = delete;
	constexpr basic_cstring_view substr(size_type pos=0) const = delete;


	constexpr void remove_suffix(size_type n)=delete;

	constexpr const_pointer c_str() const noexcept
	{
		return this->data();
	}
};

using cstring_view = basic_cstring_view<char>;
using wcstring_view = basic_cstring_view<wchar_t>;
using u8cstring_view = basic_cstring_view<char8_t>;
using u16cstring_view = basic_cstring_view<char16_t>;
using u32cstring_view = basic_cstring_view<char32_t>;

#ifdef _WIN32
using native_char_type = wchar_t;
#else
using native_char_type = char;
#endif

template<typename T>
concept constructible_to_path =
	std::constructible_from<cstring_view,T>||
	std::constructible_from<wcstring_view,T>||
	std::constructible_from<u8cstring_view,T>||
	std::constructible_from<u16cstring_view,T>||
	std::constructible_from<u32cstring_view,T>;

}