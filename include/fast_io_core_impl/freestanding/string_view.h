#pragma once

#if __STDC_HOSTED__==1 && (!defined(_GLIBCXX_HOSTED) || _GLIBCXX_HOSTED==1)
#include<string_view>
namespace fast_io::freestanding
{

using ::std::char_traits;
using ::std::basic_string_view;
using ::std::string_view;
using ::std::wstring_view;
using ::std::u8string_view;
using ::std::u16string_view;
using ::std::u32string_view;

}

#else

namespace fast_io::freestanding
{

/*
dummy char_traits which do not do anything
*/
template<typename T>
struct char_traits
{};

template<std::integral CharT,typename traits=char_traits<CharT>>
class basic_string_view
{
public:
	using char_type = CharT;
	using size_type = std::size_t;
	using ptrdiff_t = std::ptrdiff_t;
	using pointer = CharT*;
	using const_pointer = CharT const*;
	using iterator = CharT*;
	using const_iterator = CharT const*;
	char_type const* ptr{};
	size_type n{};
	constexpr basic_string_view() noexcept=default;
	constexpr basic_string_view(basic_string_view const& other) noexcept = default;
	constexpr basic_string_view(char_type const* s, size_type count):ptr(s),n(count){}
	constexpr basic_string_view(char_type const* s):ptr(s),n(::fast_io::cstr_len(s)){}
	template<contiguous_iterator Iter>
	requires std::same_as<iter_value_t<Iter>,char>
	constexpr basic_string_view(Iter first, Iter last):ptr(to_address(first)),n(static_cast<size_type>(last-first)){}
	constexpr basic_string_view& operator=(basic_string_view const& view) noexcept = default;
	constexpr const_pointer data() const noexcept
	{
		return ptr;
	}
	constexpr const_iterator begin() const noexcept
	{
		return ptr;
	}
	constexpr const_iterator cbegin() const noexcept
	{
		return ptr;
	}
	constexpr const_iterator end() const noexcept
	{
		return ptr+n;
	}
	constexpr const_iterator cend() const noexcept
	{
		return ptr+n;
	}
	constexpr size_type size() const noexcept
	{
		return n;
	}
	inline static constexpr std::size_t npos = -1;
};

using string_view = basic_string_view<char>;
using wstring_view = basic_string_view<wchar_t>;
using u8string_view = basic_string_view<char8_t>;
using u16string_view = basic_string_view<char16_t>;
using u32string_view = basic_string_view<char32_t>;

}

#endif