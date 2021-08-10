#pragma once
#if defined(_MSC_VER) && !defined(__clang__)
#include <intrin.h>
#elif defined(__INTEL_COMPILER)
#include <x86gprintrin.h>
#endif

namespace fast_io::details::intrinsics
{

template<typename T>
#if __cpp_lib_concepts >= 202002L
requires (std::unsigned_integral<T>)
#endif
inline constexpr bool add_carry_naive(bool carry,T a,T b,T& out) noexcept
{
	T temp{carry+a};
	out=temp+b;
	return (out < b) | (temp < a);
}

template<typename T>
#if __cpp_lib_concepts >= 202002L
requires (std::unsigned_integral<T>)
#endif
inline constexpr bool add_carry(bool carry,T a,T b,T& out) noexcept
{
#if (defined(_MSC_VER) || defined(__x86_64__) || defined(__i386__)) && (!defined(__MSDOS__))
#if __cpp_lib_is_constant_evaluated >= 201811L
	if(std::is_constant_evaluated())
		return add_carry_naive(carry,a,b,out);
	else
#endif
	{
		static_assert(sizeof(T)<=8);
		if constexpr(sizeof(T)==8)
		{
			if constexpr(sizeof(std::size_t)>=sizeof(T))
			{
#if (defined(__GNUC__) || defined(__clang__)) && !defined(__INTEL_COMPILER)
				return __builtin_ia32_addcarryx_u64(carry,a,b,reinterpret_cast<unsigned long long*>(__builtin_addressof(out)));
#else
				return _addcarry_u64(carry,a,b,reinterpret_cast<unsigned long long*>(__builtin_addressof(out)));
#endif
			}
			else
			{
				using may_alias_ptr_type
#if __has_cpp_attribute(gnu::may_alias)
				[[gnu::may_alias]]
#endif
				= std::conditional_t<sizeof(unsigned)==sizeof(std::uint32_t),unsigned*,std::uint32_t*>;

				std::uint32_t a_low;
				std::uint32_t a_high;
				my_memcpy(&a_low,&a,4);
				my_memcpy(&a_high,reinterpret_cast<char const*>(&a)+4,4);
				std::uint32_t b_low;
				std::uint32_t b_high;
				my_memcpy(&b_low,&b,4);
				my_memcpy(&b_high,reinterpret_cast<char const*>(&b)+4,4);
#if (defined(__GNUC__) || defined(__clang__)) && !defined(__INTEL_COMPILER)
				return __builtin_ia32_addcarryx_u32(__builtin_ia32_addcarryx_u32(carry,
				a_low,b_low,reinterpret_cast<may_alias_ptr_type>(__builtin_addressof(out))),
				a_high,b_high,reinterpret_cast<may_alias_ptr_type>(__builtin_addressof(out))+1);
#else
				return _addcarry_u32(_addcarry_u32(carry,
				a_low,b_low,reinterpret_cast<may_alias_ptr_type>(__builtin_addressof(out))),
				a_high,b_high,reinterpret_cast<may_alias_ptr_type>(__builtin_addressof(out))+1);
#endif
			}
		}
		else if constexpr(sizeof(T)==4)
#if (defined(__GNUC__) || defined(__clang__)) && !defined(__INTEL_COMPILER)
			return __builtin_ia32_addcarry_u32(carry,a,b,reinterpret_cast<std::uint32_t*>(__builtin_addressof(out)));
#else
			return _addcarry_u32(carry,a,b,reinterpret_cast<std::uint32_t*>(__builtin_addressof(out)));
#endif
		else if constexpr(sizeof(T)==2)
#if (defined(__GNUC__) || defined(__clang__)) && !defined(__INTEL_COMPILER)
			return __builtin_ia32_addcarry_u16(carry,a,b,reinterpret_cast<std::uint32_t*>(__builtin_addressof(out)));
#else
			return _addcarry_u16(carry,a,b,reinterpret_cast<std::uint16_t*>(__builtin_addressof(out)));
#endif
		else if constexpr(sizeof(T)==1)
#if (defined(__GNUC__) || defined(__clang__)) && !defined(__INTEL_COMPILER)
			return __builtin_ia32_addcarry_u8(carry,a,b,reinterpret_cast<std::uint8_t*>(__builtin_addressof(out)));
#else
			return _addcarry_u8(carry,a,b,reinterpret_cast<std::uint8_t*>(__builtin_addressof(out)));
#endif
	}
#else
	return add_carry_naive(carry,a,b,out);
#endif
}

template<typename T>
#if __cpp_lib_concepts >= 202002L
requires (std::unsigned_integral<T>)
#endif
inline constexpr bool sub_borrow_naive(bool carry,T a,T b,T& out) noexcept
{
	T temp{a-carry};
	out=temp-b;
	return (temp<out)|(a<temp);
}

template<typename T>
#if __cpp_lib_concepts >= 202002L
requires (std::unsigned_integral<T>)
#endif
inline constexpr bool sub_borrow(bool borrow,T a,T b,T& out) noexcept
{
#if defined(__x86__) || defined(_M_IX86) || defined(__i386__)
#if __cpp_lib_is_constant_evaluated >= 201811L
	if(std::is_constant_evaluated())
		return sub_borrow_naive(borrow,a,b,out);
	else
#endif
	{
		static_assert(sizeof(T)<=8);
		if constexpr(sizeof(T)==8)
		{
			if constexpr(sizeof(std::size_t)>=sizeof(T))
			{
#if (defined(__GNUC__) || defined(__clang__)) && !defined(__INTEL_COMPILER)
				return __builtin_ia32_subborrowx_u64(borrow,a,b,reinterpret_cast<unsigned long long*>(__builtin_addressof(out)));
#else
				return _subborrow_u64(borrow,a,b,reinterpret_cast<unsigned long long*>(__builtin_addressof(out)));
#endif
			}
			else
			{
				using may_alias_ptr_type
#if __has_cpp_attribute(gnu::may_alias)
				[[gnu::may_alias]]
#endif
				= std::conditional_t<sizeof(unsigned)==sizeof(std::uint32_t),unsigned*,std::uint32_t*>;

				std::uint32_t a_low;
				std::uint32_t a_high;
				my_memcpy(&a_low,&a,4);
				my_memcpy(&a_high,reinterpret_cast<char const*>(&a)+4,4);
				std::uint32_t b_low;
				std::uint32_t b_high;
				my_memcpy(&b_low,&b,4);
				my_memcpy(&b_high,reinterpret_cast<char const*>(&b)+4,4);
#if defined(__clang__)
				return __builtin_ia32_subborrow_u32(__builtin_ia32_subborrow_u32(borrow,
				a_low,b_low,reinterpret_cast<may_alias_ptr_type>(__builtin_addressof(out))),
				a_high,b_high,reinterpret_cast<may_alias_ptr_type>(__builtin_addressof(out))+1);
#elif defined(__GNUC__) && !defined(__INTEL_COMPILER)
				return __builtin_ia32_sbb_u32(__builtin_ia32_sbb_u32(borrow,
				a_low,b_low,reinterpret_cast<may_alias_ptr_type>(__builtin_addressof(out))),
				a_high,b_high,reinterpret_cast<may_alias_ptr_type>(__builtin_addressof(out))+1);
#else
				return _subborrow_u32(_subborrow_u32(borrow,
				a_low,b_low,reinterpret_cast<may_alias_ptr_type>(__builtin_addressof(out))),
				a_high,b_high,reinterpret_cast<may_alias_ptr_type>(__builtin_addressof(out))+1);
#endif
			}
		}
		if constexpr(sizeof(T)==4)
#if (defined(__GNUC__) || defined(__clang__)) && !defined(__INTEL_COMPILER)
			return __builtin_ia32_sbb_u32(borrow,a,b,reinterpret_cast<std::uint32_t*>(__builtin_addressof(out)));
#else
			return _subborrow_u32(borrow,a,b,reinterpret_cast<std::uint32_t*>(__builtin_addressof(out)));
#endif
		else if constexpr(sizeof(T)==2)
#if (defined(__GNUC__) || defined(__clang__)) && !defined(__INTEL_COMPILER)
			return __builtin_ia32_subborrow_u16(borrow,a,b,reinterpret_cast<std::uint32_t*>(__builtin_addressof(out)));
#else
			return _subborrow_u16(borrow,a,b,reinterpret_cast<std::uint16_t*>(__builtin_addressof(out)));
#endif
		else if constexpr(sizeof(T)==1)
#if (defined(__GNUC__) || defined(__clang__)) && !defined(__INTEL_COMPILER)
			return __builtin_ia32_subborrow_u8(borrow,a,b,reinterpret_cast<std::uint8_t*>(__builtin_addressof(out)));
#else
			return _subborrow_u8(borrow,a,b,reinterpret_cast<std::uint8_t*>(__builtin_addressof(out)));
#endif
	}
#else
	return sub_borrow_naive(borrow,a,b,out);
#endif
}

inline
#if __cpp_lib_is_constant_evaluated >= 201811L
constexpr
#endif
std::uint64_t umul_naive(std::uint64_t a,std::uint64_t b,std::uint64_t& high) noexcept
{
	std::uint32_t a0(static_cast<std::uint32_t>(a));
	std::uint32_t a1(static_cast<std::uint32_t>(a>>32));
	std::uint32_t b0(static_cast<std::uint32_t>(b));
	std::uint32_t b1(static_cast<std::uint32_t>(b>>32));
	std::uint64_t c00(static_cast<std::uint64_t>(a0)*b0);
	std::uint64_t c01(static_cast<std::uint64_t>(a0)*b1);
	std::uint64_t c10(static_cast<std::uint64_t>(a1)*b0);
	std::uint64_t c11(static_cast<std::uint64_t>(a1)*b1);

	std::uint64_t d0{static_cast<std::uint32_t>(c00)};
	std::uint64_t c00_high{c00>>32};
	std::uint64_t c01_low{static_cast<std::uint32_t>(c01)};
	std::uint64_t c01_high{c01>>32};
	std::uint64_t c10_low{static_cast<std::uint32_t>(c10)};
	std::uint64_t c10_high{c10>>32};

	std::uint64_t d1{c00_high+c01_low+c10_low};
	std::uint64_t d2{(d1>>32)+c10_high+c01_high};
	std::uint64_t d3{(d2>>32)+c11};
	high=d2|(d3<<32);
	return d0|(d1<<32);
}

inline
#if __cpp_lib_is_constant_evaluated >= 201811L
constexpr
#endif
std::uint64_t umul(std::uint64_t a,std::uint64_t b,std::uint64_t& high) noexcept
{
#ifdef __SIZEOF_INT128__
#if __cpp_lib_is_constant_evaluated >= 201811L
	if(std::is_constant_evaluated())
	{
		__uint128_t res{static_cast<__uint128_t>(a)*b};
		high=static_cast<std::uint64_t>(res>>64);
		return static_cast<std::uint64_t>(res);
	}
	else

#endif
	{
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
		(__builtin_addressof(u),__builtin_addressof(res),sizeof(__uint128_t));
		high=u.high;
		return u.low;
#else
		__uint128_t res{static_cast<__uint128_t>(a)*b};
		high=static_cast<std::uint64_t>(res>>64);
		return static_cast<std::uint64_t>(res);
#endif
	}
#elif defined(_MSC_VER) && defined(_M_X64)
#if __cpp_lib_is_constant_evaluated >= 201811L
	if(std::is_constant_evaluated())
		return umul_naive(a,b,high);
	else
#endif
		return _umul128(a,b,&high);
#else
	return umul_naive(a,b,high);
#endif
}

inline constexpr std::size_t add_or_overflow_die(std::size_t a,std::size_t b) noexcept
{
#if defined(_MSC_VER) && !defined(__clang__)
#if __cpp_lib_is_constant_evaluated >= 201811L
	if(!std::is_constant_evaluated())
	{
#if defined(_M_X64)
		std::size_t res;
		if(_addcarry_u64(false,a,b,__builtin_addressof(res)))[[unlikely]]
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
	if(__builtin_add_overflow(a,b,__builtin_addressof(size)))[[unlikely]]
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
	if(__builtin_mul_overflow(a,b,__builtin_addressof(size)))[[unlikely]]
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
#if defined(_MSC_VER) && !defined(__clang__)
	constexpr std::size_t max_size{SIZE_MAX/sizeof(T)};
	if(size>max_size)
		__debugbreak();
	return size*sizeof(T);
#elif __has_builtin(__builtin_mul_overflow) && __has_builtin(__builtin_trap)
	if(__builtin_mul_overflow(size,sizeof(T),__builtin_addressof(size)))[[unlikely]]
		__builtin_trap();
	return size;
#else
	constexpr std::size_t max_size{SIZE_MAX/sizeof(T)};
	if(size>max_size)
		fast_terminate();
	return size*sizeof(T);
#endif
}

template<std::unsigned_integral U>
struct udiv_result
{
	using unsigned_type = U;
	U quotient_low,quotient_high;
	U remainder_low,remainder_high;
};

template<typename U>
inline constexpr U shiftright_naive(U low_part,U high_part,std::uint8_t shift) noexcept
{
	U const value{(static_cast<U>(1)<<shift)-1};
	constexpr std::uint8_t bits{sizeof(U)*8};
	U const v{high_part&value};
	if(shift==0)
		return low_part;
	return (low_part>>shift)|(v<<(bits-shift));
}

template<typename U>
inline constexpr U shiftright(U low_part,U high_part,std::uint8_t shift) noexcept
{
#if __cpp_lib_is_constant_evaluated >= 201811L
	if(std::is_constant_evaluated())
		return shiftright_naive(low_part,high_part,shift);
	else
#endif
	{
#if defined(_MSC_VER) && (defined(_M_X64) || defined(__i386__)) && !defined(__clang__)
		if constexpr(sizeof(U)==8&&sizeof(std::size_t)>=8)
		{
			return __shiftright128(low_part,high_part,shift);
		}
		else
#endif
		{
			return shiftright_naive(low_part,high_part,shift);
		}
	}
}

/*
https://code.woboq.org/llvm/compiler-rt/lib/builtins/udivmodti4.c.html#__udivmodti4
*/
template<std::unsigned_integral U>
inline constexpr udiv_result<U> udivmod(U n_low, U n_high,U d_low,U d_high) noexcept
{
	// special cases, X is unknown, K != 0
	if (n_high == 0)
	{
		if (d_high == 0)
		{
			return {n_low/d_low,0,n_low%d_low,0};
		}
		return {0,0,n_low,0};
	}
	constexpr std::uint32_t n_udword_bits{sizeof(U) * 8};
	constexpr std::uint32_t n_utword_bits{n_udword_bits<<1};
	constexpr std::uint32_t n_udword_bits_m1{n_udword_bits-1};
	constexpr std::uint32_t n_udword_bits_m2{n_udword_bits-2};
	// n_high != 0
	U q_low,q_high,r_low,r_high;
	std::uint32_t sr;
	if (d_low == 0)
	{
		if (d_high == 0)
			fast_terminate();
		// d_high != 0
		if (n_low == 0)
		{
			// K 0
			// ---
			// K 0
			return {n_high / d_high,0,0,n_high % d_high};
		}
		// K K
		// ---
		// K 0
		if ((d_high & (d_high - 1)) == 0)
		{
#if defined(__has_builtin)
#if __has_builtin(__builtin_ctzll)
			return {n_high >> __builtin_ctzll(d_high),0,n_low,n_high & (d_high - 1)};
#else
			return {n_high >> static_cast<std::uint32_t>(std::countl_zero(d_high)),0,n_low,n_high & (d_high - 1)};
#endif
#else
			return {n_high >> static_cast<std::uint32_t>(std::countl_zero(d_high)),0,n_low,n_high & (d_high - 1)};
#endif

		}
		// K K
		// ---
		// K 0
#if defined(__has_builtin)
#if __has_builtin(__builtin_ctzll)
		sr=static_cast<std::uint32_t>(__builtin_clzll(d_high) - __builtin_clzll(n_high));
#else
		sr=static_cast<std::uint32_t>(std::countl_zero(d_high) - std::countl_zero(n_high));
#endif
#else
		sr=static_cast<std::uint32_t>(std::countl_zero(d_high) - std::countl_zero(n_high));
#endif

		// 0 <= sr <= n_udword_bits - 2 or sr large
		if (sr > n_udword_bits_m2)
			return {0,0,n_low,n_high};
		++sr;
		// 1 <= sr <= n_udword_bits - 1
		// q.all = n.all << (n_utword_bits - sr);
		q_low=0;
		q_high=n_low << (n_udword_bits - sr);
		// r.all = n.all >> sr;
		r_high=n_high >> sr;
		r_low=(n_high << (n_udword_bits - sr)) | (n_low >> sr);
	}
	else /* d_low != 0 */
	{
		if (d_high == 0)
		{
			// K X
			// ---
			// 0 K
			if ((d_low & (d_low - 1)) == 0) /* if d is a power of 2 */
			{
				U rem{n_low & (d_low - 1)};
				if (d_low == 1)
					return {n_low,n_high,rem,0};
#if defined(__has_builtin)
#if __has_builtin(__builtin_ctzll)
				sr = static_cast<std::uint32_t>(__builtin_ctzll(d_low));
#else
				sr = static_cast<std::uint32_t>(std::countl_zero(d_low));
#endif
#else
				sr = static_cast<std::uint32_t>(std::countl_zero(d_low));
#endif
				q_high = n_high >> sr;
				q_low = (n_high << (n_udword_bits - sr)) | (n_low >> sr);
				return {q_low,q_high,rem,0};
			}
			// K X
			// ---
			// 0 K
#if defined(__has_builtin)
#if __has_builtin(__builtin_ctzll)
			sr = 1 + n_udword_bits + static_cast<std::uint32_t>(__builtin_clzll(d_low)) - static_cast<std::uint32_t>(__builtin_clzll(n_high));
#else
			sr = 1 + n_udword_bits + static_cast<std::uint32_t>(std::countl_zero(d_low)) - static_cast<std::uint32_t>(std::countl_zero(n_high));
#endif
#else
			sr = 1 + n_udword_bits + static_cast<std::uint32_t>(std::countl_zero(d_low)) - static_cast<std::uint32_t>(std::countl_zero(n_high));
#endif
			// 2 <= sr <= n_utword_bits - 1
			// q.all = n.all << (n_utword_bits - sr);
			// r.all = n.all >> sr;
			if (sr == n_udword_bits)
			{
				q_low = 0;
				q_high = n_low;
				r_high = 0;
				r_low = n_high;
			}
			else if (sr < n_udword_bits) /* 2 <= sr <= n_udword_bits - 1 */
			{
				q_low = 0;
				q_high = n_low << (n_udword_bits - sr);
				r_high = n_high >> sr;
				r_low = (n_high << (n_udword_bits - sr)) | (n_low >> sr);
			}
			else /* n_udword_bits + 1 <= sr <= n_utword_bits - 1 */
			{
				q_low = n_low << (n_utword_bits - sr);
				q_high = (n_high << (n_utword_bits - sr)) |
						(n_low >> (sr - n_udword_bits));
				r_high = 0;
				r_low = n_high >> (sr - n_udword_bits);
			}
		}
		else
		{
			// K X
			// ---
			// K K
#if defined(__has_builtin)
#if __has_builtin(__builtin_ctzll)
			sr = static_cast<std::uint32_t>(__builtin_clzll(d_high)) - static_cast<std::uint32_t>(__builtin_clzll(n_high));
#else
			sr = static_cast<std::uint32_t>(std::countl_zero(d_high)) - static_cast<std::uint32_t>(std::countl_zero(n_high));
#endif
#else
			sr = static_cast<std::uint32_t>(std::countl_zero(d_high)) - static_cast<std::uint32_t>(std::countl_zero(n_high));
#endif
			// 0 <= sr <= n_udword_bits - 1 or sr large
			if (sr > n_udword_bits_m1)
				return {0,0,n_low,n_high};
			++sr;
			// 1 <= sr <= n_udword_bits
			// q.all = n.all << (n_utword_bits - sr);
			// r.all = n.all >> sr;
			q_low = 0;
			if (sr == n_udword_bits)
			{
				q_high = n_low;
				r_high = 0;
				r_low = n_high;
			}
			else
			{
				r_high = n_high >> sr;
				r_low = (n_high << (n_udword_bits - sr)) | (n_low >> sr);
				q_high = n_low << (n_udword_bits - sr);
			}
		}
	}
	// Not a special case
	// q and r are initialized with:
	// q.all = n.all << (n_utword_bits - sr);
	// r.all = n.all >> sr;
	// 1 <= sr <= n_utword_bits - 1

	std::uint8_t carry{};
	for (;sr;--sr)
	{
		// r:q = ((r:q)	<< 1) | carry
		r_high = (r_high << 1) | (r_low >> n_udword_bits_m1);
		r_low = (r_low << 1) | (q_high >> n_udword_bits_m1);
		q_high = (q_high << 1) | (q_low >> n_udword_bits_m1);
		q_low = (q_low << 1) | static_cast<U>(carry);


		U s_low,s_high;
		sub_borrow(sub_borrow(true,d_low,r_low,s_low),d_high,r_high,s_high);
		s_low=shiftright(s_low,s_high,n_udword_bits_m1);
		s_high>>=n_udword_bits_m1;
		carry = s_low & 1;
		s_low&=d_low;
		s_high&=d_high;
		sub_borrow(sub_borrow(false,r_low,s_low,r_low),r_high,s_high,r_high);
//		r.all -= d.all & s;
	}
	q_high = (q_high << 1) | (q_low >> n_udword_bits_m1);
	q_low = (q_low << 1) | carry;
	return {q_low,q_high,r_low,r_high};
}

}