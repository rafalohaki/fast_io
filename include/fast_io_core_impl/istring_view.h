#pragma once

namespace fast_io
{

template<std::integral ch_type>
class istring_view
{
public:
	using char_type = ch_type;
	using string_view_type = std::basic_string_view<char_type>;
	string_view_type string_view;
	using const_pointer = typename string_view_type::const_pointer;
	const_pointer current;
	template<typename... Args>
	requires std::constructible_from<string_view_type,Args...>
	constexpr istring_view(Args&& ...args):string_view(std::forward<Args>(args)...),current(string_view.data()){}
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
		return std::to_address(string_view.end())-current;
	}
	constexpr void clear() noexcept {current=string_view.data();}
};


template<std::integral char_type>
inline constexpr auto ibuffer_begin(istring_view<char_type>& sp) noexcept
{
	return sp.data();
}

template<std::integral char_type>
inline constexpr auto ibuffer_curr(istring_view<char_type>& sp) noexcept
{
	return sp.current;
}

template<std::integral char_type>
inline constexpr auto ibuffer_end(istring_view<char_type>& sp) noexcept
{
	return std::to_address(sp.string_view.end());
}

template<std::integral char_type>
inline constexpr auto ibuffer_set_curr(istring_view<char_type>& sp,char_type const* ptr) noexcept
{
	sp.current=ptr;
}

template<std::integral char_type>
inline constexpr bool underflow(istring_view<char_type>& sp) noexcept
{
	return false;
}

template<std::integral char_type>
inline constexpr void underflow_forever_false(istring_view<char_type>&) noexcept{}

template<std::integral char_type,std::contiguous_iterator Iter>
requires (std::same_as<char_type,std::iter_value_t<Iter>>||std::same_as<char,char_type>)
inline constexpr Iter read(istring_view<char_type>& ob,Iter cbegin,Iter cend) noexcept
{
	if constexpr(std::same_as<char_type,std::iter_value_t<Iter>>)
	{
		std::size_t const min_size(std::min(cend-cbegin,ob.string_view.size()-ob.size()));
		details::non_overlapped_copy_n(ob.current,min_size,cbegin);
		ob.current+=min_size;
		return cbegin+min_size;
	}
	else
	{
		return read(ob,reinterpret_cast<char*>(std::to_address(cbegin)),
			reinterpret_cast<char*>(std::to_address(cend)));
	}
}

template<typename... Args>
istring_view(Args&& ...args) -> istring_view<typename std::remove_cvref_t<decltype(std::string_view(std::forward<Args>(args)...))>::value_type>;

static_assert(buffer_input_stream<istring_view<char>>);

}