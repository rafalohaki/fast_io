#pragma once

#if defined(__WINNT__) || defined(_MSC_VER) || defined(__MSDOS__)
#include<io.h>
#include<sys/stat.h>
#include<sys/types.h>
#endif
#ifndef _MSC_VER
#include<unistd.h>
#endif
#include"systemcall_details.h"
#include<fcntl.h>
#ifdef __linux__
#include<sys/uio.h>
#include<sys/sendfile.h>
#include<sys/stat.h>
struct io_uring;
#endif
#if defined(__BSD_VISIBLE) || defined(__DARWIN_C_LEVEL)
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

inline constexpr int calculate_posix_open_mode_for_win32_handle_impl(open_mode value,int mode) noexcept
{
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

enum class posix_open_mode_text_behavior
{
always_binary=0,text,wide_text,u8_text,u16_text
};

template<posix_open_mode_text_behavior behavior>
inline constexpr int calculate_posix_open_mode_for_win32_handle(open_mode value) noexcept
{
	int mode{};
	if constexpr(behavior==posix_open_mode_text_behavior::always_binary)
		mode = _O_BINARY;
	else
	{
	if((value&open_mode::text)==open_mode::none)
		mode = _O_BINARY;
	else
	{
		if constexpr(behavior==posix_open_mode_text_behavior::wide_text)
			mode |= _O_WTEXT;
		else if constexpr(behavior==posix_open_mode_text_behavior::u8_text)
			mode |= _O_U8TEXT;
		else if constexpr(behavior==posix_open_mode_text_behavior::u16_text)
			mode |= _O_U16TEXT;
		else
			mode |= _O_TEXT;
	}
	}
	return calculate_posix_open_mode_for_win32_handle_impl(value,mode);
}
#endif


inline constexpr int calculate_posix_open_mode(open_mode value) noexcept
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
	if((value&open_mode::text)==open_mode::none)
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
		mode |= O_TTY_INIT;
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
		return {};
#endif

	if((value&open_mode::temporary)!=open_mode::none)
	{
#if defined(O_TMPFILE)
		mode |= O_TMPFILE;
#elif defined(_O_TEMPORARY)
		mode |= _O_TEMPORARY;
#else
		return {};
#endif
	}
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
		return {};
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

struct posix_fs_dirent
{
	int fd{-1};
	char const* filename{};
	explicit constexpr posix_fs_dirent() = default;
	explicit constexpr posix_fs_dirent(int fdd,char const* fnm):fd(fdd),filename(fnm){}
};

struct posix_io_redirection
{
	int *pipe_fds{};
	int fd{-1};
	bool dev_null{};
};

struct posix_io_redirection_std:posix_io_redirection
{
	constexpr posix_io_redirection_std() noexcept=default;
	template<typename T>
	requires requires(T&& t)
	{
		{redirect(std::forward<T>(t))}->std::same_as<posix_io_redirection>;
	}
	constexpr posix_io_redirection_std(T&& t) noexcept:posix_io_redirection(redirect(std::forward<T>(t))){}
};

struct posix_process_io
{
	posix_io_redirection_std in;
	posix_io_redirection_std out;
	posix_io_redirection_std err;
};

struct posix_dev_null_t{};

inline constexpr posix_dev_null_t posix_dev_null() noexcept
{
	return {};
}

inline constexpr posix_io_redirection redirect(posix_dev_null_t) noexcept
{
	return {.dev_null=true};
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
		return {reinterpret_cast<void*>(_get_osfhandle(fd))};
	}
	template<nt_family family>
	explicit operator basic_nt_family_io_observer<family,char_type>() const noexcept
	{
		return {reinterpret_cast<void*>(_get_osfhandle(fd))};
	}
#endif
	constexpr native_handle_type release() noexcept
	{
		auto temp{fd};
		fd=-1;
		return temp;
	}
};

#if __cpp_lib_three_way_comparison >= 201907L

template<std::integral ch_type>
inline constexpr bool operator==(basic_posix_io_observer<ch_type> a,basic_posix_io_observer<ch_type> b) noexcept
{
	return a.fd==b.fd;
}

template<std::integral ch_type>
inline constexpr auto operator<=>(basic_posix_io_observer<ch_type> a,basic_posix_io_observer<ch_type> b) noexcept
{
	return a.fd<=>b.fd;
}

#endif

template<std::integral ch_type>
inline constexpr posix_at_entry at(basic_posix_io_observer<ch_type> piob) noexcept
{
	return posix_at_entry{piob.fd};
}

