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

namespace details
{
template<open_mode om>
struct c_open_mode
{
inline static constexpr std::string_view value=to_c_mode(om);
};

}
#if defined(_GNU_SOURCE) || defined(__MUSL__)
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
#elif defined(__unix__) || (defined(__APPLE__) && defined(__MACH__)) || defined(__BIONIC__) || defined(__NEWLIB__)
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
	explicit operator bool() const noexcept
	{
		return fp;
	}
	explicit operator basic_posix_io_observer<char_type>() const noexcept
	{
		return basic_posix_io_observer<char_type>{
#if defined(__WINNT__) || defined(_MSC_VER)
			_fileno(fp)
#elif defined(__NEWLIB__)
			fp->_file
#else
			::fileno_unlocked(fp)
#endif
		};
	}
#if defined(__WINNT__) || defined(_MSC_VER)
	explicit operator basic_win32_io_observer<char_type>() const noexcept
	{
		return static_cast<basic_win32_io_observer<char_type>>(static_cast<basic_posix_io_observer<char_type>>(*this));
	}
#endif
	constexpr native_handle_type release() noexcept
	{
		auto temp{fp};
		fp=nullptr;
		return temp;
	}
	inline constexpr void reset(native_handle_type newfp=nullptr) noexcept
	{
		fp=newfp;
	}
};

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

inline std::common_type_t<std::size_t,std::uint64_t> c_io_seek_impl(std::FILE* fp,std::common_type_t<std::ptrdiff_t,std::int64_t> offset,seekdir s)
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
#else
		fseeko64
#endif
		(fp,offset,static_cast<int>(s)))
		throw_posix_error();
	auto val{
#if defined(_WIN32)
		_ftelli64
#else
		ftello64 
#endif
		(fp)};
	if(val<0)
		throw_posix_error();
	return val;
}
#if defined(_WIN32)
inline std::common_type_t<std::size_t,std::uint64_t> c_io_seek_no_lock_impl(std::FILE* fp,std::common_type_t<std::ptrdiff_t,std::int64_t> offset,seekdir s)
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
inline std::common_type_t<std::size_t,std::uint64_t> seek(basic_c_io_observer_unlocked<ch_type> cfhd,std::common_type_t<std::ptrdiff_t,std::int64_t> offset=0,seekdir s=seekdir::cur)
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

class c_io_lock_guard;

template<std::integral ch_type>
class basic_c_io_observer
{
public:
	using lock_guard_type = c_io_lock_guard;
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
	explicit operator bool() const noexcept
	{
		return fp;
	}
	explicit operator basic_posix_io_observer<char_type>() const
	{
		auto fd(
#if defined(__WINNT__) || defined(_MSC_VER)
	_fileno(fp)
#elif defined(__NEWLIB__)
	fp->_file
#else
	::fileno(fp)
#endif
);
		if(fd<0)
			throw_posix_error();
		return basic_posix_io_observer<char_type>{fd};
	}
#if defined(__WINNT__) || defined(_MSC_VER)
	explicit operator basic_win32_io_observer<char_type>() const
	{
		return static_cast<basic_win32_io_observer<char_type>>(static_cast<basic_posix_io_observer<char_type>>(*this));
	}
	explicit operator basic_nt_io_observer<char_type>() const
	{
		return static_cast<basic_nt_io_observer<char_type>>(static_cast<basic_posix_io_observer<char_type>>(*this));
	}
#endif
	constexpr native_handle_type release() noexcept
	{
		auto temp{fp};
		fp=nullptr;
		return temp;
	}
	inline constexpr void reset(native_handle_type newfp=nullptr) noexcept
	{
		fp=newfp;
	}
};

template<std::integral T>
inline auto mutex(basic_c_io_observer<T> h)
{
	return h.native_handle();
}
template<std::integral T>
inline basic_c_io_observer_unlocked<T> unlocked_handle(basic_c_io_observer<T> h)
{
	return {h.native_handle()};
}

class c_io_lock_guard
{
	std::FILE* const fp;
public:
	c_io_lock_guard(std::FILE* f):fp(f)
	{
#if defined(_MSC_VER)||defined(_UCRT)
		_lock_file(fp);
#elif defined(_WIN32)
		win32::my_msvcrt_lock_file(fp);
#elif defined(__NEWLIB__)
#ifndef __SINGLE_THREAD__
//		flockfile(fp);	//TO FIX
#endif
#else
		flockfile(fp);
#endif
	}
	c_io_lock_guard(c_io_lock_guard const&) = delete;
	c_io_lock_guard& operator=(c_io_lock_guard const&) = delete;
	~c_io_lock_guard()
	{
#if defined(_MSC_VER)||defined(_UCRT)
		_unlock_file(fp);
#elif defined(_WIN32)
		win32::my_msvcrt_unlock_file(fp);
#elif defined(__NEWLIB__)
#ifndef __SINGLE_THREAD__
//		_funlockfile(fp); //TO FIX
#endif
#else
		funlockfile(fp);
#endif
	}
};


