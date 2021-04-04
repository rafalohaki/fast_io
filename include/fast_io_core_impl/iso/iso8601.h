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
/*
Should we use std::bit_cast and applying integer arithmatics to convert to floating points just like dragonbox or ryu are doing?
*/
	explicit constexpr operator float() noexcept
	{
		constexpr float db{static_cast<float>(uintiso_subseconds_per_second)};
		return static_cast<float>(seconds)+static_cast<float>(subseconds)/db;
	}
	explicit constexpr operator double() noexcept
	{
		constexpr double db{static_cast<double>(uintiso_subseconds_per_second)};
		return static_cast<double>(seconds)+static_cast<double>(subseconds)/db;
	}
	explicit constexpr operator long double() noexcept
	{
		constexpr long double db{static_cast<long double>(uintiso_subseconds_per_second)};
		return static_cast<long double>(seconds)+static_cast<long double>(subseconds)/db;
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
	uintiso_t res;
	uintiso_t seconds;
	bool borrow{intrinsics::sub_borrow(false,a.subseconds,b.subseconds,res)};
	intrinsics::sub_borrow(borrow,a.seconds,b.seconds,seconds);
	if(borrow)
		res+=uintiso_subseconds_per_second;
	return {seconds,res};
}
#if 0


inline constexpr basic_timestamp<0> div_uint(intiso_t rseconds,uintiso_t subseconds,uintiso_t d) noexcept
{
	if(d==0)[[unlikely]]
		fast_terminate();
	bool minus{rseconds<0};
	uintiso_t seconds{static_cast<uintiso_t>(rseconds)};
	if(minus)
		seconds=static_cast<uintiso_t>(0)-seconds;
#if UINTMAX_MAX==UINT64_MAX
#if __SIZEOF_INT128__
	__uint128_t total_subseconds{static_cast<__uint128_t>(seconds)*uintiso_subseconds_per_second+subseconds};
	std::uint64_t mid{d>>1};
	__uint128_t rr{total_subseconds%d};
	std::uint64_t r{static_cast<std::uint64_t>(rr)};
	__uint128_t q{total_subseconds/d};
	if(mid<r)
		++q;
	else if(mid==r)
	{
		if((q&1)==1)
			++q;
	}
	std::uint64_t result_seconds{static_cast<std::uint64_t>(q/uintiso_subseconds_per_second)};
	std::uint64_t result_subseconds{static_cast<std::uint64_t>(q%uintiso_subseconds_per_second)};
	if(minus)
		result_seconds=static_cast<uintiso_t>(0)-result_seconds;
	return {static_cast<intiso_t>(result_seconds),result_subseconds};
#else
	if(std::is_constant_evaluated())
		return {};
	std::uint64_t total_seconds_high;
	std::uint64_t total_seconds_low{_umul128(seconds,subseconds,&total_seconds_high)};
	std::uint64_t mid{d>>1};
	std::uint64_t r;

	std::uint64_t q_low{_udiv128(total_seconds_high,total_seconds_low,d,&r)};

	std::uint64_t q_high{total_seconds_high/d};
	if(mid<r||(mid==r&&(q_low&1)==1))
		_addcarry_u64(_addcarry_u64(false,q_low,1,&q_low),q_high,0,&q_high);
	total_seconds_low=_udiv128(q_high,q_low,uintiso_subseconds_per_second,&r);
	if(minus)
		total_seconds_low=static_cast<uintiso_t>(0)-total_seconds_low;
	return {static_cast<intiso_t>(total_seconds_low),r};
#endif
#endif	
/*
	else
	{
		basic_unsigned_extension<uintiso_t> total_subseconds{mul_extend(seconds,uintiso_subseconds_per_second)+subseconds};
		uintiso_t mid{d>>1};
		auto [rr,q]=div_mod(total_subseconds,d);
		uintiso_t r{rr};
	}

*/
}
#endif
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
#if 0
template<intiso_t off_to_epoch>
inline constexpr basic_timestamp<off_to_epoch> operator/(basic_timestamp<off_to_epoch> a,uintiso_t b) noexcept
{
	if constexpr(off_to_epoch==0)
	{
		return details::div_uint(a.seconds,a.subseconds,b);
	}
	else
	{
		auto [seconds,subseconds]=details::div_uint(a.seconds,a.subseconds,b);
		return {seconds,subseconds};
	}
}

