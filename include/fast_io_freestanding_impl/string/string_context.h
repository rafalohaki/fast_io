#pragma once

namespace fast_io
{

namespace details::ctx_scan_string
{

template<std::forward_iterator Iter>
inline constexpr Iter find_c_space(Iter first, Iter last)
{
	for(;first!=last&&!::fast_io::char_category::is_c_space(*first);++first);
	return first;
}

template<bool contiguous_only, std::forward_iterator Iter>
struct voldmort
{
	Iter iter;
	std::errc code;
	inline constexpr bool test_eof(parameter<std::basic_string<std::iter_value_t<Iter>>&> t) noexcept requires(!contiguous_only)
	{
		code={};
		return t.reference.empty();
	}
	inline constexpr void operator()(Iter begin, Iter end,parameter<std::basic_string<std::iter_value_t<Iter>>&> t) requires(!contiguous_only)
	{
		iter=find_c_space(begin,end);
		t.reference.append(begin,iter);
		if(iter!=end)[[likely]]
			code={};
	}
	inline constexpr voldmort(Iter begin, Iter end, parameter<std::basic_string<std::iter_value_t<Iter>>&> t):iter(find_c_space(begin,end))
	{
		if constexpr(contiguous_only)
		{
			code={};
			t.reference.assign(begin,iter);
		}
		else
		{
			t.reference.assign(begin,iter);
			code={};
			if(iter==end)
				code=std::errc::resource_unavailable_try_again;
		}
	}
};

}

template<details::my_integral T, std::forward_iterator Iter>
inline constexpr Iter scan_skip_define(scan_skip_type_t<parameter<std::basic_string<std::iter_value_t<Iter>>&>>, Iter beg, Iter ed) noexcept
{
	return scan_skip_space(beg, ed);
}

template<bool contiguous_only, std::forward_iterator Iter>
inline constexpr auto scan_context_define(scan_context_t<contiguous_only>, Iter begin, Iter end, parameter<std::basic_string<std::iter_value_t<Iter>>&> t)
{
	return details::ctx_scan_string::voldmort<contiguous_only, Iter>(begin, end, t);
}

}