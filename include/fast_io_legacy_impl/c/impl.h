#pragma once
#include<cstdio>
#include<cwchar>

#if defined(__MINGW32__) && !defined(_UCRT)
#include"msvcrt_lock.h"
#endif

namespace fast_io
{
namespace win32
{
#if defined(__MINGW32__) && !defined(_UCRT)
[[gnu::dllimport]] extern void __cdecl _lock_file(FILE*) noexcept asm("_lock_file");
[[gnu::dllimport]] extern void __cdecl _unlock_file(FILE*) noexcept asm("_unlock_file");
[[gnu::dllimport]] extern std::size_t __cdecl _fwrite_nolock(void const* __restrict buffer,std::size_t size,std::size_t count,FILE* __restrict) noexcept asm("_fwrite_nolock");
[[gnu::dllimport]] extern std::size_t __cdecl _fread_nolock(void* __restrict buffer,std::size_t size,std::size_t count,FILE* __restrict) noexcept asm("_fread_nolock");
[[gnu::dllimport]] extern std::size_t __cdecl fwrite(void const* __restrict buffer,std::size_t size,std::size_t count,FILE* __restrict) noexcept asm("fwrite");
[[gnu::dllimport]] extern std::size_t __cdecl fread(void* __restrict buffer,std::size_t size,std::size_t count,FILE* __restrict) noexcept asm("fread");
#endif
}

inline constexpr open_mode native_c_supported(open_mode m) noexcept
{
#ifdef _WIN32
using utype = typename std::underlying_type<open_mode>::type;
constexpr auto c_supported_values{static_cast<utype>(open_mode::text)|
	static_cast<utype>(open_mode::out)|
	static_cast<utype>(open_mode::app)|
	static_cast<utype>(open_mode::in)|
	static_cast<utype>(open_mode::trunc)};
return static_cast<open_mode>(static_cast<utype>(m)&c_supported_values);
#else
return c_supported(m);
#endif
}
inline constexpr char const* to_native_c_mode(open_mode m) noexcept
{
#ifdef _WIN32
/*
https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/fdopen-wfdopen?view=vs-2019
From microsoft's document. _fdopen only supports

"r"	Opens for reading. If the file does not exist or cannot be found, the fopen call fails.
"w"	Opens an empty file for writing. If the given file exists, its contents are destroyed.
"a"	Opens for writing at the end of the file (appending). Creates the file if it does not exist.
"r+"	Opens for both reading and writing. The file must exist.
"w+"	Opens an empty file for both reading and writing. If the file exists, its contents are destroyed.
"a+"	Opens for reading and appending. Creates the file if it does not exist.

"x" will throw EINVAL which is does not satisfy POSIX, C11 and C++17 standard.
*/
	using utype = typename std::underlying_type<open_mode>::type;
	switch(static_cast<utype>(native_c_supported(m)))
	{
//Action if file already exists;	Action if file does not exist;	c-style mode;	Explanation
//Read from start;	Failure to open;	"r";	Open a file for reading
	case static_cast<utype>(open_mode::in)|static_cast<utype>(open_mode::text):
		return "\x72";
//Destroy contents;	Create new;	"w";	Create a file for writing
	case static_cast<utype>(open_mode::out)|static_cast<utype>(open_mode::text):
	case static_cast<utype>(open_mode::out)|static_cast<utype>(open_mode::trunc)|static_cast<utype>(open_mode::text):
		return "\x77";
//Append to file;	Create new;	"a";	Append to a file
	case static_cast<utype>(open_mode::app)|static_cast<utype>(open_mode::text):
	case static_cast<utype>(open_mode::out)|static_cast<utype>(open_mode::app)|static_cast<utype>(open_mode::text):
		return "\x61";
//Read from start;	Error;	"r+";		Open a file for read/write
	case static_cast<utype>(open_mode::out)|static_cast<utype>(open_mode::in)|static_cast<utype>(open_mode::text):
		return "\x72\x2b";
//Destroy contents;	Create new;	"w+";	Create a file for read/write
	case static_cast<utype>(open_mode::out)|static_cast<utype>(open_mode::in)|static_cast<utype>(open_mode::trunc)|static_cast<utype>(open_mode::text):
		return "\x77\x2b";
//Write to end;	Create new;	"a+";	Open a file for read/write
	case static_cast<utype>(open_mode::out)|static_cast<utype>(open_mode::in)|static_cast<utype>(open_mode::app)|static_cast<utype>(open_mode::text):
	case static_cast<utype>(open_mode::in)|static_cast<utype>(open_mode::app)|static_cast<utype>(open_mode::text):
		return "\x77\x2b";

//binary support

//Action if file already exists;	Action if file does not exist;	c-style mode;	Explanation
//Read from start;	Failure to open;	"rb";	Open a file for reading
	case static_cast<utype>(open_mode::in):
		return "\x72\x62";
//Destroy contents;	Create new;	"wb";	Create a file for writing
	case static_cast<utype>(open_mode::out):
	case static_cast<utype>(open_mode::out)|static_cast<utype>(open_mode::trunc):
		return "\x77\x62";
//Append to file;	Create new;	"ab";	Append to a file
	case static_cast<utype>(open_mode::app):
	case static_cast<utype>(open_mode::out)|static_cast<utype>(open_mode::app):
		return "\x61\x62";
//Read from start;	Error;	"r+b";		Open a file for read/write
	case static_cast<utype>(open_mode::out)|static_cast<utype>(open_mode::in):
		return "\x72\x2b\x62";
//Destroy contents;	Create new;	"w+b";	Create a file for read/write
	case static_cast<utype>(open_mode::out)|static_cast<utype>(open_mode::in)|static_cast<utype>(open_mode::trunc):
		return "\x77\x2b\x62";
//Write to end;	Create new;	"a+b";	Open a file for read/write
	case static_cast<utype>(open_mode::out)|static_cast<utype>(open_mode::in)|static_cast<utype>(open_mode::app):
	case static_cast<utype>(open_mode::in)|static_cast<utype>(open_mode::app):
		return "\x61\x2b\x62";
	case 0:
		if((m&open_mode::directory)!=open_mode::none)
			return "\x72";
		[[fallthrough]];
	default:
		return "";
	}
#else
	return to_c_mode(m);
#endif
}

#if defined(__GLIBC__) || defined(__NEED___isoc_va_list)
template<stream stm>
class c_io_cookie_functions_t
{
public:

//musl libc also supports this I think
//https://gitlab.com/bminor/musl/-/blob/061843340fbf2493bb615e20e66f60c5d1ef0455/src/stdio/fopencookie.c



//musl libc also supports this I think
//https://gitlab.com/bminor/musl/-/blob/061843340fbf2493bb615e20e66f60c5d1ef0455/src/stdio/fopencookie.c

