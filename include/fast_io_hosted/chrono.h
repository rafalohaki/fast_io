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

namespace details
{

inline constexpr bool denom_is_natural_pow10(std::intmax_t denom) noexcept
{
	if(denom<0)
		return false;
	std::uintmax_t den{static_cast<std::uintmax_t>(denom)};
	for(;!(den%10u);den/=10u);
	return den==1;
}

inline constexpr std::size_t denom_natural_log10(std::uintmax_t den) noexcept
{
	std::size_t logarithm{};
	for(;!(den%10u);den/=10u)
		++logarithm;
	return logarithm;
}

template<std::uintmax_t lim,std::integral char_type,std::random_access_iterator Iter,details::my_unsigned_integral uint_type>
inline constexpr Iter subseconds_part_print_sz_impl(Iter it,uint_type nsec) noexcept
{
	if constexpr(std::numeric_limits<uint_type>::max()>=lim)
	{
		constexpr uint_type lmt{static_cast<uint_type>(lim)};
		if((!nsec)||lmt<=nsec)
			return it;
	}
	else
	{
		if(!nsec)
			return it;
	}
	if constexpr(std::same_as<char_type,char>)
		*it='.';
	else if constexpr(std::same_as<char_type,wchar_t>)
		*it=L'.';
	else
		*it=u8'.';
	++it;
	constexpr std::size_t lgsz{denom_natural_log10(lim)};
	std::size_t sz{lgsz};
	for(;nsec%10==0;--sz)
		nsec/=10;
	if constexpr(sizeof(uint_type)<=sizeof(unsigned))
		with_length_output_unsigned(it,static_cast<unsigned>(nsec),sz);
	else
		with_length_output_unsigned(it,nsec,sz);
	return it+sz;
}

template<std::integral char_type,std::random_access_iterator Iter>
inline constexpr Iter timespec_print_impl(Iter it,struct timespec spc) noexcept
{
	return subseconds_part_print_sz_impl<1000000000UL,char_type>(
		print_reserve_define(io_reserve_type<char_type,std::time_t>,it,spc.tv_sec),
		static_cast<unsigned long>(spc.tv_nsec));
}

}

template<std::integral char_type,std::random_access_iterator Iter>
constexpr Iter print_reserve_define(io_reserve_type_t<char_type,struct timespec>,Iter it,struct timespec spc) noexcept
{
	return details::timespec_print_impl<char_type>(it,spc);	
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
		||std::same_as<Period,std::milli>||std::same_as<Period,std::ratio<604800>>||
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
			*it='h';
		else if constexpr(std::same_as<char_type,wchar_t>)
			*it=L'h';
		else
			*it=u8'h';
		++it;
		return it;
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



#if __cpp_lib_chrono >= 201907L || __GLIBCXX__ >= 20201001

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


namespace details
{

template<std::integral char_type,std::random_access_iterator Iter,typename T>
inline constexpr Iter print_reserve_define_ymd_impl(Iter it,T ymd) noexcept
{
	if constexpr(requires()
	{
		ymd.year();
	})
	{
		it=chrono_year_impl(it,static_cast<int>(ymd.year()));
		if constexpr(std::same_as<char_type,char>)
			*it='-';
		else if constexpr(std::same_as<char_type,wchar_t>)
			*it=L'-';
		else
			*it=u8'-';
	}
	else
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
	}
	++it;
	it=chrono_two_digits_impl(it,static_cast<unsigned>(ymd.month()));
	if constexpr(requires()
	{
		ymd.day();
	})
	{
		if constexpr(std::same_as<char_type,char>)
			*it='-';
		else if constexpr(std::same_as<char_type,wchar_t>)
			*it=L'-';
		else
			*it=u8'-';
		++it;
		return chrono_two_digits_impl(it,static_cast<unsigned>(ymd.day()));
	}
	else
		return it;
}

}


template<std::integral char_type>
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<char_type,std::chrono::month_day>) noexcept
{
	return print_reserve_size(io_reserve_type<char_type,std::chrono::month>)+print_reserve_size(io_reserve_type<char_type,std::chrono::day>)+3;
}

template<std::integral char_type,std::random_access_iterator Iter>
inline constexpr Iter print_reserve_define(io_reserve_type_t<char_type,std::chrono::month_day>,Iter it,std::chrono::month_day md) noexcept
{
	return details::print_reserve_define_ymd_impl<char_type>(it,md);
}

template<std::integral char_type>
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<char_type,std::chrono::year_month>) noexcept
{
	return print_reserve_size(io_reserve_type<char_type,std::chrono::year>)+print_reserve_size(io_reserve_type<char_type,std::chrono::month>)+1;
}

template<std::integral char_type,std::random_access_iterator Iter>
inline constexpr Iter print_reserve_define(io_reserve_type_t<char_type,std::chrono::year_month>,Iter it,std::chrono::year_month ym) noexcept
{
	return details::print_reserve_define_ymd_impl<char_type>(it,ym);
}

template<std::integral char_type>
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<char_type,std::chrono::year_month_day>) noexcept
{
	return print_reserve_size(io_reserve_type<char_type,std::chrono::year_month>)+print_reserve_size(io_reserve_type<char_type,std::chrono::day>)+1;
}

template<std::integral char_type,std::random_access_iterator Iter>
inline constexpr Iter print_reserve_define(io_reserve_type_t<char_type,std::chrono::year_month_day>,Iter it,std::chrono::year_month_day ymd) noexcept
{
	return details::print_reserve_define_ymd_impl<char_type>(it,ymd);
}

