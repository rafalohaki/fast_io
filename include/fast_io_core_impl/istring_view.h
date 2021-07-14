#pragma once

namespace fast_io
{

template<std::integral ch_type>
class basic_istring_view
{
public:
	using char_type = ch_type;
	using string_view_type = ::fast_io::freestanding::basic_string_view<char_type>;
	string_view_type string_view;
	using const_pointer = typename string_view_type::const_pointer;
	const_pointer current;
	template<typename... Args>
	requires std::constructible_from<string_view_type,Args...>
	constexpr basic_istring_view(Args&& ...args):string_view(::fast_io::freestanding::forward<Args>(args)...),current(string_view.data()){}
	constexpr auto data() const noexcept
	{
		return current;
	}
	constexpr auto data() noexcept
	{
		return current;
	}
	constexpr std::size_t size() const noexcept
	{
		return ::fast_io::freestanding::to_address(string_view.end())-current;
	}
	constexpr void clear() noexcept {current=string_view.data();}
};


template<std::integral char_type>
inline constexpr auto ibuffer_begin(basic_istring_view<char_type>& sp) noexcept
{
	return sp.data();
}

template<std::integral char_type>
inline constexpr auto ibuffer_curr(basic_istring_view<char_type>& sp) noexcept
{
	return sp.current;
}

template<std::integral char_type>
inline constexpr auto ibuffer_end(basic_istring_view<char_type>& sp) noexcept
{
	return ::fast_io::freestanding::to_address(sp.string_view.end());
}

template<std::integral char_type>
inline constexpr auto ibuffer_set_curr(basic_istring_view<char_type>& sp,char_type const* ptr) noexcept
{
	sp.current=ptr;
}

template<std::integral char_type>
inline constexpr bool ibuffer_underflow(basic_istring_view<char_type>&) noexcept
{
	return false;
}

template<std::integral char_type>
inline constexpr void ibuffer_underflow_never(basic_istring_view<char_type>&) noexcept{}

template<std::integral char_type,::fast_io::freestanding::contiguous_iterator Iter>
requires (std::same_as<char_type,::fast_io::freestanding::iter_value_t<Iter>>||std::same_as<char,char_type>)
inline constexpr Iter read(basic_istring_view<char_type>& ob,Iter cbegin,Iter cend) noexcept
{
	if constexpr(std::same_as<char_type,::fast_io::freestanding::iter_value_t<Iter>>)
	{
		std::size_t const min_size(::fast_io::freestanding::min(static_cast<std::size_t>(cend-cbegin),
			static_cast<std::size_t>(ob.string_view.size()-ob.size())));
		details::non_overlapped_copy_n(ob.current,min_size,cbegin);
		ob.current+=min_size;
		return cbegin+min_size;
	}
	else
	{
		return read(ob,reinterpret_cast<char*>(::fast_io::freestanding::to_address(cbegin)),
			reinterpret_cast<char*>(::fast_io::freestanding::to_address(cend)));
	}
}

}