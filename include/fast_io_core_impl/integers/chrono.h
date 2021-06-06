#pragma once

/*

Time output should strictly following ISO 8601
2020-11-07T12:50:20Z

*/

namespace fast_io::details
{

template<::fast_io::freestanding::random_access_iterator Iter,my_unsigned_integral U>
inline constexpr void with_length_output_unsigned(Iter it,U u,std::size_t len) noexcept
{
	optimize_size::with_length::output_unsigned(it,u,len);
}

template<bool unchecked=false,::fast_io::freestanding::random_access_iterator Iter,my_unsigned_integral U>
inline constexpr Iter chrono_one_digit_impl(Iter it,U uv) noexcept
{
	if constexpr(!unchecked)
	{
		if(10u<=uv)[[unlikely]]
			return process_integer_output<10,false>(it,uv);
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
#ifdef __OPTIMIZE_SIZE__
	if constexpr(!unchecked)
	{
		if(100u<=u)[[unlikely]]
			return process_integer_output<10,false>(it,u);
	}
	auto v{u/10};
	u%=10;
	using char_type = ::fast_io::freestanding::iter_value_t<Iter>;
	*it=v;
	if constexpr(std::same_as<char_type,char>)
		*it+='0';
	else if constexpr(std::same_as<char_type,wchar_t>)
		*it+=L'0';
	else
		*it+=u8'0';
	++it;
	*it=u;
	if constexpr(std::same_as<char_type,char>)
		*it+='0';
	else if constexpr(std::same_as<char_type,wchar_t>)
		*it+=L'0';
	else
		*it+=u8'0';
	++it;
	return it;
#elif defined(FAST_IO_OPTIMIZE_TIME)
	using char_type = ::fast_io::freestanding::iter_value_t<Iter>;
	if constexpr(!unchecked)
	{
		if(100u<=u)[[unlikely]]
			return process_integer_output<10,false>(it,u);
	}
	return non_overlapped_copy_n(jiaendu::static_tables<char_type>::table2[u].data(),2,it);
#else
	using char_type = ::fast_io::freestanding::iter_value_t<Iter>;
	if constexpr(!unchecked)
	{
		if(100u<=u)[[unlikely]]
			return process_integer_output<10,false>(it,u);
	}
	constexpr auto tb{get_shared_inline_constexpr_base_table<char_type,10,false>().data()};
	return non_overlapped_copy_n(tb[u].data(),2,it);
#endif
}

template<bool unchecked=false,::fast_io::freestanding::random_access_iterator Iter,my_signed_integral I>
inline constexpr Iter chrono_two_digits_impl(Iter it,I i) noexcept
{
	using char_type = ::fast_io::freestanding::iter_value_t<Iter>;
	my_make_unsigned_t<I> u(static_cast<my_make_unsigned_t<I>>(i));
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
	using unsigned_type = std::make_unsigned_t<std::remove_cvref_t<integ>>;
	using char_type = ::fast_io::freestanding::iter_value_t<Iter>;
	unsigned_type u(i);
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
#ifdef __OPTIMIZE_SIZE__
	if(10000u<=u)[[unlikely]]
		return process_integer_output<10,false>(it,u);
	optimize_size::with_length::output_unsigned(it,u,4);
	return it+=4;
#elif defined(FAST_IO_OPTIMIZE_TIME)
	if(10000u<=u)[[unlikely]]
		return process_integer_output<10,false>(it,u);
	return non_overlapped_copy_n(jiaendu::static_tables<char_type>::table4[u].data(),4,it);
#else
	if(10000u<=u)[[unlikely]]
		return process_integer_output<10,false>(it,u);
	unsigned_type dv(u/100u);
	unsigned_type rmd(u%100u);
	constexpr auto tb{get_shared_inline_constexpr_base_table<char_type,10,false>().data()};
	it=non_overlapped_copy_n(tb[dv].data(),2,it);
	return non_overlapped_copy_n(tb[rmd].data(),2,it);
#endif
}


}