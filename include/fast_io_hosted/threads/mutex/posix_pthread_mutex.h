#pragma once

#include<pthread.h>

namespace fast_io
{

namespace posix
{
#if __has_cpp_attribute(gnu::dllimport)
[[gnu::dllimport]]
#endif
extern int my_pthread_mutex_lock(pthread_mutex_t*) noexcept
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__)) && __has_cpp_attribute(gnu::dllimport)
#if defined(__GNUC__)
asm("pthread_mutex_lock")
#else
asm("_pthread_mutex_lock")
#endif
#else
asm("pthread_mutex_lock")
#endif
;

#if __has_cpp_attribute(gnu::dllimport)
[[gnu::dllimport]]
#endif
extern int my_pthread_mutex_trylock(pthread_mutex_t*) noexcept
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__)) && __has_cpp_attribute(gnu::dllimport)
#if defined(__GNUC__)
asm("pthread_mutex_trylock")
#else
asm("_pthread_mutex_trylock")
#endif
#else
asm("pthread_mutex_trylock")
#endif
;

#if __has_cpp_attribute(gnu::dllimport)
[[gnu::dllimport]]
#endif
extern int my_pthread_mutex_unlock(pthread_mutex_t*) noexcept
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__)) && __has_cpp_attribute(gnu::dllimport)
#if defined(__GNUC__)
asm("pthread_mutex_unlock")
#else
asm("_pthread_mutex_unlock")
#endif
#else
asm("pthread_mutex_unlock")
#endif
;

#if __has_cpp_attribute(gnu::dllimport)
[[gnu::dllimport]]
#endif
extern int my_pthread_mutex_destroy(pthread_mutex_t*) noexcept
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__)) && __has_cpp_attribute(gnu::dllimport)
#if defined(__GNUC__)
asm("pthread_mutex_destroy")
#else
asm("_pthread_mutex_destroy")
#endif
#else
asm("pthread_mutex_destroy")
#endif
;

}

struct posix_pthread_mutex
{
	using native_handle_type = pthread_mutex_t;
	native_handle_type mutex;
	posix_pthread_mutex() noexcept:mutex(PTHREAD_MUTEX_INITIALIZER)
	{}
	posix_pthread_mutex(posix_pthread_mutex const&)=delete;
	posix_pthread_mutex& operator=(posix_pthread_mutex const&)=delete;
	void lock()
	{
		if(::fast_io::posix::my_pthread_mutex_lock(__builtin_addressof(mutex)))[[unlikely]]
			throw_posix_error();
	}
	bool try_lock() noexcept
	{
		return ::fast_io::posix::my_pthread_mutex_trylock(__builtin_addressof(mutex));
	}
	void unlock() noexcept
	{
		::fast_io::posix::my_pthread_mutex_unlock(__builtin_addressof(mutex));
	}

	~posix_pthread_mutex()
	{
		::fast_io::posix::my_pthread_mutex_destroy(__builtin_addressof(mutex));
	}
};

}