template<std::integral ch_type>
inline constexpr basic_posix_io_observer<ch_type> io_value_handle(basic_posix_io_observer<ch_type> other) noexcept
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

#if !defined(_WIN32) && defined(AT_FDCWD)

inline constexpr posix_at_entry at_fdcwd() noexcept
{
	return posix_at_entry(AT_FDCWD);
}

#endif
namespace details
{

inline std::size_t posix_read_impl(int fd,void* address,std::size_t bytes_to_read)
{
#ifdef _WIN32
	if constexpr(4<sizeof(std::size_t))
		if(static_cast<std::size_t>(INT32_MAX)<bytes_to_read)
			bytes_to_read=static_cast<std::size_t>(INT32_MAX);
#endif
	auto read_bytes(
#if defined(__linux__)
		system_call<__NR_read,std::ptrdiff_t>
#elif _WIN32 || __MSDOS__
		::_read
#else
		::read
#endif
	(fd,address,
#ifdef _WIN32
	static_cast<std::uint32_t>(bytes_to_read)
#else
	bytes_to_read
#endif
	));
	system_call_throw_error(read_bytes);
	return read_bytes;
}

#ifdef _WIN32

inline io_scatter_status_t posix_scatter_read_impl(int fd,io_scatters_t sp)
{
	std::size_t total_size{};
	for(std::size_t i{};i!=sp.len;++i)
	{
		std::size_t pos_in{posix_read_impl(fd,const_cast<void*>(sp.base[i].base),sp.base[i].len)};
		total_size+=pos_in;
		if(pos_in<sp.base[i].len)[[unlikely]]
			return {total_size,i,pos_in};
	}
	return {total_size,sp.len,0};
}

inline std::uint32_t posix_write_simple_impl(int fd,void const* address,std::size_t bytes_to_write)
{
	auto ret{_write(fd,address,static_cast<std::uint32_t>(bytes_to_write))};
	if(ret==-1)
		throw_posix_error();
	return ret;
}

inline std::size_t posix_write_nolock_impl(int fd,void const* address,std::size_t to_write)
{
	if constexpr(4<sizeof(std::size_t))		//above the size of std::uint32_t, unfortunately, we cannot guarantee the atomicity of syscall
	{
		std::size_t written{};
		for(;to_write;)
		{
			std::uint32_t to_write_this_round{INT32_MAX};
			if(to_write<static_cast<std::size_t>(INT32_MAX))
				to_write_this_round=static_cast<std::uint32_t>(to_write);
			std::uint32_t number_of_bytes_written{posix_write_simple_impl(fd,address,to_write_this_round)};
			written+=number_of_bytes_written;
			if(number_of_bytes_written<to_write_this_round)
				break;
			to_write-=to_write_this_round;
		}
		return written;
	}
	else
		return posix_write_simple_impl(fd,address,to_write);
}

inline std::size_t posix_write_lock_impl(int fd,void const* address,std::size_t to_write)
{
	auto handle{reinterpret_cast<void*>(_get_osfhandle(fd))};
	fast_io::win32::overlapped overlap{};
	fast_io::win32::details::file_lock_guard gd{
		fast_io::win32::LockFileEx(handle,0x00000002,0,UINT32_MAX,UINT32_MAX,std::addressof(overlap))?
		handle:
		reinterpret_cast<void*>(static_cast<std::uintptr_t>(-1))
	};
	return posix_write_nolock_impl(fd,address,to_write);
}

inline io_scatter_status_t posix_scatter_write_impl(int fd,io_scatters_t sp)
{
	auto handle{reinterpret_cast<void*>(_get_osfhandle(fd))};
	fast_io::win32::overlapped overlap{};
	fast_io::win32::details::file_lock_guard gd{
		fast_io::win32::LockFileEx(handle,0x00000002,0,UINT32_MAX,UINT32_MAX,std::addressof(overlap))?
		handle:
		reinterpret_cast<void*>(static_cast<std::uintptr_t>(-1))
	};
	std::size_t total_size{};
	for(std::size_t i{};i!=sp.len;++i)
	{
		std::size_t written{posix_write_nolock_impl(fd,sp.base[i].base,sp.base[i].len)};
		total_size+=written;
		if(sp.base[i].len<written)[[unlikely]]
			return {total_size,i,written};
	}
	return {total_size,sp.len,0};
}
#endif

inline std::size_t posix_write_impl(int fd,void const* address,std::size_t to_write)
{
#ifdef _WIN32
	if constexpr(4<sizeof(std::size_t))
	{
		if(static_cast<std::size_t>(INT32_MAX)<to_write)[[unlikely]]
			return posix_write_lock_impl(fd,address,to_write);
		else
			return posix_write_simple_impl(fd,address,to_write);
	}
	else
		return posix_write_simple_impl(fd,address,to_write);
#else
	auto write_bytes(
#if defined(__linux__)
		system_call<__NR_write,std::ptrdiff_t>
#elif _WIN32 || __MSDOS__
		::_write
#else
		::write
#endif
	(fd,address,to_write));
	system_call_throw_error(write_bytes);
	return write_bytes;
#endif
}

inline std::uintmax_t posix_seek_impl(int fd,std::intmax_t offset,seekdir s)
{
	auto ret(
#if defined(__linux__)
		system_call<__NR_lseek,std::ptrdiff_t>
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

inline constexpr perms st_mode_to_perms(mode_t m) noexcept
{
	return static_cast<perms>(m);
}

inline constexpr file_type st_mode_to_file_type(mode_t m) noexcept
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

template<typename stat_model>
inline constexpr posix_file_status struct_stat_to_posix_file_status(stat_model& st) noexcept
{
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
	131072,
	static_cast<std::uintmax_t>(st.st_size/512),
	{st.st_atime,{}},{st.st_mtime,{}},{st.st_ctime,{}},{-1,0},
#elif defined(__BSD_VISIBLE) || defined(__DARWIN_C_LEVEL)
	static_cast<std::uintmax_t>(st.st_blksize),
	static_cast<std::uintmax_t>(st.st_blocks),
	st.st_atimespec,st.st_mtimespec,st.st_ctimespec,
#if defined(__DARWIN_C_LEVEL)
	{-1,0}
#else
	st.st_birthtimespec
#endif
	,
#else
	static_cast<std::uintmax_t>(st.st_blksize),
	static_cast<std::uintmax_t>(st.st_blocks),st.st_atim,st.st_mtim,st.st_ctim,{-1,0},
#endif
#if defined(__BSD_VISIBLE) || defined(__DARWIN_C_LEVEL)
	st.st_flags,st.st_gen
#else
	0,0
#endif
};
}

inline posix_file_status fstat_impl(int fd)
{
#ifdef _WIN32
	struct __stat64 st;
#elif defined(__linux__)
	struct stat64 st;
#else
	struct stat st;
#endif
	if(
#ifdef _WIN32
_fstat64
#elif defined(__linux__)
fstat64
#else
fstat
#endif
(fd,std::addressof(st))<0)
		throw_posix_error();
	return struct_stat_to_posix_file_status(st);
}

}

template<std::integral ch_type>
inline posix_file_status status(basic_posix_io_observer<ch_type> piob)
{
#ifdef _WIN32
	return status(static_cast<basic_win32_io_observer<ch_type>>(piob));
#else
	return details::fstat_impl(piob.fd);
#endif
}

#endif


#if defined(__linux__)
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


#ifdef _WIN32
template<std::integral ch_type>
inline auto redirect_handle(basic_posix_io_observer<ch_type> h) noexcept
{
#if defined(_WIN32)
	return bit_cast<void*>(_get_osfhandle(h.native_handle()));
#else
	return h.native_handle();
#endif
}
#else


template<std::integral ch_type>
inline constexpr posix_io_redirection redirect(basic_posix_io_observer<ch_type> h) noexcept
{
	return {.fd=h.fd};
}

#endif

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
#if defined(__linux__)
	system_call_throw_error(system_call<__NR_ioctl,int>(h.native_handle(),std::forward<Args>(args)...));
#else
	if(ioctl(h.native_handle(),std::forward<Args>(args)...)==-1)
		throw_posix_error();
#endif
}
#endif

