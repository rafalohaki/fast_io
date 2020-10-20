#pragma once

#if defined(__WINNT__) || defined(_MSC_VER) || defined(__MSDOS__)
#include<io.h>
#include<sys/stat.h>
#include<sys/types.h>
#endif
#include<unistd.h>
#include"systemcall_details.h"
#include<fcntl.h>
#ifdef __linux__
#include<sys/uio.h>
#include<sys/sendfile.h>
#include<sys/stat.h>
struct io_uring;
#endif
#ifdef __BSD_VISIBLE
#ifndef __NEWLIB__
#include <sys/uio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#endif
#endif

namespace fast_io
{
	
namespace details
{
#ifdef _WIN32
template<bool wide_char=false>
inline constexpr int calculate_posix_open_mode_for_win32_handle(open_mode value) noexcept
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
		return mode;
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
	if((value&open_mode::creat)!=open_mode::none)
		mode |= O_CREAT;
	if((value&open_mode::excl)!=open_mode::none)
		mode |= O_EXCL;
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
#ifdef O_TTY_INIT
	if((value&open_mode::tty_init)!=open_mode::none)
		mode != O_TTY_INIT;
#endif
#ifdef O_NOCTTY
	if((value&open_mode::no_ctty)!=open_mode::none)
		mode |= O_NOCTTY;
#endif
#ifdef O_PATH
	if((value&open_mode::path)!=open_mode::none)
		mode |= O_PATH;
#endif
#ifdef O_NOATIME
	if((value&open_mode::no_atime)!=open_mode::none)
		mode |= O_NOATIME;
#endif
	if((value&open_mode::no_block)!=open_mode::none)
#ifdef O_NONBLOCK
		mode |= O_NONBLOCK;
#else
		throw_posix_error(ENOTSUP);
#endif
#ifdef _O_TEMPORARY
	if((value&open_mode::temporary)!=open_mode::none)
		mode |= _O_TEMPORARY;
#endif
#ifdef O_TMPFILE
	if((value&open_mode::temporary)!=open_mode::none)
		mode |= O_TMPFILE;
#endif
#ifdef _O_SEQUENTIAL
	if((value&open_mode::random_access)!=open_mode::none)
		mode |= _O_SEQUENTIAL;
	else
		mode |= _O_RANDOM;
#endif
#ifdef O_LARGEFILE
	mode |= O_LARGEFILE;
#endif

	if((value&open_mode::directory)!=open_mode::none)
#ifdef O_DIRECTORY
		mode |= O_DIRECTORY;
#else
		throw_posix_error(ENOTSUP);
#endif

