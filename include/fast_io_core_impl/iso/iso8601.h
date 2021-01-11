#pragma once

namespace fast_io
{

template<intiso_t off_to_epoch>
struct basic_timestamp
{
	static constexpr intiso_t seconds_offset_to_epoch{off_to_epoch};
	intiso_t seconds{};
	uintiso_t subseconds{};
	template<intiso_t new_off_to_epoch>
	explicit constexpr operator basic_timestamp<new_off_to_epoch>() noexcept requires(off_to_epoch!=new_off_to_epoch)
	{
		constexpr intiso_t diff{off_to_epoch-new_off_to_epoch};
		return {seconds+diff,subseconds};
	}
};

template<intiso_t off_to_epoch>
inline constexpr bool operator==(basic_timestamp<off_to_epoch> a,basic_timestamp<off_to_epoch> b) noexcept
{
	return (a.seconds==b.seconds)&(a.subseconds==b.subseconds);
}

template<intiso_t off_to_epoch>
inline constexpr auto operator<=>(basic_timestamp<off_to_epoch> a,basic_timestamp<off_to_epoch> b) noexcept
{
	auto v{a.seconds<=>b.seconds};
	if(v==0)
	{
		if(a.seconds<0)
			return b.subseconds<=>a.subseconds;
		return a.subseconds<=>b.subseconds;
	}
	return v;
}

namespace details
{
struct timestamp_u
{
	uintiso_t seconds;
	uintiso_t subseconds;
};

inline constexpr timestamp_u add_impl(timestamp_u a,timestamp_u b) noexcept
{
	constexpr uintiso_t max_digit{std::numeric_limits<uintiso_t>::max()/uintiso_subseconds_per_second};
	uintiso_t res;
	bool cf;
	if constexpr(max_digit==1)
	{
		cf=intrinsics::add_carry(false,a.subseconds,b.subseconds,res);
		if(uintiso_subseconds_per_second<=res)
		{
			res-=uintiso_subseconds_per_second;
			cf=true;
		}
	}
	else
	{
		if((cf=(uintiso_subseconds_per_second<=res)))
			res-=uintiso_subseconds_per_second;
	}
	uintiso_t seconds;
	intrinsics::add_carry(cf,a.seconds,b.seconds,seconds);
	return {seconds,res};
}

inline constexpr timestamp_u sub_impl(timestamp_u a,timestamp_u b) noexcept
{
	constexpr uintiso_t max_digit{std::numeric_limits<uintiso_t>::max()/uintiso_subseconds_per_second};
	uintiso_t res;
	uintiso_t seconds;
	bool borrow{intrinsics::sub_borrow(false,a.subseconds,b.subseconds,res)};
	intrinsics::sub_borrow(borrow,a.seconds,b.seconds,seconds);
	if(borrow)
		res+=uintiso_subseconds_per_second;
	return {seconds,res};
}

}

template<intiso_t off_to_epoch>
inline constexpr basic_timestamp<off_to_epoch> operator-(basic_timestamp<off_to_epoch> a) noexcept
{
	uintiso_t high(a.seconds);
	high=0u-high;
	return {static_cast<intiso_t>(high),a.subseconds};
}

template<intiso_t off_to_epoch>
inline constexpr basic_timestamp<off_to_epoch> operator+(basic_timestamp<off_to_epoch> a,basic_timestamp<off_to_epoch> b) noexcept
{
	if(a.seconds<0)
	{
		uintiso_t a_abs(a.seconds);
		a_abs=0u-a_abs;
		if(b.seconds<0)
		{
			uintiso_t b_abs(b.seconds);
			b_abs=0u-b_abs;
			auto res{details::add_impl({a_abs,a.subseconds},
				{b_abs,b.subseconds})};
			res.seconds=0u-res.seconds;
			return {static_cast<intiso_t>(res.seconds),res.subseconds};
		}
		else
		{
			uintiso_t b_abs(b.seconds);
			if(a_abs<b_abs||(a_abs==b_abs&&a.subseconds<b.subseconds))
			{
				auto res{details::sub_impl({b_abs,b.subseconds},{a_abs,a.subseconds})};
				return {static_cast<intiso_t>(res.seconds),res.subseconds};
			}
			else
			{
				auto res{details::sub_impl({a_abs,a.subseconds},{b_abs,b.subseconds})};
				res.seconds=0u-res.seconds;
				return {static_cast<intiso_t>(res.seconds),res.subseconds};
			}
		}
	}
	else
	{
		uintiso_t a_abs(a.seconds);
		if(b.seconds<0)
		{
			uintiso_t b_abs(b.seconds);
			b_abs=0u-b_abs;
			if(a_abs<b_abs||(a_abs==b_abs&&a.subseconds<b.subseconds))
			{
				auto res{details::sub_impl({b_abs,b.subseconds},{a_abs,a.subseconds})};
				res.seconds=0u-res.seconds;
				return {static_cast<intiso_t>(res.seconds),res.subseconds};
			}
			else
			{
				auto res{details::sub_impl({a_abs,a.subseconds},{b_abs,b.subseconds})};
				return {static_cast<intiso_t>(res.seconds),res.subseconds};
			}
		}
		else
		{
			uintiso_t b_abs(b.seconds);
			auto res{details::add_impl({a_abs,a.subseconds},
				{b_abs,b.subseconds})};
			return {static_cast<intiso_t>(res.seconds),res.subseconds};
		}
	}
}
template<intiso_t off_to_epoch>
inline constexpr basic_timestamp<off_to_epoch>& operator+=(basic_timestamp<off_to_epoch>& a,basic_timestamp<off_to_epoch> b) noexcept
{
	return a=a+b;
}

template<intiso_t off_to_epoch>
inline constexpr basic_timestamp<off_to_epoch> operator-(basic_timestamp<off_to_epoch> a,basic_timestamp<off_to_epoch> b) noexcept
{
	return a+(-b);
}

template<intiso_t off_to_epoch>
inline constexpr basic_timestamp<off_to_epoch>& operator-=(basic_timestamp<off_to_epoch>& a,basic_timestamp<off_to_epoch> b) noexcept
{
	return a=a+(-b);
}

/*
https://www.epochconverter.com/seconds-days-since-y0
Seconds since year 0 (MySQL compatible)
Seconds since January 1, 1 AD
Days since year 0 (MySQL compatible)
Days since January 1, 1 AD
Days since January 1, 1900, used for Excel DAY functions
Days since January 1, 1904, formerly used for Excel DAY (Macintosh)
Days since January 1, 1970, Unix epoch
*/

using unix_timestamp=basic_timestamp<0>;		//UNIX
using win32_timestamp=basic_timestamp<-11644473600LL>;	//Windows
using csharp_timestamp=basic_timestamp<-62135712000LL>;	//C#

struct iso8601_timestamp
{
	intiso_t year{};
	std::uint8_t month{};
	std::uint8_t day{};
	std::uint8_t hours{};
	std::uint8_t minutes{};
	std::uint8_t seconds{};
	uintiso_t subseconds{};
};

template<std::integral char_type,intiso_t off_to_epoch>
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<char_type,basic_timestamp<off_to_epoch>>) noexcept
{
	return print_reserve_size(io_reserve_type<char_type,intiso_t>)+std::numeric_limits<uintiso_t>::digits10;
}