namespace details
{

#ifdef _WIN32
inline std::intptr_t my_get_osfhandle(int fd) noexcept
{
	return _get_osfhandle(fd);
}

template<posix_open_mode_text_behavior behavior>
inline int open_fd_from_handle_impl(void* handle,open_mode md)
{
	int fd{_open_osfhandle(bit_cast<std::intptr_t>(handle),
		details::calculate_posix_open_mode_for_win32_handle<behavior>(md))};
	if(fd==-1)
		throw_posix_error();
	return fd;
}

template<std::integral ch_type>
inline int open_fd_from_handle(void* handle,open_mode md)
{
	if constexpr(exec_charset_is_ebcdic<ch_type>())
		return open_fd_from_handle_impl<posix_open_mode_text_behavior::always_binary>(handle,md);
	else if constexpr(std::same_as<ch_type,char>)
		return open_fd_from_handle_impl<posix_open_mode_text_behavior::text>(handle,md);
	else if constexpr(std::same_as<ch_type,char8_t>)
		return open_fd_from_handle_impl<posix_open_mode_text_behavior::text>(handle,md);
	else if constexpr(std::same_as<ch_type,wchar_t>)
		return open_fd_from_handle_impl<posix_open_mode_text_behavior::wide_text>(handle,md);
	else if constexpr(std::same_as<ch_type,char16_t>)
		return open_fd_from_handle_impl<posix_open_mode_text_behavior::u16_text>(handle,md);
	else
		return open_fd_from_handle_impl<posix_open_mode_text_behavior::always_binary>(handle,md);
}

#else
#if defined(__NEWLIB__)||defined(__MSDOS__)

template<bool always_terminate=false>
inline int my_posix_openat(int,char const*,int,mode_t)
{
	if constexpr(always_terminate)
		fast_terminate();
	else
		throw_posix_error(EINVAL);
}
#else
template<bool always_terminate=false>
inline int my_posix_openat(int dirfd,char const* pathname,int flags,mode_t mode)
{
	int fd{
#if defined(__linux__)
	system_call<__NR_openat,int>
#else
	::openat
#endif
	(dirfd,pathname,flags,mode)};
	system_call_throw_error<always_terminate>(fd);
	return fd;
}
#endif
#ifdef __MSDOS__
extern "C" unsigned int _dos_creat(char const*,short unsigned,int*) noexcept;
extern "C" unsigned int _dos_creatnew(char const*,short unsigned,int*) noexcept;
extern "C" unsigned int _dos_open(char const*,short unsigned,int*) noexcept;
#endif
template<bool always_terminate=false>
inline int my_posix_open(char const* pathname,int flags,
#if __has_cpp_attribute(maybe_unused)
[[maybe_unused]]
#endif
mode_t mode)
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
	{
		if constexpr(always_terminate)
			fast_terminate();
		else
			throw_posix_error();
	}
	return fd;
#elif defined(__NEWLIB__) && !defined(AT_FDCWD)
	int fd{::open(pathname,flags,mode)};
	system_call_throw_error<always_terminate>(fd);
	return fd;
#else
	return my_posix_openat<always_terminate>(AT_FDCWD,pathname,flags,mode);
#endif
}