	using utype = typename std::underlying_type<open_mode>::type;
	constexpr auto supported_values{static_cast<utype>(open_mode::out)|static_cast<utype>(open_mode::app)|static_cast<utype>(open_mode::in)};
	switch(static_cast<utype>(value)&static_cast<utype>(supported_values))
	{
/*
https://en.cppreference.com/w/cpp/io/basic_filebuf/open

mode	openmode & ~ate	Action if file already exists	Action if file does not exist
"r"	in	Read from start	Failure to open
"w"	out, out|trunc	Destroy contents	Create new
"a"	app, out|app	Append to file	Create new
"r+"	out|in	Read from start	Error
"w+"	out|in|trunc	Destroy contents	Create new
"a+"	out|in|app, in|app	Write to end	Create new
"rb"	binary|in	Read from start	Failure to open
"wb"	binary|out, binary|out|trunc	Destroy contents	Create new
"ab"	binary|app, binary|out|app	Write to end	Create new
"r+b"	binary|out|in	Read from start	Error
"w+b"	binary|out|in|trunc	Destroy contents	Create new
"a+b"	binary|out|in|app, binary|in|app	Write to end	Create new

*/
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
		return mode;
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
#ifdef _WIN32
	explicit operator basic_win32_io_observer<char_type>() const noexcept
	{
		return {details::_get_osfhandle(fd)};
	}
	explicit operator basic_nt_io_observer<char_type>() const noexcept
	{
		return {details::_get_osfhandle(fd)};
	}
#endif
	constexpr native_handle_type release() noexcept
	{
		auto temp{fd};
		fd=-1;
		return temp;
	}
};

template<std::integral ch_type>
inline constexpr posix_at_entry at(basic_posix_io_observer<ch_type> piob) noexcept
{
	return posix_at_entry{piob.fd};
}

template<std::integral ch_type>
inline constexpr basic_posix_io_observer<ch_type> io_value_handle(basic_posix_io_observer<ch_type> other)
{
	return other;
}

template<std::integral ch_type>
class basic_posix_io_handle:public basic_posix_io_observer<ch_type>
{
public:
	using char_type = ch_type;
	using native_handle_type = int;
	constexpr explicit basic_posix_io_handle()=default;
	template<typename native_hd>
	requires std::same_as<native_handle_type,std::remove_cvref_t<native_hd>>
	constexpr explicit basic_posix_io_handle(native_hd fdd):basic_posix_io_observer<ch_type>{fdd}{}
	basic_posix_io_handle(basic_posix_io_handle const& dp):basic_posix_io_observer<ch_type>{details::sys_dup(dp.native_handle())}
	{
	}
	basic_posix_io_handle& operator=(basic_posix_io_handle const& dp)
	{
		this->native_handle()=details::sys_dup2(dp.native_handle(),this->native_handle());
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
			if(this->native_handle()!=-1)[[likely]]
				details::sys_close(this->native_handle());
			this->native_handle()=b.native_handle();
			b.native_handle() = -1;
		}
		return *this;
	}
	inline constexpr void reset(native_handle_type newfd=-1) noexcept
	{
		if(this->native_handle()!=-1)[[likely]]
			details::sys_close(this->native_handle());
		this->native_handle()=newfd;
	}
	void close()
	{
		if(*this)[[likely]]
		{
			details::sys_close_throw_error(this->native_handle());
			this->native_handle()=-1;
		}
	}
};

namespace details
{

inline std::size_t posix_read_impl(int fd,void* address,std::size_t bytes_to_read)
{
#ifdef _WIN32
	if constexpr(4<sizeof(std::size_t))
		if(static_cast<std::size_t>(UINT32_MAX)<bytes_to_read)
			bytes_to_read=static_cast<std::size_t>(UINT32_MAX);
#endif
	auto read_bytes(
#if defined(__linux__)&&(defined(__x86_64__) || defined(__arm64__) || defined(__aarch64__) )
		system_call<
#if defined(__x86_64__)
		0
#elif defined(__arm64__) || defined(__aarch64__)
		63
#endif
		,std::ptrdiff_t>
#elif _WIN32 || __MSDOS__
		::_read
#else
		::read
#endif
	(fd,address,bytes_to_read));
	system_call_throw_error(read_bytes);
	return read_bytes;
}

inline std::size_t posix_write_impl(int fd,void const* address,std::size_t bytes_to_write)
{
#ifdef _WIN64
	std::size_t written{};
	for(;bytes_to_write;)
	{
		std::uint32_t to_write_this_round{INT32_MAX};
		if(bytes_to_write<static_cast<std::size_t>(INT32_MAX))
			to_write_this_round=static_cast<std::uint32_t>(bytes_to_write);
		std::int32_t number_of_bytes_written{::_write(fd,address,to_write_this_round)};
		if(number_of_bytes_written<0)
			throw_posix_error();
		written+=static_cast<std::uint32_t>(number_of_bytes_written);
		if(static_cast<std::uint32_t>(number_of_bytes_written)<to_write_this_round)
			break;
		bytes_to_write-=to_write_this_round;
	}
	return written;
#else
	auto write_bytes(
#if defined(__linux__)&&(defined(__x86_64__) || defined(__arm64__) || defined(__aarch64__) )
		system_call<
#if defined(__x86_64__)
		1
#elif defined(__arm64__) || defined(__aarch64__)
		64
#endif
		,std::ptrdiff_t>
#elif _WIN32 || __MSDOS__
		::_write
#else
		::write
#endif
	(fd,address,bytes_to_write));
	system_call_throw_error(write_bytes);
	return write_bytes;
#endif
}

inline std::uintmax_t posix_seek_impl(int fd,std::intmax_t offset,seekdir s)
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
		(fd,offset,static_cast<int>(s)));
	system_call_throw_error(ret);	
	return ret;
}



}

