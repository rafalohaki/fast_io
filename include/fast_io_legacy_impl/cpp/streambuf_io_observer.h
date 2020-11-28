#pragma once
#ifdef __GLIBCXX__
#include"fp_hack/libstdc++.h"
#elif defined(_MSVC_STL_UPDATE)
#include"fp_hack/msvc_stl.h"
#elif defined(__LIBCPP_VERSION)
#include"fp_hack/libc++.h"
#endif


namespace fast_io
{

template<typename T>
requires std::derived_from<T,std::basic_streambuf<typename T::char_type,typename T::traits_type>>
class basic_general_streambuf_io_observer
{
public:
	using streambuf_type = T;
	using char_type = typename streambuf_type::char_type;
	using traits_type = typename streambuf_type::traits_type;
	using native_handle_type = streambuf_type*;
	native_handle_type fb{};
	inline constexpr native_handle_type& native_handle() const noexcept
	{
		return fb;
	}
	inline constexpr native_handle_type& native_handle() noexcept
	{
		return fb;
	}
	constexpr operator bool() const noexcept
	{
		return fb;
	}
#if defined(__GLIBCXX__) || defined(__LIBCPP_VERSION)  || defined(_MSVC_STL_UPDATE)
	explicit operator basic_c_io_observer_unlocked<char_type>() const noexcept
	{
		return basic_c_io_observer_unlocked<char_type>{details::streambuf_hack::fp_hack(fb)};
	}
	explicit operator basic_c_io_observer<char_type>() const noexcept
	{
		return basic_c_io_observer<char_type>{details::streambuf_hack::fp_hack(fb)};
	}
	explicit operator basic_posix_io_observer<char_type>() const noexcept
	{
		return static_cast<basic_posix_io_observer<char_type>>(static_cast<basic_c_io_observer<char_type>>(*this));
	}
#if defined(_WIN32)
	explicit operator basic_win32_io_observer<char_type>() const noexcept
	{
		return static_cast<basic_win32_io_observer<char_type>>
		(static_cast<basic_posix_io_observer<char_type>>(*this));
	}
	explicit operator basic_nt_io_observer<char_type>() const noexcept
	{
		return static_cast<basic_nt_io_observer<char_type>>
		(static_cast<basic_posix_io_observer<char_type>>(*this));
	}
#endif
#endif
};

template<typename T,std::contiguous_iterator Iter>
[[nodiscard]] inline Iter read(basic_general_streambuf_io_observer<T> t,Iter begin,Iter end)
{
	using char_type = typename T::char_type;
	return begin+(t.fb->sgetn(static_cast<char_type*>(static_cast<void*>(std::to_address(begin))),(end-begin)*sizeof(*begin)/sizeof(char_type))*sizeof(char_type)/sizeof(*begin));
}

template<typename T,std::contiguous_iterator Iter>
inline Iter write(basic_general_streambuf_io_observer<T> t,Iter begin,Iter end);

template<typename T>
inline void flush(basic_general_streambuf_io_observer<T> h)
{
	if(h.native_handle()->pubsync()==-1)
		throw_posix_error(EIO);
}

#if __cpp_lib_three_way_comparison >= 201907L

template<typename T>
inline constexpr bool operator==(basic_general_streambuf_io_observer<T> a,basic_general_streambuf_io_observer<T> b)
{
	return a.fb==b.fb;
}

template<typename T>
inline constexpr auto operator<=>(basic_general_streambuf_io_observer<T> a,basic_general_streambuf_io_observer<T> b)
{
	return a.fb<=>b.fb;
}

#endif

template<typename T>
requires async_stream<basic_c_io_observer_unlocked<typename T::char_type>>
inline constexpr io_async_scheduler_t<basic_c_io_observer_unlocked<typename T::char_type>>
	async_scheduler_type(basic_general_streambuf_io_observer<T>)
{
	return {};
}

template<typename T>
requires async_stream<basic_c_io_observer_unlocked<typename T::char_type>>
inline constexpr io_async_overlapped_t<basic_c_io_observer_unlocked<typename T::char_type>>
	async_overlapped_type(basic_general_streambuf_io_observer<T>)
{
	return {};
}
#if defined(_WIN32) || defined(__linux__)
template<typename T,typename... Args>
requires async_output_stream<basic_c_io_observer_unlocked<typename T::char_type>>
inline void async_write_callback(io_async_observer ioa,basic_general_streambuf_io_observer<T> h,Args&& ...args)
{
	async_write_callback(ioa,static_cast<basic_c_io_observer_unlocked<typename T::char_type>>(h),std::forward<Args>(args)...);
}

template<typename T,typename... Args>
requires async_input_stream<basic_c_io_observer_unlocked<typename T::char_type>>
inline void async_read_callback(io_async_observer ioa,basic_general_streambuf_io_observer<T> h,Args&& ...args)
{
	async_read_callback(ioa,static_cast<basic_c_io_observer_unlocked<typename T::char_type>>(h),std::forward<Args>(args)...);
}
#endif
template<typename T>
inline constexpr basic_general_streambuf_io_observer<T> io_value_handle(basic_general_streambuf_io_observer<T> other) noexcept
{
	return other;
}

template<std::integral CharT,typename Traits = std::char_traits<CharT>>
using basic_streambuf_io_observer = basic_general_streambuf_io_observer<std::basic_streambuf<CharT,Traits>>;

template<std::integral CharT,typename Traits = std::char_traits<CharT>>
using basic_filebuf_io_observer = basic_general_streambuf_io_observer<std::basic_filebuf<CharT,Traits>>;

template<std::integral CharT,typename Traits = std::char_traits<CharT>>
using basic_stringbuf_io_observer = basic_general_streambuf_io_observer<std::basic_stringbuf<CharT,Traits>>;


using streambuf_io_observer = basic_streambuf_io_observer<char>;
using filebuf_io_observer = basic_filebuf_io_observer<char>;
using stringbuf_io_observer = basic_stringbuf_io_observer<char>;

using u8streambuf_io_observer = basic_streambuf_io_observer<char8_t>;
using u8filebuf_io_observer = basic_filebuf_io_observer<char8_t>;
using u8stringbuf_io_observer = basic_stringbuf_io_observer<char8_t>;

#ifndef __MSDOS__
using wstreambuf_io_observer = basic_streambuf_io_observer<wchar_t>;
using wfilebuf_io_observer = basic_filebuf_io_observer<wchar_t>;
using wstringbuf_io_observer = basic_stringbuf_io_observer<wchar_t>;
#endif


#if defined(__GLIBCXX__) || defined(__LIBCPP_VERSION)  || defined(_MSVC_STL_UPDATE)
template<std::integral ch_type,typename Traits>
requires zero_copy_input_stream<basic_c_io_observer_unlocked<ch_type>>
inline constexpr decltype(auto) zero_copy_in_handle(basic_filebuf_io_observer<ch_type,Traits> h)
{
	return zero_copy_in_handle(static_cast<basic_c_io_observer_unlocked<ch_type>>(h));
}
template<std::integral ch_type,typename Traits>
requires zero_copy_output_stream<basic_c_io_observer_unlocked<ch_type>>
inline constexpr decltype(auto) zero_copy_out_handle(basic_filebuf_io_observer<ch_type,Traits> h)
{
	return zero_copy_out_handle(static_cast<basic_c_io_observer_unlocked<ch_type>>(h));
}

template<std::integral ch_type,typename Traits,typename... Args>
inline auto seek(basic_filebuf_io_observer<ch_type,Traits> h,Args&& ...args)
{
	h.fb->flush();
	h.fb->clear();
	return seek(static_cast<basic_c_io_observer_unlocked<ch_type>>(h),std::forward<Args>(args)...);
}

template<std::integral ch_type,typename... Args>
requires io_controllable<basic_c_io_observer_unlocked<ch_type>,Args...>
inline decltype(auto) io_control(basic_filebuf_io_observer<ch_type> h,Args&& ...args)
{
	return io_control(static_cast<basic_c_io_observer_unlocked<ch_type>>(h),std::forward<Args>(args)...);
}

template<std::integral ch_type>
requires requires(basic_c_io_observer_unlocked<ch_type> piob)
{
	status(piob);
}
inline constexpr auto status(basic_streambuf_io_observer<ch_type> ciob)
{
	return status(static_cast<basic_c_io_observer_unlocked<ch_type>>(ciob));
}

template<std::integral ch_type>
requires requires(basic_c_io_observer_unlocked<ch_type> piob)
{
	status(piob);
}
inline constexpr auto status(basic_filebuf_io_observer<ch_type> ciob)
{
	return status(static_cast<basic_c_io_observer_unlocked<ch_type>>(ciob));
}

template<std::integral char_type>
inline constexpr posix_at_entry at(basic_filebuf_io_observer<char_type> other) noexcept
{
	return posix_at_entry{details::fp_to_fd(details::streambuf_hack::fp_hack(other.fb))};
}

#endif
}


#if defined(__GLIBCXX__) || defined(__LIBCPP_VERSION) || defined(_MSVC_STL_UPDATE)
#include"general.h"
#endif


namespace fast_io
{

template<typename T,std::contiguous_iterator Iter>
inline Iter write(basic_general_streambuf_io_observer<T> t,Iter begin,Iter end)
{
	using char_type = typename T::char_type;
	std::size_t const count(end-begin);
	std::size_t const total_bytes_to_write(count*sizeof(*begin));
	std::size_t const total_count(total_bytes_to_write/sizeof(char_type));
	auto curr{obuffer_curr(t)};
	auto ed{obuffer_end(t)};
	if(curr+total_count<ed)[[likely]]
	{
		if(total_bytes_to_write)
			memcpy(curr,std::to_address(begin),total_bytes_to_write);
		obuffer_set_curr(t,curr+total_count);
		return end;
	}
	return begin+(t.fb->sputn(static_cast<char_type const*>(static_cast<void const*>(std::to_address(begin))),(end-begin)*sizeof(*begin)/sizeof(char_type)))*sizeof(char_type)/sizeof(*begin);
}

}
