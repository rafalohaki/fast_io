#pragma once
#if defined(__GLIBCXX__)
#include<ext/stdio_filebuf.h>
#endif
#include"streambuf_io_observer.h"
namespace fast_io
{

namespace details
{

inline constexpr std::ios::openmode calculate_fstream_file_open_mode(open_mode om)
{
	std::ios::openmode ios_om{};
	if((om&open_mode::app)!=open_mode::none)
		ios_om=ios_om|std::ios::app;
	if((om&open_mode::binary)!=open_mode::none)
		ios_om=ios_om|std::ios::binary;
	if((om&open_mode::in)!=open_mode::none)
		ios_om=ios_om|std::ios::in;
	if((om&open_mode::out)!=open_mode::none)
		ios_om=ios_om|std::ios::out;
	if((om&open_mode::trunc)!=open_mode::none)
		ios_om=ios_om|std::ios::trunc;
	if((om&open_mode::ate)!=open_mode::none)
		ios_om=ios_om|std::ios::ate;
	return ios_om;
}

template<open_mode om>
struct fstream_open_mode
{
	inline static constexpr auto value=calculate_fstream_file_open_mode(om);
};

template<open_mode om>
inline constexpr std::ios::openmode calculate_fstream_open_value(open_interface_t<om>)
{
	return details::fstream_open_mode<om>::value;
}

inline constexpr std::ios::openmode calculate_fstream_open_value(open_mode om)
{
	return calculate_fstream_file_open_mode(om);
}
inline constexpr std::ios::openmode calculate_fstream_open_value(std::string_view c_mode)
{
	return calculate_fstream_open_value(from_c_mode(c_mode));
}

}


#if defined(__GLIBCXX__)  || defined(_MSVC_STL_UPDATE) || defined(__LIBCPP_VERSION) 
template<std::integral CharT,typename Traits = std::char_traits<CharT>>
class basic_filebuf_file
{
public:
	using char_type = CharT;
	using traits_type = Traits;
	using native_handle_type =
#if defined(__GLIBCXX__)
__gnu_cxx::stdio_filebuf<char_type,traits_type>
#else
std::basic_filebuf<char_type,traits_type>
#endif
;
#if defined (__linux__) || defined(__WINNT__) || defined(_MSC_VER)
	using async_scheduler_type = io_async_observer;
#endif
	native_handle_type fb;
	basic_filebuf_file()=default;
#if defined(__GLIBCXX__)
	template<typename T>
	basic_filebuf_file(basic_posix_io_handle<char_type>&& piohd,T&& t):
		fb(piohd.native_handle(),details::calculate_fstream_open_value(std::forward<T>(t)),fast_io::details::cal_buffer_size<CharT,true>())
	{
		piohd.release();
	}
#elif defined(__LIBCPP_VERSION)
	template<typename T>
	basic_filebuf_file(basic_posix_io_handle<char_type>&& piohd,T&& t)
	{
		fb.__open(piohd.native_handle(),details::calculate_fstream_open_value(std::forward<T>(t)));
		piohd.release();
	}
#else
	template<typename T>
	basic_filebuf_file(basic_c_io_handle_unlocked<char_type>&& chd,T&& t):fb(chd.native_handle())
	{
		chd.release();
	}
	template<typename T>
	basic_filebuf_file(basic_posix_io_handle<char_type>&& piohd,T&& t):
		basic_filebuf_file(basic_c_file_unlocked<char_type>(std::move(piohd),std::forward<T>(t)),std::forward<T>(t))
	{
	}
#endif

#if defined(__WINNT__) || defined(_MSC_VER)
//windows specific. open posix file from win32 io handle
	template<typename T>
	basic_filebuf_file(basic_win32_io_handle<char_type>&& win32_handle,T&& t):
		basic_filebuf_file(basic_posix_file<char_type>(std::move(win32_handle),std::forward<T>(t)),std::forward<T>(t))
	{
	}
#endif