inline int my_posix_openat_file_internal_impl(int dirfd,char const* filepath,open_mode om,perms pm)
{
	return my_posix_openat(dirfd,filepath,details::calculate_posix_open_mode(om),static_cast<mode_t>(pm));
}

template<std::integral char_type>
inline int my_posix_openat_file_impl(int dirfd,basic_cstring_view<char_type> filepath,open_mode om,perms pm)
{
	if constexpr(std::same_as<char_type,char>)
	{
		return my_posix_openat_file_internal_impl(dirfd,filepath.c_str(),om,pm);
	}
	else if constexpr(std::same_as<char_type,char8_t>)
	{
		return my_posix_openat_file_internal_impl(dirfd,reinterpret_cast<char const*>(filepath.c_str()),om,pm);
	}
	else
	{
		::fast_io::details::local_operator_new_array_ptr<char8_t> buffer(
			::fast_io::details::intrinsics::add_or_overflow_die(
			::fast_io::details::cal_decorated_reserve_size<sizeof(char_type),sizeof(char8_t)>(filepath.size()),1));
		*::fast_io::details::codecvt::general_code_cvt_full(filepath.data(),filepath.data()+filepath.size(),buffer.ptr)=0;
		return my_posix_openat_file_internal_impl(dirfd,reinterpret_cast<char const*>(buffer.ptr),om,pm);
	}
}

inline int my_posix_open_file_internal_impl(char const* filepath,open_mode om,perms pm)
{
	return my_posix_open(filepath,details::calculate_posix_open_mode(om),static_cast<mode_t>(pm));
}

