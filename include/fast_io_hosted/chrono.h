#pragma once
#include<chrono>

namespace fast_io
{
#ifdef __MSDOS__
struct timespec
{
std::time_t tv_sec;
long tv_nsec;
};
#endif
template<std::integral char_type>
constexpr std::size_t print_reserve_size(io_reserve_type_t<char_type,struct timespec>) noexcept
{
	return print_reserve_size(io_reserve_type<char_type,std::time_t>)+1+print_reserve_size(io_reserve_type<char_type,long>);
}

template<std::integral char_type,std::random_access_iterator Iter>
constexpr Iter print_reserve_define(io_reserve_type_t<char_type,struct timespec>,Iter it,struct timespec spc) noexcept
{
	it=print_reserve_define(io_reserve_type<char_type,std::time_t>,it,spc.tv_sec);
	unsigned long nsec{static_cast<unsigned long>(spc.tv_nsec)};
	if(nsec==0UL||999999999UL<nsec)
		return it;
	if constexpr(std::same_as<char_type,char>)
		*it='.';
	else if constexpr(std::same_as<char_type,wchar_t>)
		*it=L'.';
	else
		*it=u8'.';
	++it;
	std::size_t sz{9};
	for(;nsec%10==0;--sz)
		nsec/=10;
	details::with_length_output_unsigned(it,nsec,sz);
	return it+sz;
}

template<std::integral char_type,typename Rep,typename Period>
requires (reserve_printable<char_type,Rep>&&(std::same_as<Period,std::nano>||std::same_as<Period,std::micro>||std::same_as<Period,std::milli>||
	std::same_as<Period,std::ratio<1>>||std::same_as<Period,std::ratio<60>>||
	std::same_as<Period,std::ratio<3600>>||std::same_as<Period,std::ratio<86400>>||
	std::same_as<Period,std::ratio<604800>>||std::same_as<Period,std::ratio<2629746>>||
	std::same_as<Period,std::ratio<31556952>>))
constexpr std::size_t print_reserve_size(io_reserve_type_t<char_type,std::chrono::duration<Rep,Period>>) noexcept
{
	constexpr std::size_t sz{print_reserve_size(io_reserve_type<char_type,Rep>)};
	if constexpr(std::same_as<Period,std::nano>||std::same_as<Period,std::micro>
		||std::same_as<Period,std::milli>||std::same_as<Period,std::ratio<3600>>||std::same_as<Period,std::ratio<604800>>||
		std::same_as<Period,std::ratio<2629746>>||std::same_as<Period,std::ratio<31556952>>)
		return sz+2;
	else if constexpr(std::same_as<Period,std::ratio<60>>)
		return sz+3;
	else
		return sz+1;		
}

template<std::integral char_type,std::random_access_iterator Iter,typename Rep,typename Period>
requires (reserve_printable<char_type,Rep>&&(std::same_as<Period,std::nano>||std::same_as<Period,std::micro>||std::same_as<Period,std::milli>||
	std::same_as<Period,std::ratio<1>>||std::same_as<Period,std::ratio<60>>||
	std::same_as<Period,std::ratio<3600>>||std::same_as<Period,std::ratio<86400>>||
	std::same_as<Period,std::ratio<604800>>||std::same_as<Period,std::ratio<2629746>>||
	std::same_as<Period,std::ratio<31556952>>))
constexpr Iter print_reserve_define(io_reserve_type_t<char_type,std::chrono::duration<Rep,Period>>,Iter it,std::chrono::duration<Rep,Period> duration) noexcept
{
	it=print_reserve_define(io_reserve_type<char_type,Rep>,it,duration.count());
	if constexpr(std::same_as<Period,std::nano>)
	{
		if constexpr(std::same_as<char_type,char>)
			return details::copy_string_literal("ns",it);
		else if constexpr(std::same_as<char_type,wchar_t>)
			return details::copy_string_literal(L"ns",it);
		else if constexpr(std::same_as<char_type,char16_t>)
			return details::copy_string_literal(u"ns",it);
		else if constexpr(std::same_as<char_type,char32_t>)
			return details::copy_string_literal(U"ns",it);
		else
			return details::copy_string_literal(u8"ns",it);
	}
	else if constexpr(std::same_as<Period,std::micro>)
	{
		if constexpr(std::same_as<char_type,char>)
			return details::copy_string_literal("us",it);
		else if constexpr(std::same_as<char_type,wchar_t>)
			return details::copy_string_literal(L"us",it);
		else if constexpr(std::same_as<char_type,char16_t>)
			return details::copy_string_literal(u"us",it);
		else if constexpr(std::same_as<char_type,char32_t>)
			return details::copy_string_literal(U"us",it);
		else
			return details::copy_string_literal(u8"us",it);
	}
	else if constexpr(std::same_as<Period,std::milli>)
	{
		if constexpr(std::same_as<char_type,char>)
			return details::copy_string_literal("ms",it);
		else if constexpr(std::same_as<char_type,wchar_t>)
			return details::copy_string_literal(L"ms",it);
		else if constexpr(std::same_as<char_type,char16_t>)
			return details::copy_string_literal(u"ms",it);
		else if constexpr(std::same_as<char_type,char32_t>)
			return details::copy_string_literal(U"ms",it);
		else
			return details::copy_string_literal(u8"ms",it);
	}
	else if constexpr(std::same_as<Period,std::ratio<1>>)
	{
		if constexpr(std::same_as<char_type,char>)
			*it='s';
		else if constexpr(std::same_as<char_type,wchar_t>)
			*it=L's';
		else
			*it=u8's';
		++it;
		return it;
	}
	else if constexpr(std::same_as<Period,std::ratio<60>>)
	{
		if constexpr(std::same_as<char_type,char>)
			return details::copy_string_literal("min",it);
		else if constexpr(std::same_as<char_type,wchar_t>)
			return details::copy_string_literal(L"min",it);
		else if constexpr(std::same_as<char_type,char16_t>)
			return details::copy_string_literal(u"min",it);
		else if constexpr(std::same_as<char_type,char32_t>)
			return details::copy_string_literal(U"min",it);
		else
			return details::copy_string_literal(u8"min",it);
	}
	else if constexpr(std::same_as<Period,std::ratio<3600>>)
	{
		if constexpr(std::same_as<char_type,char>)
			return details::copy_string_literal("hr",it);
		else if constexpr(std::same_as<char_type,wchar_t>)
			return details::copy_string_literal(L"hr",it);
		else if constexpr(std::same_as<char_type,char16_t>)
			return details::copy_string_literal(u"hr",it);
		else if constexpr(std::same_as<char_type,char32_t>)
			return details::copy_string_literal(U"hr",it);
		else
			return details::copy_string_literal(u8"hr",it);
	}
	else if constexpr(std::same_as<Period,std::ratio<86400>>)
	{
		if constexpr(std::same_as<char_type,char>)
			*it='d';
		else if constexpr(std::same_as<char_type,wchar_t>)
			*it=L'd';
		else
			*it=u8'd';
		++it;
		return it;
	}
	else if constexpr(std::same_as<Period,std::ratio<604800>>)
	{
		if constexpr(std::same_as<char_type,char>)
			return details::copy_string_literal("wk",it);
		else if constexpr(std::same_as<char_type,wchar_t>)
			return details::copy_string_literal(L"wk",it);
		else if constexpr(std::same_as<char_type,char16_t>)
			return details::copy_string_literal(u"wk",it);
		else if constexpr(std::same_as<char_type,char32_t>)
			return details::copy_string_literal(U"wk",it);
		else
			return details::copy_string_literal(u8"wk",it);
	}
	else if constexpr(std::same_as<Period,std::ratio<2629746>>)
	{
		if constexpr(std::same_as<char_type,char>)
			return details::copy_string_literal("mo",it);
		else if constexpr(std::same_as<char_type,wchar_t>)
			return details::copy_string_literal(L"mo",it);
		else if constexpr(std::same_as<char_type,char16_t>)
			return details::copy_string_literal(u"mo",it);
		else if constexpr(std::same_as<char_type,char32_t>)
			return details::copy_string_literal(U"mo",it);
		else
			return details::copy_string_literal(u8"mo",it);
	}
	else if constexpr(std::same_as<Period,std::ratio<31556952>>)
	{
		if constexpr(std::same_as<char_type,char>)
			return details::copy_string_literal("yr",it);
		else if constexpr(std::same_as<char_type,wchar_t>)
			return details::copy_string_literal(L"yr",it);
		else if constexpr(std::same_as<char_type,char16_t>)
			return details::copy_string_literal(u"yr",it);
		else if constexpr(std::same_as<char_type,char32_t>)
			return details::copy_string_literal(U"yr",it);
		else
			return details::copy_string_literal(u8"yr",it);
	}
}



#if __cpp_lib_chrono >= 201907L || __GNUC__>= 11

template<std::integral char_type>
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<char_type,std::chrono::year>) noexcept
{
	return print_reserve_size(io_reserve_type<char_type,int>);
}

