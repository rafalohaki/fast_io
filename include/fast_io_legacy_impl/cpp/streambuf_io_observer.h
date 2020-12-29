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
	explicit constexpr operator bool() const noexcept
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
#ifdef _WIN32
	explicit operator basic_win32_io_observer<char_type>() const noexcept
	{
		return static_cast<basic_win32_io_observer<char_type>>
		(static_cast<basic_posix_io_observer<char_type>>(*this));
	}
	template<nt_family fam>
	explicit operator basic_nt_family_io_observer<fam,char_type>() const noexcept
	{
		return static_cast<basic_nt_family_io_observer<fam,char_type>>
		(static_cast<basic_posix_io_observer<char_type>>(*this));
	}
#endif
#endif
};

template<typename T>
inline void flush(basic_general_streambuf_io_observer<T> h)
{
	if(h.native_handle()->pubsync()==-1)
		throw_posix_error(EIO);
}

#if __cpp_lib_three_way_comparison >= 201907L

template<typename T>
inline constexpr bool operator==(basic_general_streambuf_io_observer<T> a,basic_general_streambuf_io_observer<T> b) noexcept
{
	return a.fb==b.fb;
}

template<typename T>
inline constexpr auto operator<=>(basic_general_streambuf_io_observer<T> a,basic_general_streambuf_io_observer<T> b) noexcept
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

#ifndef __MSDOS__
using wstreambuf_io_observer = basic_streambuf_io_observer<wchar_t>;
using wfilebuf_io_observer = basic_filebuf_io_observer<wchar_t>;
using wstringbuf_io_observer = basic_stringbuf_io_observer<wchar_t>;
#endif
using u8streambuf_io_observer = basic_streambuf_io_observer<char8_t>;
using u8filebuf_io_observer = basic_filebuf_io_observer<char8_t>;
using u8stringbuf_io_observer = basic_stringbuf_io_observer<char8_t>;

using u16streambuf_io_observer = basic_streambuf_io_observer<char16_t>;
using u16filebuf_io_observer = basic_filebuf_io_observer<char16_t>;
using u16stringbuf_io_observer = basic_stringbuf_io_observer<char16_t>;

using u32streambuf_io_observer = basic_streambuf_io_observer<char32_t>;
using u32filebuf_io_observer = basic_filebuf_io_observer<char32_t>;
using u32stringbuf_io_observer = basic_stringbuf_io_observer<char32_t>;

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

namespace details
{

template<std::integral char_type,typename traits_type>
inline std::size_t streambuf_write_report_eh_impl(std::basic_streambuf<char_type,traits_type>* fb,
	char_type const* src, std::size_t count)
{
	return static_cast<std::size_t>(fb->sputn(src,static_cast<std::streamsize>(count)));
}

template<std::integral char_type,typename traits_type>
inline std::size_t streambuf_read_report_eh_impl(std::basic_streambuf<char_type,traits_type>* fb,char_type* dest, std::size_t count)
{
	return static_cast<std::size_t>(fb->sgetn(dest,static_cast<std::streamsize>(count)));
}


template<std::integral char_type,typename traits_type>
inline std::size_t streambuf_write_impl(basic_streambuf_io_observer<char_type,traits_type> t,char_type const* src, std::size_t count)
{
	auto curr{obuffer_curr(t)};
	auto ed{obuffer_end(t)};
	if(count<static_cast<std::size_t>(ed-curr))[[likely]]
	{
		traits_type::copy(curr,src,count);
		obuffer_set_curr(t,curr+count);
		return count;
	}
	return streambuf_write_report_eh_impl(t.fb,src,count);
}

template<std::integral char_type,typename traits_type>
inline std::size_t streambuf_read_impl(basic_streambuf_io_observer<char_type,traits_type> t,char_type* dest, std::size_t count)
{
	auto curr{ibuffer_curr(t)};
	auto ed{ibuffer_end(t)};
	if(count<static_cast<std::size_t>(ed-curr))[[likely]]
	{
		traits_type::copy(dest,curr,count);
		ibuffer_set_curr(t,curr+count);
		return count;
	}
	return streambuf_read_report_eh_impl(t.fb,dest,count);
}


}

template<typename T,std::contiguous_iterator Iter>
requires (std::same_as<typename T::char_type,std::iter_value_t<Iter>>||std::same_as<typename T::char_type,char>)
[[nodiscard]] inline Iter read(basic_general_streambuf_io_observer<T> t,Iter begin,Iter end)
{
	using char_type = typename T::char_type;
	using traits_type = typename T::traits_type;
	if constexpr(std::same_as<typename T::char_type,std::iter_value_t<Iter>>)
		return begin+details::streambuf_read_impl(basic_streambuf_io_observer<char_type,traits_type>{t.fb},
			std::to_address(begin),static_cast<std::size_t>(end-begin));
	else
		return begin+details::streambuf_read_impl(basic_streambuf_io_observer<char_type,traits_type>{t.fb},
			reinterpret_cast<char*>(std::to_address(begin)),static_cast<std::size_t>(end-begin)*sizeof(*begin))/(*begin);
}

template<typename T,std::contiguous_iterator Iter>
requires (std::same_as<typename T::char_type,std::iter_value_t<Iter>>||std::same_as<typename T::char_type,char>)
inline Iter write(basic_general_streambuf_io_observer<T> t,Iter begin,Iter end)
{
	using char_type = typename T::char_type;
	using traits_type = typename T::traits_type;
	if constexpr(std::same_as<typename T::char_type,std::iter_value_t<Iter>>)
		return begin+details::streambuf_write_impl(basic_streambuf_io_observer<char_type,traits_type>{t.fb},
			std::to_address(begin),static_cast<std::size_t>(end-begin));
	else
		return begin+details::streambuf_write_impl(basic_streambuf_io_observer<char_type,traits_type>{t.fb},
			reinterpret_cast<char const*>(std::to_address(begin)),static_cast<std::size_t>(end-begin)*sizeof(*begin))/(*begin);
}

}