template<intiso_t off_to_epoch>
inline constexpr basic_timestamp<off_to_epoch>& operator/=(basic_timestamp<off_to_epoch>& a,uintiso_t b) noexcept
{
	if constexpr(off_to_epoch==0)
	{
		return a=details::div_uint(a.seconds,a.subseconds,b);
	}
	else
	{
		auto [seconds,subseconds]=details::div_uint(a.seconds,a.subseconds,b);
		a.seconds=seconds;
		a.subseconds=subseconds;
		return a;
	}
}
#endif
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
using universe_timestamp=basic_timestamp<-436117076900000000LL>;		//Pesudo timestamp since the big bang of universe
/*
Referenced from: https://81018.com/universeclock/
*/

struct iso8601_timestamp
{
	intiso_t year{};
	std::uint8_t month{};
	std::uint8_t day{};
	std::uint8_t hours{};
	std::uint8_t minutes{};
	std::uint8_t seconds{};
	uintiso_t subseconds{};
	long timezone{};
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
//ISO 8601 timestamp with timestamp : 2021-01-03T10:29:56.999999+99:99
	return print_reserve_size(io_reserve_type<char_type,intiso_t>)+16+print_reserve_size(io_reserve_type<char_type,uintiso_t>)+print_reserve_size(io_reserve_type<char_type,long>)+3+4+2;
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
	if(timestamp.timezone==0)
	{
		if constexpr(std::same_as<char_type,char>)
			*iter='Z';
		else if constexpr(std::same_as<char_type,wchar_t>)
			*iter=L'Z';
		else
			*iter=u8'Z';
		++iter;
	}
	else
	{
		unsigned long unsigned_tz{static_cast<unsigned long>(timestamp.timezone)};
		if(timestamp.timezone<0)
		{
			if constexpr(std::same_as<char_type,char>)
				*iter='-';
			else if constexpr(std::same_as<char_type,wchar_t>)
				*iter=L'-';
			else
				*iter=u8'-';
			unsigned_tz=0UL-unsigned_tz;
		}
		else
		{
			if constexpr(std::same_as<char_type,char>)
				*iter='+';
			else if constexpr(std::same_as<char_type,wchar_t>)
				*iter=L'+';
			else
				*iter=u8'+';
		}
		++iter;
		std::uint8_t tz_ss{static_cast<std::uint8_t>(unsigned_tz%60)};
		unsigned_tz/=60;
		std::uint8_t tz_mm{static_cast<std::uint8_t>(unsigned_tz%60)};
		unsigned_tz/=60;
		iter=chrono_two_digits_impl(iter,unsigned_tz);
		if constexpr(std::same_as<char_type,char>)
			*iter=':';
		else if constexpr(std::same_as<char_type,wchar_t>)
			*iter=L':';
		else
			*iter=u8':';
		++iter;
		iter=chrono_two_digits_impl<true>(iter,tz_mm);
		if(tz_ss)
		{
			if constexpr(std::same_as<char_type,char>)
				*iter=':';
			else if constexpr(std::same_as<char_type,wchar_t>)
				*iter=L':';
			else
				*iter=u8':';
			++iter;
			iter=chrono_two_digits_impl<true>(iter,tz_ss);
		}
	}
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
inline constexpr Iter print_reserve_define(io_reserve_type_t<char_type,iso8601_timestamp>,Iter iter,iso8601_timestamp const& timestamp) noexcept
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
	return details::intrinsics::add_or_overflow_die(value,std::max(ts.precision,val1));
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