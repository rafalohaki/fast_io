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

//We use seconds since seconds is the standard unit of SI
//Use my own tweaked ryu algorithm for counting seconds
template<std::integral char_type,typename Rep,typename Period>
constexpr std::size_t print_reserve_size(io_reserve_type_t<char_type,std::chrono::duration<Rep,Period>>) noexcept
{
	return print_reserve_size(io_reserve_type<char_type,double>)+1;
}

template<std::integral char_type,std::random_access_iterator Iter,typename Rep,typename Period>
constexpr Iter print_reserve_define(io_reserve_type_t<char_type,std::chrono::duration<Rep,Period>>,Iter it,std::chrono::duration<Rep,Period> duration) noexcept
{
	*(it=print_reserve_define(io_reserve_type<char_type,double>,it,std::chrono::duration_cast<std::chrono::duration<double>>(duration).count()))=u8's';
	return ++it;
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

#endif

}