template<std::integral T,std::contiguous_iterator Iter>
inline Iter read(basic_c_io_observer<T> cfhd,Iter begin,Iter end)
{
	c_io_lock_guard lg{cfhd.fp};
	basic_c_io_observer_unlocked<T> cfhd_unlocked{cfhd.fp};
	return read(cfhd_unlocked,begin,end);
}

template<std::integral T,std::contiguous_iterator Iter>
inline decltype(auto) write(basic_c_io_observer<T> cfhd,Iter begin,Iter end)
{
	c_io_lock_guard lg{cfhd.fp};
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
inline std::common_type_t<std::size_t,std::uint64_t> seek(basic_c_io_observer<ch_type> cfhd,std::common_type_t<std::ptrdiff_t,std::int64_t> offset=0,seekdir s=seekdir::cur)
{
	return details::c_io_seek_impl(cfhd.fp,offset,s);
}

namespace details
{
template<typename T>
class basic_c_io_handle_impl:public T
{
public:
	using char_type = typename T::char_type;
	using native_handle_type = std::FILE*;
	constexpr basic_c_io_handle_impl()=default;
	constexpr basic_c_io_handle_impl(native_handle_type fp2) noexcept:T{fp2}{}
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


template<typename T>
class basic_c_file_impl:public T
{
public:
	using T::native_handle;
	using char_type=typename T::char_type;
	using native_handle_type=typename T::native_handle_type;
	basic_c_file_impl()=default;
	template<typename native_hd>
	requires std::same_as<native_handle_type,std::remove_cvref_t<native_hd>>
	basic_c_file_impl(native_hd hd):T(hd){}
/*
	basic_c_file_impl(std::string_view name,std::string_view mode):T(std::fopen(name.data(),mode.data()))
	{
		if(native_handle()==nullptr)
			throw_posix_error();
	}
	basic_c_file_impl(std::string_view file,open_mode const& m):basic_c_file_impl(file,c_style(m))
	{
		if(with_ate(m))
			seek(*this,0,seekdir::end);
	}*/
//fdopen interface
	basic_c_file_impl(native_interface_t,basic_posix_io_handle<typename T::char_type>&& posix_handle,char const* mode):T(
#if defined(__WINNT__) || defined(_MSC_VER)
			::_fdopen(
#elif defined(__NEWLIB__)
			::_fdopen_r(_REENT,
#else
			::fdopen(
#endif
		posix_handle.fd,mode)
			)
	{
		if(native_handle()==nullptr)
			throw_posix_error();
		posix_handle.release();
		if constexpr(std::same_as<wchar_t,typename T::char_type>)
		{
			fwide(this->native_handle(),1);
		}
		else if constexpr(!std::same_as<char,typename T::char_type>)
		{
		//close buffer for non char and wchar_t types like libstdc++ does. All these hacks just violate strict-aliasing rule
			if(setvbuf(this->native_handle(),nullptr,_IONBF,0))
			{
				std::fclose(this->native_handle());
				throw_posix_error();
			}
		}
	}

	basic_c_file_impl(basic_posix_io_handle<char_type>&& posix_handle,open_mode om):
		basic_c_file_impl(native_interface,std::move(posix_handle),to_c_mode(om).data()){}
	template<open_mode om>
	basic_c_file_impl(basic_posix_io_handle<char_type>&& posix_handle,open_interface_t<om>):
		basic_c_file_impl(native_interface,std::move(posix_handle),details::c_open_mode<om>::value.data()){}
	basic_c_file_impl(basic_posix_io_handle<char_type>&& posix_handle,std::string_view mode):
		basic_c_file_impl(native_interface,std::move(posix_handle),to_c_mode(from_c_mode(mode)).data()){}
#if defined(__WINNT__) || defined(_MSC_VER)
//windows specific. open posix file from win32 io handle
	basic_c_file_impl(basic_win32_io_handle<char_type>&& win32_handle,std::string_view mode):
		basic_c_file_impl(basic_posix_file<char_type>(std::move(win32_handle),mode),mode)
	{
	}
	basic_c_file_impl(basic_win32_io_handle<char_type>&& win32_handle,open_mode om):
		basic_c_file_impl(basic_posix_file<char_type>(std::move(win32_handle),om),to_c_mode(om))
	{
	}
	template<open_mode om>
	basic_c_file_impl(basic_win32_io_handle<char_type>&& win32_handle,open_interface_t<om>):
		basic_c_file_impl(basic_posix_file<char_type>(std::move(win32_handle),open_interface<om>),
			details::c_open_mode<om>::value)//open::c_style_interface_t<om>::mode)
	{
	}
#endif

	template<open_mode om,typename... Args>
	basic_c_file_impl(std::string_view file,open_interface_t<om>,Args&& ...args):
		basic_c_file_impl(basic_posix_file<typename T::char_type>(file,open_interface<om>,std::forward<Args>(args)...),
			open_interface<om>)
	{}
	template<typename... Args>
	basic_c_file_impl(std::string_view file,open_mode om,Args&& ...args):
		basic_c_file_impl(basic_posix_file<typename T::char_type>(file,om,std::forward<Args>(args)...),om)
	{}
	template<typename... Args>
	basic_c_file_impl(std::string_view file,std::string_view mode,Args&& ...args):
		basic_c_file_impl(basic_posix_file<typename T::char_type>(file,mode,std::forward<Args>(args)...),mode)
	{}

#if defined (__linux__) || defined(__WINNT__) || defined(_MSC_VER)
	template<open_mode om,typename... Args>
	basic_c_file_impl(io_async_t,io_async_observer ioa,std::string_view file,open_interface_t<om>,Args&& ...args):
		basic_c_file_impl(basic_posix_file<typename T::char_type>(io_async,ioa,file,open_interface<om>,std::forward<Args>(args)...),
			open_interface<om>)
	{}
	template<typename... Args>
	basic_c_file_impl(io_async_t,io_async_observer ioa,std::string_view file,open_mode om,Args&& ...args):
		basic_c_file_impl(basic_posix_file<typename T::char_type>(io_async,ioa,file,om,std::forward<Args>(args)...),om)
	{}
	template<typename... Args>
	basic_c_file_impl(io_async_t,io_async_observer ioa,std::string_view file,std::string_view mode,Args&& ...args):
		basic_c_file_impl(basic_posix_file<typename T::char_type>(io_async,ioa,file,mode,std::forward<Args>(args)...),mode)
	{}
#endif

	template<stream stm,typename... Args>
	basic_c_file_impl(io_cookie_t,std::string_view mode,std::in_place_type_t<stm>,Args&& ...args)
	{
#if defined(_GNU_SOURCE) || defined(__MUSL__)
		std::unique_ptr<stm> up{std::make_unique<std::remove_cvref_t<stm>>(std::forward<std::remove_cvref_t<stm>>(args)...)};
		if(!(this->native_handle()=fopencookie(up.get(),mode.data(),c_io_cookie_functions<std::remove_cvref_t<stm>>.native_functions)))[[unlikely]]
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
	basic_c_file_impl(io_cookie_t,std::string_view mode,stm& reff)
	{
#if defined(_GNU_SOURCE) || defined(__MUSL__)
		if(!(this->native_handle()=fopencookie(std::addressof(reff),mode.data(),c_io_cookie_functions<stm&>.native_functions)))[[unlikely]]
               			throw_posix_error();
#elif defined(__BSD_VISIBLE) || defined(__BIONIC__) || defined(__NEWLIB__)
		this->native_handle()=details::funopen_wrapper<stm&>(std::addressof(reff));
#else
		throw_posix_error(EOPNOTSUPP);
#endif
	}
	template<stream stm>
	basic_c_file_impl(io_cookie_t,std::string_view mode,stm&& rref):basic_c_file_impl(io_cookie,mode,std::in_place_type<stm>,std::move(rref)){}


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

template<std::integral char_type>
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<basic_c_io_observer_unlocked<char_type>>)
{
	return print_reserve_size(io_reserve_type<void*>);
}

template<std::integral char_type,std::contiguous_iterator caiter,typename U>
inline constexpr caiter print_reserve_define(io_reserve_type_t<basic_c_io_observer_unlocked<char_type>>,caiter iter,U&& v)
{
	return print_reserve_define(io_reserve_type<void*>,iter,v.fp);
}

template<std::integral char_type>
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<basic_c_io_observer<char_type>>)
{
	return print_reserve_size(io_reserve_type<void*>);
}

template<std::integral char_type,std::contiguous_iterator caiter,typename U>
inline constexpr caiter print_reserve_define(io_reserve_type_t<basic_c_io_observer<char_type>>,caiter iter,U&& v)
{
	return print_reserve_define(io_reserve_type<void*>,iter,v.fp);
}

}
#if defined(_MSC_VER)||defined(_UCRT)
#include"universal_crt.h"
#elif defined(__WINNT__)
#include"msvcrt.h"
#elif defined(__GLIBC__)
#include"glibc.h"
#elif defined(__MUSL__)
#include"musl.h"
#elif defined(__BSD_VISIBLE)
#if defined(__NEWLIB__)
#ifndef (__CUSTOM_FILE_IO__)
#include"newlib.h"
#endif
#else
#include"bsd.h"
#endif
#endif

#include"general.h"
#include"done.h"
