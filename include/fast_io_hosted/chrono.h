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
	*it=u8'.';
	++it;
	std::size_t sz{};
	for(;nsec%10==0;++sz)
		nsec/=10;
	sz=9-sz;
	auto res{it+sz};
	it=res;
	--it;
	for(std::size_t i{};i!=sz;++i)
	{
		unsigned long const temp(nsec/10);
		*it=static_cast<char8_t>(static_cast<char8_t>(nsec%10)+u8'0');
		--it;
		nsec=temp;
	}
	return res;
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

/*

Use the emoji characters of Chinese Semantics for date time since English is a disaster for this.
No order. Everything is represented by word instead of number
2019✝️12🌙30☀️ ⛪6 19:10:03
2019年12月30日 星期六 19:10:03

Literal Meaning:
✝️ 年       Year
the period of 365 days (or 366 days in leap years) starting from the first of January, used for reckoning time in ordinary affairs.
🌙 月 Moon Month
☀️ 日 Sun  month day
⛪星期     Week  day
Order:From High to Low
*/

namespace details
{

template<std::integral ch_type,std::random_access_iterator Iter>
inline constexpr Iter year_reserve_define_impl(Iter it,std::chrono::year y) noexcept
{
	it=print_reserve_define(io_reserve_type<ch_type,int>,it,static_cast<int>(y));
	if constexpr(std::same_as<ch_type,char>)
		return copy_string_literal("✝️",it);
#ifndef __MSDOS__
	else if constexpr(std::same_as<ch_type,wchar_t>)
		return copy_string_literal(L"✝️",it);
#endif
	else if constexpr(std::same_as<ch_type,char8_t>)
		return copy_string_literal(u8"✝️",it);
	else if constexpr(std::same_as<ch_type,char16_t>)
		return copy_string_literal(u"✝️",it);
	else if constexpr(std::same_as<ch_type,char32_t>)
		return copy_string_literal(U"✝️",it);
}

template<std::integral ch_type,std::random_access_iterator Iter>
inline constexpr Iter month_reserve_define_impl(Iter it,std::chrono::month m) noexcept
{
	it=print_reserve_define(io_reserve_type<ch_type,unsigned>,it,static_cast<unsigned>(m));
	if constexpr(std::same_as<ch_type,char>)
		return copy_string_literal("🌙",it);
#ifndef __MSDOS__
	else if constexpr(std::same_as<ch_type,wchar_t>)
		return copy_string_literal(L"🌙",it);
#endif
	else if constexpr(std::same_as<ch_type,char8_t>)
		return copy_string_literal(u8"🌙",it);
	else if constexpr(std::same_as<ch_type,char16_t>)
		return copy_string_literal(u"🌙",it);
	else if constexpr(std::same_as<ch_type,char32_t>)
		return copy_string_literal(U"🌙",it);
}

template<std::integral ch_type,std::random_access_iterator Iter>
inline constexpr Iter day_reserve_define_impl(Iter it,std::chrono::day m) noexcept
{
	it=print_reserve_define(io_reserve_type<ch_type,unsigned>,it,static_cast<unsigned>(m));
	if constexpr(std::same_as<ch_type,char>)
		return copy_string_literal("☀️",it);
#ifndef __MSDOS__
	else if constexpr(std::same_as<ch_type,wchar_t>)
		return copy_string_literal(L"☀️",it);
#endif
	else if constexpr(std::same_as<ch_type,char8_t>)
		return copy_string_literal(u8"☀️",it);
	else if constexpr(std::same_as<ch_type,char16_t>)
		return copy_string_literal(u"☀️",it);
	else if constexpr(std::same_as<ch_type,char32_t>)
		return copy_string_literal(U"☀️",it);
}

template<std::integral ch_type,std::random_access_iterator Iter>
inline constexpr Iter weekday_reserve_define_impl(Iter it,std::chrono::weekday m) noexcept
{
if constexpr(std::same_as<ch_type,char>)
		return print_reserve_define(io_reserve_type<ch_type,unsigned>,copy_string_literal("⛪",it),m.c_encoding());
#ifndef __MSDOS__
	else if constexpr(std::same_as<ch_type,wchar_t>)
		return print_reserve_define(io_reserve_type<ch_type,unsigned>,copy_string_literal(L"⛪",it),m.c_encoding());
#endif
	else if constexpr(std::same_as<ch_type,char8_t>)
		return print_reserve_define(io_reserve_type<ch_type,unsigned>,copy_string_literal(u8"⛪",it),m.c_encoding());
	else if constexpr(std::same_as<ch_type,char16_t>)
		return print_reserve_define(io_reserve_type<ch_type,unsigned>,copy_string_literal(u"⛪",it),m.c_encoding());
	else if constexpr(std::same_as<ch_type,char32_t>)
		return print_reserve_define(io_reserve_type<ch_type,unsigned>,copy_string_literal(U"⛪",it),m.c_encoding());
}

template<std::integral ch_type,std::random_access_iterator Iter,std::integral index_type>
inline constexpr Iter index_reserve_define_impl(Iter it,index_type m) noexcept
{
	if constexpr(exec_charset_is_ebcdic<ch_type>())
		*it=0x5B;
	else
		*it=u8'[';
	it=print_reserve_define(io_reserve_type<ch_type,unsigned>,++it,m);
	if constexpr(exec_charset_is_ebcdic<ch_type>())
		*it=0x5D;
	else
		*it=u8']';
	return ++it;
}

template<std::integral ch_type,std::random_access_iterator Iter>
inline constexpr Iter weekday_indexed_reserve_define_impl(Iter it,std::chrono::weekday_indexed m) noexcept
{
	return weekday_reserve_define_impl<ch_type>(index_reserve_define_impl<ch_type>(it,m.index()),m.weekday());
}

template<std::integral ch_type,std::random_access_iterator Iter>
inline constexpr Iter last_reserve_define_impl(Iter it)
{
	if constexpr(std::same_as<ch_type,char>)
		return copy_string_literal("[last]",it);
#ifndef __MSDOS__
	else if constexpr(std::same_as<ch_type,wchar_t>)
		return copy_string_literal(L"[last]",it);
#endif
	else if constexpr(std::same_as<ch_type,char8_t>)
		return copy_string_literal(u8"[last]",it);
	else if constexpr(std::same_as<ch_type,char16_t>)
		return copy_string_literal(u"[last]",it);
	else if constexpr(std::same_as<ch_type,char32_t>)
		return copy_string_literal(U"[last]",it);
}

template<std::integral ch_type,std::random_access_iterator Iter>
inline constexpr Iter weekday_last_reserve_define_impl(Iter it,std::chrono::weekday_last m) noexcept
{
	return weekday_reserve_define_impl<ch_type>(last_reserve_define_impl<ch_type>(it),m.weekday());
}

template<std::integral ch_type,std::random_access_iterator Iter>
inline constexpr Iter month_day_reserve_define_impl(Iter it,std::chrono::month_day m) noexcept
{
	return day_reserve_define_impl<ch_type>(month_reserve_define_impl<ch_type>(it,m.month(),m.day()));
}

template<std::integral ch_type,std::random_access_iterator Iter>
inline constexpr Iter month_weekday_reserve_define_impl(Iter it,std::chrono::month_weekday m) noexcept
{
	return weekday_indexed_reserve_define_impl<ch_type>(month_reserve_define_impl<ch_type>(it,m.month()),m.weekday_indexed());
}

template<std::integral ch_type,std::random_access_iterator Iter>
inline constexpr Iter month_weekday_last_reserve_define_impl(Iter it,std::chrono::month_weekday_last m) noexcept
{
	return weekday_last_reserve_define_impl<ch_type>(month_reserve_define_impl<ch_type>(it,m.month()),m.weekday_last());
}

template<std::integral ch_type,std::random_access_iterator Iter>
inline constexpr Iter year_month_reserve_define_impl(Iter it,std::chrono::year_month m) noexcept
{
	return month_reserve_define_impl<ch_type>(year_month_reserve_define_impl<ch_type>(it,m.year()),m.month());
}

template<std::integral ch_type,std::random_access_iterator Iter>
inline constexpr Iter year_month_day_reserve_define_impl(Iter it,std::chrono::year_month_day ymd) noexcept
{
	return day_reserve_define_impl<ch_type>(month_reserve_define_impl<ch_type>(year_reserve_define_impl<ch_type>(it,ymd.year()),ymd.month()),ymd.day());
}

template<std::integral ch_type,std::random_access_iterator Iter>
inline constexpr Iter month_day_last_reserve_define_impl(Iter it,std::chrono::month_day_last mdl) noexcept
{
	it=last_reserve_define_impl<ch_type>(month_reserve_define_impl<ch_type>(it,mdl.month()));
	if constexpr(std::same_as<ch_type,char>)
		return copy_string_literal("☀️",it);
#ifndef __MSDOS__
	else if constexpr(std::same_as<ch_type,wchar_t>)
		return copy_string_literal(L"☀️",it);
#endif
	else if constexpr(std::same_as<ch_type,char8_t>)
		return copy_string_literal(u8"☀️",it);
	else if constexpr(std::same_as<ch_type,char16_t>)
		return copy_string_literal(u"☀️",it);
	else if constexpr(std::same_as<ch_type,char32_t>)
		return copy_string_literal(U"☀️",it);
}

template<std::integral ch_type,std::random_access_iterator Iter>
inline constexpr Iter year_month_day_last_reserve_define_impl(Iter it,std::chrono::year_month_day_last ymdl) noexcept
{
	return month_day_last_reserve_define_impl<ch_type>(year_reserve_define_impl<ch_type>(it,ymdl.year()),ymdl.month_day_last());
}

template<std::integral ch_type,std::random_access_iterator Iter>
inline constexpr Iter year_month_weekday_reserve_define_impl(Iter it,std::chrono::year_month_weekday ymdl) noexcept
{
	return weekday_indexed_reserve_define_impl<ch_type>(
		month_reserve_define_impl<ch_type>(year_reserve_define_impl<ch_type>(it,ymdl.year()),
		ymdl.month()),ymdl.weekday_indexed());
}

template<std::integral ch_type,std::random_access_iterator Iter>
inline constexpr Iter year_month_weekday_last_reserve_define_impl(Iter it,std::chrono::year_month_weekday_last ymdl) noexcept
{
	return weekday_last_reserve_define_impl<ch_type>(
		month_reserve_define_impl<ch_type>(year_reserve_define_impl<ch_type>(it,ymdl.year()),
		ymdl.month()),ymdl.weekday_last());
}

}