template<std::integral char_type,intiso_t off_to_epoch>
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<char_type,manipulators::comma_t<basic_timestamp<off_to_epoch>>>) noexcept
{
	return print_reserve_size(io_reserve_type<char_type,intiso_t>)+std::numeric_limits<uintiso_t>::digits10;
}

template<std::integral char_type>
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<char_type,iso8601_timestamp>) noexcept
{
//ISO 8601 timestamp example : 2021-01-03T10:29:56Z
	return print_reserve_size(io_reserve_type<char_type,intiso_t>)+16+print_reserve_size(io_reserve_type<char_type,uintiso_t>);
}

namespace details
{
template<std::random_access_iterator Iter>
inline constexpr Iter print_reserve_iso8601_timestamp_impl(Iter iter,iso8601_timestamp timestamp) noexcept
{
	using char_type = std::iter_value_t<Iter>;
	iter=chrono_year_impl(iter,timestamp.year);
	if constexpr(std::same_as<char_type,char>)
		*iter='-';
	else if constexpr(std::same_as<char_type,wchar_t>)
		*iter=L'-';
	else
		*iter=u8'-';
	++iter;
	iter=chrono_two_digits_impl<true>(iter,timestamp.month);
	if constexpr(std::same_as<char_type,char>)
		*iter='-';
	else if constexpr(std::same_as<char_type,wchar_t>)
		*iter=L'-';
	else
		*iter=u8'-';
	++iter;
	iter=chrono_two_digits_impl<true>(iter,timestamp.day);
	if constexpr(std::same_as<char_type,char>)
		*iter='T';
	else if constexpr(std::same_as<char_type,wchar_t>)
		*iter=L'T';
	else
		*iter=u8'T';
	++iter;
	iter=chrono_two_digits_impl<true>(iter,timestamp.hours);
	if constexpr(std::same_as<char_type,char>)
		*iter=':';
	else if constexpr(std::same_as<char_type,wchar_t>)
		*iter=L':';
	else
		*iter=u8':';
	++iter;
	iter=chrono_two_digits_impl<true>(iter,timestamp.minutes);
	if constexpr(std::same_as<char_type,char>)
		*iter=':';
	else if constexpr(std::same_as<char_type,wchar_t>)
		*iter=L':';
	else
		*iter=u8':';
	++iter;
	iter=chrono_two_digits_impl<true>(iter,timestamp.seconds);
	if(timestamp.subseconds)
		iter=output_iso8601_subseconds(iter,timestamp.subseconds);
	if constexpr(std::same_as<char_type,char>)
		*iter='Z';
	else if constexpr(std::same_as<char_type,wchar_t>)
		*iter=L'Z';
	else
		*iter=u8'Z';
	++iter;
	return iter;
}

template<bool comma=false,std::random_access_iterator Iter>
inline constexpr Iter print_reserve_bsc_timestamp_impl(Iter iter,unix_timestamp timestamp) noexcept
{
	using char_type = std::iter_value_t<Iter>;
	iter=print_reserve_define(io_reserve_type<char_type,intiso_t>,iter,timestamp.seconds);
	if(timestamp.subseconds)
		iter=output_iso8601_subseconds<comma>(iter,timestamp.subseconds);
	return iter;
}

}