template<std::integral ch_type,std::contiguous_iterator Iter>
[[nodiscard]] inline Iter read(basic_posix_io_observer<ch_type> h,Iter begin,Iter end)
{
	return begin+details::posix_read_impl(h.fd,std::to_address(begin),(end-begin)*sizeof(*begin))/sizeof(*begin);
}
template<std::integral ch_type,std::contiguous_iterator Iter>
inline Iter write(basic_posix_io_observer<ch_type> h,Iter cbegin,Iter cend)
{
	return cbegin+details::posix_write_impl(h.fd,std::to_address(cbegin),(cend-cbegin)*sizeof(*cbegin))/sizeof(*cbegin);
}

template<std::integral ch_type>
inline std::uintmax_t seek(basic_posix_io_observer<ch_type> h,std::intmax_t i=0,seekdir s=seekdir::cur)
{
	return details::posix_seek_impl(h.fd,i,s);
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

#if !defined(__NEWLIB__)
namespace details
{

#ifdef _WIN32
using mode_t = int;
#endif

inline constexpr perms st_mode_to_perms(mode_t m)
{
	return static_cast<perms>(m);
}

inline constexpr file_type st_mode_to_file_type(mode_t m)
{
/*
https://linux.die.net/man/2/fstat64
The following POSIX macros are defined to check the file type using the st_mode field:

S_ISREG(m)
is it a regular file?

S_ISDIR(m)

directory?

S_ISCHR(m)

character device?

S_ISBLK(m)

block device?

S_ISFIFO(m)

FIFO (named pipe)?

S_ISLNK(m)

symbolic link? (Not in POSIX.1-1996.)

S_ISSOCK(m)

socket? (Not in POSIX.1-1996.)
*/
#if defined(_WIN32)
/*
https://github.com/Alexpux/mingw-w64/blob/master/mingw-w64-headers/crt/sys/stat.h

#define _S_IFDIR 0x4000
#define _S_IFCHR 0x2000
#define _S_IFIFO 0x1000
#define _S_IFREG 0x8000
#define	_S_IFBLK 0x3000
*/
	if((m&0xF000)==0x8000)
		return file_type::regular;
	else if((m&0xF000)==0x4000)
		return file_type::directory;
	else if((m&0xF000)==0x2000)
		return file_type::character;
	else if((m&0xF000)==0x3000)
		return file_type::block;
	else if((m&0xF000)==0x1000)
		return file_type::fifo;
	else
		return file_type::unknown;

#else
	if(S_ISREG(m))
		return file_type::regular;
	else if(S_ISDIR(m))
		return file_type::directory;
	else if(S_ISCHR(m))
		return file_type::character;
	else if(S_ISBLK(m))
		return file_type::block;
	else if(S_ISFIFO(m))
		return file_type::fifo;
#ifdef S_ISLNK
	else if(S_ISLNK(m))
		return file_type::symlink;
#endif
#ifdef S_ISSOCK
	else if(S_ISSOCK(m))
		return file_type::socket;
#endif
	else
		return file_type::unknown;
#endif
}

inline posix_file_status fstat_impl(int fd)
{
#ifdef _WIN32
	struct __stat64 st;
#elif defined(__MSDOS__)
	struct stat st;
#else
	struct stat64 st;
#endif
	if(
#ifdef _WIN32
_fstat64
#elif defined(__MSDOS__)
fstat
#else
fstat64
#endif
(fd,std::addressof(st))<0)
		throw_posix_error();

	return {static_cast<std::uintmax_t>(st.st_dev),
	static_cast<std::uintmax_t>(st.st_ino),
	st_mode_to_perms(st.st_mode),
	st_mode_to_file_type(st.st_mode),
	static_cast<std::uintmax_t>(st.st_nlink),
	static_cast<std::uintmax_t>(st.st_uid),
	static_cast<std::uintmax_t>(st.st_gid),
	static_cast<std::uintmax_t>(st.st_rdev),
	static_cast<std::uintmax_t>(st.st_size),
#if defined(_WIN32)||defined(__MSDOS__)
	65536,
	static_cast<std::uintmax_t>(st.st_size/512),
	{st.st_atime,{}},{st.st_mtime,{}},{st.st_ctime,{}},
#else
	static_cast<std::uintmax_t>(st.st_blksize),
	static_cast<std::uintmax_t>(st.st_blocks),st.st_atim,st.st_mtim,st.st_ctim,
#endif
#ifdef __BSD_VISIBLE
	st.st_st_flags,st.st_gen
#else
	0,0
#endif
};
}

}

template<std::integral ch_type>
inline posix_file_status status(basic_posix_io_observer<ch_type> piob)
{
	return details::fstat_impl(piob.fd);
}

#endif


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
#if defined(_WIN32)
	return bit_cast<void*>(_get_osfhandle(h.native_handle()));
#else
	return h.native_handle();
#endif
}