	using native_functions_type = cookie_io_functions_t;
	native_functions_type native_functions{};
	explicit constexpr c_io_cookie_functions_t()
	{
		using value_type = std::remove_reference_t<stm>;
		if constexpr(!std::is_reference_v<stm>)
			native_functions.close=[](void* cookie) noexcept->int
			{
				delete bit_cast<value_type*>(cookie);
				return 0;
			};
		if constexpr(input_stream<value_type>)
			native_functions.read=[](void* cookie,char* buf,std::size_t size) noexcept->std::ptrdiff_t
			{
#ifdef __cpp_exceptions
				try
				{
#endif
					return read(*bit_cast<value_type*>(cookie),buf,buf+size)-buf;
#ifdef __cpp_exceptions
				}
				catch(fast_io::posix_error const& err)
				{
					errno=err.code();
					return -1;
				}
				catch(...)
				{
					errno=EIO;
					return -1;
				}
#endif
			};
		if constexpr(output_stream<value_type>)
		{
			native_functions.write=[](void* cookie,char const* buf,std::size_t size) noexcept->std::ptrdiff_t
			{
#ifdef __cpp_exceptions
				try
				{
#endif
					if constexpr(std::same_as<decltype(write(*bit_cast<value_type*>(cookie),buf,buf+size)),void>)
					{
						write(*bit_cast<value_type*>(cookie),buf,buf+size);
						return static_cast<std::ptrdiff_t>(size);
					}
					else
						return write(*bit_cast<value_type*>(cookie),buf,buf+size)-buf;
#ifdef __cpp_exceptions
				}
				catch(fast_io::posix_error const& err)
				{
					errno=err.code();
					return -1;
				}
				catch(...)
				{
					errno=EIO;
					return -1;
				}
#endif
			};
		}
		if constexpr(random_access_stream<value_type>)
		{
			native_functions.seek=[](void *cookie, off64_t *offset, int whence) noexcept->int
			{
#ifdef __cpp_exceptions
				try
				{
#endif
					*offset=seek(*bit_cast<value_type*>(cookie),*offset,static_cast<fast_io::seekdir>(whence));
					return 0;
#ifdef __cpp_exceptions
				}
				catch(fast_io::posix_error const& err)
				{
					errno=err.code();
					return -1;
				}
				catch(...)
				{
					errno=EIO;
					return -1;
				}
#endif
			};
		}
	}

};

template<typename stm>
inline constexpr c_io_cookie_functions_t<stm> c_io_cookie_functions{};
#elif defined(__BSD_VISIBLE) || defined(__DARWIN_C_LEVEL) || defined(__BIONIC__) || defined(__NEWLIB__)
namespace details
{
#ifdef __NEWLIB__
# ifdef __LARGE64_FILES
extern FILE	*funopen (const void *__cookie,
		int (*__readfn)(void *__c, char *__buf,
				_READ_WRITE_BUFSIZE_TYPE __n),
		int (*__writefn)(void *__c, const char *__buf,
				 _READ_WRITE_BUFSIZE_TYPE __n),
		_fpos64_t (*__seekfn)(void *__c, _fpos64_t __off, int __whence),
		int (*__closefn)(void *__c)) noexcept asm("funopen");
#else
extern FILE	*funopen (const void *__cookie,
		int (*__readfn)(void *__c, char *__buf,
				int __n),
		int (*__writefn)(void *__c, const char *__buf,
				 int __n),
		fpos_t  (*__seekfn)(void *__c, fpos_t  __off, int __whence),
		int (*__closefn)(void *__c)) noexcept asm("funopen");
#endif
#endif
//funopen
template<stream value_type>
inline std::FILE* funopen_wrapper(value_type* cookie)
{
	using bf_size_type =
#ifdef 	_READ_WRITE_BUFSIZE_TYPE
_READ_WRITE_BUFSIZE_TYPE
#else
	int
#endif
;
	int (*readfn)(void *, char *, bf_size_type)=nullptr;
	int (*writefn)(void *, const char *, bf_size_type)=nullptr;
	int (*closefn)(void *)=nullptr;
	fpos_t (*seekfn)(void *, fpos_t, int)=nullptr;

	closefn=[](void* cookie) noexcept->int
	{
		delete bit_cast<value_type*>(cookie);
		return 0;
	};
	if constexpr(input_stream<value_type>)
		readfn=[](void* cookie,char* buf,bf_size_type size) noexcept->int
		{
#ifdef __cpp_exceptions
			try
			{
#endif
				return read(*bit_cast<value_type*>(cookie),buf,buf+size)-buf;
#ifdef __cpp_exceptions
			}
			catch(fast_io::posix_error const& err)
			{
				errno=err.code();
				return -1;
			}
			catch(...)
			{
				errno=EIO;
				return -1;
			}
#endif
		};
	if constexpr(output_stream<value_type>)
		writefn=[](void* cookie,char const* buf,bf_size_type size) noexcept->int
		{
#ifdef __cpp_exceptions
			try
			{
#endif
				if constexpr(std::same_as<decltype(write(*bit_cast<value_type*>(cookie),buf,buf+size)),void>)
				{
					write(*bit_cast<value_type*>(cookie),buf,buf+size);
					return size;
				}
				else
					return write(*bit_cast<value_type*>(cookie),buf,buf+size)-buf;
#ifdef __cpp_exceptions
			}
			catch(fast_io::posix_error const& err)
			{
				errno=err.code();
				return -1;
			}
			catch(...)
			{
				errno=EIO;
				return -1;
			}
#endif
		};
	if constexpr(random_access_stream<value_type>)
	{
		seekfn=[](void *cookie, fpos_t offset, int whence) noexcept->fpos_t
		{
#ifdef __cpp_exceptions
			try
			{
#endif
				return static_cast<fpos_t>(seek(*bit_cast<value_type*>(cookie),offset,static_cast<fast_io::seekdir>(whence)));
#ifdef __cpp_exceptions
			}
			catch(fast_io::posix_error const& err)
			{
				errno=err.code();
				return -1;
			}
			catch(...)
			{
				errno=EIO;
				return -1;
			}
#endif
		};
	}
	auto fp{funopen(cookie,readfn,writefn,seekfn,closefn)};
	if(fp==nullptr)
		throw_posix_error();
	return fp;
}

}
#endif

namespace details
{

#if defined(__MSDOS__)
extern int fileno(FILE*) noexcept asm("_fileno");
extern std::FILE* fdopen(int,char const*) noexcept asm("_fdopen");
#elif defined(__CYGWIN__)
[[gnu::dllimport]] extern int fileno(FILE*) noexcept 
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if defined(__GNUC__)
asm("fileno")
#else
asm("_fileno")
#endif
#else
asm("fileno")
#endif
;
[[gnu::dllimport]] extern std::FILE* fdopen(int,char const*) noexcept
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if defined(__GNUC__)
asm("fdopen")
#else
asm("_fdopen")
#endif
#else
asm("fdopen")
#endif
;

#endif

inline int fp_unlocked_to_fd(FILE* fp) noexcept
{
	if(fp==nullptr)
		return -1;
	return 
#ifdef _WIN32
		noexcept_call(_fileno,fp)
#elif defined(__NEWLIB__) || defined(__DARWIN_C_LEVEL)
		fp->_file
#elif defined(__MSDOS__)
		noexcept_call(fileno,fp)
#else
		noexcept_call(fileno_unlocked,fp)
#endif
	;
}

inline int fp_to_fd(FILE* fp) noexcept
{
	if(fp==nullptr)
		return -1;
	return 
#ifdef _WIN32
		noexcept_call(_fileno,fp)
#elif defined(__NEWLIB__)
		fp->_file
#else
		noexcept_call(fileno,fp)
#endif
	;
}

}

template<std::integral ch_type>
class basic_c_io_observer_unlocked
{
public:
	using char_type = ch_type;
	using native_handle_type = std::FILE*;
	native_handle_type fp{};
	constexpr auto& native_handle() noexcept
	{
		return fp;
	}
	constexpr auto& native_handle() const noexcept
	{
		return fp;
	}
	constexpr operator bool() const noexcept
	{
		return fp;
	}
	explicit operator basic_posix_io_observer<char_type>() const noexcept
	{
		return basic_posix_io_observer<char_type>{details::fp_unlocked_to_fd(fp)};
	}
#ifdef _WIN32
	explicit operator basic_win32_io_observer<char_type>() const noexcept
	{
		return static_cast<basic_win32_io_observer<char_type>>(static_cast<basic_posix_io_observer<char_type>>(*this));
	}
	template<nt_family fam>
	explicit operator basic_nt_family_io_observer<fam,char_type>() const noexcept
	{
		return static_cast<basic_nt_family_io_observer<fam,char_type>>(static_cast<basic_posix_io_observer<char_type>>(*this));
	}
#endif
	constexpr native_handle_type release() noexcept
	{
		auto temp{fp};
		fp=nullptr;
		return temp;
	}
};

template<std::integral ch_type>
inline constexpr posix_at_entry at(basic_c_io_observer_unlocked<ch_type> other) noexcept
{
	return posix_at_entry{details::fp_unlocked_to_fd(other.fp)};
}

template<std::integral ch_type>
inline auto redirect_handle(basic_c_io_observer_unlocked<ch_type> h)
{
#ifdef _WIN32
	return static_cast<basic_win32_io_observer<ch_type>>(h).handle;
#else
	return static_cast<basic_posix_io_observer<ch_type>>(h).fd;
#endif
}

using c_io_observer_unlocked = basic_c_io_observer_unlocked<char>;

#if !defined(_WIN32) || defined(FAST_IO_WIN32_USE_SYS_FWRITE)
template<std::integral T,::fast_io::freestanding::contiguous_iterator Iter>
requires (std::same_as<T,::fast_io::freestanding::iter_value_t<Iter>>||std::same_as<T,char>)
inline Iter read(basic_c_io_observer_unlocked<T> cfhd,Iter begin,Iter end);


template<std::integral T,::fast_io::freestanding::contiguous_iterator Iter>
requires (std::same_as<T,::fast_io::freestanding::iter_value_t<Iter>>||std::same_as<T,char>)
inline Iter write(basic_c_io_observer_unlocked<T> cfhd,Iter begin,Iter end);
#endif

namespace details
{

inline void c_flush_impl(std::FILE* fp)
{
	if(std::fflush(fp))
		throw_posix_error();
}

inline void c_flush_unlocked_impl(std::FILE* fp)
{
	if(noexcept_call(
#if defined(_MSC_VER) || defined(_UCRT)
		_fflush_nolock
#elif defined(_POSIX_SOURCE)
		fflush_unlocked
#else
		fflush
#endif
	,fp))
		throw_posix_error();
}
}

template<std::integral T>
inline void flush(basic_c_io_observer_unlocked<T> cfhd)
{
	details::c_flush_unlocked_impl(cfhd.fp);
}

namespace details
{

inline std::uintmax_t c_io_seek_impl(std::FILE* fp,std::intmax_t offset,seekdir s)
{
/*
We avoid standard C functions since they cannot deal with large file on 32 bits platforms

Reference:

https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/fseek-nolock-fseeki64-nolock?view=vs-2019

https://www.gnu.org/software/libc/manual/html_node/File-Positioning.html

*/
	if(
#if defined(_WIN32)
		_fseeki64
#elif defined(__linux__) && !defined(__mlibc__)
		fseeko64
#else
		fseek
#endif
		(fp,offset,static_cast<int>(s)))
		throw_posix_error();
	auto val{
#if defined(_WIN32)
		_ftelli64
#elif defined(__linux__) && !defined(__mlibc__)
		ftello64 
#else
		ftell
#endif
		(fp)};
	if(val<0)
		throw_posix_error();
	return val;
}
#if defined(_WIN32)

#if defined(_MSC_VER) || defined(_UCRT)  || __MSVCRT_VERSION__ >= 0x800

inline std::uintmax_t c_io_seek_no_lock_impl(std::FILE* fp,std::intmax_t offset,seekdir s)
{
	if(_fseeki64_nolock(fp,offset,static_cast<int>(s)))
		throw_posix_error();
	auto val{_ftelli64_nolock(fp)};
	if(val<0)
		throw_posix_error();
	return val;
}
#else
inline std::uintmax_t c_io_seek_no_lock_impl(std::FILE* fp,std::intmax_t offset,seekdir s)
{
	if(fseeko64(fp,offset,static_cast<int>(s)))
		throw_posix_error();
	auto val{ftello64(fp)};
	if(val<0)
		throw_posix_error();
	return val;
}
#endif
#endif

#if defined(__CYGWIN__) && !defined(__SINGLE_THREAD__)

[[gnu::dllimport]] extern void my_cygwin_pthread_mutex_lock(void*) noexcept
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if defined(__GNUC__)
asm("pthread_mutex_lock")
#else
asm("_pthread_mutex_lock")
#endif
#else
asm("pthread_mutex_lock")
#endif
;

[[gnu::dllimport]] extern void my_cygwin_pthread_mutex_unlock(void*) noexcept
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if defined(__GNUC__)
asm("pthread_mutex_unlock")
#else
asm("_pthread_mutex_unlock")
#endif
#else
asm("pthread_mutex_unlock")
#endif
;

inline void my_cygwin_flockfile(std::FILE* fp) noexcept
{
	if(!((fp->_flags)&__SSTR))
		my_cygwin_pthread_mutex_lock(fp->_lock);
}

inline void my_cygwin_funlockfile(std::FILE* fp) noexcept
{
	if(!((fp->_flags)&__SSTR))
		my_cygwin_pthread_mutex_unlock(fp->_lock);
}

#endif

}

template<std::integral ch_type>
inline std::uintmax_t seek(basic_c_io_observer_unlocked<ch_type> cfhd,std::intmax_t offset=0,seekdir s=seekdir::cur)
{
#if defined(_WIN32)
	return details::c_io_seek_no_lock_impl(cfhd.fp,offset,s);
#else
	return details::c_io_seek_impl(cfhd.fp,offset,s);
#endif
}

template<std::integral ch_type,typename... Args>
requires io_controllable<basic_posix_io_observer<ch_type>,Args...>
inline decltype(auto) io_control(basic_c_io_observer_unlocked<ch_type> h,Args&& ...args)
{
	return io_control(static_cast<basic_posix_io_observer<ch_type>>(h),std::forward<Args>(args)...);
}


template<std::integral ch_type>
class basic_c_io_observer
{
public:
	using char_type = ch_type;
	using native_handle_type = std::FILE*;
	native_handle_type fp{};
	constexpr auto& native_handle() const noexcept
	{
		return fp;
	}
	constexpr auto& native_handle() noexcept
	{
		return fp;
	}
	constexpr operator bool() const noexcept
	{
		return fp;
	}
	explicit operator basic_posix_io_observer<char_type>() const noexcept
	{
		return {details::fp_to_fd(fp)};
	}
#ifdef _WIN32
	explicit operator basic_win32_io_observer<char_type>() const noexcept
	{
		return static_cast<basic_win32_io_observer<char_type>>(static_cast<basic_posix_io_observer<char_type>>(*this));
	}
	template<nt_family fam>
	explicit operator basic_nt_family_io_observer<fam,char_type>() const noexcept
	{
		return static_cast<basic_nt_family_io_observer<fam,char_type>>(static_cast<basic_posix_io_observer<char_type>>(*this));
	}
#endif
	constexpr native_handle_type release() noexcept
	{
		auto temp{fp};
		fp=nullptr;
		return temp;
	}
	inline void lock() const noexcept
	{
#if !defined(__SINGLE_THREAD__)
#if defined(_MSC_VER)||defined(_UCRT)
	_lock_file(fp);
#elif defined(_WIN32)
	win32::my_msvcrt_lock_file(fp);
#elif defined(__NEWLIB__)
#if defined(__CYGWIN__)
	details::my_cygwin_flockfile(fp);
#elif !defined(__SINGLE_THREAD__)
//	_flockfile(fp);	//TO FIX undefined reference to `__cygwin_lock_lock' why?
#endif
#elif defined(__MSDOS__) || (defined(__wasi__) &&!defined(__wasilibc_unmodified_upstream) && !defined(_REENTRANT)) || defined(__mlibc__)
#else
	flockfile(fp);
#endif
#endif
	}
	inline void unlock() const noexcept
	{
#if !defined(__SINGLE_THREAD__)
#if defined(_MSC_VER)||defined(_UCRT)
	_unlock_file(fp);
#elif defined(_WIN32)
	win32::my_msvcrt_unlock_file(fp);
#elif defined(__NEWLIB__)
#if defined(__CYGWIN__)
	details::my_cygwin_funlockfile(fp);
#elif !defined(__SINGLE_THREAD__)
//	_funlockfile(fp); //TO FIX
#endif
#elif defined(__MSDOS__) || (defined(__wasi__) &&!defined(__wasilibc_unmodified_upstream) && !defined(_REENTRANT)) || defined(__mlibc__)
#else
	funlockfile(fp);
#endif
#endif
	}
	inline constexpr basic_c_io_observer_unlocked<ch_type> unlocked_handle() const noexcept
	{
		return {fp};
	}
};

template<std::integral T>
inline constexpr posix_at_entry at(basic_c_io_observer<T> other) noexcept
{
	return posix_at_entry{details::fp_to_fd(other.fp)};
}

template<std::integral T>
inline constexpr basic_c_io_observer<T> io_value_handle(basic_c_io_observer<T> other)
{
	return other;
}

template<std::integral T>
inline constexpr basic_c_io_observer_unlocked<T> io_value_handle(basic_c_io_observer_unlocked<T> other)
{
	return other;
}

template<std::integral T,::fast_io::freestanding::contiguous_iterator Iter>
[[nodiscard]] inline Iter read(basic_c_io_observer<T> cfhd,Iter begin,Iter end)
{
	details::lock_guard lg{cfhd};
	basic_c_io_observer_unlocked<T> cfhd_unlocked{cfhd.fp};
	return read(cfhd_unlocked,begin,end);
}

namespace details
{
inline void c_write_impl(void const* __restrict ptr,std::size_t size,std::size_t count,std::FILE* __restrict fp)
{
	if(fwrite(ptr,size,count,fp)<count)
		throw_posix_error();
	return;	
}
}

template<std::integral T,::fast_io::freestanding::contiguous_iterator Iter>
requires (std::same_as<T,char>||std::same_as<::fast_io::freestanding::iter_value_t<Iter>,T>)
inline Iter write(basic_c_io_observer<T> cfhd,Iter begin,Iter end)
{
	details::lock_guard lg{cfhd};
	basic_c_io_observer_unlocked<T> cfhd_unlocked{cfhd.fp};
	return write(cfhd_unlocked,begin,end);
}

template<std::integral T>
inline void flush(basic_c_io_observer<T> cfhd)
{
	c_flush_impl(cfhd.fp);
}

template<std::integral ch_type>
inline std::uintmax_t seek(basic_c_io_observer<ch_type> cfhd,std::intmax_t offset=0,seekdir s=seekdir::cur)
{
	return details::c_io_seek_impl(cfhd.fp,offset,s);
}

#if __cpp_lib_three_way_comparison >= 201907L

template<std::integral ch_type>
inline constexpr bool operator==(basic_c_io_observer<ch_type> a,basic_c_io_observer<ch_type> b) noexcept
{
	return a.fp==b.fp;
}

template<std::integral ch_type>
inline constexpr bool operator==(basic_c_io_observer_unlocked<ch_type> a,basic_c_io_observer_unlocked<ch_type> b) noexcept
{
	return a.fp==b.fp;
}

template<std::integral ch_type>
inline constexpr auto operator<=>(basic_c_io_observer<ch_type> a,basic_c_io_observer<ch_type> b) noexcept
{
	return a.fp<=>b.fp;
}

template<std::integral ch_type>
inline constexpr auto operator<=>(basic_c_io_observer_unlocked<ch_type> a,basic_c_io_observer_unlocked<ch_type> b) noexcept
{
	return a.fp<=>b.fp;
}
#endif

namespace details
{
template<typename T>
class basic_c_io_handle_impl:public T
{
public:
	using char_type = typename T::char_type;
	using native_handle_type = std::FILE*;
	constexpr basic_c_io_handle_impl()=default;
	template<typename native_hd>
	requires std::same_as<native_handle_type,std::remove_cvref_t<native_hd>>
	constexpr basic_c_io_handle_impl(native_hd fp2) noexcept:T{fp2}{}
	basic_c_io_handle_impl(basic_c_io_handle_impl const&)=delete;
	basic_c_io_handle_impl& operator=(basic_c_io_handle_impl const&)=delete;
	constexpr basic_c_io_handle_impl(basic_c_io_handle_impl&& b) noexcept : T{b.fp}
	{
		b.fp = nullptr;
	}
	basic_c_io_handle_impl& operator=(basic_c_io_handle_impl&& b) noexcept
	{
		if(b.fp==this->fp)[[unlikely]]
			return *this;
		if(this->fp)[[likely]]
			std::fclose(this->fp);
		this->fp=b.release();
		return *this;
	}
	void close()
	{
		if(this->fp)[[likely]]
		{
			if(std::fclose(this->fp)==EOF)
				throw_posix_error();
			this->fp=nullptr;
		}
	}
	inline constexpr void reset(native_handle_type newfp=nullptr) noexcept
	{
		if(this->fp)[[likely]]
			std::fclose(this->fp);
		this->fp=newfp;
	}
};

inline std::FILE* my_fdopen_impl(int fd,char const* mode) 
{
	auto fp{
#if defined(_WIN32)
			::_fdopen(
#elif defined(__NEWLIB__) && !defined(__CYGWIN__)
			::_fdopen_r(_REENT,
#elif defined(__MSDOS__) || defined(__CYGWIN__)
			fdopen(
#else
			::fdopen(
#endif
		fd,mode)};
	if(fp==nullptr)
		throw_posix_error();
	return fp;
}

#if defined(__GLIBC__) || defined(__NEED___isoc_va_list)
inline std::FILE* open_cookie_throw_cookie_impl(void* ptr,open_mode mode,cookie_io_functions_t func)
{
	auto fp{::fopencookie(ptr,to_native_c_mode(mode),func)};
	if(fp==nullptr)[[unlikely]]
		throw_posix_error();
	return fp;
}

template<typename T>
inline std::FILE* open_cookie_throw_cookie_type_impl(T* ptr,open_mode mode)
{
	return open_cookie_throw_cookie_impl(ptr,mode,c_io_cookie_functions<T>.native_functions);
}

template<typename T,typename... Args>
inline std::FILE* open_cookie_type_with_mode(open_mode mode,Args&&...args)
{
	std::unique_ptr<T> up{new T(std::forward<Args>(args)...)};
	auto fp{open_cookie_throw_cookie_type_impl(up.get(),mode)};
	up.release();
	return fp;
}

#elif defined(__BSD_VISIBLE) || defined(__DARWIN_C_LEVEL) || defined(__BIONIC__) || defined(__NEWLIB__)
template<typename T,typename... Args>
inline std::FILE* open_cookie_type(Args&&...args)
{
	std::unique_ptr<T> uptr(new T(std::forward<Args>(args)...));
	auto fp{funopen_wrapper<T>(uptr.get())};
	uptr.release();
	return fp;
}
template<typename T,typename... Args>
inline std::FILE* open_cookie_type_with_mode(open_mode,Args&&...args)
{
	return open_cookie_type<T>(std::forward<Args>(args)...);
}
#else
[[noreturn]]inline void open_cookie_throw_einval_impl()
{
	throw_posix_error(EINVAL);
}

template<typename,typename... Args>
[[noreturn]] inline std::FILE* open_cookie_type_with_mode(open_mode,Args&&...)
{
	open_cookie_throw_einval_impl();
}
#endif

template<typename T>
class basic_c_file_impl:public T
{
public:
	using T::native_handle;
	using char_type=typename T::char_type;
	using native_handle_type=typename T::native_handle_type;
	constexpr basic_c_file_impl()=default;
	template<typename native_hd>
	requires std::same_as<native_handle_type,std::remove_cvref_t<native_hd>>
	explicit constexpr basic_c_file_impl(native_hd hd):T(hd){}

	basic_c_file_impl(basic_posix_io_handle<char_type>&& posix_handle,open_mode om):
		T(my_fdopen_impl(posix_handle.fd,to_native_c_mode(om)))
	{
		posix_handle.release();
	}

#ifdef _WIN32
//windows specific. open posix file from win32 io handle
	basic_c_file_impl(basic_win32_io_handle<char_type>&& win32_handle,open_mode om):
		basic_c_file_impl(basic_posix_file<char_type>(std::move(win32_handle),om),to_native_c_mode(om))
	{
	}
	template<nt_family family>
	basic_c_file_impl(basic_nt_family_io_handle<family,char_type>&& nt_handle,open_mode om):
		basic_c_file_impl(basic_posix_file<char_type>(std::move(nt_handle),om),to_native_c_mode(om))
	{
	}
#endif

	basic_c_file_impl(native_fs_dirent ent,open_mode om,perms pm=static_cast<perms>(436)):
		basic_c_file_impl(basic_posix_file<typename T::char_type>(ent,om,pm),om)
	{}

	basic_c_file_impl(cstring_view file,open_mode om,perms pm=static_cast<perms>(436)):
		basic_c_file_impl(basic_posix_file<typename T::char_type>(file,om,pm),om)
	{}
	basic_c_file_impl(native_at_entry nate,cstring_view file,open_mode om,perms pm=static_cast<perms>(436)):
		basic_c_file_impl(basic_posix_file<typename T::char_type>(nate,file,om,pm),om)
	{}
	basic_c_file_impl(wcstring_view file,open_mode om,perms pm=static_cast<perms>(436)):
		basic_c_file_impl(basic_posix_file<typename T::char_type>(file,om,pm),om)
	{}
	basic_c_file_impl(native_at_entry nate,wcstring_view file,open_mode om,perms pm=static_cast<perms>(436)):
		basic_c_file_impl(basic_posix_file<typename T::char_type>(nate,file,om,pm),om)
	{}
	basic_c_file_impl(u8cstring_view file,open_mode om,perms pm=static_cast<perms>(436)):
		basic_c_file_impl(basic_posix_file<typename T::char_type>(file,om,pm),om)
	{}
	basic_c_file_impl(native_at_entry nate,u8cstring_view file,open_mode om,perms pm=static_cast<perms>(436)):
		basic_c_file_impl(basic_posix_file<typename T::char_type>(nate,file,om,pm),om)
	{}
	basic_c_file_impl(u16cstring_view file,open_mode om,perms pm=static_cast<perms>(436)):
		basic_c_file_impl(basic_posix_file<typename T::char_type>(file,om,pm),om)
	{}
	basic_c_file_impl(native_at_entry nate,u16cstring_view file,open_mode om,perms pm=static_cast<perms>(436)):
		basic_c_file_impl(basic_posix_file<typename T::char_type>(nate,file,om,pm),om)
	{}
	basic_c_file_impl(u32cstring_view file,open_mode om,perms pm=static_cast<perms>(436)):
		basic_c_file_impl(basic_posix_file<typename T::char_type>(file,om,pm),om)
	{}
	basic_c_file_impl(native_at_entry nate,u32cstring_view file,open_mode om,perms pm=static_cast<perms>(436)):
		basic_c_file_impl(basic_posix_file<typename T::char_type>(nate,file,om,pm),om)
	{}
	template<stream stm,typename... Args>
	basic_c_file_impl(open_mode om,std::in_place_type_t<stm>,[[maybe_unused]] Args&& ...args)
		:basic_c_file_impl(open_cookie_type_with_mode<stm>(om,std::forward<Args>(args)...))
	{}
	basic_c_file_impl(basic_c_file_impl const&)=default;
	basic_c_file_impl& operator=(basic_c_file_impl const&)=default;
	constexpr basic_c_file_impl(basic_c_file_impl&&) noexcept=default;
	basic_c_file_impl& operator=(basic_c_file_impl&&) noexcept=default;
	~basic_c_file_impl()
	{
		if(this->fp)[[likely]]
			std::fclose(this->fp);
	}
};

}

template<std::integral ch_type>
using basic_c_io_handle=details::basic_c_io_handle_impl<basic_c_io_observer<ch_type>>;

template<std::integral ch_type>
using basic_c_file=details::basic_c_file_impl<basic_c_io_handle<ch_type>>;

template<std::integral ch_type>
using basic_c_io_handle_unlocked=details::basic_c_io_handle_impl<basic_c_io_observer_unlocked<ch_type>>;

template<std::integral ch_type>
using basic_c_file_unlocked=details::basic_c_file_impl<basic_c_io_handle_unlocked<ch_type>>;

template<std::integral ch_type>
requires requires(basic_posix_io_observer<ch_type> piob)
{
	status(piob);
}
inline constexpr auto status(basic_c_io_observer<ch_type> ciob)
{
	return status(static_cast<basic_posix_io_observer<ch_type>>(ciob));
}

template<std::integral ch_type>
requires requires(basic_posix_io_observer<ch_type> piob)
{
	status(piob);
}
inline constexpr auto status(basic_c_io_observer_unlocked<ch_type> ciob)
{
	return status(static_cast<basic_posix_io_observer<ch_type>>(ciob));
}

namespace details
{
inline bool c_is_character_device_unlocked_impl(std::FILE* fp) noexcept
{
	return posix_is_character_device(fp_unlocked_to_fd(fp));
}

inline bool c_is_character_device_impl(std::FILE* fp) noexcept
{
	return posix_is_character_device(fp_to_fd(fp));
}

inline void c_clear_screen_unlocked_impl(std::FILE* fp)
{
#ifdef _WIN32
	int fd{fp_unlocked_to_fd(fp)};
	void* handle{my_get_osfile_handle(fd)};
	if(!::fast_io::win32::details::win32_is_character_device(handle))
		return;
	c_flush_unlocked_impl(fp);
	::fast_io::win32::details::win32_clear_screen_main(handle);
#else
	int fd{fp_unlocked_to_fd(fp)};
	if(!posix_is_character_device(fd))
		return;
	c_flush_unlocked_impl(fp);
	posix_clear_screen_main(fd);
#endif
}

inline void c_clear_screen_impl(std::FILE* fp)
{
	basic_c_io_observer<char> ciob{fp};
	lock_guard guard{ciob};
	c_clear_screen_unlocked_impl(fp);
}

}

template<std::integral ch_type>
inline bool is_character_device(basic_c_io_observer_unlocked<ch_type> ciob) noexcept
{
	return details::c_is_character_device_unlocked_impl(ciob.fp);
}

template<std::integral ch_type>
inline bool is_character_device(basic_c_io_observer<ch_type> ciob) noexcept
{
	return details::c_is_character_device_impl(ciob.fp);
}

template<std::integral ch_type>
inline void clear_screen(basic_c_io_observer_unlocked<ch_type> ciob)
{
	details::c_clear_screen_unlocked_impl(ciob.fp);
}

template<std::integral ch_type>
inline void clear_screen(basic_c_io_observer<ch_type> ciob)
{
	details::c_clear_screen_impl(ciob.fp);
}

template<std::integral ch_type>
inline auto redirect_handle(basic_c_io_observer<ch_type> h)
{
#if defined(_WIN32)
	return static_cast<basic_win32_io_observer<ch_type>>(h).handle;
#else
	return static_cast<basic_posix_io_observer<ch_type>>(h).fd;
#endif
}
#if 0
template<std::integral char_type>
requires async_stream<basic_posix_io_observer<char_type>>
inline constexpr io_async_scheduler_t<basic_posix_io_observer<char_type>> async_scheduler_type(basic_c_io_observer<char_type>)
{
	return {};
}

template<std::integral char_type>
requires async_stream<basic_posix_io_observer<char_type>>
inline constexpr io_async_overlapped_t<basic_posix_io_observer<char_type>> async_overlapped_type(basic_c_io_observer<char_type>)
{
	return {};
}

template<std::integral char_type>
requires async_stream<basic_posix_io_observer<char_type>>
inline constexpr io_async_scheduler_t<basic_posix_io_observer<char_type>> async_scheduler_type(basic_c_io_observer_unlocked<char_type>)
{
	return {};
}

template<std::integral char_type>
requires async_stream<basic_posix_io_observer<char_type>>
inline constexpr io_async_overlapped_t<basic_posix_io_observer<char_type>> async_overlapped_type(basic_c_io_observer_unlocked<char_type>)
{
	return {};
}
#if defined(_WIN32) || defined(__linux__)
template<std::integral char_type,typename... Args>
requires async_output_stream<basic_posix_io_observer<char_type>>
inline void async_write_callback(io_async_observer ioa,basic_c_io_observer<char_type> h,Args&& ...args)
{
	async_write_callback(ioa,static_cast<basic_posix_io_observer<char_type>>(h),std::forward<Args>(args)...);
}

template<std::integral char_type,typename... Args>
requires async_output_stream<basic_posix_io_observer<char_type>>
inline void async_write_callback(io_async_observer ioa,basic_c_io_observer_unlocked<char_type> h,Args&& ...args)
{
	async_write_callback(ioa,static_cast<basic_posix_io_observer<char_type>>(h),std::forward<Args>(args)...);
}

template<std::integral char_type,typename... Args>
requires async_input_stream<basic_posix_io_observer<char_type>>
inline void async_read_callback(io_async_observer ioa,basic_c_io_observer<char_type> h,Args&& ...args)
{
	async_read_callback(ioa,static_cast<basic_posix_io_observer<char_type>>(h),std::forward<Args>(args)...);
}

template<std::integral char_type,typename... Args>
requires async_input_stream<basic_posix_io_observer<char_type>>
inline void async_read_callback(io_async_observer ioa,basic_c_io_observer_unlocked<char_type> h,Args&& ...args)
{
	async_read_callback(ioa,static_cast<basic_posix_io_observer<char_type>>(h),std::forward<Args>(args)...);
}
#endif
#endif

using c_io_observer_unlocked=basic_c_io_observer_unlocked<char>;
using c_io_observer=basic_c_io_observer<char>;
using c_io_handle_unlocked = basic_c_io_handle_unlocked<char>;
using c_io_handle = basic_c_io_handle<char>;
using c_file = basic_c_file<char>;
using c_file_unlocked = basic_c_file_unlocked<char>;
using wc_io_observer_unlocked=basic_c_io_observer_unlocked<wchar_t>;
using wc_io_observer=basic_c_io_observer<wchar_t>;
using wc_io_handle_unlocked = basic_c_io_handle_unlocked<wchar_t>;
using wc_io_handle = basic_c_io_handle<wchar_t>;
using wc_file = basic_c_file<wchar_t>;
using wc_file_unlocked = basic_c_file_unlocked<wchar_t>;
using u8c_io_observer_unlocked=basic_c_io_observer_unlocked<char8_t>;
using u8c_io_observer=basic_c_io_observer<char8_t>;
using u8c_io_handle_unlocked = basic_c_io_handle_unlocked<char8_t>;
using u8c_io_handle = basic_c_io_handle<char8_t>;
using u8c_file = basic_c_file<char8_t>;
using u8c_file_unlocked = basic_c_file_unlocked<char8_t>;

using u16c_io_observer_unlocked=basic_c_io_observer_unlocked<char16_t>;
using u16c_io_observer=basic_c_io_observer<char16_t>;
using u16c_io_handle_unlocked = basic_c_io_handle_unlocked<char16_t>;
using u16c_io_handle = basic_c_io_handle<char16_t>;
using u16c_file = basic_c_file<char16_t>;
using u16c_file_unlocked = basic_c_file_unlocked<char16_t>;

using u32c_io_observer_unlocked=basic_c_io_observer_unlocked<char32_t>;
using u32c_io_observer=basic_c_io_observer<char32_t>;
using u32c_io_handle_unlocked = basic_c_io_handle_unlocked<char32_t>;
using u32c_io_handle = basic_c_io_handle<char32_t>;
using u32c_file = basic_c_file<char32_t>;
using u32c_file_unlocked = basic_c_file_unlocked<char32_t>;

template<std::integral ch_type>
requires zero_copy_input_stream<basic_posix_io_observer<ch_type>>
inline decltype(auto) zero_copy_in_handle(basic_c_io_observer_unlocked<ch_type> h)
{
	return zero_copy_in_handle(static_cast<basic_posix_io_observer<ch_type>>(h));
}

template<std::integral ch_type>
requires zero_copy_output_stream<basic_posix_io_observer<ch_type>>
inline decltype(auto) zero_copy_out_handle(basic_c_io_observer_unlocked<ch_type> h)
{
	return zero_copy_out_handle(static_cast<basic_posix_io_observer<ch_type>>(h));
}

}

#if defined(__UCLIBC__)
#if defined(__STDIO_BUFFERS)
#include"uclibc.h"
#endif
#elif defined(__mlibc__)
#include"mlibc.h"
#elif defined(__GLIBC__)
#include"glibc.h"
#elif defined(__wasi__)
#include"musl.h"
#elif defined(__NEED___isoc_va_list)
#include"musl.h"
#elif defined(__BSD_VISIBLE) ||defined(__DARWIN_C_LEVEL) \
	|| (defined(__NEWLIB__) &&!defined(__CUSTOM_FILE_IO__)) \
	|| defined(__BIONIC__) || defined(__MSDOS__)  \
	|| (defined(_WIN32)&&defined(FAST_IO_WIN32_USE_SYS_FWRITE))
#include"unix.h"
#endif
#if defined(_WIN32) && !defined(FAST_IO_WIN32_USE_SYS_FWRITE)
#include"wincrt.h"
#else
#if !defined(__MSDOS__)
#include"general.h"
#endif
#include"done.h"
#endif