	template<open_mode om,typename... Args>
	basic_filebuf_file(std::string_view file,open_interface_t<om>,Args&& ...args):
		basic_filebuf_file(basic_posix_file<char_type>(file,open_interface<om>,std::forward<Args>(args)...),
			open_interface<om>)
	{}
	template<typename... Args>
	basic_filebuf_file(std::string_view file,open_mode om,Args&& ...args):
		basic_filebuf_file(basic_posix_file<char_type>(file,om,std::forward<Args>(args)...),om)
	{}
	template<typename... Args>
	basic_filebuf_file(std::string_view file,std::string_view mode,Args&& ...args):
		basic_filebuf_file(basic_posix_file<char_type>(file,mode,std::forward<Args>(args)...),mode)
	{}

#if defined (__linux__) || defined(__WINNT__) || defined(_MSC_VER)
	template<open_mode om,typename... Args>
	basic_filebuf_file(io_async_t,io_async_observer ioa,std::string_view file,open_interface_t<om>,Args&& ...args):
		basic_filebuf_file(basic_posix_file<char_type>(io_async,ioa,file,open_interface<om>,std::forward<Args>(args)...),
			open_interface<om>)
	{}
	template<typename... Args>
	basic_filebuf_file(io_async_t,io_async_observer ioa,std::string_view file,open_mode om,Args&& ...args):
		basic_filebuf_file(basic_posix_file<char_type>(io_async,ioa,file,om,std::forward<Args>(args)...),om)
	{}
	template<typename... Args>
	basic_filebuf_file(io_async_t,io_async_observer ioa,std::string_view file,std::string_view mode,Args&& ...args):
		basic_filebuf_file(basic_posix_file<char_type>(io_async,ioa,file,mode,std::forward<Args>(args)...),mode)
	{}
#endif


