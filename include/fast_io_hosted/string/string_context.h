#pragma once

namespace fast_io
{

namespace details::ctx_scan_string
{

template<::fast_io::freestanding::forward_iterator Iter>
inline constexpr Iter find_c_space(Iter first, Iter last)
{
	for(;first!=last&&!::fast_io::char_category::is_c_space(*first);++first);
	return first;
}

template<bool contiguous_only, typename T, ::fast_io::freestanding::forward_iterator Iter>
struct voldmort
{
	Iter iter;
	parse_code code;
	inline constexpr bool test_eof(parameter<T&> t) noexcept requires(!contiguous_only)
	{
		code=parse_code::ok;
		return !t.reference.empty();
	}
	inline constexpr void operator()(Iter begin, Iter end,parameter<T&> t) requires(!contiguous_only)
	{
		iter=find_c_space(begin,end);
		t.reference.append(begin,iter);
		if(iter!=end)[[likely]]
			code=parse_code::ok;
	}
	inline constexpr voldmort(Iter begin, Iter end, parameter<T&> t):iter(find_c_space(begin,end))
	{
		if constexpr(contiguous_only)
		{
			code=parse_code::ok;
			t.reference.assign(begin,iter);
		}
		else
		{
			t.reference.assign(begin,iter);
			code=parse_code::ok;
			if(iter==end)
				code=parse_code::partial;
		}
	}
};

}

template<::fast_io::freestanding::forward_iterator Iter,typename traits_type,typename allocator_type>
inline constexpr Iter scan_skip_define(
	scan_skip_type_t<parameter<std::basic_string<::fast_io::freestanding::iter_value_t<Iter>,traits_type,allocator_type>&>>,
	Iter beg, Iter ed) noexcept
{
	return scan_skip_space(beg, ed);
}

template<bool contiguous_only, ::fast_io::freestanding::forward_iterator Iter,typename traits_type,typename allocator_type>
inline constexpr auto scan_context_define(
	scan_context_t<contiguous_only>, Iter begin, Iter end,
	parameter<std::basic_string<::fast_io::freestanding::iter_value_t<Iter>,traits_type,allocator_type>&> t)
{
	return details::ctx_scan_string::voldmort<contiguous_only,std::basic_string<::fast_io::freestanding::iter_value_t<Iter>,traits_type,allocator_type>, Iter>(begin, end, t);
}


namespace manipulators
{

template<typename T>
struct line_get_t
{
	using manip_tag = manip_tag_t;
	T reference;
};

template<typename T,std::integral ch_type>
struct line_get_ch_t
{
	using manip_tag = manip_tag_t;
	using char_type = ch_type;
	using element_type = T;
	T reference;
	char_type ch;
};

template<typename T,std::integral ch_type>
struct line_get_strvw_t
{
	using manip_tag = manip_tag_t;
	using char_type = ch_type;
	using string_view_type = ::fast_io::freestanding::basic_string_view<ch_type>;
	using element_type = T;
	T reference;
	string_view_type strvw;
};

template<typename T>
inline constexpr line_get_t<T&> line_get(T& line_str) noexcept
{
	return {line_str};
}

template<typename T>
inline constexpr line_get_ch_t<T&,typename T::value_type> line_get(T& line_str,typename T::value_type ch) noexcept
{
	return {line_str,ch};
}

template<typename T>
inline constexpr line_get_strvw_t<T&,typename T::value_type> line_get(T& line_str,::fast_io::freestanding::basic_string_view<typename T::value_type> strvw) noexcept
{
	return {line_str,strvw};
}


}

namespace details::ctx_scan_line_get
{

template<::fast_io::freestanding::forward_iterator Iter>
inline constexpr Iter find_lf(Iter first, Iter last)
{
	using char_type = ::fast_io::freestanding::iter_value_t<Iter>;
	if constexpr(std::same_as<char_type,char>)
		return ::fast_io::freestanding::find(first,last,'\n');
	else if constexpr(std::same_as<char_type,wchar_t>)
		return ::fast_io::freestanding::find(first,last,L'\n');
	else if constexpr(std::same_as<char_type,char16_t>)
		return ::fast_io::freestanding::find(first,last,u'\n');
	else if constexpr(std::same_as<char_type,char32_t>)
		return ::fast_io::freestanding::find(first,last,U'\n');
	else
		return ::fast_io::freestanding::find(first,last,u8'\n');
}

template<bool contiguous_only,bool dynamic_ch,typename T, ::fast_io::freestanding::forward_iterator Iter>
struct voldmort
{
	Iter iter;
	parse_code code=parse_code::ok;
#if __has_cpp_attribute(no_unique_address)
[[no_unique_address]]
#endif
	std::conditional_t<contiguous_only,empty,bool> not_empty_str{};
	inline constexpr bool test_eof([[maybe_unused]]T) noexcept requires(!contiguous_only)
	{
		code={};
		return not_empty_str;
	}
	inline constexpr void operator()(Iter begin, Iter end,T t) requires(!contiguous_only)
	{
		using char_type = ::fast_io::freestanding::iter_value_t<Iter>;
		if(not_empty_str)
		{
			if constexpr(dynamic_ch)
			{
				iter=::fast_io::freestanding::find(begin,end,t.ch);
			}
			else
			{
				iter=find_lf(begin,end);
			}
			t.reference.append(begin,iter);
		}
		else
		{
			if constexpr(dynamic_ch)
			{
				if(*begin==t.ch)
					++begin;
			}
			else
			{
				if constexpr(std::same_as<char_type,char>)
				{
					if(*begin=='\n')
						++begin;
				}
				else if constexpr(std::same_as<char_type,wchar_t>)
				{
					if(*begin==L'\n')
						++begin;
				}
				else
				{
					if(*begin==u8'\n')
						++begin;
				}
			}
			if constexpr(dynamic_ch)
			{
				iter=::fast_io::freestanding::find(begin,end,t.ch);
			}
			else
			{
				iter=find_lf(begin,end);
			}
			t.reference.assign(begin,iter);
			not_empty_str=true;
		}
		if(iter!=end)[[likely]]
			code={};
	}
	inline constexpr voldmort(Iter begin, Iter end, T t)
	{
		if(begin==end)
		{
			iter=begin;
			code=parse_code::partial;
			return;
		}
		using char_type = ::fast_io::freestanding::iter_value_t<Iter>;
		if constexpr(std::same_as<char_type,char>)
		{
			if(*begin=='\n')
				++begin;
		}
		else if constexpr(std::same_as<char_type,wchar_t>)
		{
			if(*begin==L'\n')
				++begin;
		}
		else
		{
			if(*begin==u8'\n')
				++begin;
		}
		if constexpr(dynamic_ch)
		{
			iter=::fast_io::freestanding::find(begin,end,t.ch);
		}
		else
		{
			iter=find_lf(begin,end);
		}
		t.reference.assign(begin,iter);
		code={};
		if constexpr(!contiguous_only)
		{
			not_empty_str=true;
			if(iter==end)
				code=parse_code::partial;
		}
	}
};

}

template<bool contiguous_only, ::fast_io::freestanding::forward_iterator Iter,typename traits_type,typename allocator_type>
inline constexpr auto scan_context_define(scan_context_t<contiguous_only>, Iter begin, Iter end,
	manipulators::line_get_t<std::basic_string<::fast_io::freestanding::iter_value_t<Iter>,traits_type,allocator_type>&> t)
{
	return details::ctx_scan_line_get::voldmort<contiguous_only,
		false,
		manipulators::line_get_t<std::basic_string<::fast_io::freestanding::iter_value_t<Iter>,traits_type,allocator_type>&>,
		Iter>(begin, end, t);
}

template<bool contiguous_only, ::fast_io::freestanding::forward_iterator Iter,typename traits_type,typename allocator_type>
inline constexpr auto scan_context_define(scan_context_t<contiguous_only>, Iter begin, Iter end,
	manipulators::line_get_ch_t<std::basic_string<::fast_io::freestanding::iter_value_t<Iter>,traits_type,allocator_type>&,::fast_io::freestanding::iter_value_t<Iter>> t)
{
	return details::ctx_scan_line_get::voldmort<contiguous_only,
		true,
		manipulators::line_get_ch_t<std::basic_string<::fast_io::freestanding::iter_value_t<Iter>,traits_type,allocator_type>&,::fast_io::freestanding::iter_value_t<Iter>>,
		Iter>(begin, end, t);
}


}