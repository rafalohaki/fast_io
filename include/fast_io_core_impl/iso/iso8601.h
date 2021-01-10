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
	if(!v)
	{
		auto v2{a.subseconds<=>b.subseconds};
		if(a.seconds<0)
			return -v2;
		return v2;
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
		if(cf=(uintiso_subseconds_per_second<=res))
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
			b_abs=0u-b_abs;
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

template<std::integral char_type>
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<char_type,iso8601_timestamp>) noexcept
{
//ISO 8601 timestamp example : 2021-01-03T10:29:56Z
	return print_reserve_size(io_reserve_type<char_type,intiso_t>)+
	16+print_reserve_size(io_reserve_type<char_type,uintiso_t>);
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

template<std::random_access_iterator Iter>
inline constexpr Iter print_reserve_bsc_timestamp_impl(Iter iter,unix_timestamp timestamp) noexcept
{
	using char_type = std::iter_value_t<Iter>;
	iter=print_reserve_define(io_reserve_type<char_type,intiso_t>,iter,timestamp.seconds);
	if(timestamp.subseconds)
		iter=output_iso8601_subseconds(iter,timestamp.subseconds);
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

template<std::integral char_type,std::random_access_iterator Iter>
inline constexpr Iter print_reserve_define(io_reserve_type_t<char_type,iso8601_timestamp>,Iter iter,iso8601_timestamp timestamp) noexcept
{
	return details::print_reserve_iso8601_timestamp_impl(iter,timestamp);
}

}