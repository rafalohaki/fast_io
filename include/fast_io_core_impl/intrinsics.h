#pragma once
#if defined(_MSC_VER)
#include <intrin.h>
#elif (defined(__x86_64__) || defined(__i386__)) && !defined(__MSDOS__)
#include <x86intrin.h>
#endif

namespace fast_io::details::intrinsics
{
template<typename T>
#if __cpp_lib_concepts >= 202002L
requires (std::unsigned_integral<T>)
#endif
inline constexpr bool add_carry(bool carry,T a,T b,T& out) noexcept
{
#if (defined(_MSC_VER) || defined(__x86_64__) || defined(__i386__)) && (!defined(__MSDOS__))
#if __cpp_lib_is_constant_evaluated >= 201811L
	if(std::is_constant_evaluated())
		return (out=a+b+carry)<=a;
	else
#endif
	{
		if constexpr(sizeof(T)==8)
#if defined(__x86_64__)
			return _addcarry_u64(carry,a,b,
#if !defined(__INTEL_COMPILER ) &&(defined(__GNUC__) || defined(__clang__))
			reinterpret_cast<unsigned long long*>(&out));
#else
			&out);
#endif
#else
			return (out=a+b+carry)<=a;
#endif

		else if constexpr(sizeof(T)==4)
			return _addcarry_u32(carry,a,b,reinterpret_cast<std::uint32_t*>(&out));
		else if constexpr(sizeof(T)==2)
			return _addcarry_u16(carry,a,b,reinterpret_cast<std::uint16_t*>(&out));
		else if constexpr(sizeof(T)==1)
			return _addcarry_u8(carry,a,b,reinterpret_cast<std::uint8_t*>(&out));
	}
#else
	return (out=a+b+carry)<=a;
#endif
}

template<typename T>
#if __cpp_lib_concepts >= 202002L
requires (std::unsigned_integral<T>)
#endif
inline constexpr bool sub_borrow(bool borrow,T a,T b,T& out) noexcept
{
#if (defined(_MSC_VER) || defined(__x86_64__) || defined(__i386__)) && (!defined(__MSDOS__))
#if __cpp_lib_is_constant_evaluated >= 201811L
	if(std::is_constant_evaluated())
		return (out=a-b-borrow)>=a;
	else
#endif
	{
		if constexpr(sizeof(T)==8)
#if defined(__x86_64__)
			return _subborrow_u64(borrow,a,b,
#if !defined(__INTEL_COMPILER ) &&(defined(__GNUC__) || defined(__clang__))
			reinterpret_cast<unsigned long long*>(&out));
#else
			&out);
#endif
#else
			return (out=a-b-borrow)>=a;
#endif

		if constexpr(sizeof(T)==4)
			return _subborrow_u32(borrow,a,b,reinterpret_cast<std::uint32_t*>(&out));
		else if constexpr(sizeof(T)==2)
			return _subborrow_u16(borrow,a,b,reinterpret_cast<std::uint16_t*>(&out));
		else if constexpr(sizeof(T)==1)
			return _subborrow_u8(borrow,a,b,reinterpret_cast<std::uint8_t*>(&out));
	}
#else
	return (out=a-b-borrow)>=a;
#endif
}

inline
#if __cpp_lib_is_constant_evaluated >= 201811L
constexpr
#endif
std::uint64_t umul(std::uint64_t a,std::uint64_t b,std::uint64_t& high) noexcept
{
#if SIZE_MAX < UINT64_MAX || (defined(_MSC_VER)&&!defined(__x86__))
	std::uint32_t a0(static_cast<std::uint32_t>(a));
	std::uint32_t a1(static_cast<std::uint32_t>(a>>32));
	std::uint32_t b0(static_cast<std::uint32_t>(b));
	std::uint32_t b1(static_cast<std::uint32_t>(b>>32));
	std::uint64_t c0(static_cast<std::uint64_t>(a0)*b0);
	std::uint64_t c1(static_cast<std::uint64_t>(a1)*b0+static_cast<std::uint64_t>(a0)*b1);
	c1+=c0>>32;
	c0=static_cast<std::uint32_t>(c0);
	std::uint64_t c2(static_cast<std::uint64_t>(a1)*b1);
	c2+=c1>>32;
	c1=static_cast<std::uint32_t>(c1);
	high=c1;
	return c0|(c1<<32);
#else

#if defined(_MSC_VER) && !defined(__clang__)
#if __cpp_lib_is_constant_evaluated >= 201811L
	if(std::is_constant_evaluated())
	{
		std::uint32_t a0(static_cast<std::uint32_t>(a));
		std::uint32_t a1(static_cast<std::uint32_t>(a>>32));
		std::uint32_t b0(static_cast<std::uint32_t>(b));
		std::uint32_t b1(static_cast<std::uint32_t>(b>>32));
		std::uint64_t c0(static_cast<std::uint64_t>(a0)*b0);
		std::uint64_t c1(static_cast<std::uint64_t>(a1)*b0+static_cast<std::uint64_t>(a0)*b1);
		c1+=c0>>32;
		c0=static_cast<std::uint32_t>(c0);
		std::uint64_t c2(static_cast<std::uint64_t>(a1)*b1);
		c2+=c1>>32;
		c1=static_cast<std::uint32_t>(c1);
		high=c1;
		return c0|(c1<<32);
	}
	else
	{
#endif

		return _umul128(a,b,&high);
#if __cpp_lib_is_constant_evaluated >= 201811L
	}
#endif
#else
#if __cpp_lib_is_constant_evaluated >= 201811L
	if(std::is_constant_evaluated())
	{
		__uint128_t res{static_cast<__uint128_t>(a)*b};
		high=static_cast<std::uint64_t>(res>>64);
		return static_cast<std::uint64_t>(res);
	}
	else
	{
#endif
#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__ || __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
		struct u64x2_t
		{
		#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
			std::uint64_t high,low;
		#else
			std::uint64_t low,high;
		#endif
		};
		static_assert(sizeof(__uint128_t)==sizeof(u64x2_t));
		__uint128_t res{static_cast<__uint128_t>(a)*b};	
		u64x2_t u;
#if defined(__has_builtin)
#if __has_builtin(__builtin_memcpy)
		__builtin_memcpy
#else
		std::memcpy
#endif
#else
		std::memcpy
#endif
		(&u,&res,sizeof(__uint128_t));
		high=u.high;
		return u.low;
#else
		__uint128_t res{static_cast<__uint128_t>(a)*b};
		high=static_cast<std::uint64_t>(res>>64);
		return static_cast<std::uint64_t>(res);
#endif
#if __cpp_lib_is_constant_evaluated >= 201811L
	}
#endif
#endif

#endif
}

