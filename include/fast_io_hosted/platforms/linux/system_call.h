#pragma once

#if defined(__linux__)
#include<asm/unistd.h>
#ifdef __x86_64__
#include"amd64.h"
#elif defined(__arm64__) || defined(__aarch64__) 
#include"aarch64.h"
#else
#include"generic.h"
#endif
#endif


namespace fast_io
{

template<bool always_terminate=false,std::integral I>
requires(sizeof(I)>=1)
inline void system_call_throw_error(I v)
{
#if defined(__linux__)
	using unsigned_t = std::make_unsigned_t<I>;
	if(static_cast<unsigned_t>(v)+static_cast<unsigned_t>(4096)<static_cast<unsigned_t>(4096))
	{
		if constexpr(always_terminate)
			fast_terminate();
		else
		{
#ifdef __cpp_exceptions
			throw posix_error(static_cast<int>(-v));
#else
			fast_terminate();
#endif
		}
	}
#else
	if(v<0)
	{
		if constexpr(always_terminate)
			fast_terminate();
		else
		{
#ifdef __cpp_exceptions
			throw posix_error();
#else
			fast_terminate();
#endif
		}
	}
#endif
}

}
