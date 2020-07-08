#pragma once


namespace fast_io
{

template<std::integral ch_type,std::size_t extend=std::dynamic_extent>
class ospan
{
public:
	using char_type = ch_type;
	using span_type = std::span<char_type,extend>;
	span_type span;
	using pointer = span_type::pointer;
	pointer current;
	template<typename... Args>
	requires std::constructible_from<span_type,Args...>
	constexpr ospan(Args&& ...args):span(std::forward<Args>(args)...),current(span.data()){}
	constexpr auto data() const noexcept
	{
		return span.data();
	}
	constexpr auto data() noexcept
	{
		return span.data();
	}
	constexpr std::size_t size() const noexcept
	{
		return current-span.data();
	}
	constexpr void clear() noexcept {current=span.data();}
};

template<std::integral char_type,std::size_t extend>
inline constexpr auto obuffer_begin(ospan<char_type,extend>& sp) noexcept
{
	return sp.data();
}

template<std::integral char_type,std::size_t extend>
inline constexpr auto obuffer_curr(ospan<char_type,extend>& sp) noexcept
{
	return sp.current;
}

template<std::integral char_type,std::size_t extend>
inline constexpr auto obuffer_end(ospan<char_type,extend>& sp) noexcept
{
	return std::to_address(sp.span.end());
}

template<std::integral char_type,std::size_t extend>
inline constexpr auto obuffer_set_curr(ospan<char_type,extend>& sp,char_type* ptr) noexcept
{
	sp.current=ptr;
}

template<std::integral char_type,std::size_t extend>
inline constexpr auto overflow(ospan<char_type,extend>&,char_type) noexcept
{
	fast_terminate();
}

template<std::integral char_type,std::size_t extend,std::contiguous_iterator Iter>
requires (std::same_as<char_type,std::iter_value_t<Iter>>||std::same_as<char,char_type>)
inline constexpr void write(ospan<char_type,extend>& ob,Iter cbegin,Iter cend) noexcept	//contract : cend-begin + curr <= span's size
{
	if constexpr(std::same_as<char_type,std::iter_value_t<Iter>>)
	{
		std::size_t const sz(cend-cbegin);
		if(ob.span.size()<ob.size()+sz)[[unlikely]]
			fast_terminate();
		details::non_overlapped_copy_n(cbegin,sz,ob.current);
		ob.current+=sz;
	}
	else
	{
		write(ob,reinterpret_cast<char const*>(std::to_address(cbegin)),
			reinterpret_cast<char const*>(std::to_address(cend)));
	}
}

template<typename... Args>
ospan(Args&& ...args) -> ospan<typename std::remove_cvref_t<decltype(std::span(std::forward<Args>(args)...))>::value_type,
		std::remove_cvref_t<decltype(std::span(std::forward<Args>(args)...))>::extent>;

}
