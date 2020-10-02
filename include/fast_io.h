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
inline constexpr void print_after_io_forward(Args ...args)
{
	if constexpr(line)
		println_freestanding_decay(c_stdout(),args...);
	else
		print_freestanding_decay(c_stdout(),args...);
}

template<bool line,typename... Args>
inline constexpr void perr_after_io_forward(Args ...args)
{
	if constexpr(line)
		println_freestanding_decay(err(),args...);
	else
		print_freestanding_decay(err(),args...);
}

template<bool line,typename... Args>
inline constexpr void debug_print_after_io_forward(Args ...args)
{
	if constexpr(line)
		println_freestanding_decay(out(),args...);
	else
		print_freestanding_decay(out(),args...);
}



}

}

template<typename T,typename... Args>
inline constexpr void print(T&& t,Args&& ...args)
{
	if constexpr(fast_io::output_stream<std::remove_cvref_t<T>>)
		fast_io::print_freestanding_decay(fast_io::io_ref(t),fast_io::io_forward(fast_io::io_print_alias<typename std::remove_cvref_t<T>::char_type>(args))...);
	else
		fast_io::details::print_after_io_forward<false>(fast_io::io_forward(fast_io::io_print_alias<char>(t)),fast_io::io_forward(fast_io::io_print_alias<typename std::remove_cvref_t<T>::char_type>(args))...);
}

template<typename T,typename... Args>
inline constexpr void println(T&& t,Args&& ...args)
{
	if constexpr(fast_io::output_stream<std::remove_cvref_t<T>>)
		fast_io::println_freestanding_decay(fast_io::io_ref(t),fast_io::io_forward(fast_io::io_print_alias<typename std::remove_cvref_t<T>::char_type>(args))...);
	else
		fast_io::details::print_after_io_forward<true>(fast_io::io_forward(fast_io::io_print_alias<char>(t)),fast_io::io_forward(fast_io::io_print_alias<typename std::remove_cvref_t<T>::char_type>(args))...);
}

template<typename... Args>
inline constexpr void perr(Args&&... args)
{
	fast_io::details::perr_after_io_forward<false>(fast_io::io_forward(fast_io::io_print_alias<char>(args))...);
}

template<typename... Args>
inline constexpr void perrln(Args&&... args)
{
	fast_io::details::perr_after_io_forward<true>(fast_io::io_forward(fast_io::io_print_alias<char>(args))...);
}

template<typename... Args>
inline constexpr void panic(Args&&... args) noexcept
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
inline constexpr void panicln(Args&&... args) noexcept
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
inline constexpr void debug_print(T&& t,Args&& ...args)
{
	if constexpr(fast_io::output_stream<std::remove_cvref_t<T>>)
		fast_io::print_freestanding_decay(io_ref(t),fast_io::io_forward(fast_io::io_print_alias<char>(args))...);
	else
		fast_io::details::debug_print_after_io_forward<false>(fast_io::io_forward(fast_io::io_print_alias<char>(t)),fast_io::io_forward(fast_io::io_print_alias<typename std::remove_cvref_t<T>::char_type>(args))...);
}

template<typename T,typename... Args>
inline constexpr void debug_println(T&& t,Args&& ...args)
{
	if constexpr(fast_io::output_stream<std::remove_cvref_t<T>>)
		fast_io::println_freestanding_decay(io_ref(t),fast_io::io_forward(fast_io::io_print_alias<char>(args))...);
	else
		fast_io::details::debug_print_after_io_forward<true>(fast_io::io_forward(fast_io::io_print_alias<char>(t)),fast_io::io_forward(fast_io::io_print_alias<typename std::remove_cvref_t<T>::char_type>(args))...);
}

template<typename... Args>
inline constexpr void debug_perr(Args&&... args)
{
	::perr(std::forward<Args>(args)...);
}

template<typename... Args>
inline constexpr void debug_perrln(Args&&... args)
{
	::perrln(std::forward<Args>(args)...);
}

template<typename... Args>
inline constexpr void debug_panic(Args&&... args) noexcept
{
	::panic(std::forward<Args>(args)...);
}

template<typename... Args>
inline constexpr void debug_panicln(Args&&... args) noexcept
{
	::panicln(std::forward<Args>(args)...);
}
#endif

template<bool report_eof=false,typename T,typename... Args>
inline constexpr auto scan(T&& t,Args&& ...args)
{
	if constexpr(fast_io::input_stream<std::remove_cvref_t<T>>)
		return fast_io::scan<report_eof>(std::forward<T>(t),std::forward<Args>(args)...);
	else
		return scan<report_eof>(fast_io::c_io_observer{stdin},std::forward<T>(t),std::forward<Args>(args)...);
}
