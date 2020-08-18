#pragma once


namespace fast_io
{

template<std::integral ch_type,std::size_t extent=std::dynamic_extent,bool override_not_overflow=false>
class ospan
{
public:
	using char_type = ch_type;
	using span_type = std::span<char_type,extent>;
	span_type span;
	using pointer = typename span_type::pointer;
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

template<std::integral char_type,std::size_t extent,bool override_not_overflow>
inline constexpr auto obuffer_begin(ospan<char_type,extent,override_not_overflow>& sp) noexcept
{
	return sp.data();
}

template<std::integral char_type,std::size_t extent,bool override_not_overflow>
inline constexpr auto obuffer_curr(ospan<char_type,extent,override_not_overflow>& sp) noexcept
{
	return sp.current;
}

template<std::integral char_type,std::size_t extent,bool override_not_overflow>
inline constexpr auto obuffer_end(ospan<char_type,extent,override_not_overflow>& sp) noexcept
{
	return std::to_address(sp.span.end());
}

template<std::integral char_type,std::size_t extent,bool override_not_overflow>
inline constexpr auto obuffer_set_curr(ospan<char_type,extent,override_not_overflow>& sp,char_type* ptr) noexcept
{
	sp.current=ptr;
}

template<std::integral char_type,std::size_t extent,bool override_not_overflow>
inline constexpr void overflow(ospan<char_type,extent,override_not_overflow>&,char_type) noexcept{}

template<std::integral char_type,std::size_t extent>
inline constexpr void overflow_never(ospan<char_type,extent,true>&) noexcept{}


template<std::integral char_type,std::size_t extent,bool override_not_overflow,std::contiguous_iterator Iter>
requires (std::same_as<char_type,std::iter_value_t<Iter>>||std::same_as<char,char_type>)
inline constexpr void write(ospan<char_type,extent,override_not_overflow>& ob,Iter cbegin,Iter cend) noexcept
{
	if constexpr(std::same_as<char_type,std::iter_value_t<Iter>>)
	{
		std::size_t sz(cend-cbegin);
		if constexpr(override_not_overflow)
		{
			std::size_t const remain_space(ob.span.size()-ob.size());
			if(remain_space<sz)[[unlikely]]
				sz=remain_space;
		}
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
		std::remove_cvref_t<decltype(std::span(std::forward<Args>(args)...))>::extent,false>;

}