template<std::integral ch_type>
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<ch_type,std::chrono::year>) noexcept
{
	std::size_t size{print_reserve_size(io_reserve_type<ch_type,unsigned>)};
	if constexpr(std::same_as<ch_type,char>)
		return size+sizeof("✝️")-1;
#ifndef __MSDOS__
	else if constexpr(std::same_as<ch_type,wchar_t>)
		return size+sizeof(L"✝️")-1;
#endif
	else if constexpr(std::same_as<ch_type,char8_t>)
		return size+sizeof(u8"✝️")-1;
	else if constexpr(std::same_as<ch_type,char16_t>)
		return size+sizeof(u"✝️")-1;
	else if constexpr(std::same_as<ch_type,char32_t>)
		return size+sizeof(U"✝️")-1;
}

template<std::integral ch_type,std::random_access_iterator Iter>
inline constexpr Iter print_reserve_define(io_reserve_type_t<ch_type,std::chrono::year>,Iter it,std::chrono::year y) noexcept
{
	return details::year_reserve_define_impl<ch_type>(it,y);
}

template<std::integral ch_type>
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<ch_type,std::chrono::month>) noexcept
{
	std::size_t size{print_reserve_size(io_reserve_type<ch_type,unsigned>)};
	if constexpr(std::same_as<ch_type,char>)
		return size+sizeof("🌙")-1;
#ifndef __MSDOS__
	else if constexpr(std::same_as<ch_type,wchar_t>)
		return size+sizeof(L"🌙")-1;
#endif
	else if constexpr(std::same_as<ch_type,char8_t>)
		return size+sizeof(u8"🌙")-1;
	else if constexpr(std::same_as<ch_type,char16_t>)
		return size+sizeof(u"🌙")-1;
	else if constexpr(std::same_as<ch_type,char32_t>)
		return size+sizeof(U"🌙")-1;
}