#if defined(_WIN32)
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

namespace details
{

#ifndef _WIN32

inline int my_posix_openat(int dirfd,char const* pathname,int flags,mode_t mode)
{
#if defined(__NEWLIB__)||defined(__MSDOS__)
	throw_posix_error(ENOTSUP);
#else
	int fd{
#if defined(__linux__)&&defined(__x86_64__)
	system_call<257,int>
#elif defined(__linux__)&&(defined(__arm64__) || defined(__aarch64__))
	system_call<56,int>
#else
	::openat
#endif
	(dirfd,pathname,flags,mode)};
	system_call_throw_error(fd);
	return fd;
#endif
}

#ifdef __MSDOS__
extern "C" unsigned int _dos_creat(char const*,short unsigned,int*) noexcept;
extern "C" unsigned int _dos_creatnew(char const*,short unsigned,int*) noexcept;
extern "C" unsigned int _dos_open(char const*,short unsigned,int*) noexcept;
#endif

inline int my_posix_open(char const* pathname,int flags,mode_t mode)
{
#ifdef __MSDOS__
/*
Referenced from
https://dl.acm.org/doi/pdf/10.1145/70931.70935?casa_token=rWDy5JyhhkMAAAAA:BdkF0zbbWgurns3mU3yEJI2HnHXWhe6wyYGtKxjRewlEgLg6lk-cGGNLZTTdr3vUjtFg6Cnia2b4
An Example of Multiple Inheritance in C++: A Model of the Iostream Library
*/
	int fd{-1};
	unsigned int ret{};
	if(((flags&O_CREAT)==O_CREAT))
	{
		if((flags&O_EXCL)!=O_EXCL)
			ret=_dos_creat(pathname,0,&fd);
		else
			ret=_dos_creatnew(pathname,0,&fd);
	}
	else
		ret=_dos_open(pathname,flags,&fd);
	if(ret)
		throw_posix_error();
	return fd;
#elif defined(__NEWLIB__)
	int fd{::open(pathname,flags,mode)};
	system_call_throw_error(fd);
	return fd;
#else
	return my_posix_openat(AT_FDCWD,pathname,flags,mode);
#endif
}
#endif
}

template<std::integral ch_type>
class basic_posix_file:public basic_posix_io_handle<ch_type>
{
#ifdef _WIN32
	using mode_t = int;
#endif
public:
	using char_type = ch_type;
	using native_handle_type = typename basic_posix_io_handle<char_type>::native_handle_type;
	using basic_posix_io_handle<ch_type>::native_handle;
	constexpr basic_posix_file() noexcept = default;
	template<typename native_hd>
	requires std::same_as<native_handle_type,std::remove_cvref_t<native_hd>>
	constexpr basic_posix_file(native_hd fd) noexcept: basic_posix_io_handle<ch_type>(fd){}

