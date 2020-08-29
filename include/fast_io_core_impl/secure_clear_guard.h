#pragma once

namespace fast_io
{

namespace win32
{
#if defined(_MSC_VER)
extern "C" void __stdcall RtlSecureZeroMemory(void*,std::size_t);
#endif
}

inline void secure_clear(void* data,std::size_t size)
{
#if defined(_MSC_VER)
	win32::RtlSecureZeroMemory(data, size);
#else
/*
https://github.com/bminor/glibc/blob/master/string/explicit_bzero.c
Referenced from glibc
*/
	std::memset(data,0,size);
	__asm__ __volatile__("" ::: "memory");
#endif
}

template<typename T>
class secure_clear_guard
{
public:
	T* region;
	std::size_t count;
	secure_clear_guard(T* rg,std::size_t bts):region(rg),count(bts){}
	secure_clear_guard(secure_clear_guard const&)=delete;
	secure_clear_guard& operator=(secure_clear_guard const&)=delete;
	~secure_clear_guard()
	{
		secure_clear(region,count*sizeof(T));
	}
};

template<typename T>
class secure_clear_no_op
{
public:
	constexpr secure_clear_no_op(T*,std::size_t){};
};

template<typename T,bool condition>
using conditional_secure_clear_guard=std::conditional_t<condition,secure_clear_guard<T>,secure_clear_no_op<T>>;

}