template<std::integral ch_type,std::random_access_iterator Iter>
inline constexpr Iter print_reserve_define(io_reserve_type_t<ch_type,std::chrono::month>,Iter it,std::chrono::month m) noexcept
{
	return details::month_reserve_define_impl<ch_type>(it,m);
}

template<std::integral ch_type>
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<ch_type,std::chrono::day>) noexcept
{
	std::size_t size{print_reserve_size(io_reserve_type<ch_type,unsigned>)};
	if constexpr(std::same_as<ch_type,char>)
		return size+sizeof("☀️")-1;
#ifndef __MSDOS__
	else if constexpr(std::same_as<ch_type,wchar_t>)
		return size+sizeof(L"☀️")-1;
#endif
	else if constexpr(std::same_as<ch_type,char8_t>)
		return size+sizeof(u8"☀️")-1;
	else if constexpr(std::same_as<ch_type,char16_t>)
		return size+sizeof(u"☀️")-1;
	else if constexpr(std::same_as<ch_type,char32_t>)
		return size+sizeof(U"☀️")-1;
}

template<std::integral ch_type,std::random_access_iterator Iter>
inline constexpr Iter print_reserve_define(io_reserve_type_t<ch_type,std::chrono::day>,Iter it,std::chrono::day d) noexcept
{
	return details::day_reserve_define_impl<ch_type>(it,d);
}