template<std::integral char_type,std::random_access_iterator Iter>
inline constexpr Iter print_reserve_define(io_reserve_type_t<char_type,std::chrono::year>,Iter it,std::chrono::year year) noexcept
{
	return details::chrono_year_impl(it,static_cast<int>(year));
}

template<std::integral char_type>
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<char_type,std::chrono::month>) noexcept
{
	return print_reserve_size(io_reserve_type<char_type,unsigned>);
}

template<std::integral char_type,std::random_access_iterator Iter>
inline constexpr Iter print_reserve_define(io_reserve_type_t<char_type,std::chrono::month>,Iter it,std::chrono::month m) noexcept
{
	return details::chrono_two_digits_impl(it,static_cast<unsigned>(m));
}

template<std::integral char_type>
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<char_type,std::chrono::day>) noexcept
{
	return print_reserve_size(io_reserve_type<char_type,unsigned>);
}

template<std::integral char_type,std::random_access_iterator Iter>
inline constexpr Iter print_reserve_define(io_reserve_type_t<char_type,std::chrono::day>,Iter it,std::chrono::day m) noexcept
{
	return details::chrono_two_digits_impl(it,static_cast<unsigned>(m));
}

template<std::integral char_type>
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<char_type,std::chrono::weekday>) noexcept
{
	return print_reserve_size(io_reserve_type<char_type,unsigned>);
}

