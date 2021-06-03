#pragma once

namespace fast_io
{

namespace details
{

template<char8_t ch,std::integral char_type>
constexpr char_type get_char_with_type() noexcept
{
	if constexpr(ch==u8'+')
	{
		if constexpr(std::same_as<std::remove_cvref_t<char_type>,char>)
			return '+';
		else if constexpr(std::same_as<std::remove_cvref_t<char_type>,wchar_t>)
			return L'+';
		else
			return u8'+';
	}
	else if constexpr(ch==u8'-')
	{
		if constexpr(std::same_as<std::remove_cvref_t<char_type>,char>)
			return '-';
		else if constexpr(std::same_as<std::remove_cvref_t<char_type>,wchar_t>)
			return L'-';
		else
			return u8'-';
	}
	else if constexpr(ch==u8'0')
	{
		if constexpr(std::same_as<std::remove_cvref_t<char_type>,char>)
			return '0';
		else if constexpr(std::same_as<std::remove_cvref_t<char_type>,wchar_t>)
			return L'0';
		else
			return u8'0';
	}
}

template<char8_t base,::fast_io::freestanding::random_access_iterator Iter,my_integral T>
inline constexpr parse_result<Iter> scan_int_contiguous_none_space_part_define_impl(Iter first,Iter last,T& t) noexcept
{
	using char_type = ::fast_io::freestanding::iter_value_t<Iter>;
	using unsigned_char_type = std::make_unsigned_t<char_type>;
	using unsigned_type = my_make_unsigned_t<std::remove_cvref_t<T>>;
	constexpr unsigned_char_type base_char_type{base};
	bool sign{};
	if constexpr(my_signed_integral<T>)
	{
		if(first==last)
			return {first,parse_code::invalid};
		constexpr auto plus_sign{get_char_with_type<u8'+',char_type>()};
		constexpr auto minus_sign{get_char_with_type<u8'-',char_type>()};
		if((sign=(minus_sign==*first))|(plus_sign==*first))
			++first;
	}
	unsigned_type res{};
	auto first_saved_point{first};
	bool overflow{};
	for(;first!=last;++first)
	{
		unsigned_char_type ch{static_cast<unsigned_char_type>(*first)};
		if(char_digit_to_literal<base,char_type>(ch))[[unlikely]]
			break;
		overflow|=__builtin_mul_overflow(res,base_char_type,__builtin_addressof(res));
		overflow|=__builtin_add_overflow(res,ch,__builtin_addressof(res));
	}
	if(first==first_saved_point)
	{
		return {first,parse_code::invalid};
	}
	if(overflow)
		return {first,parse_code::overflow};
	if constexpr(my_signed_integral<T>)
	{
		constexpr unsigned_type umax{static_cast<unsigned_type>(-1)};
		constexpr unsigned_type imax{umax>>1};
		if(res>(static_cast<my_make_unsigned_t<T>>(imax)+sign))
			return {first,parse_code::overflow};
		if(sign)
			t=static_cast<T>(static_cast<unsigned_type>(0)-res);
		else
			t=static_cast<T>(res);
	}
	else
		t=res;
	return {first,parse_code::ok};
}

template<char8_t base,::fast_io::freestanding::random_access_iterator Iter,details::my_integral T>
inline constexpr parse_result<Iter> scan_int_contiguous_define_impl(Iter first,Iter last,T& t) noexcept
{
	for(;first!=last&&::fast_io::char_category::is_c_space(*first);++first);
	if(first==last)
		return {first,parse_code::invalid};
	return scan_int_contiguous_none_space_part_define_impl<base>(first,last,t);
}
}

template<::fast_io::freestanding::random_access_iterator Iter,details::my_integral T>
inline constexpr parse_result<Iter> scan_contiguous_define(io_reserve_type_t<::fast_io::freestanding::iter_value_t<Iter>,parameter<T&>>,Iter begin,Iter end,parameter<T&> t) noexcept
{
	if constexpr(std::contiguous_iterator<Iter>&&!std::is_pointer_v<Iter>)
	{
		auto [it,ec] = details::scan_int_contiguous_define_impl<10>(::fast_io::freestanding::to_address(begin),::fast_io::freestanding::to_address(end),t.reference);
		return {it-::fast_io::freestanding::to_address(begin)+begin,ec};
	}
	else
		return details::scan_int_contiguous_define_impl<10>(begin,end,t.reference);
}

enum class scan_integral_context_phase:char8_t
{
space,
sign,
zero,
digit,
overflow
};

template<std::integral char_type,details::my_integral T>
inline constexpr auto scan_context_type(io_reserve_type_t<char_type,parameter<T&>>) noexcept
{
	using unsigned_type = details::my_make_unsigned_t<std::remove_cvref_t<T>>;
	constexpr std::size_t max_size{details::cal_max_int_size<unsigned_type,10>()+2};
	struct scan_integer_context
	{
		::fast_io::freestanding::array<char_type,max_size> buffer;
		char8_t size{};
		scan_integral_context_phase integer_phase{};
		inline constexpr void reset() noexcept
		{
			size=0;
			integer_phase=scan_integral_context_phase::space;
		}
	};
	return io_type_t<scan_integer_context>{};
}

namespace details
{


template<char8_t base,::fast_io::freestanding::random_access_iterator Iter>
inline constexpr Iter scan_skip_all_digits_impl(Iter first,Iter last)
{
	using char_type = ::fast_io::freestanding::iter_value_t<Iter>;
	for(;first!=last&&char_digit_probe_overflow<base,char_type>(*first);++first);
	return first;
}

template<char8_t base,typename State,::fast_io::freestanding::random_access_iterator Iter,my_integral T>
inline constexpr parse_result<Iter> scan_context_define_parse_impl(State& st,Iter first_start,Iter last,T& t) noexcept
{
	using char_type = ::fast_io::freestanding::iter_value_t<Iter>;
	auto first{first_start};
	auto phase{st.integer_phase};
	switch(phase)
	{
	case scan_integral_context_phase::space:
	{
		for(;first!=last&&::fast_io::char_category::is_c_space(*first);++first);
		[[fallthrough]];
	}
	case scan_integral_context_phase::sign:
	{
		if constexpr(my_signed_integral<T>)
		{
			constexpr auto plus{get_char_with_type<u8'+',char_type>()};
			constexpr auto minus{get_char_with_type<u8'-',char_type>()};
			if(first==last)
			{
				st.integer_phase=scan_integral_context_phase::space;
				return {first,parse_code::partial};
			}
			if((*first==plus)|(*first==minus))
			{
				*st.buffer.data()=*first;
				st.size=1;
				++first;
				phase=scan_integral_context_phase::sign;
			}
		}
		[[fallthrough]];
	}
	case scan_integral_context_phase::zero:
	{
		constexpr auto zero{get_char_with_type<u8'0',char_type>()};
		auto first_b{first};
		for(;first!=last&&*first==zero;++first);
		if(first_b==first)
		{
			if(first==last)
			{
				st.integer_phase=scan_integral_context_phase::sign;
				return {first,parse_code::partial};
			}
			else if(!char_digit_probe_overflow<base,char_type>(*first))
				return {first,parse_code::invalid};
		}
		[[fallthrough]];
	}
	case scan_integral_context_phase::digit:
	{
		auto it{scan_skip_all_digits_impl<base>(first,last)};
		auto diff{st.buffer.size()-st.size};
		std::size_t const first_it_diff{static_cast<std::size_t>(it-first)};
		if(first_it_diff<diff)
		{
			auto start{st.buffer.data()+st.size};
			auto e{non_overlapped_copy_n(first,first_it_diff,start)};
			st.size+=static_cast<char8_t>(first_it_diff);
			if(it==last)
				return {it,parse_code::partial};
			auto [p,ec]=scan_int_contiguous_none_space_part_define_impl<base>(st.buffer.data(),e,t);
			return {p-start+it,ec};
		}
		else
		{
			if(it==last)
			{
				st.integer_phase=scan_integral_context_phase::overflow;
				return {it,parse_code::partial};
			}
			else
				return {it,parse_code::overflow};
		}
	}
	default:
	{
		first=scan_skip_all_digits_impl<base>(first,last);
		return {first,(first==last)?parse_code::partial:parse_code::invalid};
	}
	}
}

template<char8_t base,typename State,my_integral T>
[[gnu::cold]]
inline constexpr parse_code scan_context_eof_define_parse_impl(State& st,T& t) noexcept
{
	switch(st.integer_phase)
	{
	case scan_integral_context_phase::digit:
		return scan_int_contiguous_none_space_part_define_impl<base>(st.buffer.data(),st.buffer.data()+st.size,t).code;
	case scan_integral_context_phase::overflow:
		return parse_code::overflow;
	default:
		return parse_code::invalid;
	}
}

}

template<::fast_io::freestanding::random_access_iterator Iter,typename State,details::my_integral T>
inline constexpr parse_result<Iter> scan_context_define2(io_reserve_type_t<::fast_io::freestanding::iter_value_t<Iter>,parameter<T&>>,State& state,Iter begin,Iter end,parameter<T&> t) noexcept
{
	return details::scan_context_define_parse_impl<10>(state,begin,end,t.reference);
}

template<std::integral char_type,typename State,details::my_integral T>
inline constexpr parse_code scan_context_eof_define(io_reserve_type_t<char_type,parameter<T&>>,State& state,parameter<T&> t) noexcept
{
	return details::scan_context_eof_define_parse_impl<10>(state,t.reference);
}

}