template<std::integral ch_type>
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<ch_type,std::chrono::weekday>) noexcept
{
	std::size_t size{print_reserve_size(io_reserve_type<ch_type,unsigned>)};
	if constexpr(std::same_as<ch_type,char>)
		return size+sizeof("⛪")-1;
#ifndef __MSDOS__
	else if constexpr(std::same_as<ch_type,wchar_t>)
		return size+sizeof(L"⛪")-1;
#endif
	else if constexpr(std::same_as<ch_type,char8_t>)
		return size+sizeof(u8"⛪")-1;
	else if constexpr(std::same_as<ch_type,char16_t>)
		return size+sizeof(u"⛪")-1;
	else if constexpr(std::same_as<ch_type,char32_t>)
		return size+sizeof(U"⛪")-1;
}

template<std::integral ch_type,std::random_access_iterator Iter>
inline constexpr Iter print_reserve_define(io_reserve_type_t<ch_type,std::chrono::weekday>,Iter it,std::chrono::weekday d) noexcept
{
	return details::weekday_reserve_define_impl<ch_type>(it,d);
}

template<std::integral ch_type>
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<ch_type,std::chrono::weekday_indexed>) noexcept
{
	return print_reserve_size(io_reserve_type<ch_type,std::chrono::weekday>)+print_reserve_size(io_reserve_type<ch_type,unsigned>)+2;
}

template<std::integral ch_type,std::random_access_iterator Iter>
inline constexpr Iter print_reserve_define(io_reserve_type_t<ch_type,std::chrono::weekday>,Iter it,std::chrono::weekday_indexed d) noexcept
{
	return details::weekday_indexed_reserve_define_impl<ch_type>(it,d);
}

template<std::integral ch_type>
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<ch_type,std::chrono::weekday_last>) noexcept
{
	return print_reserve_size(io_reserve_type<ch_type,std::chrono::weekday>)+6;
}

template<std::integral ch_type,std::random_access_iterator Iter>
inline constexpr Iter print_reserve_define(io_reserve_type_t<ch_type,std::chrono::weekday_last>,Iter it,std::chrono::weekday_last d) noexcept
{
	return details::weekday_last_reserve_define_impl<ch_type>(it,d);
}

template<std::integral ch_type>
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<ch_type,std::chrono::month_day>) noexcept
{
	return print_reserve_size(io_reserve_type<ch_type,std::chrono::month>)+print_reserve_size(io_reserve_type<ch_type,std::chrono::day>);
}

template<std::integral ch_type,std::random_access_iterator Iter>
inline constexpr Iter print_reserve_define(io_reserve_type_t<ch_type,std::chrono::month_day>,Iter it,std::chrono::month_day md) noexcept
{
	return details::month_day_reserve_define_impl<ch_type>(it,md);
}

template<std::integral ch_type>
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<ch_type,std::chrono::month_day_last>) noexcept
{
	constexpr std::size_t size{print_reserve_size(io_reserve_type<ch_type,std::chrono::month>)+6};
	if constexpr(std::same_as<ch_type,char>)
		return size+sizeof("☀️")-1;
#ifndef __MSDOS__
	else if constexpr(std::same_as<ch_type,wchar_t>)
		return size+sizeof(L"☀️")-1;
#endif
	else if constexpr(std::same_as<ch_type,char8_t>)
		return size+sizeof(u8"☀️")-1;
	else if constexpr(std::same_as<ch_type,char16_t>)
		return size+sizeof(u"☀️")-1;
	else if constexpr(std::same_as<ch_type,char32_t>)
		return size+sizeof(U"☀️")-1;
}

template<std::integral ch_type,std::random_access_iterator Iter>
inline constexpr Iter print_reserve_define(io_reserve_type_t<ch_type,std::chrono::month_day_last>,Iter it,std::chrono::month_day_last md) noexcept
{
	return details::month_day_last_reserve_define_impl<ch_type>(it,md);
}

