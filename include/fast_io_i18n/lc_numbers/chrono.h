#pragma once
/*
C++ 20 chrono considered harmful
Howard Hinnant is an OOP loser and has no idea of how computer architectures, operating systems, and compilers work.

WTF you want both iso_encoding() and c_encoding() with weekday?
January? Why does the identifier in the standard library start as the upper case?

Unfortunately, WG21 is full of garbage like him.
What the fuck of all these interfaces which passing const references?
Reference to unsigned short??? FUCK FUCK FUCK FUCK FUCK.

std::chrono::year stored as short while returns int? It only works with the year range [-32768,32767].
Why does WG21 want another Year of 2000 dumbshit?
You might say 32768 AD does not make sense since human being would probably disappear as a race,
but what about 32769 BC??

What about std::chrono::parse? WTF you parse date time with locale? C++ locale does not even work correctly.

Of course, Herb Sutter is another loser who advertises int which is proven harmful for enormous time.
Even today, OpenSSL BIO contains legacy garbage like this.
C++ Core Guidelines Considered harmful. It is Google C++ Coding Style 2.0.

FUCK FUCK FUCK FUCK FUCK FUCK.
WTF of the shit gets added into C++20? Modern C++ is just objectively harmful.
*/
namespace fast_io
{

template<std::integral char_type>
inline constexpr std::size_t print_reserve_size(basic_lc_all<char_type> const* __restrict all,std::chrono::weekday wkd) noexcept
{
// This is BRAIN DEATH. Why is this unsigned while stored as unsigned short??? Why you have the differences between c_encoding() and iso_encoding()?
// FUCK SHIT
	unsigned value(wkd.c_encoding());
	if(7<value)
	{
		constexpr std::size_t unsigned_size{print_reserve_size(io_reserve_type<char_type,unsigned>)};
		return unsigned_size;
	}
	else
	{
		if(value==7)
			value=0;
		return all->time.day[value].len;
	}
}

template<std::random_access_iterator Iter>
inline constexpr Iter print_reserve_define(basic_lc_all<std::iter_value_t<Iter>> const* __restrict all,Iter it,std::chrono::weekday wkd) noexcept
{
	unsigned value{wkd.c_encoding()};
	if(7u<value)
	{
		return print_reserve_define(io_reserve_type<std::iter_value_t<Iter>,unsigned>,it,value);
	}
	else
	{
		if(value==7u)
			value={};
		return details::non_overlapped_copy_n(all->time.day[value].base,all->time.day[value].len,it);
	}
}

template<std::integral char_type>
inline constexpr std::size_t print_reserve_size(basic_lc_all<char_type> const* __restrict all,std::chrono::month m) noexcept
{
	unsigned value(m);
	--value;
	if(value<12u)
		return all->time.mon[value].len;
	else
	{
		constexpr std::size_t unsigned_size{print_reserve_size(io_reserve_type<char_type,unsigned>)};
		return unsigned_size;
	}
}

template<std::random_access_iterator Iter,std::integral char_type>
inline constexpr Iter print_reserve_define(basic_lc_all<char_type> const* __restrict all,Iter iter,std::chrono::month m) noexcept
{
	unsigned value(m);
	unsigned value1(value);
	--value;
	if(value<12u)
		return details::non_overlapped_copy_n(all->time.mon[value].base,all->time.mon[value].len,iter);
	else
		return print_reserve_define(io_reserve_type<char_type,unsigned>,iter,value1);
}

/*
Referenced from IBM
LC_TIME Category for the Locale Definition Source File Format
https://www.ibm.com/support/knowledgecenter/ssw_aix_71/filesreference/LC_TIME.html
*/

namespace manipulators
{

template<typename T>
struct abbr_t
{
	using manip_tag = manip_tag_t;
	T reference;
};

template<typename T>
struct alt_t
{
	using manip_tag = manip_tag_t;
	T reference;
};

template<typename T>
struct alt_num_t
{
	using manip_tag = manip_tag_t;
	T reference;
};

template<typename T>
struct am_pm_t
{
	using manip_tag = manip_tag_t;
	T reference;
};

inline constexpr am_pm_t<bool> am_pm(bool is_pm) noexcept
{
	return {is_pm};
}

inline constexpr abbr_t<std::chrono::weekday> abbr(std::chrono::weekday wkd) noexcept
{
	return {wkd};
}

inline constexpr abbr_t<std::chrono::month> abbr(std::chrono::month wkd) noexcept
{
	return {wkd};
}

inline constexpr abbr_t<alt_t<std::chrono::month>> abbr_alt(std::chrono::month wkd) noexcept
{
	return {{wkd}};
}

template<typename T>
requires (std::same_as<T,std::chrono::month>||
std::same_as<T,std::chrono::day>||
std::same_as<T,std::chrono::weekday>)
inline constexpr alt_num_t<T> alt_num(T m) noexcept
{
	return {m};
}


template<std::integral char_type>
inline constexpr basic_io_scatter_t<char_type> print_scatter_define(basic_lc_all<char_type> const* __restrict all,am_pm_t<bool> ampm) noexcept
{
	return all->time.am_pm[ampm.reference];
}

template<std::integral char_type>
inline constexpr std::size_t print_reserve_size(basic_lc_all<char_type> const* __restrict all,abbr_t<std::chrono::weekday> wkd) noexcept
{
	unsigned value(wkd.reference.c_encoding());
	if(7<value)
	{
		constexpr std::size_t unsigned_size{print_reserve_size(io_reserve_type<char_type,unsigned>)};
		return unsigned_size;
	}
	else
	{
		if(value==7)
			value=0;
		return all->time.abday[value].len;
	}
}

template<std::random_access_iterator Iter>
inline constexpr Iter print_reserve_define(basic_lc_all<std::iter_value_t<Iter>> const* __restrict all,Iter it,abbr_t<std::chrono::weekday> wkd) noexcept
{
	unsigned value(wkd.reference.c_encoding());
	if(7u<value)
	{
		return print_reserve_define(io_reserve_type<std::iter_value_t<Iter>,unsigned>,it,value);
	}
	else
	{
		if(value==7u)
			value={};
		return details::non_overlapped_copy_n(all->time.abday[value].base,all->time.abday[value].len,it);
	}
}

template<std::integral char_type>
inline constexpr std::size_t print_reserve_size(basic_lc_all<char_type> const* __restrict all,abbr_t<std::chrono::month> m) noexcept
{
	unsigned value(m.reference);
	--value;
	if(value<12u)
		return all->time.abmon[value].len;
	else
	{
		constexpr std::size_t unsigned_size{print_reserve_size(io_reserve_type<char_type,unsigned>)};
		return unsigned_size;
	}
}

template<std::random_access_iterator Iter,std::integral char_type>
inline constexpr Iter print_reserve_define(basic_lc_all<char_type> const* __restrict all,Iter iter,abbr_t<std::chrono::month> m) noexcept
{
	unsigned value(m.reference);
	unsigned value1(value);
	--value;
	if(value<12u)
		return details::non_overlapped_copy_n(all->time.abmon[value].base,all->time.abmon[value].len,iter);
	else
		return print_reserve_define(io_reserve_type<char_type,unsigned>,iter,value1);
}

template<std::integral char_type>
inline constexpr std::size_t print_reserve_size(basic_lc_all<char_type> const* __restrict all,abbr_t<alt_t<std::chrono::month>> m) noexcept
{
	unsigned value(m.reference.reference);
	--value;
	if(value<12u)
	{
		if(all->time.ab_alt_mon[value].len==0)
			return all->time.abmon[value].len;
		else
			return all->time.ab_alt_mon[value].len;
	}
	else
	{
		constexpr std::size_t unsigned_size{print_reserve_size(io_reserve_type<char_type,unsigned>)};
		return unsigned_size;
	}
}

template<std::random_access_iterator Iter,std::integral char_type>
inline constexpr Iter print_reserve_define(basic_lc_all<char_type> const* __restrict all,Iter iter,abbr_t<alt_t<std::chrono::month>> m) noexcept
{
	unsigned value(m.reference.reference);
	unsigned value1(value);
	--value;
	if(value<12u)
	{
		if(all->time.ab_alt_mon[value].len==0)
			return details::non_overlapped_copy_n(all->time.abmon[value].base,all->time.abmon[value].len,iter);
		else
			return details::non_overlapped_copy_n(all->time.ab_alt_mon[value].base,all->time.ab_alt_mon[value].len,iter);
	}
	else
		return print_reserve_define(io_reserve_type<char_type,unsigned>,iter,value1);
}

template<std::integral char_type,typename T>
requires (std::same_as<T,std::chrono::month>||
std::same_as<T,std::chrono::day>||
std::same_as<T,std::chrono::weekday>)
inline constexpr std::size_t print_reserve_size(basic_lc_all<char_type> const* __restrict all,
	alt_num_t<T> m) noexcept
{
	using namespace std::chrono;
	if constexpr(std::same_as<month,T>||std::same_as<day,T>)
	{
		unsigned value(m.reference);
		if(value<all->time.alt_digits.len)
			return all->time.alt_digits.base[value].len;
		else
		{
			constexpr std::size_t unsigned_size{print_reserve_size(io_reserve_type<char_type,T>)};
			return unsigned_size;
		}
	}
	else
	{
		unsigned value(m.reference.iso_encoding());
		if(value<all->time.alt_digits.len)
			return all->time.alt_digits.base[value].len;
		else
		{
			constexpr std::size_t unsigned_size{print_reserve_size(io_reserve_type<char_type,T>)};
			return unsigned_size;
		}
	}
}

template<std::integral char_type,std::random_access_iterator Iter,typename T>
requires (std::same_as<T,std::chrono::month>||
std::same_as<T,std::chrono::day>||
std::same_as<T,std::chrono::weekday>)
inline constexpr Iter print_reserve_define(basic_lc_all<char_type> const* __restrict all,Iter iter,
	alt_num_t<T> m) noexcept
{
	using namespace std::chrono;
	if constexpr(std::same_as<month,T>||std::same_as<day,T>)
	{
		unsigned value(m.reference);
		if(value<all->time.alt_digits.len)
			return details::non_overlapped_copy_n(all->time.alt_digits.base[value].base,
				all->time.alt_digits.base[value].len,iter);
		else
			return print_reserve_define(io_reserve_type<char_type,T>,iter,m.reference);
	}
	else
	{
		unsigned value(m.reference.iso_encoding());
		if(value<all->time.alt_digits.len)
			return details::non_overlapped_copy_n(all->time.alt_digits.base[value].base,
				all->time.alt_digits.base[value].len,iter);
		else
			return print_reserve_define(io_reserve_type<char_type,T>,iter,m.reference);
	}
}

#if 0
template<std::integral char_type>
inline constexpr std::size_t print_reserve_size(basic_lc_all<char_type> const* __restrict all,std::chrono::year_month_day m) noexcept
{
	constexpr std::size_t unitsize{std::max(print_reserve_size(io_reserve_type<char_type,int>),print_reserve_size(io_reserve_type<char_type,unsigned>))};
	return unitsize*all->time.d_fmt.len;
}

template<std::random_access_iterator Iter>
inline constexpr Iter print_reserve_define(basic_lc_all<std::iter_value_t<Iter>> const* __restrict all,Iter iter,std::chrono::year_month_day m) noexcept
{

}
#endif
}
}