template<std::integral char_type,std::random_access_iterator Iter,intiso_t off_to_epoch>
inline constexpr Iter print_reserve_define(io_reserve_type_t<char_type,basic_timestamp<off_to_epoch>>,
		Iter iter,basic_timestamp<off_to_epoch> timestamp) noexcept
{
	if constexpr(off_to_epoch==0)
		return details::print_reserve_bsc_timestamp_impl(iter,timestamp);
	else
		return details::print_reserve_bsc_timestamp_impl(iter,{timestamp.seconds,timestamp.subseconds});
}

template<std::integral char_type,std::random_access_iterator Iter,intiso_t off_to_epoch>
inline constexpr Iter print_reserve_define(io_reserve_type_t<char_type,manipulators::comma_t<basic_timestamp<off_to_epoch>>>,
		Iter iter,manipulators::comma_t<basic_timestamp<off_to_epoch>> timestamp) noexcept
{
	if constexpr(off_to_epoch==0)
		return details::print_reserve_bsc_timestamp_impl<true>(iter,timestamp.reference);
	else
		return details::print_reserve_bsc_timestamp_impl<true>(iter,{timestamp.reference.seconds,timestamp.reference.subseconds});
}

template<std::integral char_type,std::random_access_iterator Iter>
inline constexpr Iter print_reserve_define(io_reserve_type_t<char_type,iso8601_timestamp>,Iter iter,iso8601_timestamp timestamp) noexcept
{
	return details::print_reserve_iso8601_timestamp_impl(iter,timestamp);
}

namespace manipulators
{

template<manipulators::rounding_mode mode=manipulators::rounding_mode::nearest_to_even,std::size_t base=10,intiso_t off_to_epoch>
requires (base==10)
inline constexpr floating_format_precision_t<unix_timestamp,floating_representation::fixed,mode,base>
	fixed(basic_timestamp<off_to_epoch> ts,std::size_t precision) noexcept
{
	if constexpr(off_to_epoch==0)
		return {ts,precision};
	else
		return {{ts.seconds,ts.subseconds},precision};
}

template<intiso_t off_to_epoch>
inline constexpr comma_t<unix_timestamp> comma(basic_timestamp<off_to_epoch> ts)
{
	if constexpr(off_to_epoch==0)
		return {ts};
	else
		return {{ts.seconds,ts.subseconds}};
}

template<manipulators::rounding_mode mode=manipulators::rounding_mode::nearest_to_even,std::size_t base=10,intiso_t off_to_epoch>
requires (base==10)
inline constexpr comma_t<floating_format_precision_t<unix_timestamp,floating_representation::fixed,mode,base>>
	comma_fixed(basic_timestamp<off_to_epoch> ts,std::size_t precision) noexcept
{
	if constexpr(off_to_epoch==0)
		return {{ts,precision}};
	else
		return {{{ts.seconds,ts.subseconds},precision}};
}

}

