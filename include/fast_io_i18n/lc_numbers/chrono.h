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


}