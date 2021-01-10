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
unix_timestamp posix_clock_getres(posix_clock_id pclk_id)
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
#else
	auto clk{details::posix_clock_id_to_native_value(pclk_id)};
	struct timespec res;
#if defined(__linux__) && defined(__x86_64__)
	system_call_throw_error(system_call<__NR_clock_getres,int>(clk,std::addressof(res)));
#else
//vdso
	if(::clock_getres(clk,std::addressof(res))<0)
		throw_posix_error();
#endif
	constexpr uintiso_t mul_factor{uintiso_subseconds_per_second/1000000000u};
	return {static_cast<intiso_t>(res.tv_sec),static_cast<uintiso_t>(res.tv_nsec)*mul_factor};
#endif
}

inline unix_timestamp posix_clock_gettime(posix_clock_id pclk_id)
{
#ifdef _WIN32
	switch(pclk_id)
	{
	case posix_clock_id::realtime:
	case posix_clock_id::realtime_alarm:
	case posix_clock_id::realtime_coarse:
	case posix_clock_id::tai:
	{
		win32::filetime ftm;
		win32::GetSystemTimePreciseAsFileTime(std::addressof(ftm));
		return static_cast<unix_timestamp>(to_win32_timestamp(ftm));
	}
	case posix_clock_id::monotonic:
	case posix_clock_id::monotonic_coarse:
	case posix_clock_id::monotonic_raw:
	case posix_clock_id::boottime:
	{
		std::uint64_t ftm{};
		if(!win32::QueryUnbiasedInterruptTime(std::addressof(ftm)))
			throw_win32_error();
		constexpr uintiso_t mul_factor{uintiso_subseconds_per_second/10000000u};
		std::uint64_t seconds{ftm/10000000ULL};
		std::uint64_t subseconds{ftm%10000000ULL};
		return {static_cast<intiso_t>(seconds),static_cast<uintiso_t>(subseconds*mul_factor)};
	}
	case posix_clock_id::process_cputime_id:
	{
		auto hprocess{bit_cast<void*>(std::intptr_t(-1))};
		win32::filetime creation_time,exit_time,kernel_time,user_time;
		if(!win32::GetProcessTimes(hprocess,std::addressof(creation_time),
		std::addressof(exit_time),
		std::addressof(kernel_time),
		std::addressof(user_time)))
			throw_win32_error();
		std::uint64_t ftm{win32::filetime_to_uint64_t(kernel_time)+
			win32::filetime_to_uint64_t(user_time)};
		std::uint64_t seconds{ftm/10000000ULL};
		std::uint64_t subseconds{ftm%10000000ULL};
		constexpr uintiso_t mul_factor{uintiso_subseconds_per_second/10000000u};
		return {static_cast<intiso_t>(seconds),static_cast<uintiso_t>(subseconds*mul_factor)};
	}
	case posix_clock_id::thread_cputime_id:
	{
		auto hthread{bit_cast<void*>(std::intptr_t(-2))};
		win32::filetime creation_time,exit_time,kernel_time,user_time;
		if(!win32::GetThreadTimes(hthread,std::addressof(creation_time),
		std::addressof(exit_time),
		std::addressof(kernel_time),
		std::addressof(user_time)))
			throw_win32_error();
		std::uint64_t ftm{win32::filetime_to_uint64_t(kernel_time)+
			win32::filetime_to_uint64_t(user_time)};
		std::uint64_t seconds{ftm/10000000ULL};
		std::uint64_t subseconds{ftm%10000000ULL};
		constexpr uintiso_t mul_factor{uintiso_subseconds_per_second/10000000u};
		return {static_cast<intiso_t>(seconds),static_cast<uintiso_t>(subseconds*mul_factor)};
	}
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
#else
	struct timespec res;
	auto clk{details::posix_clock_id_to_native_value(pclk_id)};
#if defined(__linux__) && defined(__x86_64__)
	system_call_throw_error(system_call<__NR_clock_gettime,int>(clk,std::addressof(res)));
#else
//vdso
	if(::clock_gettime(clk,std::addressof(res))<0)
		throw_posix_error();
#endif
	constexpr uintiso_t mul_factor{uintiso_subseconds_per_second/1000000000u};
	return {static_cast<intiso_t>(res.tv_sec),static_cast<uintiso_t>(res.tv_nsec)*mul_factor};
#endif
}

inline unix_timestamp posix_clock_settime(posix_clock_id pclk_id,unix_timestamp timestamp)
{
#ifdef _WIN32
	switch(pclk_id)
	{
	case posix_clock_id::realtime:
	case posix_clock_id::realtime_alarm:
	case posix_clock_id::realtime_coarse:
	case posix_clock_id::tai:
	{
		win32_timestamp win32ts(timestamp);
		constexpr uintiso_t mul_factor{uintiso_subseconds_per_second/10000000u};
		std::uint64_t tms(static_cast<uintiso_t>(win32ts.seconds)*10000000ULL+win32ts.subseconds/mul_factor);
		std::uint64_t old_tms{};
		auto ntstatus{win32::nt::nt_set_system_time<false>(std::addressof(tms),std::addressof(old_tms))};
		if(ntstatus)
			throw_nt_error(ntstatus);
		return static_cast<unix_timestamp>(win32::to_win32_timestamp_ftu64(old_tms));	
	}
	default:
		throw_nt_error(0xC00000EF);
	};
#elif defined(__MSDOS__)
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
#else
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
#endif
}

}