template<std::integral char_type,details::my_integral int_type,std::intmax_t den>
requires (details::denom_is_natural_pow10(den))
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<char_type,
	std::chrono::hh_mm_ss<std::chrono::duration<int_type,std::ratio<1,den>>>>) noexcept
{
	if constexpr(den==1)
	{
		return print_reserve_size(io_reserve_type<char_type,std::chrono::hours::rep>)+
			print_reserve_size(io_reserve_type<char_type,std::chrono::minutes::rep>)+
			print_reserve_size(io_reserve_type<char_type,std::chrono::seconds::rep>)+3;
	}
	else
	{
		return print_reserve_size(io_reserve_type<char_type,std::chrono::hours::rep>)+
			print_reserve_size(io_reserve_type<char_type,std::chrono::minutes::rep>)+
			print_reserve_size(io_reserve_type<char_type,std::chrono::seconds::rep>)+
			print_reserve_size(io_reserve_type<char_type,std::intmax_t>)+4;
	}
}

namespace details
{

template<std::integral char_type,std::random_access_iterator Iter,my_integral int_type,std::intmax_t den>
inline constexpr Iter print_hh_mm_ss_reserve_define_impl(Iter it,
	std::chrono::hh_mm_ss<std::chrono::duration<int_type,std::ratio<1,den>>> hms) noexcept
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
	if constexpr(den==1)
		return it;
	else
		return subseconds_part_print_sz_impl<static_cast<std::uintmax_t>(den),char_type>(it,
			static_cast<std::make_unsigned_t<std::common_type_t<std::intmax_t,
			std::chrono::seconds::rep>>>(hms.subseconds().count()));
}
}

template<std::integral char_type,std::random_access_iterator Iter,details::my_integral int_type,std::intmax_t den>
requires (details::denom_is_natural_pow10(den))
inline constexpr Iter print_reserve_define(
	io_reserve_type_t<char_type,std::chrono::hh_mm_ss<std::chrono::duration<int_type,std::ratio<1,den>>>>,
	Iter it,std::chrono::hh_mm_ss<std::chrono::duration<int_type,std::ratio<1,den>>> hms) noexcept
{
	return details::print_hh_mm_ss_reserve_define_impl<char_type>(it,hms);
}

namespace details
{

template<std::integral char_type,std::random_access_iterator Iter,typename Duration>
inline constexpr Iter print_system_clock_reserve_impl(Iter it,std::chrono::time_point<std::chrono::system_clock,Duration> tp) noexcept
{
	std::chrono::sys_days sdy{std::chrono::time_point_cast<std::chrono::days>(tp)};
	std::chrono::year_month_day ymd(sdy);
	it=chrono_year_impl(it,static_cast<int>(ymd.year()));
	if constexpr(std::same_as<char_type,char>)
		*it='-';
	else if constexpr(std::same_as<char_type,wchar_t>)
		*it=L'-';
	else
		*it=u8'-';
	++it;
	it=chrono_two_digits_impl<true>(it,static_cast<unsigned>(ymd.month()));
	if constexpr(std::same_as<char_type,char>)
		*it='-';
	else if constexpr(std::same_as<char_type,wchar_t>)
		*it=L'-';
	else
		*it=u8'-';
	++it;
	it=chrono_two_digits_impl<true>(it,static_cast<unsigned>(ymd.day()));
	if constexpr(std::same_as<char_type,char>)
		*it='T';
	else if constexpr(std::same_as<char_type,wchar_t>)
		*it=L'T';
	else
		*it=u8'T';
	++it;
	std::chrono::hh_mm_ss hms(tp.time_since_epoch()-std::chrono::duration_cast<Duration>(sdy.time_since_epoch()));
	it=details::chrono_two_digits_impl<true>(it,hms.hours().count());
	if constexpr(std::same_as<char_type,char>)
		*it=':';
	else if constexpr(std::same_as<char_type,wchar_t>)
		*it=L':';
	else
		*it=u8':';
	++it;
	it=details::chrono_two_digits_impl<true>(it,hms.minutes().count());
	if constexpr(std::same_as<char_type,char>)
		*it=':';
	else if constexpr(std::same_as<char_type,wchar_t>)
		*it=L':';
	else
		*it=u8':';
	++it;
	it=details::chrono_two_digits_impl<true>(it,hms.seconds().count());
	if constexpr(Duration::period::num==1&&Duration::period::den!=1)
		it=subseconds_part_print_sz_impl<static_cast<std::uintmax_t>(Duration::period::den),char_type>(	
			it,static_cast<std::make_unsigned_t<std::common_type_t<std::intmax_t,
			std::chrono::seconds::rep>>>(hms.subseconds().count()));
	if constexpr(std::same_as<char_type,char>)
		*it='Z';
	else if constexpr(std::same_as<char_type,wchar_t>)
		*it=L'Z';
	else
		*it=u8'Z';
	++it;
	return it;
}

}

template<std::integral char_type,typename Duration>
requires(std::same_as<Duration,std::chrono::system_clock::duration>)
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<char_type,
	std::chrono::time_point<std::chrono::system_clock,Duration>>) noexcept
{
/*
ISO8601 Time stamp
2020-11-08T07:23:27Z
*/
	return print_reserve_size(io_reserve_type<char_type,int>)+sizeof(u8"-11-08T07:23:27Z")+1+print_reserve_size(io_reserve_type<char_type,std::intmax_t>);
}
template<std::integral char_type,std::random_access_iterator Iter,typename Duration>
requires(std::same_as<Duration,std::chrono::system_clock::duration>)
inline constexpr Iter print_reserve_define(io_reserve_type_t<char_type,
	std::chrono::time_point<std::chrono::system_clock,Duration>>
	,Iter it,std::chrono::time_point<std::chrono::system_clock,Duration> tp) noexcept
{
	return details::print_system_clock_reserve_impl<char_type>(it,tp);
}

#endif

}