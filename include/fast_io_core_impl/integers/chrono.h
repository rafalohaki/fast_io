#pragma once

/*

Time output should strictly following ISO 8601
2020-11-07T12:50:20Z

*/

namespace fast_io::details
{

template<bool unchecked=false,::fast_io::freestanding::random_access_iterator Iter,my_unsigned_integral U>
inline constexpr Iter chrono_one_digit_impl(Iter it,U uv) noexcept
{
	if constexpr(!unchecked)
	{
		if(10u<=uv)[[unlikely]]
			return print_reserve_integral_define<10>(it,uv);
	}
	using char_type = ::fast_io::freestanding::iter_value_t<Iter>;
	if constexpr(std::same_as<char,char_type>)
		*it=static_cast<char_type>(uv+'0');
	else if constexpr(std::same_as<wchar_t,char_type>)
		*it=static_cast<char_type>(uv+L'0');
	else
		*it=static_cast<char_type>(uv+u8'0');
	++it;
	return it;
}

template<bool unchecked=false,::fast_io::freestanding::random_access_iterator Iter,my_unsigned_integral U>
inline constexpr Iter chrono_two_digits_impl(Iter it,U u) noexcept
{
	using char_type = ::fast_io::freestanding::iter_value_t<Iter>;
	if constexpr(!unchecked)
	{
		if(100u<=u)[[unlikely]]
			return print_reserve_integral_define<10>(it,u);
	}
	constexpr auto tb{get_shared_inline_constexpr_base_table<char_type,10,false>().element};
	return non_overlapped_copy_n(tb[u].element,2,it);
}

template<bool unchecked=false,::fast_io::freestanding::random_access_iterator Iter,my_signed_integral I>
inline constexpr Iter chrono_two_digits_impl(Iter it,I i) noexcept
{
	using char_type = ::fast_io::freestanding::iter_value_t<Iter>;
	my_make_unsigned_t<I> u{static_cast<my_make_unsigned_t<I>>(i)};
	if(i<0)[[unlikely]]
	{
		u = 0u - u;
		if constexpr(std::same_as<char_type,char>)
			*it='-';
		else if constexpr(std::same_as<char_type,wchar_t>)
			*it=L'-';
		else
			*it=u8'-';
		++it;
	}
	return chrono_two_digits_impl<unchecked>(it,u);
}

template<::fast_io::freestanding::random_access_iterator Iter,std::signed_integral integ>
inline constexpr Iter chrono_year_impl(Iter it,integ i) noexcept
{
	using unsigned_type = my_make_unsigned_t<std::remove_cvref_t<integ>>;
	using char_type = ::fast_io::freestanding::iter_value_t<Iter>;
	unsigned_type u{static_cast<unsigned_type>(i)};
	if(i<0)[[unlikely]]
	{
		u = 0u - u;
		if constexpr(std::same_as<char_type,char>)
			*it='-';
		else if constexpr(std::same_as<char_type,wchar_t>)
			*it=L'-';
		else
			*it=u8'-';
		++it;
	}
	if(10000u<=u)[[unlikely]]
		return print_reserve_integral_define<10>(it,u);
	unsigned_type dv(u/100u);
	unsigned_type rmd(u%100u);
	constexpr auto tb{get_shared_inline_constexpr_base_table<char_type,10,false>().element};
	it=non_overlapped_copy_n(tb[dv].element,2,it);
	return non_overlapped_copy_n(tb[rmd].element,2,it);
}


}