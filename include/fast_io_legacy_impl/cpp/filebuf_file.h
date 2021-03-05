#pragma once
#if defined(__GLIBCXX__)
#include<ext/stdio_filebuf.h>
#endif
#include"streambuf_io_observer.h"
namespace fast_io
{

namespace details
{

inline constexpr std::ios::openmode calculate_fstream_file_open_mode(open_mode om) noexcept
{
	std::ios::openmode ios_om{};
	if((om&open_mode::app)!=open_mode::none)
		ios_om=ios_om|std::ios::app;
	if((om&open_mode::text)==open_mode::none)
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

inline constexpr std::ios::openmode calculate_fstream_open_value(open_mode om) noexcept
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
	basic_filebuf_file(basic_posix_io_handle<char_type>&& piohd,open_mode mode):
		basic_filebuf_io_observer<CharT,Traits>{new __gnu_cxx::stdio_filebuf<char_type,traits_type>(piohd.fd,details::calculate_fstream_open_value(mode),fast_io::details::cal_buffer_size<CharT,true>())}
	{
/*
https://github.com/gcc-mirror/gcc/blob/41d6b10e96a1de98e90a7c0378437c3255814b16/libstdc%2B%2B-v3/config/io/basic_file_stdio.cc#L216
__basic_file<char>::sys_open(int __fd, ios_base::openmode __mode) throw ()
This function never fails. but what if fdopen fails?
*/
		if(!this->fb->is_open())
		{
			delete this->fb;
			throw_posix_error();
		}
		piohd.release();
	}
	basic_filebuf_file(basic_c_io_handle_unlocked<char_type>&& chd,open_mode mode):
		basic_filebuf_io_observer<CharT,Traits>{new __gnu_cxx::stdio_filebuf<char_type,traits_type>(chd.fp,details::calculate_fstream_open_value(mode),fast_io::details::cal_buffer_size<CharT,true>())}
	{
		if(!this->fb->is_open())
		{
			delete this->fb;
			throw_posix_error();
		}
		chd.release();
		details::streambuf_hack::hack_set_close(this->fb);
	}
	basic_filebuf_file(basic_c_io_handle<char_type>&& chd,open_mode mode):
		basic_filebuf_io_observer<CharT,Traits>{new __gnu_cxx::stdio_filebuf<char_type,traits_type>(chd.fp,details::calculate_fstream_open_value(mode),fast_io::details::cal_buffer_size<CharT,true>())}
	{
		if(!this->fb->is_open())
		{
			delete this->fb;
			throw_posix_error();
		}
		chd.release();
		details::streambuf_hack::hack_set_close(this->fb);
	}
#elif defined(__LIBCPP_VERSION)
	basic_filebuf_file(basic_posix_io_handle<char_type>&& piohd,open_mode mode):
		basic_filebuf_io_observer<CharT,Traits>{new std::basic_filebuf<char_type,traits_type>}
	{
		fb.__open(piohd.native_handle(),details::calculate_fstream_open_value(mode));
		if(!this->fb->is_open())
		{
			delete this->fb;
			throw_posix_error();
		}
		piohd.release();
	}
#else
	basic_filebuf_file(basic_c_io_handle_unlocked<char_type>&& chd,open_mode):basic_filebuf_io_observer<CharT,Traits>{new std::basic_filebuf<char_type,traits_type>(chd.native_handle())}
	{
		if(!this->fb->is_open())
		{
			delete this->fb;
			throw_posix_error();
		}
		chd.release();
#if _MSVC_STL_UPDATE
		details::streambuf_hack::msvc_hack_set_close(this->fb);
#endif
	}
	basic_filebuf_file(basic_c_io_handle<char_type>&& chd,open_mode):basic_filebuf_io_observer<CharT,Traits>{new std::basic_filebuf<char_type,traits_type>(chd.fp)}
	{
		if(!this->fb->is_open())
		{
			delete this->fb;
			throw_posix_error();
		}
		chd.release();
#if _MSVC_STL_UPDATE
		details::streambuf_hack::msvc_hack_set_close(this->fb);
#endif
	}
	basic_filebuf_file(basic_posix_io_handle<char_type>&& piohd,open_mode mode):
		basic_filebuf_file(basic_c_file_unlocked<char_type>(std::move(piohd),mode),mode)
	{
	}
#endif

#ifdef _WIN32
//windows specific. open posix file from win32 io handle
	basic_filebuf_file(basic_win32_io_handle<char_type>&& win32_handle,open_mode mode):
		basic_filebuf_file(basic_posix_file<char_type>(std::move(win32_handle),mode),mode)
	{
	}
	template<nt_family family>
	basic_filebuf_file(basic_nt_family_io_handle<family,char_type>&& nt_handle,open_mode mode):
		basic_filebuf_file(basic_posix_file<char_type>(std::move(nt_handle),mode),mode)
	{
	}
#endif

	basic_filebuf_file(native_fs_dirent fsdirent,open_mode om,perms pm=static_cast<perms>(436)):
		basic_filebuf_file(basic_posix_file<char_type>(fsdirent,om,pm),om)
	{}
	basic_filebuf_file(cstring_view file,open_mode om,perms pm=static_cast<perms>(436)):
		basic_filebuf_file(basic_posix_file<char_type>(file,om,pm),om)
	{}
	basic_filebuf_file(native_at_entry nate,cstring_view file,open_mode om,perms pm=static_cast<perms>(436)):
		basic_filebuf_file(basic_posix_file<char_type>(nate,file,om,pm),om)
	{}
	basic_filebuf_file(wcstring_view file,open_mode om,perms pm=static_cast<perms>(436)):
		basic_filebuf_file(basic_posix_file<char_type>(file,om,pm),om)
	{}
	basic_filebuf_file(native_at_entry nate,wcstring_view file,open_mode om,perms pm=static_cast<perms>(436)):
		basic_filebuf_file(basic_posix_file<char_type>(nate,file,om,pm),om)
	{}
	basic_filebuf_file(u8cstring_view file,open_mode om,perms pm=static_cast<perms>(436)):
		basic_filebuf_file(basic_posix_file<char_type>(file,om,pm),om)
	{}
	basic_filebuf_file(native_at_entry nate,u8cstring_view file,open_mode om,perms pm=static_cast<perms>(436)):
		basic_filebuf_file(basic_posix_file<char_type>(nate,file,om,pm),om)
	{}
	basic_filebuf_file(u16cstring_view file,open_mode om,perms pm=static_cast<perms>(436)):
		basic_filebuf_file(basic_posix_file<char_type>(file,om,pm),om)
	{}
	basic_filebuf_file(native_at_entry nate,u16cstring_view file,open_mode om,perms pm=static_cast<perms>(436)):
		basic_filebuf_file(basic_posix_file<char_type>(nate,file,om,pm),om)
	{}
	basic_filebuf_file(u32cstring_view file,open_mode om,perms pm=static_cast<perms>(436)):
		basic_filebuf_file(basic_posix_file<char_type>(file,om,pm),om)
	{}
	basic_filebuf_file(native_at_entry nate,u32cstring_view file,open_mode om,perms pm=static_cast<perms>(436)):
		basic_filebuf_file(basic_posix_file<char_type>(nate,file,om,pm),om)
	{}
	basic_filebuf_file& operator=(basic_filebuf_file const&)=delete;
	basic_filebuf_file(basic_filebuf_file const&)=delete;
	basic_filebuf_file(basic_filebuf_file&& other) noexcept:basic_filebuf_io_observer<CharT,Traits>{other.release()}{}

public:
	basic_filebuf_file& operator=(basic_filebuf_file&& bf) noexcept
	{
		if(this->fb==bf.fb)[[unlikely]]
			return *this;
		delete this->fb;
		this->fb=bf.release();
		return *this;
	}
	void close()
	{
		if(this->fb)[[likely]]
		{
			this->fb->clear();
			this->fb->close();
			if(this->fb->bad())[[unlikely]]
				throw_posix_error();
			delete this->fb;
			this->fb=nullptr;
		}
	}
	void reset(native_handle_type fb=nullptr) noexcept
	{
		delete this->fb;
		this->fb=fb;
	}
	~basic_filebuf_file()
	{
		delete this->fb;
	}
};

using filebuf_file=basic_filebuf_file<char>;
using wfilebuf_file=basic_filebuf_file<wchar_t>;
#if defined(_MSVC_STL_UPDATE)
#pragma warning(push, _STL_WARNING_LEVEL)
#pragma warning(disable : 4996)
#endif
using u8filebuf_file=basic_filebuf_file<char8_t>;
using u16filebuf_file=basic_filebuf_file<char16_t>;
using u32filebuf_file=basic_filebuf_file<char32_t>;
#if defined(_MSVC_STL_UPDATE)
#pragma warning(pop)
#endif

}