template<std::integral ch_type>
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<ch_type,std::chrono::month_weekday>) noexcept
{
	return print_reserve_size(io_reserve_type<ch_type,std::chrono::month>)+print_reserve_size(io_reserve_type<ch_type,std::chrono::weekday_indexed>);
}

template<std::integral ch_type,std::random_access_iterator Iter>
inline constexpr Iter print_reserve_define(io_reserve_type_t<ch_type,std::chrono::month_weekday>,Iter it,std::chrono::month_weekday md) noexcept
{
	return details::month_weekday_reserve_define_impl<ch_type>(it,md);
}

template<std::integral ch_type>
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<ch_type,std::chrono::month_weekday_last>) noexcept
{
	return print_reserve_size(io_reserve_type<ch_type,std::chrono::month>)+print_reserve_size(io_reserve_type<ch_type,std::chrono::weekday_indexed>);
}

template<std::integral ch_type,std::random_access_iterator Iter>
inline constexpr Iter print_reserve_define(io_reserve_type_t<ch_type,std::chrono::month_weekday_last>,Iter it,std::chrono::month_weekday_last md) noexcept
{
	return details::month_weekday_last_reserve_define_impl<ch_type>(it,md);
}

template<std::integral ch_type>
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<ch_type,std::chrono::year_month>) noexcept
{
	return print_reserve_size(io_reserve_type<ch_type,std::chrono::year>)+print_reserve_size(io_reserve_type<ch_type,std::chrono::month>);
}

template<std::integral ch_type,std::random_access_iterator Iter>
inline constexpr Iter print_reserve_define(io_reserve_type_t<ch_type,std::chrono::year_month>,Iter it,std::chrono::year_month ym) noexcept
{
	return details::year_month_reserve_define_impl<ch_type>(it,ym);
}

template<std::integral ch_type>
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<ch_type,std::chrono::year_month_day>) noexcept
{
	return print_reserve_size(io_reserve_type<ch_type,std::chrono::year>)+print_reserve_size(io_reserve_type<ch_type,std::chrono::month>);
}

template<std::integral ch_type,std::random_access_iterator Iter>
inline constexpr Iter print_reserve_define(io_reserve_type_t<ch_type,std::chrono::year_month_day>,Iter it,std::chrono::year_month_day ymd) noexcept
{
	return details::year_month_day_reserve_define_impl<ch_type>(it,ymd);
}

template<std::integral ch_type>
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<ch_type,std::chrono::year_month_day_last>) noexcept
{
	return print_reserve_size(io_reserve_type<ch_type,std::chrono::year>)+print_reserve_size(io_reserve_type<ch_type,std::chrono::month_day_last>);
}

template<std::integral ch_type,std::random_access_iterator Iter>
inline constexpr Iter print_reserve_define(io_reserve_type_t<ch_type,std::chrono::year_month_day_last>,Iter it,std::chrono::year_month_day_last ymdl) noexcept
{
	return details::year_month_day_last_reserve_define_impl<ch_type>(it,ymdl);
}

template<std::integral ch_type>
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<ch_type,std::chrono::year_month_weekday>) noexcept
{
	return print_reserve_size(io_reserve_type<ch_type,std::chrono::year>)+
		print_reserve_size(io_reserve_type<ch_type,std::chrono::month>)+
		print_reserve_size(io_reserve_type<ch_type,std::chrono::weekday_indexed>);
}

template<std::integral ch_type,std::random_access_iterator Iter>
inline constexpr Iter print_reserve_define(io_reserve_type_t<ch_type,std::chrono::year_month_weekday>,Iter it,std::chrono::year_month_weekday ymw) noexcept
{
	return details::year_month_weekday_reserve_define_impl<ch_type>(it,ymw);
}

template<std::integral ch_type>
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<ch_type,std::chrono::year_month_weekday_last>) noexcept
{
	return print_reserve_size(io_reserve_type<ch_type,std::chrono::year>)+
		print_reserve_size(io_reserve_type<ch_type,std::chrono::month>)+
		print_reserve_size(io_reserve_type<ch_type,std::chrono::weekday_last>);
}

template<std::integral ch_type,std::random_access_iterator Iter>
inline constexpr Iter print_reserve_define(io_reserve_type_t<ch_type,std::chrono::year_month_weekday_last>,Iter it,std::chrono::year_month_weekday_last ymwl) noexcept
{
	return details::year_month_weekday_last_reserve_define_impl<ch_type>(it,ymwl);
}

#endif

}