template<std::integral char_type,std::random_access_iterator Iter>
inline constexpr Iter print_reserve_define(io_reserve_type_t<char_type,std::chrono::weekday>,Iter it,std::chrono::weekday wkd) noexcept
{
	return details::chrono_one_digit_impl(it,static_cast<unsigned>(wkd.iso_encoding()));
}


template<std::integral char_type>
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<char_type,std::chrono::month_day>) noexcept
{
	return print_reserve_size(io_reserve_type<char_type,std::chrono::month>)+print_reserve_size(io_reserve_type<char_type,std::chrono::day>)+3;
}

template<std::integral char_type,std::random_access_iterator Iter>
inline constexpr Iter print_reserve_define(io_reserve_type_t<char_type,std::chrono::month_day>,Iter it,std::chrono::month_day md) noexcept
{
	if constexpr(std::same_as<char_type,char>)
		it=details::copy_string_literal("--",it);
	else if constexpr(std::same_as<char_type,wchar_t>)
		it=details::copy_string_literal(L"--",it);
	else if constexpr(std::same_as<char_type,char16_t>)
		it=details::copy_string_literal(u"--",it);
	else if constexpr(std::same_as<char_type,char32_t>)
		it=details::copy_string_literal(U"--",it);
	else
		it=details::copy_string_literal(u8"--",it);
	it=print_reserve_define(io_reserve_type<char_type,std::chrono::month>,it,md.month());
	if constexpr(std::same_as<char_type,char>)
		*it='-';
	else if constexpr(std::same_as<char_type,wchar_t>)
		*it=L'-';
	else
		*it=u8'-';
	++it;
	return print_reserve_define(io_reserve_type<char_type,std::chrono::day>,it,md.day());
}