	inline constexpr auto& native_handle() noexcept
	{
		return fb;
	}
	inline constexpr auto& native_handle() const noexcept
	{
		return fb;
	}
	inline constexpr auto rdbuf() noexcept
	{
		return std::addressof(fb);
	}
	inline constexpr auto rdbuf() const noexcept
	{
		return std::addressof(fb);
	}
	operator basic_filebuf_io_observer<char_type,traits_type>() noexcept
	{
		return basic_filebuf_io_observer<char_type,traits_type>{std::addressof(fb)};
	}
#if defined(__GLIBCXX__) || defined(__LIBCPP_VERSION)  || defined(_MSVC_STL_UPDATE)
	explicit operator basic_c_io_observer_unlocked<char_type>()
	{
		return static_cast<basic_c_io_observer_unlocked<char_type>>(static_cast<basic_filebuf_io_observer<char_type,traits_type>>(*this));
	}
	explicit operator basic_c_io_observer<char_type>()
	{
		return static_cast<basic_c_io_observer<char_type>>(static_cast<basic_filebuf_io_observer<char_type,traits_type>>(*this));
	}
	explicit operator basic_posix_io_observer<char_type>()
	{
		return static_cast<basic_posix_io_observer<char_type>>(static_cast<basic_c_io_observer_unlocked<char_type>>(*this));
	}
#if defined(__WINNT__) || defined(_MSC_VER)
	explicit operator basic_win32_io_observer<char_type>()
	{
		return static_cast<basic_win32_io_observer<char_type>>
		(static_cast<basic_posix_io_observer<char_type>>(*this));
	}
	explicit operator basic_nt_io_observer<char_type>()
	{
		return static_cast<basic_nt_io_observer<char_type>>
		(static_cast<basic_posix_io_observer<char_type>>(*this));
	}
#endif
#endif

#if defined(_MSVC_STL_UPDATE)
	basic_filebuf_file& operator=(basic_filebuf_file const&)=delete;
	basic_filebuf_file(basic_filebuf_file const&)=delete;
	basic_filebuf_file(basic_filebuf_file&& bf) noexcept:fb(std::move(bf.fb))
	{
	}
	basic_filebuf_file& operator=(basic_filebuf_file&& bf) noexcept
	{
		fb.close();
		fb=std::move(bf.fb);
		return *this;
	}
	~basic_filebuf_file()
	{
		fb.close();
	}
#endif
};

template<std::integral CharT,typename Traits,std::contiguous_iterator Iter>
inline Iter read(basic_filebuf_file<CharT,Traits>& t,Iter begin,Iter end)
{
	basic_filebuf_io_observer<CharT,Traits> fiob{t.rdbuf()};
	return read(fiob,begin,end);
}

template<std::integral CharT,typename Traits,std::contiguous_iterator Iter>
inline Iter write(basic_filebuf_file<CharT,Traits>& t,Iter begin,Iter end)
{
	basic_filebuf_io_observer<CharT,Traits> fiob{t.rdbuf()};
	return write(fiob,begin,end);
}
template<std::integral CharT,typename Traits>
inline void flush(basic_filebuf_file<CharT,Traits>& t)
{
	basic_filebuf_io_observer<CharT,Traits> fiob{t.rdbuf()};
	flush(fiob);
}

template<std::integral CharT,typename Traits>
requires buffer_input_stream<basic_filebuf_io_observer<CharT,Traits>>
inline auto ibuffer_begin(basic_filebuf_file<CharT,Traits>& t)
{
	basic_filebuf_io_observer<CharT,Traits> fiob{t.rdbuf()};
	return ibuffer_begin(fiob);
}

template<std::integral CharT,typename Traits>
requires buffer_input_stream<basic_filebuf_io_observer<CharT,Traits>>
inline auto ibuffer_curr(basic_filebuf_file<CharT,Traits>& t)
{
	basic_filebuf_io_observer<CharT,Traits> fiob{t.rdbuf()};
	return ibuffer_curr(fiob);
}

template<std::integral CharT,typename Traits>
requires buffer_input_stream<basic_filebuf_io_observer<CharT,Traits>>
inline auto ibuffer_end(basic_filebuf_file<CharT,Traits>& t)
{
	basic_filebuf_io_observer<CharT,Traits> fiob{t.rdbuf()};
	return ibuffer_end(fiob);
}

template<std::integral CharT,typename Traits>
requires buffer_input_stream<basic_filebuf_io_observer<CharT,Traits>>
inline void ibuffer_set_curr(basic_filebuf_file<CharT,Traits>& t,CharT* ptr)
{
	basic_filebuf_io_observer<CharT,Traits> fiob{t.rdbuf()};
	ibuffer_set_curr(fiob,ptr);
}

template<std::integral CharT,typename Traits>
requires buffer_input_stream<basic_filebuf_io_observer<CharT,Traits>>
inline auto underflow(basic_filebuf_file<CharT,Traits>& t)
{
	basic_filebuf_io_observer<CharT,Traits> fiob{t.rdbuf()};
	return underflow(fiob);
}

template<std::integral CharT,typename Traits>
requires buffer_output_stream<basic_filebuf_io_observer<CharT,Traits>>
inline auto obuffer_begin(basic_filebuf_file<CharT,Traits>& t)
{
	basic_filebuf_io_observer<CharT,Traits> fiob{t.rdbuf()};
	return obuffer_begin(fiob);
}

template<std::integral CharT,typename Traits>
requires buffer_output_stream<basic_filebuf_io_observer<CharT,Traits>>
inline auto obuffer_curr(basic_filebuf_file<CharT,Traits>& t)
{
	basic_filebuf_io_observer<CharT,Traits> fiob{t.rdbuf()};
	return obuffer_curr(fiob);
}

template<std::integral CharT,typename Traits>
requires buffer_output_stream<basic_filebuf_io_observer<CharT,Traits>>
inline auto obuffer_end(basic_filebuf_file<CharT,Traits>& t)
{
	basic_filebuf_io_observer<CharT,Traits> fiob{t.rdbuf()};
	return obuffer_end(fiob);
}

template<std::integral CharT,typename Traits>
requires buffer_output_stream<basic_filebuf_io_observer<CharT,Traits>>
inline void obuffer_set_curr(basic_filebuf_file<CharT,Traits>& t,CharT* ptr)
{
	basic_filebuf_io_observer<CharT,Traits> fiob{t.rdbuf()};
	obuffer_set_curr(fiob,ptr);
}
template<std::integral CharT,typename Traits>
requires buffer_output_stream<basic_filebuf_io_observer<CharT,Traits>>
inline void overflow(basic_filebuf_file<CharT,Traits>& t,CharT ch)
{
	basic_filebuf_io_observer<CharT,Traits> fiob{t.rdbuf()};
	overflow(fiob,ch);
}

template<std::integral CharT,typename Traits>
requires zero_copy_input_stream<basic_filebuf_io_observer<CharT,Traits>>
inline constexpr decltype(auto) zero_copy_in_handle(basic_filebuf_file<CharT,Traits>& t)
{
	basic_filebuf_io_observer<CharT,Traits> fiob{t.rdbuf()};
	return zero_copy_in_handle(fiob);
}

template<std::integral CharT,typename Traits>
requires zero_copy_output_stream<basic_filebuf_io_observer<CharT,Traits>>
inline constexpr decltype(auto) zero_copy_out_handle(basic_filebuf_file<CharT,Traits>& t)
{
	basic_filebuf_io_observer<CharT,Traits> fiob{t.rdbuf()};
	return zero_copy_out_handle(fiob);
}

template<std::integral CharT,typename Traits,typename... Args>
requires random_access_stream<basic_filebuf_io_observer<CharT,Traits>>
inline auto seek(basic_filebuf_file<CharT,Traits>& t,Args&& ...args)
{
	basic_filebuf_io_observer<CharT,Traits> fiob{t.rdbuf()};
	return seek(fiob,std::forward<Args>(args)...);
}

template<std::integral CharT,typename Traits,typename... Args>
requires io_controllable<basic_filebuf_io_observer<CharT,Traits>,Args...>
inline decltype(auto) io_control(basic_filebuf_file<CharT,Traits>& t,Args&& ...args)
{
	basic_filebuf_io_observer<CharT,Traits> fiob{t.rdbuf()};
	return io_control(fiob,std::forward<Args>(args)...);
}


template<std::integral CharT,typename Traits>
requires async_stream<basic_c_io_observer_unlocked<CharT>>
inline constexpr io_async_scheduler_t<basic_c_io_observer_unlocked<CharT>>
	async_scheduler_type(basic_filebuf_file<CharT,Traits>&)
{
	return {};
}

template<std::integral CharT,typename Traits>
requires async_stream<basic_c_io_observer_unlocked<CharT>>
inline constexpr io_async_overlapped_t<basic_c_io_observer_unlocked<CharT>>
	async_overlapped_type(basic_filebuf_file<CharT,Traits>&)
{
	return {};
}

template<std::integral CharT,typename Traits,typename... Args>
requires async_output_stream<basic_c_io_observer_unlocked<CharT>>
inline void async_write_callback(io_async_observer ioa,basic_filebuf_file<CharT,Traits>& h,Args&& ...args)
{
	async_write_callback(ioa,static_cast<basic_c_io_observer_unlocked<CharT>>(h),std::forward<Args>(args)...);
}

template<std::integral CharT,typename Traits,typename... Args>
requires async_input_stream<basic_c_io_observer_unlocked<CharT>>
inline void async_read_callback(io_async_observer ioa,basic_filebuf_file<CharT,Traits>& h,Args&& ...args)
{
	async_read_callback(ioa,static_cast<basic_c_io_observer_unlocked<CharT>>(h),std::forward<Args>(args)...);
}


using filebuf_file=basic_filebuf_file<char>;
using wfilebuf_file=basic_filebuf_file<wchar_t>;
using u8filebuf_file=basic_filebuf_file<char8_t>;

#endif
}
