#pragma once

#if defined(__WINNT__) || defined(_MSC_VER)
#include<io.h>
#else
#include<unistd.h>
#endif
#include<fcntl.h>
#ifdef __linux__
#include<sys/uio.h>
#include<sys/sendfile.h>
struct io_uring;
#endif
#ifdef __BSD_VISIBLE
#ifndef __NEWLIB__
#include <sys/uio.h>
#include <sys/types.h>
#include <sys/socket.h>
#endif
#endif

namespace fast_io
{
	
namespace details
{
#if defined(__WINNT__) || defined(_MSC_VER)
template<bool wide_char=false>
inline constexpr int calculate_posix_open_mode_for_win32_handle(open_mode value)
{
	int mode{};
	if((value&open_mode::binary)!=open_mode::none)
		mode = _O_BINARY;
	else
	{
		if constexpr(wide_char)
			mode |= _O_WTEXT;
		else
			mode |= _O_TEXT;
	}
	constexpr auto supported_values{open_mode::out|open_mode::app|open_mode::in};
	using utype = typename std::underlying_type<open_mode>::type;
	switch(static_cast<utype>(supported_values)&static_cast<utype>(value))
	{
//Action if file already exists;	Action if file does not exist;	c-style mode;	Explanation
//Read from start;	Failure to open;	"r";	Open a file for reading
	case static_cast<utype>(open_mode::in):
		return mode | O_RDONLY;
//Destroy contents;	Create new;	"w";	Create a file for writing
	case static_cast<utype>(open_mode::out):
//Read from start;	Error;	"r+";		Open a file for read/write
	case static_cast<utype>(open_mode::out)|static_cast<utype>(open_mode::in):
		return mode;
//Append to file;	Create new;	"a";	Append to a file
	case static_cast<utype>(open_mode::app):
	case static_cast<utype>(open_mode::out)|static_cast<utype>(open_mode::app):
		return mode | O_APPEND;
//Write to end;	Create new;	"a+";	Open a file for read/write
	case static_cast<utype>(open_mode::out)|static_cast<utype>(open_mode::in)|static_cast<utype>(open_mode::app):
	case static_cast<utype>(open_mode::in)|static_cast<utype>(open_mode::app):
		return mode | O_APPEND;
//Destroy contents;	Error;	"wx";	Create a file for writing
	default:
		throw_posix_error(EINVAL);
	}
}
template<open_mode om>
struct posix_file_openmode_for_win32_handle
{
	static int constexpr mode = calculate_posix_open_mode_for_win32_handle(om);
};
#endif


inline constexpr int calculate_posix_open_mode(open_mode value)
{
	int mode
	{
#ifdef O_NOFOLLOW
		O_NOFOLLOW
#endif
	};
	if((value&open_mode::follow)!=open_mode::none)
		mode = {};
	if((value&open_mode::inherit)==open_mode::none)
#ifdef O_CLOEXEC
		mode |= O_CLOEXEC;
#elif _O_NOINHERIT
		mode |= _O_NOINHERIT;
#endif
#ifdef O_BINARY
	if((value&open_mode::binary)!=open_mode::none)
		mode |= O_BINARY;
#endif
	if((value&open_mode::excl)!=open_mode::none)
		mode |= O_CREAT | O_EXCL;
	if((value&open_mode::trunc)!=open_mode::none)
		mode |= O_TRUNC;
#ifdef O_DIRECT
	if((value&open_mode::direct)!=open_mode::none)
		mode |= O_DIRECT;
#endif

#ifdef O_SYNC
	if((value&open_mode::sync)!=open_mode::none)
		mode |= O_SYNC;
#endif
/*
	if((value&open_mode::directory)!=open_mode::none)
#ifdef O_DIRECTORY
		mode |= O_DIRECTORY;
#else
		throw_posix_error(EOPNOTSUPP);
#endif
*/
#ifdef O_NOCTTY
	if((value&open_mode::no_ctty)!=open_mode::none)
		mode |= O_NOCTTY;
#endif
#ifdef O_NOATIME
	if((value&open_mode::no_atime)!=open_mode::none)
		mode |= O_NOATIME;
#endif
	if((value&open_mode::no_block)!=open_mode::none)
#ifdef O_NONBLOCK
		mode |= O_NONBLOCK;
#else
		throw_posix_error(EOPNOTSUPP);
#endif

#ifdef _O_TEMPORARY
	if((value&open_mode::temprorary)!=open_mode::none)
		mode |= _O_TEMPORARY;
#endif
#ifdef _O_SEQUENTIAL
	if((value&open_mode::random_access)!=open_mode::none)
		mode |= _O_SEQUENTIAL;
	else
		mode |= _O_RANDOM;
#endif
	using utype = typename std::underlying_type<open_mode>::type;
	constexpr auto supported_values{static_cast<utype>(open_mode::out)|static_cast<utype>(open_mode::app)|static_cast<utype>(open_mode::in)};
	switch(static_cast<utype>(value)&static_cast<utype>(supported_values))
	{
//Action if file already exists;	Action if file does not exist;	c-style mode;	Explanation
//Read from start;	Failure to open;	"r";	Open a file for reading
	case static_cast<utype>(open_mode::in):
		return mode | O_RDONLY;
//Destroy contents;	Create new;	"w";	Create a file for writing
	case static_cast<utype>(open_mode::out):
		return mode | O_WRONLY | O_CREAT | O_TRUNC;
//Append to file;	Create new;	"a";	Append to a file
	case static_cast<utype>(open_mode::app):
	case static_cast<utype>(open_mode::out)|static_cast<utype>(open_mode::app):
		return mode | O_WRONLY | O_CREAT | O_APPEND;
//Read from start;	Error;	"r+";		Open a file for read/write
	case static_cast<utype>(open_mode::out)|static_cast<utype>(open_mode::in):
		return mode | O_RDWR;
//Write to end;	Create new;	"a+";	Open a file for read/write
	case static_cast<utype>(open_mode::out)|static_cast<utype>(open_mode::in)|static_cast<utype>(open_mode::app):
	case static_cast<utype>(open_mode::in)|static_cast<utype>(open_mode::app):
		return mode | O_RDWR | O_CREAT | O_APPEND;
//Destroy contents;	Error;	"wx";	Create a file for writing
	default:
		throw_posix_error(EINVAL);
	}
}
template<open_mode om>
struct posix_file_openmode
{
	static int constexpr mode = calculate_posix_open_mode(om);
};
}

#ifdef __linux__
class io_uring_observer
{
public:
	using native_handle_type = struct ::io_uring*;
	native_handle_type ring{};
	constexpr native_handle_type& native_handle() noexcept
	{
		return ring;
	}
	constexpr native_handle_type const& native_handle() const noexcept
	{
		return ring;
	}
	constexpr native_handle_type release() noexcept
	{
		auto temp{ring};
		ring=nullptr;
		return temp;
	}
};
using io_async_observer=io_uring_observer;
#endif

template<std::integral ch_type>
class basic_posix_io_observer
{
public:
	using char_type = ch_type;
	using native_handle_type = int;
#if defined (__linux__) || defined(__WINNT__) || defined(_MSC_VER)
	using async_scheduler_type = io_async_observer;
#endif
	native_handle_type fd=-1;
	constexpr auto& native_handle() noexcept
	{
		return fd;
	}
	constexpr auto& native_handle() const noexcept
	{
		return fd;
	}
	explicit constexpr operator bool() const noexcept
	{
		return fd!=-1;
	}
#if defined(__WINNT__) || defined(_MSC_VER)
	explicit operator basic_win32_io_observer<char_type>() const
	{
		auto os_handle(_get_osfhandle(fd));
		if(os_handle==-1)
			throw_posix_error();
		return {bit_cast<void*>(os_handle)};
	}
	explicit operator basic_nt_io_observer<char_type>() const
	{
		return static_cast<basic_nt_io_observer<char_type>>(static_cast<basic_win32_io_observer<char_type>>(*this));
	}
#endif
};

template<std::integral ch_type>
class basic_posix_io_handle:public basic_posix_io_observer<ch_type>
{
protected:
	void close_impl() noexcept
	{
		if(this->native_handle()!=-1)
#if defined(__linux__)&&(defined(__x86_64__) || defined(__arm64__) || defined(__aarch64__) )
			system_call<
#if defined(__x86_64__)
			3
#elif defined(__arm64__) || defined(__aarch64__) 
			57
#endif
			,int>(this->native_handle());
#else
			close(this->native_handle());
#endif
	}
public:
	using char_type = ch_type;
	using native_handle_type = int;
#if defined (__linux__) || defined(__WINNT__) || defined(_MSC_VER)
	using async_scheduler_type = io_async_observer;
#endif
	constexpr explicit basic_posix_io_handle()=default;
	constexpr explicit basic_posix_io_handle(int fdd):basic_posix_io_observer<ch_type>{fdd}{}
	basic_posix_io_handle(basic_posix_io_handle const& dp):basic_posix_io_observer<ch_type>{
#if defined(__linux__)&&(defined(__x86_64__) || defined(__arm64__) || defined(__aarch64__) )
		system_call<
#if defined(__x86_64__)
		32
#elif defined(__arm64__) || defined(__aarch64__) 
		23
#endif
		,int>
#else
		dup
#endif
(dp.native_handle())}
	{
		system_call_throw_error(this->native_handle());
	}
	basic_posix_io_handle& operator=(basic_posix_io_handle const& dp)
	{
		auto newfd(
#if defined(__linux__)&&(defined(__x86_64__) || defined(__arm64__) || defined(__aarch64__) )
		system_call<
#if defined(__x86_64__)
		33
#elif defined(__arm64__) || defined(__aarch64__) 
		1041
#endif
		,int>
#else
		dup2
#endif
(dp.native_handle(),this->native_handle()));
		system_call_throw_error(newfd);
		this->native_handle()=newfd;
		return *this;
	}
	constexpr basic_posix_io_handle(basic_posix_io_handle&& b) noexcept : basic_posix_io_handle(b.native_handle())
	{
		b.native_handle() = -1;
	}
	basic_posix_io_handle& operator=(basic_posix_io_handle&& b) noexcept
	{
		if(b.native_handle()!=this->native_handle())
		{
			close_impl();
			this->native_handle()=b.native_handle();
			b.native_handle() = -1;
		}
		return *this;
	}
	void detach()
	{
		this->native_handle()=-1;
	}
};

template<std::integral ch_type>
inline bool valid(basic_posix_io_observer<ch_type> h)
{
	return h.native_handle()!=-1;
}

template<std::integral ch_type,std::contiguous_iterator Iter>
inline Iter read(basic_posix_io_observer<ch_type> h,Iter begin,Iter end)
{
	auto read_bytes(
#if defined(__linux__)&&(defined(__x86_64__) || defined(__arm64__) || defined(__aarch64__) )
		system_call<
#if defined(__x86_64__)
		0
#elif defined(__arm64__) || defined(__aarch64__)
		63
#endif
		,std::ptrdiff_t>
#else
		::read
#endif
	(h.native_handle(),std::to_address(begin),(end-begin)*sizeof(*begin)));
	system_call_throw_error(read_bytes);
	return begin+(read_bytes/sizeof(*begin));
}
template<std::integral ch_type,std::contiguous_iterator Iter>
inline Iter write(basic_posix_io_observer<ch_type> h,Iter begin,Iter end)
{
	auto write_bytes(
#if defined(__linux__)&&(defined(__x86_64__) || defined(__arm64__) || defined(__aarch64__) )
		system_call<
#if defined(__x86_64__)
		1
#elif defined(__arm64__) || defined(__aarch64__)
		64
#endif
		,std::ptrdiff_t>
#else
		::write
#endif
(h.native_handle(),std::to_address(begin),(end-begin)*sizeof(*begin)));
	system_call_throw_error(write_bytes);
	return begin+(write_bytes/sizeof(*begin));
}

template<std::integral ch_type,typename T,std::integral R>
inline std::common_type_t<std::int64_t, std::size_t> seek(basic_posix_io_observer<ch_type> h,seek_type_t<T>,R i=0,seekdir s=seekdir::cur)
{
	auto ret(
#if defined(__linux__)&&(defined(__x86_64__) || defined(__arm64__) || defined(__aarch64__) )
		system_call<
#if defined(__x86_64__)
		8
#elif defined(__arm64__) || defined(__aarch64__)
		62
#endif
		,std::ptrdiff_t>
#elif defined(__linux__)
		::lseek64
#elif defined(__WINNT__) || defined(_MSC_VER)
		::_lseeki64
#else
		::lseek
#endif
		(h.native_handle(),seek_precondition<std::int64_t,T,ch_type>(i),static_cast<int>(s)));
	system_call_throw_error(ret);
	return ret;
}
template<std::integral ch_type,std::integral R>
inline auto seek(basic_posix_io_observer<ch_type> h,R i=0,seekdir s=seekdir::cur)
{
	return seek(h,seek_type<ch_type>,i,s);
}
/*
template<std::integral ch_type>
inline void flush(basic_posix_io_observer<ch_type>)
{
	// no need fsync. OS can deal with it
//		if(::fsync(fd)==-1)
//			throw posix_error();
}
*/

#if defined(__linux__) || defined(__BSD_VISIBLE)
template<std::integral ch_type>
inline auto zero_copy_in_handle(basic_posix_io_observer<ch_type> h)
{
	return h.native_handle();
}
template<std::integral ch_type>
inline auto zero_copy_out_handle(basic_posix_io_observer<ch_type> h)
{
	return h.native_handle();
}
#endif
template<std::integral ch_type>
inline auto redirect_handle(basic_posix_io_observer<ch_type> h)
{
#if defined(__WINNT__) || defined(_MSC_VER)
	return bit_cast<void*>(_get_osfhandle(h.native_handle()));
#else
	return h.native_handle();
#endif
}

#if defined(__WINNT__) || defined(_MSC_VER)
template<std::integral ch_type,typename... Args>
requires io_controllable<basic_win32_io_observer<ch_type>,Args...>
inline decltype(auto) io_control(basic_posix_io_observer<ch_type> h,Args&& ...args)
{
	return io_control(static_cast<basic_win32_io_observer<ch_type>>(h),std::forward<Args>(args)...);
}
#else

extern "C" int ioctl(int fd, unsigned long request, ...) noexcept;
template<std::integral ch_type,typename... Args>
requires requires(basic_posix_io_observer<ch_type> h,Args&& ...args)
{
	ioctl(h.native_handle(),std::forward<Args>(args)...);
}
inline void io_control(basic_posix_io_observer<ch_type> h,Args&& ...args)
{
#if defined(__linux__)&&(defined(__x86_64__) || defined(__arm64__) || defined(__aarch64__) )
	system_call_throw_error(system_call<
#if defined(__x86_64__)
		16
#elif defined(__arm64__) || defined(__aarch64__)
		29
#endif
	,int>(h.native_handle(),std::forward<Args>(args)...));
#else
	if(ioctl(h.native_handle(),std::forward<Args>(args)...)==-1)
	{
		throw_posix_error();
	}
#endif
}
#endif

template<std::integral ch_type>
class basic_posix_file:public basic_posix_io_handle<ch_type>
{
	void seek_end_local()
	{
		basic_posix_file<ch_type> local{this->native_handle()};
		seek(*this,0,seekdir::end);
		local.detach();
	};
#if defined(__WINNT__) || defined(_MSC_VER)
	using mode_t = int;
#endif
public:
	using char_type = ch_type;
	using native_handle_type = typename basic_posix_io_handle<char_type>::native_handle_type;
	using basic_posix_io_handle<ch_type>::native_handle;
#if defined (__linux__) || defined(__WINNT__) || defined(_MSC_VER)
	using async_scheduler_type = io_async_observer;
#endif
	constexpr basic_posix_file() noexcept = default;
	constexpr basic_posix_file(int fd) noexcept: basic_posix_io_handle<ch_type>(fd){}
	template<typename ...Args>
	requires requires(Args&& ...args)
	{
		{
#if defined(__WINNT__) || defined(_MSC_VER)
			::_open(
#elif defined(__NEWLIB__)
			::open(
#else
			::openat(AT_FDCWD,
#endif
		std::forward<Args>(args)...)}->std::same_as<int>;
	}
	basic_posix_file(native_interface_t,Args&& ...args):basic_posix_io_handle<ch_type>(
#if defined(__WINNT__) || defined(_MSC_VER)
			::_open(
#elif defined(__NEWLIB__)
			::open(
#else
#if defined(__linux__)&&(defined(__x86_64__) || defined(__arm64__) || defined(__aarch64__) )
		system_call<
#if defined(__x86_64__)
		257
#elif defined(__arm64__) || defined(__aarch64__)
		56
#endif
		,int>(AT_FDCWD,
#else
		::openat(AT_FDCWD,
#endif
#endif
	std::forward<Args>(args)...))
	{
/*	if(native_handle()<0)
		throw_posix_error();
*/
		system_call_throw_error(native_handle());
	}
#if defined(__WINNT__) || defined(_MSC_VER)
//windows specific. open posix file from win32 io handle
	template<open_mode om>
	basic_posix_file(basic_win32_io_handle<char_type>&& hd,open_interface_t<om>):
		basic_posix_io_handle<char_type>(::_open_osfhandle(bit_cast<std::intptr_t>(hd.native_handle()),details::posix_file_openmode_for_win32_handle<om>::mode))
	{
		if(native_handle()==-1)
			throw_posix_error();
		hd.detach();
	}
	basic_posix_file(basic_win32_io_handle<char_type>&& hd,open_mode m):
		basic_posix_io_handle<char_type>(::_open_osfhandle(bit_cast<std::intptr_t>(hd.native_handle()),details::calculate_posix_open_mode_for_win32_handle(m)))
	{
		if(native_handle()==-1)
			throw_posix_error();
		hd.detach();
	}
	basic_posix_file(basic_win32_io_handle<char_type>&& hd,std::string_view mode):basic_posix_file(std::move(hd),from_c_mode(mode)){}
	template<open_mode om,typename... Args>
	basic_posix_file(std::string_view file,open_interface_t<om>,Args&& ...args):
		basic_posix_file(basic_win32_file<char_type>(file,open_interface<om>,std::forward<Args>(args)...),open_interface<om>)
	{}
	template<typename... Args>
	basic_posix_file(std::string_view file,open_mode om,Args&& ...args):
		basic_posix_file(basic_win32_file<char_type>(file,om,std::forward<Args>(args)...),om)
	{}
	template<typename... Args>
	basic_posix_file(std::string_view file,std::string_view mode,Args&& ...args):
		basic_posix_file(basic_win32_file<char_type>(file,mode,std::forward<Args>(args)...),mode)
	{}

	template<open_mode om,typename... Args>
	basic_posix_file(io_async_t,io_async_observer ioa,std::string_view file,open_interface_t<om>,Args&& ...args):
		basic_posix_file(basic_win32_file<char_type>(io_async,ioa,file,open_interface<om>,std::forward<Args>(args)...),open_interface<om>)
	{}
	template<typename... Args>
	basic_posix_file(io_async_t,io_async_observer ioa,std::string_view file,open_mode om,Args&& ...args):
		basic_posix_file(basic_win32_file<char_type>(io_async,ioa,file,om,std::forward<Args>(args)...),om)
	{}
	template<typename... Args>
	basic_posix_file(io_async_t,io_async_observer ioa,std::string_view file,std::string_view mode,Args&& ...args):
		basic_posix_file(basic_win32_file<char_type>(io_async,ioa,file,mode,std::forward<Args>(args)...),mode)
	{}
#else
	template<open_mode om,perms pm>
	basic_posix_file(std::string_view file,open_interface_t<om>,perms_interface_t<pm>):basic_posix_file(native_interface,file.data(),details::posix_file_openmode<om>::mode,static_cast<mode_t>(pm))
	{
		if constexpr ((om&open_mode::ate)!=open_mode::none)
			seek_end_local();
	}
	template<open_mode om>
	basic_posix_file(std::string_view file,open_interface_t<om>):basic_posix_file(native_interface,file.data(),details::posix_file_openmode<om>::mode,static_cast<mode_t>(436))
	{
		if constexpr ((om&open_mode::ate)!=open_mode::none)
			seek_end_local();
	}
	template<open_mode om>
	basic_posix_file(std::string_view file,open_interface_t<om>,perms pm):basic_posix_file(native_interface,file.data(),details::posix_file_openmode<om>::mode,static_cast<mode_t>(pm))
	{
		if constexpr ((om&open_mode::ate)!=open_mode::none)
			seek_end_local();
	}
	//potential support modification prv in the future
	basic_posix_file(std::string_view file,open_mode om,perms pm=static_cast<perms>(436)):basic_posix_file(native_interface,file.data(),details::calculate_posix_open_mode(om),static_cast<mode_t>(pm))
	{
		if((om&open_mode::ate)!=open_mode::none)
			seek_end_local();
	}
	basic_posix_file(std::string_view file,std::string_view mode,perms pm=static_cast<perms>(436)):basic_posix_file(file,from_c_mode(mode),pm){}
#ifdef __linux__
	template<typename ...Args>
	basic_posix_file(io_async_t,io_uring_observer,std::string_view file,Args&& ...args):basic_posix_file(file,std::forward<Args>(args)...){}
#endif

#endif
	~basic_posix_file()
	{
		this->close_impl();
	}
};
#if !defined(__NEWLIB__)
template<std::integral ch_type>
inline void truncate(basic_posix_io_observer<ch_type> h,std::size_t size)
{
#if defined(__WINNT__) || defined(_MSC_VER)
	auto err(_chsize_s(h.native_handle(),size));
	if(err)
		throw_posix_error(err);
#else
	if(::ftruncate(h.native_handle(),size)<0)
		throw_posix_error();
#endif
}
#endif

template<std::integral ch_type>
class basic_posix_pipe
{
public:
	using char_type = ch_type;
	using native_handle_type = std::array<basic_posix_file<ch_type>,2>;
#if defined (__linux__) || defined(__WINNT__) || defined(_MSC_VER)
	using async_scheduler_type = io_async_observer;
#endif
private:
	native_handle_type pipes;
public:
	basic_posix_pipe()
	{
		std::array<int,2> a2{pipes.front().native_handle(),pipes.back().native_handle()};
#if defined(__WINNT__) || defined(_MSC_VER)
		if(_pipe(a2.data(),1048576,_O_BINARY)==-1)
#else
		if(::pipe(a2.data())==-1)
#endif
			throw_posix_error();
		pipes.front().native_handle()=a2.front();
		pipes.back().native_handle()=a2.back();
	}
	auto& native_handle()
	{
		return pipes;
	}
	auto& in()
	{
		return pipes.front();
	}
	auto& out()
	{
		return pipes.back();
	}
	void swap(basic_posix_pipe& o) noexcept
	{
		using std::swap;
		swap(pipes,o.pipes);
	}
};
template<std::integral ch_type>
inline void swap(basic_posix_pipe<ch_type>& a,basic_posix_pipe<ch_type>& b) noexcept
{
	a.swap(b);
}

template<std::integral ch_type,std::contiguous_iterator Iter>
inline Iter read(basic_posix_pipe<ch_type>& h,Iter begin,Iter end)
{
	return read(h.in(),begin,end);
}
template<std::integral ch_type,std::contiguous_iterator Iter>
inline Iter write(basic_posix_pipe<ch_type>& h,Iter begin,Iter end)
{
	return write(h.out(),begin,end);
}

template<std::integral ch_type>
inline void flush(basic_posix_pipe<ch_type>&)
{
	// no need fsync. OS can deal with it
//		if(::fsync(fd)==-1)
//			throw posix_error();
}

template<std::integral ch_type>
inline std::array<int*,2> redirect_handle(basic_posix_pipe<ch_type>& h)
{
	return {std::addressof(h.in().native_handle()),
		std::addressof(h.out().native_handle())};
}

#ifdef __linux__
template<std::integral ch_type>
inline auto zero_copy_in_handle(basic_posix_pipe<ch_type>& h)
{
	return h.in().native_handle();
}
template<std::integral ch_type>
inline auto zero_copy_out_handle(basic_posix_pipe<ch_type>& h)
{
	return h.out().native_handle();
}
#endif

using posix_io_observer=basic_posix_io_observer<char>;
using posix_io_handle=basic_posix_io_handle<char>;
using posix_file=basic_posix_file<char>;
using posix_pipe=basic_posix_pipe<char>;

using u8posix_io_observer=basic_posix_io_observer<char8_t>;
using u8posix_io_handle=basic_posix_io_handle<char8_t>;
using u8posix_file=basic_posix_file<char8_t>;
using u8posix_pipe=basic_posix_pipe<char8_t>;

using wposix_io_observer=basic_posix_io_observer<wchar_t>;
using wposix_io_handle=basic_posix_io_handle<wchar_t>;
using wposix_file=basic_posix_file<wchar_t>;
using wposix_pipe=basic_posix_pipe<wchar_t>;

inline int constexpr posix_stdin_number = 0;
inline int constexpr posix_stdout_number = 1;
inline int constexpr posix_stderr_number = 2;
#if defined(__linux__)||(defined(__BSD_VISIBLE)&&!defined(__NEWLIB__))

//zero copy IO for linux

//To verify whether other BSD platforms support sendfile
namespace details
{

template<bool random_access=false,bool report_einval=false,zero_copy_output_stream output,zero_copy_input_stream input>
inline std::conditional_t<report_einval,std::pair<std::size_t,bool>,std::size_t>
	zero_copy_transmit_once(output& outp,input& inp,std::size_t bytes,std::intmax_t offset)
{
#ifdef __linux__
	std::intmax_t *np{};
	if constexpr(random_access)
		np=std::addressof(offset);
	auto transmitted_bytes(::sendfile(zero_copy_out_handle(outp),zero_copy_in_handle(inp),np,bytes));
#else
	off_t np{};
	if constexpr(random_access)
		np=static_cast<off_t>(offset);
	auto transmitted_bytes(::sendfile(zero_copy_out_handle(outp),zero_copy_in_handle(inp),np,bytes,nullptr,nullptr,0));
	//it looks like BSD platforms supports async I/O for sendfile. To do
#endif
	if(transmitted_bytes==-1)
	{
		if constexpr(report_einval)
		{
			return {0,true};
		}
		else
		{
			throw_posix_error();
		}
	}
	if constexpr(report_einval)
		return {transmitted_bytes,false};
	else
		return transmitted_bytes;
}


template<bool random_access=false,bool report_einval=false,zero_copy_output_stream output,zero_copy_input_stream input>
inline std::conditional_t<report_einval,std::pair<std::uintmax_t,bool>,std::uintmax_t> zero_copy_transmit
(output& outp,input& inp,std::uintmax_t bytes,std::intmax_t offset)
{
	constexpr std::size_t maximum_transmit_bytes(2147479552);
	std::uintmax_t transmitted{};
	for(;bytes;)
	{
		std::size_t should_transfer(maximum_transmit_bytes);
		if(bytes<should_transfer)
			should_transfer=bytes;
		std::size_t transferred_this_round{};
		auto ret(details::zero_copy_transmit_once<random_access,report_einval>(outp,inp,should_transfer,offset));
		if constexpr(report_einval)
		{
			if(ret.second)
				return {transmitted,true};
			transferred_this_round=ret.first;
		}
		else
			transferred_this_round=ret;
		transmitted+=transferred_this_round;
		if(transferred_this_round!=should_transfer)
		{
			if constexpr(report_einval)
				return {transmitted,false};
			else
				return transmitted;
		}
		bytes-=transferred_this_round;
	}
	if constexpr(report_einval)
		return {transmitted,false};
	else
		return transmitted;
}
template<bool random_access=false,bool report_einval=false,zero_copy_output_stream output,zero_copy_input_stream input>
inline std::conditional_t<report_einval,std::pair<std::uintmax_t,bool>,std::uintmax_t> zero_copy_transmit(output& outp,input& inp,std::intmax_t offset)
{
	constexpr std::size_t maximum_transmit_bytes(2147479552);
	for(std::uintmax_t transmitted{};;)
	{
		std::size_t transferred_this_round{};
		auto ret(details::zero_copy_transmit_once<random_access,report_einval>(outp,inp,maximum_transmit_bytes,offset));
		if constexpr(report_einval)
		{
			if(ret.second)
				return {transmitted,true};
			transferred_this_round=ret.first;
		}
		else
			transferred_this_round=ret;
		transmitted+=transferred_this_round;
		if(transferred_this_round!=maximum_transmit_bytes)
		{
			if constexpr(report_einval)
				return {transmitted,false};
			else
				return transmitted;
		}
	}
}
}

#endif
template<std::integral char_type=char>
inline constexpr basic_posix_io_observer<char_type> posix_stdin()
{
	return {posix_stdin_number};
}
template<std::integral char_type=char>
inline constexpr basic_posix_io_observer<char_type> posix_stdout()
{
	return basic_posix_io_observer<char_type>{posix_stdout_number};
}
template<std::integral char_type=char>
inline constexpr basic_posix_io_observer<char_type> posix_stderr()
{
	return basic_posix_io_observer<char_type>{posix_stderr_number};
}

#if defined(__WINNT__) || defined(_MSC_VER)

template<std::integral char_type>
inline constexpr io_type_t<win32_io_observer> async_scheduler_type(basic_posix_io_observer<char_type>)
{
	return {};
}

template<std::integral char_type>
inline constexpr io_type_t<iocp_overlapped> async_overlapped_type(basic_posix_io_observer<char_type>)
{
	return {};
}

template<std::integral char_type,typename... Args>
inline void async_write_callback(io_async_observer ioa,basic_posix_io_observer<char_type> h,Args&& ...args)
{
	async_write_callback(ioa,static_cast<basic_win32_io_observer<char_type>>(h),std::forward<Args>(args)...);
}

template<std::integral char_type,typename... Args>
inline void async_read_callback(io_async_observer ioa,basic_posix_io_observer<char_type> h,Args&& ...args)
{
	async_read_callback(ioa,static_cast<basic_win32_io_observer<char_type>>(h),std::forward<Args>(args)...);
}


#else
template<std::integral char_type=char>
inline constexpr basic_posix_io_observer<char_type> native_stdin()
{
	return {posix_stdin_number};
}
template<std::integral char_type=char>
inline constexpr basic_posix_io_observer<char_type> native_stdout()
{
	return basic_posix_io_observer<char_type>{posix_stdout_number};
}
template<std::integral char_type=char>
inline constexpr basic_posix_io_observer<char_type> native_stderr()
{
	return basic_posix_io_observer<char_type>{posix_stderr_number};
}
/*
template<std::integral ch_type>
inline std::size_t scatter_read(basic_posix_io_observer<ch_type> h,std::span<io_scatter_t> sp)
{
	return h.fd;
}
template<std::integral ch_type>
inline std::size_t scatter_write(basic_posix_io_observer<ch_type> h,std::span<io_scatter_t> sp)
{
}*/

#ifndef __NEWLIB__
namespace details
{
struct __attribute__((__may_alias__)) iovec_may_alias:iovec
{};

inline std::size_t posix_scatter_read_impl(int fd,std::span<io_scatter_t const> sp)
{

#if defined(__linux__)&&(defined(__x86_64__) || defined(__arm64__) || defined(__aarch64__) )
	static_assert(sizeof(unsigned long)==sizeof(std::size_t));
	auto val{system_call<
#if defined(__x86_64__)
		19
#elif defined(__arm64__) || defined(__aarch64__) 
		65
#endif
,std::ptrdiff_t>(static_cast<unsigned int>(fd),sp.data(),sp.size())};
	system_call_throw_error(val);
	return val;
#else

	std::size_t sz{sp.size()};
	if(static_cast<std::size_t>(std::numeric_limits<int>::max())<sz)
		sz=static_cast<std::size_t>(std::numeric_limits<int>::max());
	auto ptr{reinterpret_cast<iovec_may_alias const*>(sp.data())};
	std::ptrdiff_t val{::readv(fd,ptr,static_cast<int>(sz))};
	if(val<0)
		throw_posix_error();
	return val;
#endif
}

inline std::size_t posix_scatter_write_impl(int fd,std::span<io_scatter_t const> sp)
{

#if defined(__linux__)&&(defined(__x86_64__) || defined(__arm64__) || defined(__aarch64__) )
	static_assert(sizeof(unsigned long)==sizeof(std::size_t));
	auto val{system_call<
#if defined(__x86_64__)
		20
#else
		66
#endif
,std::ptrdiff_t>(static_cast<unsigned int>(fd),sp.data(),sp.size())};
	system_call_throw_error(val);
	return val;
#else
	std::size_t sz{sp.size()};
	if(static_cast<std::size_t>(std::numeric_limits<int>::max())<sz)
		sz=static_cast<std::size_t>(std::numeric_limits<int>::max());
	auto ptr{reinterpret_cast<iovec_may_alias const*>(sp.data())};
	std::ptrdiff_t val{::writev(fd,ptr,static_cast<int>(sz))};
	if(val<0)
		throw_posix_error();
	return val;
#endif
}

}

template<std::integral ch_type,typename... Args>
inline auto scatter_read(basic_posix_io_observer<ch_type> h,Args&& ...args)
{
	return details::posix_scatter_read_impl(h.fd,std::forward<Args>(args)...);
}

template<std::integral ch_type,typename... Args>
inline auto scatter_write(basic_posix_io_observer<ch_type> h,Args&& ...args)
{
	return details::posix_scatter_write_impl(h.fd,std::forward<Args>(args)...);
}

template<std::integral ch_type,typename... Args>
inline auto scatter_read(basic_posix_pipe<ch_type>& h,Args&& ...args)
{
	return details::posix_scatter_read_impl(h.in().fd,std::forward<Args>(args)...);
}

template<std::integral ch_type,typename... Args>
inline auto scatter_write(basic_posix_pipe<ch_type>& h,Args&& ...args)
{
	return details::posix_scatter_write_impl(h.out().fd,std::forward<Args>(args)...);
}
#endif
#endif
template<std::integral char_type>
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<basic_posix_io_observer<char_type>>)
{
	return print_reserve_size(io_reserve_type<int>);
}

template<std::integral char_type,std::contiguous_iterator caiter,typename U>
inline constexpr caiter print_reserve_define(io_reserve_type_t<basic_posix_io_observer<char_type>>,caiter iter,U&& v)
{
	return print_reserve_define(io_reserve_type<int>,iter,v.fd);
}

}
