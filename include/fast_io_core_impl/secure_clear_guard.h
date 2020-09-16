#pragma once

namespace fast_io
{

namespace win32
{
#if defined(_MSC_VER)
extern "C" void __stdcall RtlSecureZeroMemory(void*,std::size_t) noexcept;
#endif
}

/*
Based on security paper Dead Store Elimination (Still) Considered Harmful

https://www.usenix.org/system/files/conference/usenixsecurity17/sec17-yang.pdf

https://www.usenix.org/sites/default/files/conference/protected-files/usenixsecurity17_slides_zhaomo_yang.pdf

Existing Technique: Using memory barrier
GCC supports a memory barrier expressed using an inline assembly statement.
According to GCC’s documentation, the clobber argument "memory" tells the
compiler that the inline assembly statement may read or write memory that is not
specified in the input or output arguments.

Used in: zap from Kerberos, memzero_explicit from Linux.
Availability: GCC and Clang.
Effectiveness: effective
Not effective on Clang:
	std::memset(data,0,size);
	__asm__ __volatile__("" ::: "memory");


Effective on Clang
	std::memset(data,0,size);
	__asm__ __volatile__("" ::"r"(data): "memory");

How difficult to create a reliable scrubbing function
However, it does not work with Clang.
A more reliable and portable memory barrier is shown below (which is also used
used in memzero_explicit):


*/

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
	__asm__ __volatile__("" ::"r"(data): "memory");
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