inline constexpr std::size_t add_or_overflow_die(std::size_t a,std::size_t b) noexcept
{

#if defined(_MSC_VER)
#if __cpp_lib_is_constant_evaluated >= 201811L
	if(!std::is_constant_evaluated())
	{
#if defined(_M_X64)
		std::size_t res;
		if(_addcarry_u64(false,a,b,&res))[[unlikely]]
			__debugbreak();
		return res;
#elif defined(_M_X32)
		std::size_t size{a+b};
		if(size<a)[[unlikely]]
			__debugbreak();
		return size;
#else
		std::size_t size{a+b};
		if(size<a)[[unlikely]]
			__debugbreak();
		return size;
#endif
	}
	else
#endif
	{
		std::size_t size{a+b};
		if(size<a)[[unlikely]]
			__debugbreak();
		return size;
	}
#elif __has_builtin(__builtin_add_overflow)&& __has_builtin(__builtin_trap)
	std::size_t size;
	if(__builtin_add_overflow(a,b,&size))[[unlikely]]
		__builtin_trap();
	return size;
#else
	std::size_t size{a+b};
	if(size<a)[[unlikely]]
		fast_terminate();
	return size;
#endif
}

template<typename... Args>
requires (std::same_as<std::size_t,std::remove_cvref_t<Args>>&&...)
inline constexpr std::size_t add_or_overflow_die_chain(std::size_t size,Args... args) noexcept
{
	if constexpr(sizeof...(Args)==0)
		return size;
	else
		return add_or_overflow_die(size,add_or_overflow_die_chain(args...));
}

inline constexpr std::size_t mul_or_overflow_die(std::size_t a,std::size_t b) noexcept
{
#if defined(_MSC_VER)
    std::size_t const max{SIZE_MAX/b};
		if(max<a)[[unlikely]]
			__debugbreak();
		return a*b;
#elif __has_builtin(__builtin_mul_overflow)&& __has_builtin(__builtin_trap)
	std::size_t size;
	if(__builtin_mul_overflow(a,b,&size))[[unlikely]]
		__builtin_trap();
	return size;
#else
	std::size_t size{a+b};
	if(size<a)[[unlikely]]
		fast_terminate();
	return size;
#endif
}

template<typename... Args>
requires (std::same_as<std::size_t,std::remove_cvref_t<Args>>&&...)
inline constexpr std::size_t mul_or_overflow_die_chain(std::size_t size,Args... args) noexcept
{
	if constexpr(sizeof...(Args)==0)
		return size;
	else
		return mul_or_overflow_die(size,mul_or_overflow_die_chain(args...));
}

template<typename T>
inline constexpr std::size_t cal_allocation_size_or_die(std::size_t size) noexcept
{
#if defined(_MSC_VER)
	constexpr std::size_t max_size{SIZE_MAX/sizeof(T)};
	if(size>max_size)
		__debugbreak();
	return size*sizeof(T);
#elif __has_builtin(__builtin_mul_overflow) && __has_builtin(__builtin_trap)
	if(__builtin_mul_overflow(size,sizeof(T),&size))[[unlikely]]
		__builtin_trap();
	return size;
#else
	constexpr std::size_t max_size{SIZE_MAX/sizeof(T)};
	if(size>max_size)
		fast_terminate();
	return size*sizeof(T);
#endif
}

}