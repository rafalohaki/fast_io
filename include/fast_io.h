#pragma once

#include"fast_io_hosted.h"

#include"fast_io_legacy_impl/c/impl.h"

namespace fast_io
{

inline c_io_observer c_stdin() noexcept
{
	return {stdin};
}

inline c_io_observer c_stdout() noexcept
{
	return {stdout};
}

inline c_io_observer c_stderr() noexcept
{
	return {stderr};
}
inline wc_io_observer wc_stdin() noexcept
{
	return {stdin};
}

inline wc_io_observer wc_stdout() noexcept
{
	return {stdout};
}

inline wc_io_observer wc_stderr() noexcept
{
	return {stderr};
}

inline
#ifndef _WIN32
constexpr
#endif
decltype(auto) in() noexcept
{
	return native_stdin();
}

inline
#ifndef _WIN32
constexpr
#endif
decltype(auto) out() noexcept
{
	return native_stdout();
}

inline
#ifndef _WIN32
constexpr
#endif
decltype(auto) err() noexcept
{
	return native_stderr();
}


inline
#ifndef _WIN32
constexpr
#endif
decltype(auto) u8in() noexcept
{
	return native_stdin<char8_t>();
}

inline
#ifndef _WIN32
constexpr
#endif
decltype(auto) u8out() noexcept
{
	return native_stdout<char8_t>();
}

inline
#ifndef _WIN32
constexpr
#endif
decltype(auto) u8err() noexcept
{
	return native_stderr<char8_t>();
}



using in_buf_type = basic_ibuf<native_io_observer>;
using out_buf_type = basic_obuf<native_io_observer>;

inline auto in_buf()
{
	return in_buf_type(native_stdin());
}

inline auto out_buf()
{
	return out_buf_type(native_stdout());
}

inline auto err_buf()
{
	return out_buf_type(native_stderr());
}

namespace details
{

template<bool line,typename... Args>
#if __has_cpp_attribute(gnu::sysv_abi) && !defined(_MSC_VER)
[[gnu::sysv_abi]]
#endif
inline constexpr void print_after_io_forward(Args ...args)
{
	if constexpr(line)
		println_freestanding_decay(c_stdout(),args...);
	else
		print_freestanding_decay(c_stdout(),args...);
}

template<bool line,typename... Args>
#if __has_cpp_attribute(gnu::sysv_abi) && !defined(_MSC_VER)
[[gnu::sysv_abi]]
#endif
inline constexpr void perr_after_io_forward(Args ...args)
{
	if constexpr(line)
		println_freestanding_decay(err(),args...);
	else
		print_freestanding_decay(err(),args...);
}

template<bool line,typename... Args>
#if __has_cpp_attribute(gnu::sysv_abi) && !defined(_MSC_VER)
[[gnu::sysv_abi]]
#endif
inline constexpr void debug_print_after_io_forward(Args ...args)
{
	if constexpr(line)
		println_freestanding_decay(out(),args...);
	else
		print_freestanding_decay(out(),args...);
}


template<bool report,typename... Args>
#if __has_cpp_attribute(gnu::sysv_abi) && !defined(_MSC_VER)
[[gnu::sysv_abi]]
#endif
inline constexpr std::conditional_t<report,bool,void> scan_after_io_forward(Args ...args)
{
	if constexpr(report)
		return scan_freestanding_decay(c_stdin(),args...);
	else
	{
		if(!scan_freestanding_decay(c_stdin(),args...))
			throw_scan_error(std::errc::resource_unavailable_try_again);
	}
}


}

}

template<typename T,typename... Args>
#if __has_cpp_attribute(gnu::sysv_abi) && !defined(_MSC_VER)
[[gnu::sysv_abi]]
#endif
inline constexpr void print(T&& t,Args&& ...args)
{
	if constexpr(fast_io::output_stream<std::remove_cvref_t<T>>||fast_io::status_output_stream<std::remove_cvref_t<T>>)
		fast_io::print_freestanding_decay(fast_io::io_ref(t),fast_io::io_forward(fast_io::io_print_alias<typename std::remove_cvref_t<T>::char_type>(args))...);
	else
		fast_io::details::print_after_io_forward<false>(fast_io::io_forward(fast_io::io_print_alias<char>(t)),fast_io::io_forward(fast_io::io_print_alias<char>(args))...);
}

template<typename T,typename... Args>
#if __has_cpp_attribute(gnu::sysv_abi) && !defined(_MSC_VER)
[[gnu::sysv_abi]]
#endif
inline constexpr void println(T&& t,Args&& ...args)
{
	if constexpr(fast_io::output_stream<std::remove_cvref_t<T>>||fast_io::status_output_stream<std::remove_cvref_t<T>>)
		fast_io::println_freestanding_decay(fast_io::io_ref(t),fast_io::io_forward(fast_io::io_print_alias<typename std::remove_cvref_t<T>::char_type>(args))...);
	else
		fast_io::details::print_after_io_forward<true>(fast_io::io_forward(fast_io::io_print_alias<char>(t)),fast_io::io_forward(fast_io::io_print_alias<char>(args))...);
}

