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
	if(((om&open_mode::directory)!=open_mode::none)&&ios_om==std::ios::openmode{})
		ios_om=ios_om|std::ios::in;
	return ios_om;
}

inline constexpr std::ios::openmode calculate_fstream_open_value(open_mode om)
{
	return calculate_fstream_file_open_mode(om);
}
}


template<std::integral CharT,typename Traits = std::char_traits<CharT>>
class basic_filebuf_file:public basic_filebuf_io_observer<CharT,Traits>
{
public:
	using char_type = CharT;
	using traits_type = Traits;
	using native_handle_type = std::basic_filebuf<char_type,traits_type>*;
	constexpr basic_filebuf_file()=default;
	template<typename native_hd>
	requires std::same_as<native_handle_type,std::remove_cvref_t<native_hd>>
	constexpr basic_filebuf_file(native_hd fb):basic_filebuf_io_observer<CharT,Traits>{fb}{}
#if defined(__GLIBCXX__)
	template<typename T>
	basic_filebuf_file(basic_posix_io_handle<char_type>&& piohd,T&& t):
		basic_filebuf_io_observer<CharT,Traits>{new __gnu_cxx::stdio_filebuf<char_type,traits_type>(piohd.native_handle(),details::calculate_fstream_open_value(std::forward<T>(t)),fast_io::details::cal_buffer_size<CharT,true>())}
	{
/*
https://github.com/gcc-mirror/gcc/blob/41d6b10e96a1de98e90a7c0378437c3255814b16/libstdc%2B%2B-v3/config/io/basic_file_stdio.cc#L216
__basic_file<char>::sys_open(int __fd, ios_base::openmode __mode) throw ()
This function never fails. but what if fdopen fails?
*/
		if(!this->native_handle()->is_open())
		{
			delete this->native_handle();
			throw_posix_error();
		}
		piohd.release();
	}
#elif defined(__LIBCPP_VERSION)
	template<typename T>
	basic_filebuf_file(basic_posix_io_handle<char_type>&& piohd,T&& t):
		basic_filebuf_io_observer<CharT,Traits>{new std::basic_filebuf<char_type,traits_type>}
	{
		fb.__open(piohd.native_handle(),details::calculate_fstream_open_value(std::forward<T>(t)));
		if(!this->native_handle()->is_open())
		{
			delete this->native_handle();
			throw_posix_error();
		}
		piohd.release();
	}
#else
	template<typename T>
	basic_filebuf_file(basic_c_io_handle_unlocked<char_type>&& chd,T&& t):basic_filebuf_io_observer<CharT,Traits>{new std::basic_filebuf<char_type,traits_type>(chd.native_handle())}
	{
		if(!this->native_handle()->is_open())
		{
			delete this->native_handle();
			throw_posix_error();
		}
		chd.release();
	}
	template<typename T>
	basic_filebuf_file(basic_posix_io_handle<char_type>&& piohd,T&& t):
		basic_filebuf_file(basic_c_file_unlocked<char_type>(std::move(piohd),std::forward<T>(t)),std::forward<T>(t))
	{
	}
#endif

#ifdef _WIN32
//windows specific. open posix file from win32 io handle
	template<typename T>
	basic_filebuf_file(basic_win32_io_handle<char_type>&& win32_handle,T&& t):
		basic_filebuf_file(basic_posix_file<char_type>(std::move(win32_handle),std::forward<T>(t)),std::forward<T>(t))
	{
	}
	basic_filebuf_file(wcstring_view file,open_mode om,perms pm=static_cast<perms>(436)):
		basic_filebuf_file(basic_posix_file<char_type>(file,om,pm),om)
	{}
	basic_filebuf_file(native_at_entry nate,wcstring_view file,open_mode om,perms pm=static_cast<perms>(436)):
		basic_filebuf_file(basic_posix_file<char_type>(nate,file,om,pm),om)
	{}
#endif
	basic_filebuf_file(cstring_view file,open_mode om,perms pm=static_cast<perms>(436)):
		basic_filebuf_file(basic_posix_file<char_type>(file,om,pm),om)
	{}
	basic_filebuf_file(native_at_entry nate,cstring_view file,open_mode om,perms pm=static_cast<perms>(436)):
		basic_filebuf_file(basic_posix_file<char_type>(nate,file,om,pm),om)
	{}


	basic_filebuf_file& operator=(basic_filebuf_file const&)=delete;
	basic_filebuf_file(basic_filebuf_file const&)=delete;
	basic_filebuf_file(basic_filebuf_file&& other) noexcept:basic_filebuf_io_observer<CharT,Traits>{other.release()}{}

private:
	void close_impl() noexcept
	{
#if defined(_MSVC_STL_UPDATE)
		if(this->native_handle())[[likely]]
		{
			this->native_handle()->close();
			delete this->native_handle();
		}
#else
		delete this->native_handle();
#endif
	}
public:
	basic_filebuf_file& operator=(basic_filebuf_file&& bf) noexcept
	{
		if(this->native_handle()==bf.native_handle())[[unlikely]]
			return *this;
		close_impl();
		this->native_handle()=bf.release();
		return *this;
	}
	void close()
	{
		if(this->native_handle())[[likely]]
		{
			this->native_handle()->clear();
			this->native_handle()->close();
			if(this->native_handle()->bad())[[unlikely]]
				throw_posix_error();
			delete this->native_handle();
			this->native_handle()=nullptr;
		}
	}
	void reset(native_handle_type fb=nullptr) noexcept
	{
		close_impl();
		this->native_handle()=fb;
	}
	~basic_filebuf_file()
	{
		close_impl();
	}
};

using filebuf_file=basic_filebuf_file<char>;
using wfilebuf_file=basic_filebuf_file<wchar_t>;
using u8filebuf_file=basic_filebuf_file<char8_t>;

static_assert(std::is_standard_layout_v<filebuf_file>);

}
