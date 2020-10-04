#pragma once
#include<chrono>

namespace fast_io
{
template<std::integral char_type>
constexpr std::size_t print_reserve_size(io_reserve_type_t<char_type,struct timespec>)
{
	return print_reserve_size(io_reserve_type<char_type,std::time_t>)+1+print_reserve_size(io_reserve_type<char_type,long>);
}

template<std::integral char_type,std::random_access_iterator Iter>
constexpr Iter print_reserve_define(io_reserve_type_t<char_type,struct timespec>,Iter it,struct timespec spc)
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
constexpr std::size_t print_reserve_size(io_reserve_type_t<char_type,std::chrono::duration<Rep,Period>>)
{
	return print_reserve_size(io_reserve_type<char_type,double>)+1;
}

template<std::integral char_type,std::random_access_iterator Iter,typename Rep,typename Period>
constexpr Iter print_reserve_define(io_reserve_type_t<char_type,std::chrono::duration<Rep,Period>>,Iter it,std::chrono::duration<Rep,Period> duration)
{
	*(it=print_reserve_define(io_reserve_type<char_type,double>,it,std::chrono::duration_cast<std::chrono::duration<double>>(duration).count()))=u8's';
	return ++it;
}


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

template<character_output_stream output>
inline constexpr void print_define(output& out,std::tm t)
//due to std::tm is often not thread safe. We make a copy
{
	print_freestanding(out,1900+t.tm_year,u8"✝️");
	if(t.tm_mon<9)
		put(out,u8'0');
	print_freestanding(out,1+t.tm_mon,u8"🌙");
	if(t.tm_mday<9)
		put(out,u8'0');
	print_freestanding(out,1+t.tm_mday,u8"☀️ ⛪",t.tm_wday);
	put(out,u8' ');
	if(t.tm_hour<10)
		put(out,u8'0');
	print_freestanding(out,t.tm_hour);
	put(out,u8':');
	if(t.tm_min<10)
		put(out,u8'0');
	print_freestanding(out,t.tm_min);
	put(out,u8':');
	if(t.tm_sec<10)
		put(out,u8'0');
	print_freestanding(out,t.tm_sec);
}

template<character_output_stream output>
inline constexpr void print_define(output& out,manip::chinese<std::tm const> ref)
//due to std::tm is often not thread safe. We make a copy
{
	std::tm t(ref.reference);
	print_freestanding(out,1900+t.tm_year,u8"年");
	if(t.tm_mon<9)
		put(out,u8'0');
	print_freestanding(out,1+t.tm_mon,u8"月");
	if(t.tm_mday<9)
		put(out,u8'0');
	print_freestanding(out,1+t.tm_mday,u8"日 星期");
/*
Unfortunately Chinese encoding in Unicode is not contiguous
t.tm_wday
Number	小写	大写
0	〇	零
1	一	壹
2	二	貳
3	三	叁
4	四	肆
5	五	伍
6	六	陸
7	七	柒
8	八	捌
9	九	玖
https://en.wikibooks.org/wiki/Written_Chinese/Numbers
*/
	switch(t.tm_wday)
	{
	case 0:
		print_freestanding(out,u8"日");
	break;
	case 1:
		print_freestanding(out,u8"一");
	break;
	case 2:
		print_freestanding(out,u8"二");
	break;
	case 3:
		print_freestanding(out,u8"三");
	break;
	case 4:
		print_freestanding(out,u8"四");
	break;
	case 5:
		print_freestanding(out,u8"五");
	break;
	case 6:
		print_freestanding(out,u8"六");
	break;
	default:
		put(out,u8'?');
	}
	put(out,u8' ');
	if(t.tm_hour<10)
		put(out,u8'0');
	print_freestanding(out,t.tm_hour,u8"时");
	if(t.tm_min<10)
		put(out,u8'0');
	print_freestanding(out,t.tm_min,u8"分");
	if(t.tm_sec<10)
		put(out,u8'0');
	print_freestanding(out,t.tm_sec,u8"秒");
}

template<character_output_stream output,typename Clock,typename Duration>
inline constexpr void print_define(output& out, std::chrono::time_point<Clock,Duration> const& tmp)
{
	auto tmt(Clock::to_time_t(tmp));
	print_define(out,*localtime(std::addressof(tmt)));
}

template<character_output_stream output,typename Clock,typename Duration>
inline constexpr void print_define(output& out, manip::local<std::chrono::time_point<Clock,Duration> const> tmp)
{
	auto tmt(Clock::to_time_t(tmp.reference));
	print_define(out,*print_define(std::addressof(tmt)));
}

template<character_output_stream output,typename Clock,typename Duration>
inline constexpr void print_define(output& out, manip::utc<std::chrono::time_point<Clock,Duration> const> tmp)
{
	auto tmt(Clock::to_time_t(tmp.reference));
	print_define(out,*gmtime(std::addressof(tmt)));
}

template<character_output_stream output,typename Clock,typename Duration>
inline constexpr void print_define(output& out, manip::unix<std::chrono::time_point<Clock,Duration> const> tmp)
{
	print_define(out,std::chrono::duration_cast<std::chrono::duration<double>>(tmp.reference.time_since_epoch()));
}

template<character_output_stream output,typename Clock,typename Duration>
inline constexpr void print_define(output& out, manip::local_chinese<std::chrono::time_point<Clock,Duration> const> tmp)
{
	auto tmt(Clock::to_time_t(tmp.reference));
	print_define(out,chinese(*localtime(std::addressof(tmt))));
}

template<character_output_stream output,typename Clock,typename Duration>
inline constexpr void print_define(output& out, manip::utc_chinese<std::chrono::time_point<Clock,Duration> const> tmp)
{
	auto tmt(Clock::to_time_t(tmp.reference));
	print_define(out,chinese(*gmtime(std::addressof(tmt))));
}

/*
//current libraries have no these facilities
template<output_stream output>
inline constexpr void print_define(output& out, std::chrono::time_zone const& tmp)
{
	print_freestanding(out,tmp.name());
}
*/

}