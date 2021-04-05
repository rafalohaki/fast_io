#pragma once

namespace fast_io
{
namespace details
{
inline constexpr auto posix_clock_id_to_native_value(posix_clock_id pcid)
{
#ifdef _WIN32
	switch(pcid)
	{
	case posix_clock_id::realtime:
	case posix_clock_id::tai:
	return posix_clock_id::tai;
	break;
	case posix_clock_id::monotonic:
	return posix_clock_id::monotonic;
	break;
	default:
		throw_win32_error(0x00000057);
	};
#else
	switch(pcid)
	{
#ifdef CLOCK_REALTIME
	case posix_clock_id::realtime:
	return CLOCK_REALTIME;
	break;
#endif
#ifdef CLOCK_REALTIME_ALARM
	case posix_clock_id::realtime_alarm:
	return CLOCK_REALTIME_ALARM;
	break;
#elif defined(CLOCK_REALTIME)
	case posix_clock_id::realtime_alarm:
	return CLOCK_REALTIME;
	break;
#endif
#ifdef CLOCK_REALTIME_COARSE
	case posix_clock_id::realtime_coarse:
	return CLOCK_REALTIME_COARSE;
	break;
#elif defined(CLOCK_REALTIME)
	case posix_clock_id::realtime_coarse:
	return CLOCK_REALTIME;
	break;
#endif
#ifdef CLOCK_TAI
	case posix_clock_id::tai:
	return CLOCK_TAI;
	break;
#elif defined(CLOCK_REALTIME)
	case posix_clock_id::tai:
	return CLOCK_REALTIME;
	break;
#endif
#ifdef CLOCK_MONOTONIC
	case posix_clock_id::monotonic:
	return CLOCK_MONOTONIC;
	break;
#endif
#ifdef CLOCK_MONOTONIC_COARSE
	case posix_clock_id::monotonic_coarse:
	return CLOCK_MONOTONIC_COARSE;
	break;
#endif
#ifdef CLOCK_MONOTONIC_RAW
	case posix_clock_id::monotonic_raw:
	return CLOCK_MONOTONIC_RAW;
	break;
#endif
#ifdef CLOCK_MONOTONIC_RAW_APPROX
	case posix_clock_id::monotonic_raw_approx:
	return CLOCK_MONOTONIC_RAW_APPROX;
	break;
#endif
#ifdef CLOCK_BOOTTIME
	case posix_clock_id::boottime:
	return CLOCK_BOOTTIME;
	break;
#else
#ifdef CLOCK_MONOTONIC
	case posix_clock_id::boottime:
	return CLOCK_MONOTONIC;
	break;
#endif
#endif
#ifdef CLOCK_BOOTTIME_ALARM
	case posix_clock_id::boottime_alarm:
	return CLOCK_BOOTTIME_ALARM;
	break;
#endif
#ifdef CLOCK_UPTIME_RAW
	case posix_clock_id::uptime_raw:
	return CLOCK_UPTIME_RAW;
	break;
#endif
#ifdef CLOCK_UPTIME_RAW_APPROX
	case posix_clock_id::uptime_raw_approx:
	return CLOCK_UPTIME_RAW_APPROX;
	break;
#endif
#ifdef CLOCK_PROCESS_CPUTIME_ID
	case posix_clock_id::process_cputime_id:
	return CLOCK_PROCESS_CPUTIME_ID;
	break;
#endif
#ifdef CLOCK_THREAD_CPUTIME_ID
	case posix_clock_id::thread_cputime_id:
	return CLOCK_THREAD_CPUTIME_ID;
	break;
#endif
	default:
		throw_posix_error(EINVAL);
	};
#endif
}
}

inline
#if defined(_WIN32) || defined(__MSDOS__)
constexpr
#endif
unix_timestamp posix_clock_getres([[maybe_unused]] posix_clock_id pclk_id)
{
#ifdef _WIN32
	switch(pclk_id)
	{
	case posix_clock_id::realtime:
	case posix_clock_id::realtime_alarm:
	case posix_clock_id::realtime_coarse:
	case posix_clock_id::tai:
	case posix_clock_id::process_cputime_id:
	case posix_clock_id::thread_cputime_id:
	case posix_clock_id::monotonic:
	case posix_clock_id::monotonic_coarse:
	case posix_clock_id::monotonic_raw:
	case posix_clock_id::boottime:
	{
		constexpr uintiso_t mul_factor{uintiso_subseconds_per_second/10000000u};
		return {0,mul_factor};
	}
	break;
	default:
		throw_win32_error(0x00000057);
	}
#elif defined(__MSDOS__)
	switch(pclk_id)
	{
	case posix_clock_id::realtime:
	case posix_clock_id::realtime_alarm:
	case posix_clock_id::realtime_coarse:
	case posix_clock_id::tai:
	case posix_clock_id::monotonic:
	case posix_clock_id::monotonic_coarse:
	case posix_clock_id::monotonic_raw:
	case posix_clock_id::boottime:
	{
		constexpr uintiso_t mul_factor{uintiso_subseconds_per_second/1000u};
		return {0,mul_factor};
	}
	case posix_clock_id::process_cputime_id:
	case posix_clock_id::thread_cputime_id:
	{
		constexpr uintiso_t mul_factor{uintiso_subseconds_per_second/UCLOCKS_PER_SEC};
		return {0,mul_factor};
	}
	default:
		throw_posix_error(EINVAL);
	};
#elif !defined(__NEWLIB__) || defined(_POSIX_TIMERS)
	auto clk{details::posix_clock_id_to_native_value(pclk_id)};
	struct timespec res;
//vdso
	if(::clock_getres(clk,std::addressof(res))<0)
		throw_posix_error();
	constexpr uintiso_t mul_factor{uintiso_subseconds_per_second/1000000000u};
	return {static_cast<intiso_t>(res.tv_sec),static_cast<uintiso_t>(res.tv_nsec)*mul_factor};
#else
	throw_posix_error(EINVAL);
#endif
}
#ifdef _WIN32
namespace win32::details
{

inline unix_timestamp win32_posix_clock_gettime_tai_impl() noexcept
{
	win32::filetime ftm;
	win32::GetSystemTimePreciseAsFileTime(std::addressof(ftm));
	return static_cast<unix_timestamp>(to_win32_timestamp(ftm));
}

inline unix_timestamp win32_posix_clock_gettime_boottime_impl()
{
	std::uint64_t ftm{};
	if(!win32::QueryUnbiasedInterruptTime(std::addressof(ftm)))
		throw_win32_error();
	constexpr uintiso_t mul_factor{uintiso_subseconds_per_second/10000000u};
	std::uint64_t seconds{ftm/10000000ULL};
	std::uint64_t subseconds{ftm%10000000ULL};
	return {static_cast<intiso_t>(seconds),static_cast<uintiso_t>(subseconds*mul_factor)};
}

template<bool is_thread>
inline unix_timestamp win32_posix_clock_gettime_process_or_thread_time_impl()
{
	win32::filetime creation_time,exit_time,kernel_time,user_time;
	if constexpr(is_thread)
	{
		auto hthread{bit_cast<void*>(std::intptr_t(-2))};
		if(!win32::GetThreadTimes(hthread,std::addressof(creation_time),
		std::addressof(exit_time),
		std::addressof(kernel_time),
		std::addressof(user_time)))
			throw_win32_error();
	}
	else
	{
		auto hprocess{bit_cast<void*>(std::intptr_t(-1))};
		if(!win32::GetProcessTimes(hprocess,std::addressof(creation_time),
		std::addressof(exit_time),
		std::addressof(kernel_time),
		std::addressof(user_time)))
			throw_win32_error();
	}
	std::uint64_t ftm{win32::filetime_to_uint64_t(kernel_time)+
		win32::filetime_to_uint64_t(user_time)};
	std::uint64_t seconds{ftm/10000000ULL};
	std::uint64_t subseconds{ftm%10000000ULL};
	constexpr uintiso_t mul_factor{uintiso_subseconds_per_second/10000000u};
	return {static_cast<intiso_t>(seconds),static_cast<uintiso_t>(subseconds*mul_factor)};
}

}

#endif

inline unix_timestamp posix_clock_gettime([[maybe_unused]] posix_clock_id pclk_id)
{
#ifdef _WIN32
	switch(pclk_id)
	{
	case posix_clock_id::realtime:
	case posix_clock_id::realtime_alarm:
	case posix_clock_id::realtime_coarse:
	case posix_clock_id::tai:
		return win32::details::win32_posix_clock_gettime_tai_impl();
	case posix_clock_id::monotonic:
	case posix_clock_id::monotonic_coarse:
	case posix_clock_id::monotonic_raw:
	case posix_clock_id::boottime:
		return win32::details::win32_posix_clock_gettime_boottime_impl();
	case posix_clock_id::process_cputime_id:
		return win32::details::win32_posix_clock_gettime_process_or_thread_time_impl<false>();
	case posix_clock_id::thread_cputime_id:
		return win32::details::win32_posix_clock_gettime_process_or_thread_time_impl<true>();
	default:
		throw_win32_error(0x00000057);
	}
#elif defined(__MSDOS__)
	switch(pclk_id)
	{
	case posix_clock_id::realtime:
	case posix_clock_id::realtime_alarm:
	case posix_clock_id::realtime_coarse:
	case posix_clock_id::tai:
	case posix_clock_id::monotonic:
	case posix_clock_id::monotonic_coarse:
	case posix_clock_id::monotonic_raw:
	case posix_clock_id::boottime:
	{
		timeval tv;
		constexpr uintiso_t mul_factor{uintiso_subseconds_per_second/1000000u};
		if(::gettimeofday(std::addressof(tv), nullptr)<0)
			throw_posix_error();
		return {static_cast<intiso_t>(tv.tv_sec),static_cast<uintiso_t>(tv.tv_usec)*mul_factor};
	}
	case posix_clock_id::process_cputime_id:
	case posix_clock_id::thread_cputime_id:
	{
		std::make_unsigned_t<decltype(uclock())> u(uclock());
		uintiso_t seconds(u/UCLOCKS_PER_SEC);
		uintiso_t subseconds(u%UCLOCKS_PER_SEC);
		constexpr uintiso_t mul_factor{uintiso_subseconds_per_second/UCLOCKS_PER_SEC};
		return {static_cast<intiso_t>(seconds),static_cast<uintiso_t>(subseconds)*mul_factor};
	}
	default:
		throw_posix_error(EINVAL);
	}
#elif !defined(__NEWLIB__) || defined(_POSIX_TIMERS)
	struct timespec res;
	auto clk{details::posix_clock_id_to_native_value(pclk_id)};
//vdso
	if(::clock_gettime(clk,std::addressof(res))<0)
		throw_posix_error();
	constexpr uintiso_t mul_factor{uintiso_subseconds_per_second/1000000000u};
	return {static_cast<intiso_t>(res.tv_sec),static_cast<uintiso_t>(res.tv_nsec)*mul_factor};
#else
	throw_posix_error(EINVAL);
#endif
}

#if defined(_WIN32) || defined(__CYGWIN__)
template<nt_family family,intiso_t off_to_epoch>
requires (family==nt_family::nt||family==nt_family::zw)
inline basic_timestamp<off_to_epoch> nt_family_clock_settime(posix_clock_id pclk_id,basic_timestamp<off_to_epoch> timestamp)
{
	if constexpr(std::same_as<win32_timestamp,basic_timestamp<off_to_epoch>>)
	{
		switch(pclk_id)
		{
			case posix_clock_id::realtime:
			case posix_clock_id::realtime_alarm:
			case posix_clock_id::realtime_coarse:
			case posix_clock_id::tai:
			{
				constexpr uintiso_t mul_factor{uintiso_subseconds_per_second/10000000u};
				std::uint64_t tms(static_cast<uintiso_t>(timestamp.seconds)*10000000ULL+timestamp.subseconds/mul_factor);
				std::uint64_t old_tms{};
				auto ntstatus{win32::nt::nt_set_system_time<family==nt_family::zw>(std::addressof(tms),std::addressof(old_tms))};
				if(ntstatus)
					throw_nt_error(ntstatus);
				return win32::to_win32_timestamp_ftu64(old_tms);
			}
			default:
				throw_nt_error(0xC00000EF);
		};
	}
	else
	{
		return static_cast<basic_timestamp<off_to_epoch>>(nt_family_clock_settime<family>(pclk_id,static_cast<win32_timestamp>(timestamp)));
	}
}
template<intiso_t off_to_epoch>
inline basic_timestamp<off_to_epoch> nt_clock_settime(posix_clock_id pclk_id,basic_timestamp<off_to_epoch> timestamp)
{
	return nt_family_clock_settime<nt_family::nt>(pclk_id,timestamp);
}

template<intiso_t off_to_epoch>
inline basic_timestamp<off_to_epoch> zw_clock_settime(posix_clock_id pclk_id,basic_timestamp<off_to_epoch> timestamp)
{
	return nt_family_clock_settime<nt_family::zw>(pclk_id,timestamp);
}

#endif

namespace details
{

template<bool local_tm>
inline struct tm unix_timestamp_to_tm_impl(intiso_t seconds)
{
#if defined(_TIME64_T)
	time64_t val{static_cast<time64_t>(seconds)};
	struct tm t;
	if constexpr(local_tm)
	{
	if(localtime64_r(&val,&t)==0)
		throw_posix_error();
	}
	else
	{
	if(gmtime64_r(&val,&t)==0)
		throw_posix_error();
	}
	return t;
#elif defined(_WIN32)
	__time64_t val{static_cast<__time64_t>(seconds)};
	struct tm t;
	if constexpr(local_tm)
	{
	auto errn{noexcept_call(_localtime64_s,&t,&val)};
	if(errn)
		throw_posix_error(static_cast<int>(errn));
	}
	else
	{
	auto errn{noexcept_call(_gmtime64_s,&t,&val)};
	if(errn)
		throw_posix_error(static_cast<int>(errn));
	}
	return t;
#else
	time_t val{static_cast<time_t>(seconds)};
	struct tm t;
	if constexpr(local_tm)
	{
	if(localtime_r(&val,&t)==0)
		throw_posix_error();
	}
	else
	{
	if(gmtime_r(&val,&t)==0)
		throw_posix_error();
	}
	return t;
#endif
}

template<bool local_tm>
inline iso8601_timestamp to_iso8601_utc_or_local_impl(unix_timestamp stamp)
{
	auto res{unix_timestamp_to_tm_impl<local_tm>(stamp.seconds)};
	if constexpr(local_tm)
	{
	long tm_gmtoff{};
#if defined(_WIN32)
	#if defined(_MSC_VER) || defined(_UCRT)
	{
		auto errn{_get_timezone(&tm_gmtoff)};
		if(errn)
			throw_posix_error(static_cast<int>(errn));
	}
	#else
		tm_gmtoff=_timezone;
	#endif
	unsigned long ulong_tm_gmtoff{static_cast<unsigned long>(tm_gmtoff)};
	long seconds{};
#if defined(_MSC_VER) || defined(_UCRT)
	{
		auto errn{_get_dstbias(&seconds)};
		if(errn)
			throw_posix_error(static_cast<int>(errn));
	}
#else
	seconds=_dstbias;
#endif
	unsigned long ulong_seconds{static_cast<unsigned>(seconds)};
	ulong_tm_gmtoff+=ulong_seconds;
	ulong_tm_gmtoff=0ul-ulong_tm_gmtoff;
	tm_gmtoff=static_cast<long>(ulong_tm_gmtoff);
#elif defined(__TM_GMTOFF)
	tm_gmtoff=res.__TM_GMTOFF;
#elif defined(__NEWLIB__)
	tm_gmtoff=0;
#else
	tm_gmtoff=res.tm_gmtoff;
#endif
	return {res.tm_year+1900,
		static_cast<std::uint8_t>(res.tm_mon),
		static_cast<std::uint8_t>(res.tm_mday),
		static_cast<std::uint8_t>(res.tm_hour),
		static_cast<std::uint8_t>(res.tm_min),
		static_cast<std::uint8_t>(res.tm_sec),stamp.subseconds,tm_gmtoff};
	}
	else
	{
	return {res.tm_year+1900,
		static_cast<std::uint8_t>(res.tm_mon),
		static_cast<std::uint8_t>(res.tm_mday),
		static_cast<std::uint8_t>(res.tm_hour),
		static_cast<std::uint8_t>(res.tm_min),
		static_cast<std::uint8_t>(res.tm_sec),stamp.subseconds,0};
	}
}

/*
Referenced from musl libc
https://git.musl-libc.org/cgit/musl/tree/src/time/__secs_to_tm.c?h=v0.9.15
*/

inline constexpr char8_t days_in_month[]{31,30,31,30,31,31,30,31,30,31,31,29};

inline constexpr intiso_t leapoch{946684800LL + 86400LL*(31LL+29LL)};
inline constexpr std::uint32_t days_per_400_year{365LL*400LL+97LL};
inline constexpr std::uint32_t days_per_100_year{365LL*100LL+24LL};
inline constexpr std::uint32_t days_per_4_year{365LL*4LL+1LL};

template<std::signed_integral T>
inline constexpr T sub_overflow(T a,T b) noexcept
{
#if defined(__has_builtin)
#if __has_builtin(__builtin_sub_overflow)
	T c;
	if(__builtin_sub_overflow(a,b,&c))[[unlikely]]
		fast_terminate();
	return c;
#else

	if(b<=0)[[unlikely]]
	{
		if(a>std::numeric_limits<T>::max()+b)[[unlikely]]
			fast_terminate();

	}
	else
	{
		if(a<std::numeric_limits<T>::min()+b)[[unlikely]]
			fast_terminate();
	}
	return a-b;
#endif
#else
	if(b<=0)[[unlikely]]
	{
		if(a>std::numeric_limits<T>::max()+b)[[unlikely]]
			fast_terminate();
	}
	else
	{
		if(a<std::numeric_limits<T>::min()+b)[[unlikely]]
			fast_terminate();
	}
	return a-b;
#endif
}

inline constexpr iso8601_timestamp unix_timestamp_to_iso8601_tsp_impl_internal(intiso_t t,uintiso_t subseconds,long timezone) noexcept
{
	intiso_t secs{sub_overflow(t,leapoch)};
	intiso_t days{secs/86400};
	intiso_t remsecs{secs%86400};
	if(remsecs<0)
	{
		remsecs+=86400;
		--days;
	}

	intiso_t qc_cycles{days / days_per_400_year};
	intiso_t remdays{days % days_per_400_year};
	if (remdays < 0)
	{
		remdays += days_per_400_year;
		--qc_cycles;
	}

	intiso_t c_cycles{remdays / days_per_100_year};
	if (c_cycles == 4)
		--c_cycles;
	remdays -= c_cycles * days_per_100_year;

	intiso_t q_cycles{remdays / days_per_4_year};
	if (q_cycles == 25)
		--q_cycles;
	remdays -= q_cycles * days_per_4_year;

	intiso_t remyears{remdays / 365};
	if (remyears == 4)
		--remyears;
	remdays -= remyears * 365;

	bool leap{!remyears && (q_cycles || !c_cycles)};
	intiso_t yday{remdays + 31 + 28 + leap};
	if (yday >= 365+leap)
		yday -= 365+leap;
	intiso_t years{remyears + 4*q_cycles + 100*c_cycles + 400*qc_cycles};
	std::uint8_t months{};
	for (; days_in_month[months] <= remdays; ++months)
		remdays -= days_in_month[months];
	return {years+2000,
		static_cast<std::uint8_t>(months+2),
		static_cast<std::uint8_t>(remdays+1),
		static_cast<std::uint8_t>(remsecs/3600),
		static_cast<std::uint8_t>(remsecs/60%60),
		static_cast<std::uint8_t>(remsecs%60),subseconds,timezone};
}

inline constexpr iso8601_timestamp unix_timestamp_to_iso8601_tsp_impl(intiso_t t,uintiso_t subseconds) noexcept
{
	return unix_timestamp_to_iso8601_tsp_impl_internal(t,subseconds,0);
}

#ifdef __NEWLIB__
#if __has_cpp_attribute(gnu::dllimport)&&defined(__CYGWIN__)
[[gnu::dllimport]]
#endif
extern void m_tzset() noexcept asm("tzset");
#endif
}
#if 0
template<intiso_t off_to_epoch>
inline iso8601_timestamp tai(basic_timestamp<off_to_epoch> timestamp)
{
	return details::unix_timestamp_to_tai_impl(static_cast<unix_timestamp>(timestamp));
}
#endif

inline void posix_tzset() noexcept
{
#if _WIN32
	noexcept_call(_tzset);
#elif __NEWLIB__
	details::m_tzset();
#else
	noexcept_call(tzset);
#endif
}

template<intiso_t off_to_epoch>
inline constexpr iso8601_timestamp utc(basic_timestamp<off_to_epoch> timestamp) noexcept
{
	if constexpr(std::same_as<basic_timestamp<off_to_epoch>,unix_timestamp>)
	{
		return details::unix_timestamp_to_iso8601_tsp_impl(timestamp.seconds,timestamp.subseconds);
	}
	else
	{
		unix_timestamp tsp{static_cast<unix_timestamp>(timestamp)};
		return details::unix_timestamp_to_iso8601_tsp_impl(tsp.seconds,tsp.subseconds);
	}
}

template<intiso_t off_to_epoch>
inline iso8601_timestamp local(basic_timestamp<off_to_epoch> timestamp)
{
	return details::to_iso8601_utc_or_local_impl<true>(static_cast<unix_timestamp>(timestamp));
}

#if defined(_WIN32) && (defined(_UCRT) || defined(_MSC_VER))
struct win32_timezone_t
{
	std::size_t tz_name_len{};
	bool is_dst{};
};

inline int posix_daylight() noexcept
{
	int hours;
	if(_get_daylight(&hours))
		return false;
	return hours;
}

inline win32_timezone_t timezone_name(bool is_dst=posix_daylight())
{
	win32_timezone_t tzt{.is_dst=is_dst};
	auto errn{_get_tzname(&tzt.tz_name_len,nullptr,0,is_dst)};
	if(errn)
		throw_posix_error(static_cast<int>(errn));
	return tzt;
}

inline constexpr std::size_t print_reserve_size(io_reserve_type_t<char,win32_timezone_t>,win32_timezone_t tzt) noexcept
{
	return tzt.tz_name_len;
}

namespace details
{

inline std::size_t print_reserve_define_impl(char* first,win32_timezone_t tzt)
{
	auto errn{_get_tzname(&tzt.tz_name_len,first,tzt.tz_name_len,tzt.is_dst)};
	if(errn)
		throw_posix_error(static_cast<int>(errn));
	return tzt.tz_name_len;
}

}

template<std::contiguous_iterator Iter>
requires std::same_as<std::iter_value_t<Iter>,char>
inline constexpr Iter print_reserve_define(io_reserve_type_t<char,win32_timezone_t>,Iter first,win32_timezone_t tzt)
{
	return details::print_reserve_define_impl(std::to_address(first),tzt)+first;
}

#else

#ifdef __NEWLIB__
extern char *m_tzname[2] __asm__("_tzname");
extern int m_daylight __asm__("_daylight");
#endif

inline int posix_daylight() noexcept
{
#ifdef __NEWLIB__
	return m_daylight;
#elif defined(__MSDOS__)
	return 0;
#else
	return _daylight;
#endif
}

inline basic_io_scatter_t<char> timezone_name(bool dst=posix_daylight()) noexcept
{
#if defined(__NEWLIB__)
	auto nm{m_tzname[dst]};
#else
	auto nm{tzname[dst]};
#endif
	return {nm,::fast_io::details::my_strlen(nm)};
}

#endif

template<intiso_t off_to_epoch>
inline basic_timestamp<off_to_epoch> posix_clock_settime(posix_clock_id pclk_id,basic_timestamp<off_to_epoch> timestamp)
{
#ifdef _WIN32
	return nt_clock_settime(pclk_id,timestamp);
#else
	if constexpr(std::same_as<basic_timestamp<off_to_epoch>,unix_timestamp>)
	{
#if defined(__MSDOS__)
	switch(pclk_id)
	{
	case posix_clock_id::realtime:
	case posix_clock_id::realtime_alarm:
	case posix_clock_id::realtime_coarse:
	case posix_clock_id::tai:
	{
		if constexpr(sizeof(std::time_t)<sizeof(intiso_t))
		{
			if(static_cast<intiso_t>(std::numeric_limits<std::time_t>::max())<timestamp.seconds)
				throw_posix_error(EOVERFLOW);
		}
		constexpr uintiso_t mul_factor{uintiso_subseconds_per_second/1000000u};
		timeval tv{static_cast<std::time_t>(timestamp.seconds),
		static_cast<long>(timestamp.subseconds/mul_factor)};
		if(::settimeofday(std::addressof(tv), nullptr)<0)
			throw_posix_error();
		return {static_cast<intiso_t>(tv.tv_sec),static_cast<uintiso_t>(tv.tv_usec)*mul_factor};
	}
	default:
		throw_posix_error(EINVAL);
	};
#elif !defined(__NEWLIB__) || defined(_POSIX_TIMERS)
	constexpr uintiso_t mul_factor{uintiso_subseconds_per_second/1000000000u};
	struct timespec res{static_cast<std::time_t>(timestamp.seconds),
	static_cast<long>(timestamp.subseconds/mul_factor)};
	auto clk{details::posix_clock_id_to_native_value(pclk_id)};
#ifdef __linux__
	system_call_throw_error(system_call<__NR_clock_settime,int>(clk,std::addressof(res)));
#else
	if(::clock_settime(clk,std::addressof(res))<0)
		throw_posix_error();
#endif
	return {static_cast<intiso_t>(res.tv_sec),static_cast<uintiso_t>(res.tv_nsec)*mul_factor};
#else
	throw_posix_error(EINVAL);
#endif
	}
	else
	{
		return static_cast<basic_timestamp<off_to_epoch>>(pclk_id,static_cast<unix_timestamp>(timestamp));
	}
#endif
}

}