template<std::integral char_type>
inline int my_posix_open_file_impl(basic_cstring_view<char_type> filepath,open_mode om,perms pm)
{
#if defined(__MSDOS__) || (defined(__NEWLIB__) && !defined(AT_FDCWD))
	if constexpr(std::same_as<char_type,char>)
	{
		return my_posix_open_file_internal_impl(filepath.c_str(),om,pm);
	}
	else if constexpr(std::same_as<char_type,char8_t>)
	{
		return my_posix_open_file_internal_impl(reinterpret_cast<char const*>(filepath.c_str()),om,pm);
	}
	else
	{
		::fast_io::details::local_operator_new_array_ptr<char8_t> buffer(
			::fast_io::details::intrinsics::add_or_overflow_die(
			::fast_io::details::cal_decorated_reserve_size<sizeof(char_type),sizeof(char8_t)>(filepath.size()),1));
		*::fast_io::details::codecvt::general_code_cvt_full(filepath.data(),filepath.data()+filepath.size(),buffer.ptr)=0;
		return my_posix_open_file_internal_impl(reinterpret_cast<char const*>(buffer.ptr),om,pm);
	}
#else
	return my_posix_openat_file_impl(AT_FDCWD,filepath,om,pm);
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
		basic_posix_io_handle<char_type>{details::open_fd_from_handle<ch_type>(hd.handle,m)}
	{
		hd.release();
	}
	template<nt_family family>
	basic_posix_file(basic_nt_family_io_handle<family,char_type>&& hd,open_mode m):
		basic_posix_io_handle<char_type>{details::open_fd_from_handle<ch_type>(hd.handle,m)}
	{
		hd.release();
	}
	basic_posix_file(nt_fs_dirent fsdirent,open_mode om,perms pm=static_cast<perms>(436)):
		basic_posix_file(basic_win32_file<char_type>(fsdirent,om,pm),om){}
	basic_posix_file(cstring_view file,open_mode om,perms pm=static_cast<perms>(436)):
		basic_posix_file(basic_win32_file<char_type>(file,om,pm),om)
	{}
	basic_posix_file(wcstring_view file,open_mode om,perms pm=static_cast<perms>(436)):
		basic_posix_file(basic_win32_file<char_type>(file,om,pm),om)
	{}
	basic_posix_file(u8cstring_view file,open_mode om,perms pm=static_cast<perms>(436)):
		basic_posix_file(basic_win32_file<char_type>(file,om,pm),om)
	{}
	basic_posix_file(u16cstring_view file,open_mode om,perms pm=static_cast<perms>(436)):
		basic_posix_file(basic_win32_file<char_type>(file,om,pm),om)
	{}
	basic_posix_file(u32cstring_view file,open_mode om,perms pm=static_cast<perms>(436)):
		basic_posix_file(basic_win32_file<char_type>(file,om,pm),om)
	{}
	basic_posix_file(nt_at_entry nate,cstring_view file,open_mode om,perms pm=static_cast<perms>(436)):
		basic_posix_file(basic_win32_file<char_type>(nate,file,om,pm),om)
	{}
	basic_posix_file(nt_at_entry nate,wcstring_view file,open_mode om,perms pm=static_cast<perms>(436)):
		basic_posix_file(basic_win32_file<char_type>(nate,file,om,pm),om)
	{}
	basic_posix_file(nt_at_entry nate,u8cstring_view file,open_mode om,perms pm=static_cast<perms>(436)):
		basic_posix_file(basic_win32_file<char_type>(nate,file,om,pm),om)
	{}
	basic_posix_file(nt_at_entry nate,u16cstring_view file,open_mode om,perms pm=static_cast<perms>(436)):
		basic_posix_file(basic_win32_file<char_type>(nate,file,om,pm),om)
	{}
	basic_posix_file(nt_at_entry nate,u32cstring_view file,open_mode om,perms pm=static_cast<perms>(436)):
		basic_posix_file(basic_win32_file<char_type>(nate,file,om,pm),om)
	{}

#else
	basic_posix_file(posix_fs_dirent fsdirent,open_mode om,perms pm=static_cast<perms>(436)):
		basic_posix_file(details::my_posix_openat_file_internal_impl(fsdirent.fd,fsdirent.filename,om,pm)){}
	basic_posix_file(cstring_view file,open_mode om,perms pm=static_cast<perms>(436)):basic_posix_file(details::my_posix_open_file_impl(file,om,pm)){}
	basic_posix_file(posix_at_entry pate,cstring_view file,open_mode om,perms pm=static_cast<perms>(436)):basic_posix_file(details::my_posix_openat_file_impl(pate.fd,file,om,pm)){}
	basic_posix_file(wcstring_view file,open_mode om,perms pm=static_cast<perms>(436)):basic_posix_file(details::my_posix_open_file_impl(file,om,pm)){}
	basic_posix_file(posix_at_entry pate,wcstring_view file,open_mode om,perms pm=static_cast<perms>(436)):basic_posix_file(details::my_posix_openat_file_impl(pate.fd,file,om,pm)){}
	basic_posix_file(u8cstring_view file,open_mode om,perms pm=static_cast<perms>(436)):basic_posix_file(details::my_posix_open_file_impl(file,om,pm)){}
	basic_posix_file(posix_at_entry pate,u8cstring_view file,open_mode om,perms pm=static_cast<perms>(436)):basic_posix_file(details::my_posix_openat_file_impl(pate.fd,file,om,pm)){}
	basic_posix_file(u16cstring_view file,open_mode om,perms pm=static_cast<perms>(436)):basic_posix_file(details::my_posix_open_file_impl(file,om,pm)){}
	basic_posix_file(posix_at_entry pate,u16cstring_view file,open_mode om,perms pm=static_cast<perms>(436)):basic_posix_file(details::my_posix_openat_file_impl(pate.fd,file,om,pm)){}
	basic_posix_file(u32cstring_view file,open_mode om,perms pm=static_cast<perms>(436)):basic_posix_file(details::my_posix_open_file_impl(file,om,pm)){}
	basic_posix_file(posix_at_entry pate,u32cstring_view file,open_mode om,perms pm=static_cast<perms>(436)):basic_posix_file(details::my_posix_openat_file_impl(pate.fd,file,om,pm)){}
#endif


/*
To verify whether O_TMPFILE is a thing on FreeBSD. https://github.com/FreeRDP/FreeRDP/pull/6268
*/
#if defined(O_TMPFILE)&&defined(__linux__)
	basic_posix_file(io_temp_t):basic_posix_file(
		system_call<__NR_openat,int>(AT_FDCWD,u8"/tmp",O_EXCL|O_RDWR|O_TMPFILE|O_APPEND|O_NOATIME,S_IRUSR | S_IWUSR))
	{
		system_call_throw_error(native_handle());
	}
#else
	basic_posix_file(io_temp_t)
	{
		throw_posix_error(EINVAL);
	}
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
#ifdef _WIN32
template<std::integral ch_type>
inline std::array<int*,2> redirect_handle(basic_posix_pipe<ch_type>& h)
{
	return {std::addressof(h.in().native_handle()),
		std::addressof(h.out().native_handle())};
}
#else
template<std::integral ch_type>
inline constexpr posix_io_redirection redirect(basic_posix_pipe<ch_type>& h) noexcept
{
	return {.pipe_fds=std::addressof(h.in().fd)};
}

#endif


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

#ifndef __MSDOS__
using wposix_io_observer=basic_posix_io_observer<wchar_t>;
using wposix_io_handle=basic_posix_io_handle<wchar_t>;
using wposix_file=basic_posix_file<wchar_t>;
using wposix_pipe=basic_posix_pipe<wchar_t>;
#endif

using u8posix_io_observer=basic_posix_io_observer<char8_t>;
using u8posix_io_handle=basic_posix_io_handle<char8_t>;
using u8posix_file=basic_posix_file<char8_t>;
using u8posix_pipe=basic_posix_pipe<char8_t>;

using u16posix_io_observer=basic_posix_io_observer<char16_t>;
using u16posix_io_handle=basic_posix_io_handle<char16_t>;
using u16posix_file=basic_posix_file<char16_t>;
using u16posix_pipe=basic_posix_pipe<char16_t>;

using u32posix_io_observer=basic_posix_io_observer<char32_t>;
using u32posix_io_handle=basic_posix_io_handle<char32_t>;
using u32posix_file=basic_posix_file<char32_t>;
using u32posix_pipe=basic_posix_pipe<char32_t>;

inline int constexpr posix_stdin_number = 0;
inline int constexpr posix_stdout_number = 1;
inline int constexpr posix_stderr_number = 2;

#if defined(__linux__)

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
#if 0
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
#endif
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

#if !defined(__NEWLIB__) && !defined(__MSDOS__)
namespace details
{

inline std::size_t posix_scatter_read_size_impl(int fd,io_scatters_t sp)
{
#if defined(__linux__)
	static_assert(sizeof(unsigned long)==sizeof(std::size_t));
	auto val{system_call<__NR_readv,std::ptrdiff_t>(static_cast<unsigned int>(fd),sp.base,sp.len)};
	system_call_throw_error(val);
	return val;
#else
	std::size_t sz{sp.len};
	if(static_cast<std::size_t>(std::numeric_limits<int>::max())<sz)
		sz=static_cast<std::size_t>(std::numeric_limits<int>::max());
	using iovec_may_alias_const_ptr
#if __has_cpp_attribute(gnu::may_alias)
	[[gnu::may_alias]]
#endif
	= iovec const*;
	auto ptr{reinterpret_cast<iovec_may_alias_const_ptr>(sp.base)};
	std::ptrdiff_t val{::readv(fd,ptr,static_cast<int>(sz))};
	if(val<0)
		throw_posix_error();
	return val;
#endif
}

inline std::size_t posix_scatter_write_size_impl(int fd,io_scatters_t sp)
{
#if defined(__linux__)
	static_assert(sizeof(unsigned long)==sizeof(std::size_t));
	auto val{system_call<__NR_writev,std::ptrdiff_t>(static_cast<unsigned int>(fd),sp.base,sp.len)};
	system_call_throw_error(val);
	return val;
#else
	std::size_t sz{sp.len};
	if(static_cast<std::size_t>(std::numeric_limits<int>::max())<sz)
		sz=static_cast<std::size_t>(std::numeric_limits<int>::max());
	using iovec_may_alias_const_ptr
#if __has_cpp_attribute(gnu::may_alias)
	[[gnu::may_alias]]
#endif
	= iovec const*;
	auto ptr{reinterpret_cast<iovec_may_alias_const_ptr>(sp.base)};
	std::ptrdiff_t val{::writev(fd,ptr,static_cast<int>(sz))};
	if(val<0)
		throw_posix_error();
	return val;
#endif
}

inline constexpr io_scatter_status_t scatter_size_to_status(std::size_t sz,io_scatters_t sp) noexcept
{
	std::size_t total{sz};
	for(std::size_t i{};i!=sp.len;++i)
	{
		if(total<sp.base[i].len)[[unlikely]]
			return {sz,i,total};
		total-=sp.base[i].len;
	}
	return {sz,sp.len,0};
}

inline io_scatter_status_t posix_scatter_write_impl(int fd,io_scatters_t sp)
{
	return scatter_size_to_status(posix_scatter_write_size_impl(fd,sp),sp);
}

[[nodiscard]] inline io_scatter_status_t posix_scatter_read_impl(int fd,io_scatters_t sp)
{
	return scatter_size_to_status(posix_scatter_read_size_impl(fd,sp),sp);
}

}

#endif
#endif

#if !defined(__NEWLIB__) && !defined(__MSDOS__)

template<std::integral ch_type>
[[nodiscard]] inline io_scatter_status_t scatter_read(basic_posix_io_observer<ch_type> h,io_scatters_t sp)
{
	return details::posix_scatter_read_impl(h.fd,sp);
}

template<std::integral ch_type>
inline io_scatter_status_t scatter_write(basic_posix_io_observer<ch_type> h,io_scatters_t sp)
{
	return details::posix_scatter_write_impl(h.fd,sp);
}

template<std::integral ch_type>
inline io_scatter_status_t scatter_read(basic_posix_pipe<ch_type>& h,io_scatters_t sp)
{
	return details::posix_scatter_read_impl(h.in().fd,sp);
}

template<std::integral ch_type>
inline io_scatter_status_t scatter_write(basic_posix_pipe<ch_type>& h,io_scatters_t sp)
{
	return details::posix_scatter_write_impl(h.out().fd,sp);
}

#endif


template<std::integral ch_type>
struct basic_posix_pio_entry
{
	using char_type = ch_type;
	using native_handle_type = int;
	int fd{-1};
	std::uintmax_t offset{};
};

template<std::integral char_type>
inline constexpr basic_posix_pio_entry<char_type> pio(basic_posix_io_observer<char_type> piob,std::uintmax_t off) noexcept
{
	return {piob.fd,off};
}

template<std::integral ch_type>
inline constexpr basic_posix_pio_entry<ch_type> io_value_handle(basic_posix_pio_entry<ch_type> other) noexcept
{
	return other;
}

#if _XOPEN_SOURCE >= 500 ||  _POSIX_C_SOURCE >= 200809L

namespace details
{

inline std::size_t posix_pread_impl(int fd,void* address,std::size_t bytes_to_read,std::uintmax_t offset)
{
	if constexpr(sizeof(std::uintmax_t)>sizeof(off_t))
	{
		if(static_cast<std::uintmax_t>(std::numeric_limits<off_t>::max())<offset)
			throw_posix_error(EINVAL);
	}
	auto read_bytes(
#if defined(__linux__)
		system_call<__NR_pread64,std::ptrdiff_t>
#else
		::pread
#endif
	(fd,address,bytes_to_read,static_cast<off_t>(offset)));
	system_call_throw_error(read_bytes);
	return read_bytes;
}

inline std::size_t posix_pwrite_impl(int fd,void const* address,std::size_t bytes_to_write,std::uintmax_t offset)
{
	if constexpr(sizeof(std::uintmax_t)>sizeof(off_t))
	{
		if(static_cast<std::uintmax_t>(std::numeric_limits<off_t>::max())<offset)
			throw_posix_error(EINVAL);
	}
	auto written_bytes(
#if defined(__linux__)
		system_call<__NR_pwrite64,std::ptrdiff_t>
#else
		::pwrite
#endif
	(fd,address,bytes_to_write,static_cast<off_t>(offset)));
	system_call_throw_error(written_bytes);
	return written_bytes;
}

inline std::size_t posix_scatter_pread_size_impl(int fd,io_scatters_t sp,std::uintmax_t offset)
{
	if constexpr(sizeof(std::uintmax_t)>sizeof(off_t))
	{
		if(static_cast<std::uintmax_t>(std::numeric_limits<off_t>::max())<offset)
			throw_posix_error(EINVAL);
	}
#if defined(__linux__)
//	static_assert(sizeof(unsigned long)==sizeof(std::size_t));
	auto val{system_call<__NR_preadv,std::ptrdiff_t>(static_cast<unsigned int>(fd),sp.base,sp.len,static_cast<off_t>(offset))};
	system_call_throw_error(val);
	return val;
#else
	std::size_t sz{sp.len};
	if(static_cast<std::size_t>(std::numeric_limits<int>::max())<len)
		sz=static_cast<std::size_t>(std::numeric_limits<int>::max());
	auto ptr{reinterpret_cast<iovec_may_alias const*>(sp.base)};
	std::ptrdiff_t val{::preadv(fd,ptr,static_cast<int>(sz),static_cast<off_t>(offset))};
	if(val<0)
		throw_posix_error();
	return val;
#endif
}

inline std::size_t posix_scatter_pwrite_size_impl(int fd,io_scatters_t sp,std::uintmax_t offset)
{
	if constexpr(sizeof(std::uintmax_t)>sizeof(off_t))
	{
		if(static_cast<std::uintmax_t>(std::numeric_limits<off_t>::max())<offset)
			throw_posix_error(EINVAL);
	}
#if defined(__linux__)
//	static_assert(sizeof(unsigned long)==sizeof(std::size_t));
	auto val{system_call<__NR_pwritev,std::ptrdiff_t>(static_cast<unsigned int>(fd),sp.base,sp.len,static_cast<off_t>(offset))};
	system_call_throw_error(val);
	return val;
#else
	std::size_t sz{sp.len};
	if(static_cast<std::size_t>(std::numeric_limits<int>::max())<sz)
		sz=static_cast<std::size_t>(std::numeric_limits<int>::max());
	auto ptr{reinterpret_cast<iovec_may_alias const*>(sp.base)};
	std::ptrdiff_t val{::pwritev(fd,ptr,static_cast<int>(sz),static_cast<off_t>(offset))};
	if(val<0)
		throw_posix_error();
	return val;
#endif
}

inline io_scatter_status_t posix_scatter_pwrite_impl(int fd,io_scatters_t sp,std::uintmax_t offset)
{
	return scatter_size_to_status(posix_scatter_pwrite_size_impl(fd,sp,offset),sp);
}

[[nodiscard]] inline io_scatter_status_t posix_scatter_pread_impl(int fd,io_scatters_t sp,std::uintmax_t offset)
{
	return scatter_size_to_status(posix_scatter_pread_size_impl(fd,sp,offset),sp);
}

}

template<std::integral char_type,std::contiguous_iterator Iter>
inline constexpr Iter read(basic_posix_pio_entry<char_type> ppioent,Iter begin,Iter end)
{
	return begin+details::posix_pread_impl(ppioent.fd,std::to_address(begin),(end-begin)*sizeof(*begin),ppioent.offset)/sizeof(*begin);
}

template<std::integral char_type,std::contiguous_iterator Iter>
inline constexpr Iter write(basic_posix_pio_entry<char_type> ppioent,Iter begin,Iter end)
{
	return begin+details::posix_pwrite_impl(ppioent.fd,std::to_address(begin),(end-begin)*sizeof(*begin),ppioent.offset)/sizeof(*begin);
}

template<std::integral ch_type>
[[nodiscard]] inline io_scatter_status_t scatter_read(basic_posix_pio_entry<ch_type> ppioent,io_scatters_t sp)
{
	return details::posix_scatter_pread_impl(ppioent.fd,sp,ppioent.offset);
}

template<std::integral ch_type>
inline io_scatter_status_t scatter_write(basic_posix_pio_entry<ch_type> ppioent,io_scatters_t sp)
{
	return details::posix_scatter_pwrite_impl(ppioent.fd,sp,ppioent.offset);
}

#endif

}
#ifdef __MSDOS__
#include"msdos.h"
#endif