template<std::integral char_type>
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<char_type,std::chrono::year_month>) noexcept
{
	return print_reserve_size(io_reserve_type<char_type,std::chrono::year>)+print_reserve_size(io_reserve_type<char_type,std::chrono::month>)+1;
}

template<std::integral char_type,std::random_access_iterator Iter>
inline constexpr Iter print_reserve_define(io_reserve_type_t<char_type,std::chrono::year_month>,Iter it,std::chrono::year_month ym) noexcept
{
	it=print_reserve_define(io_reserve_type<char_type,std::chrono::year>,it,ym.year());
	if constexpr(std::same_as<char_type,char>)
		*it='-';
	else if constexpr(std::same_as<char_type,wchar_t>)
		*it=L'-';
	else
		*it=u8'-';
	++it;
	return print_reserve_define(io_reserve_type<char_type,std::chrono::month>,it,ym.month());
}

template<std::integral char_type>
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<char_type,std::chrono::year_month_day>) noexcept
{
	return print_reserve_size(io_reserve_type<char_type,std::chrono::year_month>)+print_reserve_size(io_reserve_type<char_type,std::chrono::day>)+1;
}

template<std::integral char_type,std::random_access_iterator Iter>
inline constexpr Iter print_reserve_define(io_reserve_type_t<char_type,std::chrono::year_month_day>,Iter it,std::chrono::year_month_day ymd) noexcept
{
	it=print_reserve_define(io_reserve_type<char_type,std::chrono::year>,it,ymd.year());
	if constexpr(std::same_as<char_type,char>)
		*it='-';
	else if constexpr(std::same_as<char_type,wchar_t>)
		*it=L'-';
	else
		*it=u8'-';
	++it;
	it=print_reserve_define(io_reserve_type<char_type,std::chrono::month>,it,ymd.month());
	if constexpr(std::same_as<char_type,char>)
		*it='-';
	else if constexpr(std::same_as<char_type,wchar_t>)
		*it=L'-';
	else
		*it=u8'-';
	++it;
	return print_reserve_define(io_reserve_type<char_type,std::chrono::day>,it,ymd.day());
}

template<std::integral char_type,typename duration>
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<char_type,std::chrono::hh_mm_ss<duration>>) noexcept
{
	return print_reserve_size(io_reserve_type<char_type,std::chrono::hours::rep>)+
		print_reserve_size(io_reserve_type<char_type,std::chrono::minutes::rep>)+
		print_reserve_size(io_reserve_type<char_type,std::chrono::hours::rep>)+
		print_reserve_size(io_reserve_type<char_type,duration::rep>)+4;
}

template<std::integral char_type,std::random_access_iterator Iter,typename duration>
inline constexpr Iter print_reserve_define(io_reserve_type_t<char_type,std::chrono::hh_mm_ss<duration>>,Iter it,std::chrono::hh_mm_ss<duration> hms) noexcept
{
	if(hms.is_negative())
	{
		if constexpr(std::same_as<char_type,char>)
			*it='-';
		else if constexpr(std::same_as<char_type,wchar_t>)
			*it=L'-';
		else
			*it=u8'-';
		++it;
	}
	it=details::chrono_two_digits_impl(it,hms.hours().count());
	if constexpr(std::same_as<char_type,char>)
		*it=':';
	else if constexpr(std::same_as<char_type,wchar_t>)
		*it=L':';
	else
		*it=u8':';
	++it;
	it=details::chrono_two_digits_impl(it,hms.minutes().count());
	if constexpr(std::same_as<char_type,char>)
		*it=':';
	else if constexpr(std::same_as<char_type,wchar_t>)
		*it=L':';
	else
		*it=u8':';
	++it;
	it=details::chrono_two_digits_impl(it,hms.seconds().count());
	return it;
}
#if 0
template<std::integral char_type>
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<char_type,std::chrono::time_point<std::chrono::system_clock>>) noexcept
{
	return print_reserve_size(io_reserve_type_t<char_type,int>)+1
		print_reserve_size(io_reserve_type_t<char_type,unsigned>)+
		print_reserve_size(io_reserve_type_t<char_type,unsigned>)+
		print_reserve_size(io_reserve_type_t<char_type,unsigned>)+
}
#endif
#endif

}