template<std::integral char_type,manipulators::rounding_mode mode>
inline constexpr std::size_t print_reserve_size(
	io_reserve_type_t<char_type,manipulators::floating_format_precision_t<unix_timestamp,
	manipulators::floating_representation::fixed,mode>>,
	manipulators::floating_format_precision_t<unix_timestamp,
	manipulators::floating_representation::fixed,mode> ts)
{
	constexpr std::size_t value{print_reserve_size(io_reserve_type<char_type,intiso_t>)+3};
	constexpr std::size_t val1{std::numeric_limits<uintiso_t>::digits10};
	return value+std::max(ts.precision,val1);
}


template<std::integral char_type,manipulators::rounding_mode mode>
inline constexpr std::size_t print_reserve_size(
	io_reserve_type_t<char_type,
	manipulators::comma_t<manipulators::floating_format_precision_t<unix_timestamp,
	manipulators::floating_representation::fixed,mode>>>,
	manipulators::comma_t<manipulators::floating_format_precision_t<unix_timestamp,
	manipulators::floating_representation::fixed,mode>> ts)
{
	constexpr std::size_t value{print_reserve_size(io_reserve_type<char_type,intiso_t>)+3};
	constexpr std::size_t val1{std::numeric_limits<uintiso_t>::digits10};
	return value+std::max(ts.reference.precision,val1);
}

