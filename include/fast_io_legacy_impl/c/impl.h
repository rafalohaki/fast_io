#pragma once
#include<cstdio>
#include<cwchar>

#ifdef _WIN32
#if defined(_MSC_VER)||defined(_UCRT)
#else
#include"msvcrt_lock.h"
#endif
#endif
namespace fast_io
{

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
struct io_c_mode
{
	char const* mode{};
	explicit constexpr io_c_mode(cstring_view view):mode(view.c_str()){}
};

#if defined(_GNU_SOURCE) || defined(__MUSL__) || defined(__NEED___isoc_va_list)
template<typename stm>
requires stream<std::remove_reference_t<stm>>
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
#elif (defined(__APPLE__) && defined(__MACH__)) || defined(__BIONIC__) || defined(__NEWLIB__)
namespace details
{
#ifdef __NEWLIB__
# ifdef __LARGE64_FILES
extern "C" FILE	*funopen (const void *__cookie,
		int (*__readfn)(void *__c, char *__buf,
				_READ_WRITE_BUFSIZE_TYPE __n),
		int (*__writefn)(void *__c, const char *__buf,
				 _READ_WRITE_BUFSIZE_TYPE __n),
		_fpos64_t (*__seekfn)(void *__c, _fpos64_t __off, int __whence),
		int (*__closefn)(void *__c));
#else
extern "C" FILE	*funopen (const void *__cookie,
		int (*__readfn)(void *__c, char *__buf,
				_READ_WRITE_BUFSIZE_TYPE __n),
		int (*__writefn)(void *__c, const char *__buf,
				 _READ_WRITE_BUFSIZE_TYPE __n),
		fpos_t  (*__seekfn)(void *__c, fpos_t  __off, int __whence),
		int (*__closefn)(void *__c));
#endif
#endif
//funopen
template<typename stm>
requires stream<std::remove_cvref_t<stm>>
inline std::FILE* funopen_wrapper(void* cookie)
{
	using value_type = std::remove_reference_t<stm>;
	int (*readfn)(void *, char *, int)=nullptr;
	int (*writefn)(void *, const char *, int)=nullptr;
	int (*closefn)(void *)=nullptr;
	fpos_t (*seekfn)(void *, fpos_t, int)=nullptr;
	if constexpr(!std::is_reference_v<stm>)
		closefn=[](void* cookie) noexcept->int
		{
			delete bit_cast<value_type*>(cookie);
			return 0;
		};
	if constexpr(input_stream<value_type>)
		readfn=[](void* cookie,char* buf,int size) noexcept->int
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
		writefn=[](void* cookie,char const* buf,int size) noexcept->int
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
	auto fp{
funopen(
cookie,readfn,writefn,seekfn,closefn)};
	if(fp==nullptr)
		throw_posix_error();
	return fp;
}

}
#endif

namespace details
{

#ifdef __MSDOS__
extern "C" int fileno(FILE*) noexcept;
extern "C" std::FILE* fdopen(int,char const*) noexcept;
#endif

inline int fp_unlocked_to_fd(FILE* fp) noexcept
{
	if(fp==nullptr)
	{
		errno=EBADF;
		return -1;
	}
	return 
#if defined(__WINNT__) || defined(_MSC_VER)
		_fileno(fp)
#elif defined(__NEWLIB__)
		fp->_file
#elif defined(__MSDOS__)
		fileno(fp)
#else
		::fileno_unlocked(fp)
#endif
	;
}

inline int fp_to_fd(FILE* fp) noexcept
{
	if(fp==nullptr)
	{
		errno=EBADF;
		return -1;
	}
	return 
#if defined(__WINNT__) || defined(_MSC_VER)
		_fileno(fp)
#elif defined(__NEWLIB__)
		fp->_file
#elif defined(__MSDOS__)
		fileno(fp)
#else
		::fileno(fp)
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
#if defined(__WINNT__) || defined(_MSC_VER)
	return static_cast<basic_win32_io_observer<ch_type>>(h).native_handle();
#else
	return static_cast<basic_posix_io_observer<ch_type>>(h).native_handle();
#endif
}

using c_io_observer_unlocked = basic_c_io_observer_unlocked<char>;

template<std::integral T,std::contiguous_iterator Iter>
requires (std::same_as<T,std::iter_value_t<Iter>>||std::same_as<T,char>)
inline Iter read(basic_c_io_observer_unlocked<T> cfhd,Iter begin,Iter end);


template<std::integral T,std::contiguous_iterator Iter>
requires (std::same_as<T,std::iter_value_t<Iter>>||std::same_as<T,char>)
inline Iter write(basic_c_io_observer_unlocked<T> cfhd,Iter begin,Iter end);

template<std::integral T>
inline void flush(basic_c_io_observer_unlocked<T> cfhd)
{
	if(
#if defined(_MSC_VER)
		_fflush_nolock
#elif defined(_POSIX_SOURCE)
		fflush_unlocked
#else
		fflush
#endif
	(cfhd.native_handle()))
		throw_posix_error();
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
#elif defined(__NEWLIB__) || defined(__MSDOS__)
		fseek
#else
		fseeko64
#endif
		(fp,offset,static_cast<int>(s)))
		throw_posix_error();
	auto val{
#if defined(_WIN32)
		_ftelli64
#elif defined(__NEWLIB__) || defined(__MSDOS__)
		ftell
#else
		ftello64 
#endif
		(fp)};
	if(val<0)
		throw_posix_error();
	return val;
}
#if defined(_WIN32)
inline std::uintmax_t c_io_seek_no_lock_impl(std::FILE* fp,std::intmax_t offset,seekdir s)
{
	if(_fseeki64_nolock(fp,offset,static_cast<int>(s)))
		throw_posix_error();
	auto val{_ftelli64_nolock(fp)};
	if(val<0)
		throw_posix_error();
	return val;
}
#endif
}