	basic_posix_file(io_dup_t,basic_posix_io_observer<ch_type> piob):basic_posix_io_handle<ch_type>(details::sys_dup(piob.native_handle()))
	{}
#if defined(_WIN32)
//windows specific. open posix file from win32 io handle
	basic_posix_file(basic_win32_io_handle<char_type>&& hd,open_mode m):
		basic_posix_io_handle<char_type>(::_open_osfhandle(bit_cast<std::intptr_t>(hd.native_handle()),details::calculate_posix_open_mode_for_win32_handle(m)))
	{
		if(native_handle()==-1)
			throw_posix_error();
		hd.release();
	}

	basic_posix_file(cstring_view file,open_mode om,perms pm=static_cast<perms>(436)):
		basic_posix_file(basic_win32_file<char_type>(file,om,pm),om)
	{}
	basic_posix_file(wcstring_view file,open_mode om,perms pm=static_cast<perms>(436)):
		basic_posix_file(basic_win32_file<char_type>(file,om,pm),om)
	{}
	basic_posix_file(nt_at_entry nate,cstring_view file,open_mode om,perms pm=static_cast<perms>(436)):
		basic_posix_file(basic_win32_file<char_type>(nate,file,om,pm),om)
	{}
	basic_posix_file(nt_at_entry nate,wcstring_view file,open_mode om,perms pm=static_cast<perms>(436)):
		basic_posix_file(basic_win32_file<char_type>(nate,file,om,pm),om)
	{}
#else
	//potential support modification prv in the future
	basic_posix_file(cstring_view file,open_mode om,perms pm=static_cast<perms>(436)):basic_posix_file(details::my_posix_open(file.data(),details::calculate_posix_open_mode(om),static_cast<mode_t>(pm)))
	{
	}
	basic_posix_file(posix_at_entry pate,cstring_view file,open_mode om,perms pm=static_cast<perms>(436)):basic_posix_file(details::my_posix_openat(pate.fd,file.data(),details::calculate_posix_open_mode(om),static_cast<mode_t>(pm)))
	{
	}

#ifdef __linux__
/*
To verify whether O_TMPFILE is a thing on FreeBSD. https://github.com/FreeRDP/FreeRDP/pull/6268
*/
	basic_posix_file(io_temp_t):basic_posix_file(
#if defined(__x86_64__)
		system_call<257,int>
#elif defined(__arm64__) || defined(__aarch64__)
		system_call<56,int>
#else
		::openat
#endif
		(AT_FDCWD,"/tmp",O_EXCL|O_RDWR|O_TMPFILE|O_APPEND|O_NOATIME,S_IRUSR | S_IWUSR))
	{
		system_call_throw_error(native_handle());
	}
#endif
#endif

	constexpr basic_posix_file(basic_posix_file const&)=default;
	constexpr basic_posix_file& operator=(basic_posix_file const&)=default;
	constexpr basic_posix_file(basic_posix_file &&) noexcept=default;
	constexpr basic_posix_file& operator=(basic_posix_file &&) noexcept=default;

	~basic_posix_file()
	{
		if(this->native_handle()!=-1)[[likely]]
			details::sys_close(this->native_handle());
	}
};
#if !defined(__NEWLIB__)
template<std::integral ch_type>
inline void truncate(basic_posix_io_observer<ch_type> h,std::uintmax_t size)
{
#ifdef _WIN32
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
	native_handle_type pipes;
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
	constexpr auto& native_handle()
	{
		return pipes;
	}
	constexpr auto& in()
	{
		return pipes.front();
	}
	constexpr auto& out()
	{
		return pipes.back();
	}
};

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
#ifndef __MSDOS__
using wposix_io_observer=basic_posix_io_observer<wchar_t>;
using wposix_io_handle=basic_posix_io_handle<wchar_t>;
using wposix_file=basic_posix_file<wchar_t>;
using wposix_pipe=basic_posix_pipe<wchar_t>;
#endif
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

#if !defined(__NEWLIB__) && !defined(__MSDOS__)
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

template<std::integral alias_char_type,std::integral char_type>
inline constexpr int print_alias_define(io_alias_t<alias_char_type>,basic_posix_io_observer<char_type> v)
{
	return v.fd;
}


}