namespace details
{

template<bool comma,std::random_access_iterator Iter>
inline constexpr Iter unix_timestamp_fixed_print_full_impl(Iter iter,unix_timestamp ts)
{
	using char_type = std::iter_value_t<Iter>;
	iter=print_reserve_define(io_reserve_type<char_type,intiso_t>,iter,ts.seconds);
	constexpr std::size_t digits(std::numeric_limits<uintiso_t>::digits10);
	if constexpr(comma)
	{
	if constexpr(std::same_as<char_type,char>)
		*iter=',';
	else if constexpr(std::same_as<char_type,wchar_t>)
		*iter=L',';
	else
		*iter=u8',';
	}
	else
	{
	if constexpr(std::same_as<char_type,char>)
		*iter='.';
	else if constexpr(std::same_as<char_type,wchar_t>)
		*iter=L'.';
	else
		*iter=u8'.';
	}
	++iter;
	auto res{iter+digits};
	with_length_output_unsigned(iter,ts.subseconds,digits);
	return res;
}

template<bool comma,std::random_access_iterator Iter>
inline constexpr Iter unix_timestamp_fixed_print_easy_case(Iter iter,unix_timestamp ts,std::size_t precision)
{
	using char_type = std::iter_value_t<Iter>;
	iter=unix_timestamp_fixed_print_full_impl<comma>(iter,ts);
	constexpr std::size_t digits{static_cast<std::size_t>(std::numeric_limits<uintiso_t>::digits10)-1};
	std::size_t const to_fill{precision-digits};
	if constexpr(std::same_as<char_type,char>)
		return my_fill_n(iter,to_fill,'0');
	else if constexpr(std::same_as<char_type,wchar_t>)
		return my_fill_n(iter,to_fill,L'0');
	else if constexpr(std::same_as<char_type,char16_t>)
		return my_fill_n(iter,to_fill,u'0');
	else if constexpr(std::same_as<char_type,char32_t>)
		return my_fill_n(iter,to_fill,U'0');
	else
		return my_fill_n(iter,to_fill,u8'0');
}

template<bool comma,::fast_io::manipulators::rounding_mode mode,std::random_access_iterator Iter>
inline constexpr Iter unix_timestamp_fixed_complex_case(Iter iter,::fast_io::manipulators::floating_format_precision_t<unix_timestamp,::fast_io::manipulators::floating_representation::fixed,mode> ts)
{
	using char_type = std::iter_value_t<Iter>;
	std::size_t const precision{ts.precision};
	uintiso_t tbv{d10_reverse_table<uintiso_t>[precision]};
	uintiso_t val{ts.reference.subseconds/tbv};
	uintiso_t upper_frac{val/10u};
	uintiso_t abs{static_cast<uintiso_t>(ts.reference.seconds)};
	bool const negative{ts.reference.seconds<0};
	if(negative)
	{
		if constexpr(std::same_as<char_type,char>)
			*iter='-';
		else if constexpr(std::same_as<char_type,wchar_t>)
			*iter=L'-';
		else
			*iter=u8'-';
		++iter;
		abs=0u-abs;
	}
	if constexpr(mode!=::fast_io::manipulators::rounding_mode::toward_zero)
	{
		uintiso_t val_mod{ts.reference.subseconds%tbv};
		uintiso_t frac{val%10u};
		bool carry{};
		if constexpr(mode==::fast_io::manipulators::rounding_mode::nearest_to_even)
		{
			if(5<frac)
			{
				carry=true;
			}
			else if(frac==5)
			{
				if(val_mod)
					carry=true;
				else
				{
					carry=upper_frac&1;
				}
			}
		}
		else if constexpr(mode==::fast_io::manipulators::rounding_mode::nearest_to_odd)
		{
			if(5<frac)
			{
				carry=true;
			}
			else if(frac==5)
			{
				if(val_mod)
					carry=true;
				else
				{
					carry=!(upper_frac&1);
				}
			}
		}
		if(carry)
		{
			++upper_frac;
			if(upper_frac*tbv*100u==uintiso_subseconds_per_second)
			{
				++abs;
				upper_frac=0;
			}
		}
	}
	iter=print_reserve_define(io_reserve_type<char_type,uintiso_t>,iter,abs);
	if(precision!=0)
	{
		if constexpr(comma)
		{
		if constexpr(std::same_as<char_type,char>)
			*iter=',';
		else if constexpr(std::same_as<char_type,wchar_t>)
			*iter=L',';
		else
			*iter=u8',';
		}
		else
		{
		if constexpr(std::same_as<char_type,char>)
			*iter='.';
		else if constexpr(std::same_as<char_type,wchar_t>)
			*iter=L'.';
		else
			*iter=u8'.';
		}
		++iter;
		with_length_output_unsigned(iter,upper_frac,ts.precision);
		iter+=ts.precision;
	}
	return iter;
}

template<bool comma,::fast_io::manipulators::rounding_mode mode,std::random_access_iterator Iter>
inline constexpr Iter unix_timestamp_fixed_print_impl(
	Iter iter,
	::fast_io::manipulators::floating_format_precision_t<unix_timestamp,::fast_io::manipulators::floating_representation::fixed,mode> ts) noexcept
{
	using char_type = std::iter_value_t<Iter>;
	constexpr std::size_t digits10m2{static_cast<std::size_t>(std::numeric_limits<uintiso_t>::digits10)-2};
	if constexpr(mode==::fast_io::manipulators::rounding_mode::toward_zero)
	{
		if(ts.precision==0)
			return print_reserve_define(io_reserve_type<char_type,intiso_t>,iter,ts.reference.seconds);
		else if(digits10m2<ts.precision)
			return unix_timestamp_fixed_print_easy_case<comma>(iter,ts.reference,ts.precision);
		else
			return unix_timestamp_fixed_complex_case<comma>(iter,ts);
	}
	else
	{
		if(digits10m2<ts.precision)
			return unix_timestamp_fixed_print_easy_case<comma>(iter,ts.reference,ts.precision);
		else
			return unix_timestamp_fixed_complex_case<comma>(iter,ts);
	}
	return iter;
}

}

template<::fast_io::manipulators::rounding_mode mode,std::random_access_iterator Iter>
inline constexpr Iter print_reserve_define(io_reserve_type_t<std::iter_value_t<Iter>,::fast_io::manipulators::floating_format_precision_t<unix_timestamp,::fast_io::manipulators::floating_representation::fixed,mode>>,
	Iter iter,
	::fast_io::manipulators::floating_format_precision_t<unix_timestamp,::fast_io::manipulators::floating_representation::fixed,mode> ts) noexcept
{
	return details::unix_timestamp_fixed_print_impl<false>(iter,ts);
}

template<::fast_io::manipulators::rounding_mode mode,std::random_access_iterator Iter>
inline constexpr Iter print_reserve_define(io_reserve_type_t<std::iter_value_t<Iter>,
	manipulators::comma_t<::fast_io::manipulators::floating_format_precision_t<unix_timestamp,::fast_io::manipulators::floating_representation::fixed,mode>>>,
	Iter iter,
	manipulators::comma_t<::fast_io::manipulators::floating_format_precision_t<unix_timestamp,::fast_io::manipulators::floating_representation::fixed,mode>> ts) noexcept
{
	return details::unix_timestamp_fixed_print_impl<true>(iter,ts.reference);
}

}