template<std::integral ch_type,std::integral U>
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
#if defined(_MSC_VER)||defined(_UCRT)
	_lock_file(fp);
#elif defined(_WIN32)
	win32::my_msvcrt_lock_file(fp);
#elif defined(__NEWLIB__)
#ifndef __SINGLE_THREAD__
//	flockfile(fp);	//TO FIX
#endif
#elif defined(__MSDOS__)
#else
	flockfile(fp);
#endif
	}
	inline void unlock() const noexcept
	{
#if defined(_MSC_VER)||defined(_UCRT)
	_unlock_file(fp);
#elif defined(_WIN32)
	win32::my_msvcrt_unlock_file(fp);
#elif defined(__NEWLIB__)
#ifndef __SINGLE_THREAD__
//		_funlockfile(fp); //TO FIX
#endif
#elif defined(__MSDOS__)
#else
	funlockfile(fp);
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

template<std::integral T,std::contiguous_iterator Iter>
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

template<std::integral T,std::contiguous_iterator Iter>
requires (std::same_as<T,char>||std::same_as<std::iter_value_t<Iter>,T>)
inline Iter write(basic_c_io_observer<T> cfhd,Iter begin,Iter end)
{
	details::lock_guard lg{cfhd};
	basic_c_io_observer_unlocked<T> cfhd_unlocked{cfhd.fp};
	return write(cfhd_unlocked,begin,end);
}

template<std::integral T>
inline void flush(basic_c_io_observer<T> cfhd)
{
	if(std::fflush(cfhd.native_handle()))
		throw_posix_error();
}

template<std::integral ch_type,std::integral U>
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
	constexpr basic_c_io_handle_impl(basic_c_io_handle_impl&& b) noexcept : T{b.native_handle()}
	{
		b.native_handle() = nullptr;
	}
	basic_c_io_handle_impl& operator=(basic_c_io_handle_impl&& b) noexcept
	{
		if(b.native_handle()==this->native_handle())[[unlikely]]
			return *this;
		if(this->native_handle())[[likely]]
			std::fclose(this->native_handle());
		this->native_handle()=b.release();
		return *this;
	}
	void close()
	{
		if(this->native_handle())[[likely]]
		{
			if(std::fclose(this->native_handle())==EOF)
				throw_posix_error();
			this->native_handle()=nullptr;
		}
	}
	inline constexpr void reset(native_handle_type newfp=nullptr) noexcept
	{
		if(this->native_handle())[[likely]]
			std::fclose(this->native_handle());
		this->native_handle()=newfp;
	}
};