template<typename... Args>
requires (sizeof...(Args)!=0)
#if __has_cpp_attribute(gnu::sysv_abi) && !defined(_MSC_VER)
[[gnu::sysv_abi]]
#endif
inline constexpr void perr(Args&&... args)
{
	fast_io::details::perr_after_io_forward<false>(fast_io::io_forward(fast_io::io_print_alias<char>(args))...);
}

template<typename... Args>
requires (sizeof...(Args)!=0)
#if __has_cpp_attribute(gnu::sysv_abi) && !defined(_MSC_VER)
[[gnu::sysv_abi]]
#endif
inline constexpr void perrln(Args&&... args)
{
	fast_io::details::perr_after_io_forward<true>(fast_io::io_forward(fast_io::io_print_alias<char>(args))...);
}

template<typename... Args>
#if __has_cpp_attribute(gnu::sysv_abi) && !defined(_MSC_VER)
[[gnu::sysv_abi]]
#endif
[[noreturn]] inline constexpr void panic(Args&&... args) noexcept
{
	if constexpr(sizeof...(Args)!=0)
	{
#ifdef __cpp_exceptions
	try
	{
#endif
		perr(std::forward<Args>(args)...);
#ifdef __cpp_exceptions
	}
	catch(...){}
#endif
	}
	fast_io::fast_terminate();
}

template<typename... Args>
#if __has_cpp_attribute(gnu::sysv_abi) && !defined(_MSC_VER)
[[gnu::sysv_abi]]
#endif
[[noreturn]] inline constexpr void panicln(Args&&... args) noexcept
{
#ifdef __cpp_exceptions
	try
	{
#endif
		perrln(std::forward<Args>(args)...);
#ifdef __cpp_exceptions
	}
	catch(...){}
#endif
	fast_io::fast_terminate();
}

//Allow debug print
#ifndef NDEBUG
//With debugging. We output to POSIX fd or Win32 Handle directly instead of C's stdout.
template<typename T,typename... Args>
#if __has_cpp_attribute(gnu::sysv_abi) && !defined(_MSC_VER)
[[gnu::sysv_abi]]
#endif
inline constexpr void debug_print(T&& t,Args&& ...args)
{
	if constexpr(fast_io::output_stream<std::remove_cvref_t<T>>)
		fast_io::print_freestanding_decay(io_ref(t),fast_io::io_forward(fast_io::io_print_alias<typename std::remove_cvref_t<T>::char_type>(args))...);
	else
		fast_io::details::debug_print_after_io_forward<false>(fast_io::io_forward(fast_io::io_print_alias<char>(t)),fast_io::io_forward(fast_io::io_print_alias<char>(args))...);
}

template<typename T,typename... Args>
#if __has_cpp_attribute(gnu::sysv_abi) && !defined(_MSC_VER)
[[gnu::sysv_abi]]
#endif
inline constexpr void debug_println(T&& t,Args&& ...args)
{
	if constexpr(fast_io::output_stream<std::remove_cvref_t<T>>)
		fast_io::println_freestanding_decay(io_ref(t),fast_io::io_forward(fast_io::io_print_alias<typename std::remove_cvref_t<T>::char_type>(args))...);
	else
		fast_io::details::debug_print_after_io_forward<true>(fast_io::io_forward(fast_io::io_print_alias<char>(t)),fast_io::io_forward(fast_io::io_print_alias<char>(args))...);
}

template<typename... Args>
#if __has_cpp_attribute(gnu::sysv_abi) && !defined(_MSC_VER)
[[gnu::sysv_abi]]
#endif
inline constexpr void debug_perr(Args&&... args)
{
	::perr(std::forward<Args>(args)...);
}

template<typename... Args>
#if __has_cpp_attribute(gnu::sysv_abi) && !defined(_MSC_VER)
[[gnu::sysv_abi]]
#endif
inline constexpr void debug_perrln(Args&&... args)
{
	::perrln(std::forward<Args>(args)...);
}

#endif

template<bool report=false,typename input,typename... Args>
#if __has_cpp_attribute(gnu::sysv_abi) && !defined(_MSC_VER)
[[gnu::sysv_abi]]
#endif
inline constexpr std::conditional_t<report,bool,void> scan(input&& in,Args&& ...args)
{
	if constexpr(fast_io::input_stream<std::remove_cvref_t<input>>)
	{
		if constexpr(report)
			return fast_io::scan_freestanding_decay(fast_io::io_ref(in),fast_io::io_scan_alias<typename std::remove_cvref_t<input>::char_type>(args)...);
		else
		{
			if(!fast_io::scan_freestanding_decay(fast_io::io_ref(in),fast_io::io_scan_alias<typename std::remove_cvref_t<input>::char_type>(args)...))
				fast_io::throw_scan_error(std::errc::resource_unavailable_try_again);
		}
	}
	else
		return fast_io::details::scan_after_io_forward<report>(fast_io::io_scan_alias<char>(in),fast_io::io_scan_alias<char>(args)...);
}