inline std::FILE* my_fdopen_impl(int fd,char const* mode) 
{
	auto fp{
#if defined(__WINNT__) || defined(_MSC_VER)
			::_fdopen(
#elif defined(__NEWLIB__)
			::_fdopen_r(_REENT,
#elif defined(__MSDOS__)
			details::fdopen(
#else
			::fdopen(
#endif
		fd,mode)};
	if(fp==nullptr)
		throw_posix_error();
	return fp;
}

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
	basic_c_file_impl(io_c_mode cmode,basic_posix_io_handle<char_type>&& posix_handle,open_mode):
		T(my_fdopen_impl(posix_handle.fd,cmode.mode))
	{
		posix_handle.release();
	}


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
	basic_c_file_impl(io_c_mode cmode,basic_win32_io_handle<char_type>&& win32_handle,open_mode om)
		:basic_c_file_impl(cmode,basic_posix_file<char_type>(std::move(win32_handle),om),om)
	{
	}

	template<nt_family family>
	basic_c_file_impl(io_c_mode cmode,basic_nt_family_io_handle<family,char_type>&& nt_handle,open_mode om)
		:basic_c_file_impl(cmode,basic_posix_file<char_type>(std::move(nt_handle),om),om)
	{
	}
	basic_c_file_impl(wcstring_view file,open_mode om,perms pm=static_cast<perms>(436)):
		basic_c_file_impl(basic_posix_file<typename T::char_type>(file,om,pm),om)
	{}
	basic_c_file_impl(native_at_entry nate,wcstring_view file,open_mode om,perms pm=static_cast<perms>(436)):
		basic_c_file_impl(basic_posix_file<typename T::char_type>(nate,file,om,pm),om)
	{}
	basic_c_file_impl(io_c_mode cmode,wcstring_view file,open_mode om,perms pm=static_cast<perms>(436)):
		basic_c_file_impl(cmode,basic_posix_file<typename T::char_type>(file,om,pm),om)
	{}
	basic_c_file_impl(io_c_mode cmode,native_at_entry nate,wcstring_view file,open_mode om,perms pm=static_cast<perms>(436)):
		basic_c_file_impl(cmode,basic_posix_file<typename T::char_type>(nate,file,om,pm),om)
	{}
#endif
	basic_c_file_impl(cstring_view file,open_mode om,perms pm=static_cast<perms>(436)):
		basic_c_file_impl(basic_posix_file<typename T::char_type>(file,om,pm),om)
	{}
	basic_c_file_impl(native_at_entry nate,cstring_view file,open_mode om,perms pm=static_cast<perms>(436)):
		basic_c_file_impl(basic_posix_file<typename T::char_type>(nate,file,om,pm),om)
	{}


	basic_c_file_impl(io_c_mode cmode,cstring_view file,open_mode om,perms pm=static_cast<perms>(436)):
		basic_c_file_impl(cmode,basic_posix_file<typename T::char_type>(file,om,pm),om)
	{}
	basic_c_file_impl(io_c_mode cmode,native_at_entry nate,cstring_view file,open_mode om,perms pm=static_cast<perms>(436)):
		basic_c_file_impl(cmode,basic_posix_file<typename T::char_type>(nate,file,om,pm),om)
	{}


	template<stream stm,typename... Args>
	basic_c_file_impl(io_cookie_t,[[maybe_unused]] cstring_view mode,std::in_place_type_t<stm>,[[maybe_unused]] Args&& ...args)
	{
#if defined(_GNU_SOURCE) || defined(__MUSL__) || defined(__NEED___isoc_va_list)
		std::unique_ptr<stm> up{std::make_unique<std::remove_cvref_t<stm>>(std::forward<std::remove_cvref_t<stm>>(args)...)};
		if(!(this->native_handle()=fopencookie(up.get(),mode.c_str(),c_io_cookie_functions<std::remove_cvref_t<stm>>.native_functions)))[[unlikely]]
               			throw_posix_error();
		up.release();
#elif defined(__BSD_VISIBLE) || defined(__BIONIC__) || defined(__NEWLIB__)
		std::unique_ptr<stm> up{std::make_unique<std::remove_cvref_t<stm>>(std::forward<Args>(args)...)};
		this->native_handle()=details::funopen_wrapper<std::remove_cvref_t<stm>>(up.get());
		up.release();
#else
		throw_posix_error(EOPNOTSUPP);
#endif
	}

	template<stream stm>
	basic_c_file_impl(io_cookie_t,[[maybe_unused]] cstring_view mode,[[maybe_unused]] stm& reff)
	{
#if defined(_GNU_SOURCE) || defined(__MUSL__) || defined(__NEED___isoc_va_list)
		if(!(this->native_handle()=fopencookie(std::addressof(reff),mode.c_str(),c_io_cookie_functions<stm&>.native_functions)))[[unlikely]]
               			throw_posix_error();
#elif defined(__BSD_VISIBLE) || defined(__BIONIC__) || defined(__NEWLIB__)
		this->native_handle()=details::funopen_wrapper<stm&>(std::addressof(reff));
#else
		throw_posix_error(EOPNOTSUPP);
#endif
	}
	template<stream stm>
	basic_c_file_impl(io_cookie_t,cstring_view mode,stm&& rref):basic_c_file_impl(io_cookie,mode,std::in_place_type<stm>,std::move(rref)){}

	basic_c_file_impl(basic_c_file_impl const&)=default;
	basic_c_file_impl& operator=(basic_c_file_impl const&)=default;
	constexpr basic_c_file_impl(basic_c_file_impl&&) noexcept=default;
	basic_c_file_impl& operator=(basic_c_file_impl&&) noexcept=default;
	~basic_c_file_impl()
	{
		if(this->native_handle())[[likely]]
			std::fclose(this->native_handle());
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


template<std::integral ch_type>
inline auto redirect_handle(basic_c_io_observer<ch_type> h)
{
#if defined(__WINNT__) || defined(_MSC_VER)
	return static_cast<basic_win32_io_observer<ch_type>>(h).native_handle();
#else
	return static_cast<basic_posix_io_observer<ch_type>>(h).native_handle();
#endif
}

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
using c_io_observer_unlocked=basic_c_io_observer_unlocked<char>;
using c_io_observer=basic_c_io_observer<char>;
using c_io_handle_unlocked = basic_c_io_handle_unlocked<char>;
using c_io_handle = basic_c_io_handle<char>;
using c_file = basic_c_file<char>;
using c_file_unlocked = basic_c_file_unlocked<char>;
#ifndef __MSDOS__
using wc_io_observer_unlocked=basic_c_io_observer_unlocked<wchar_t>;
using wc_io_observer=basic_c_io_observer<wchar_t>;
using wc_io_handle_unlocked = basic_c_io_handle_unlocked<wchar_t>;
using wc_io_handle = basic_c_io_handle<wchar_t>;
using wc_file = basic_c_file<wchar_t>;
using wc_file_unlocked = basic_c_file_unlocked<wchar_t>;
#endif

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

#if defined(_MSC_VER)||defined(_UCRT)
#include"universal_crt.h"
#elif defined(__WINNT__)
#include"msvcrt.h"
#elif defined(__GLIBC__)
#include"glibc.h"
#elif defined(__MUSL__) || defined(__NEED___isoc_va_list)
#include"musl.h"
#elif defined(__BSD_VISIBLE)
#if defined(__NEWLIB__)
#ifndef __CUSTOM_FILE_IO__
#include"newlib.h"
#endif
#else
#include"bsd.h"
#endif
#elif defined(__MSDOS__)
#include"msdos.h"
#endif
#ifndef __MSDOS__
#include"general.h"
#endif

#include"done.h"
