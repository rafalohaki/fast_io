#pragma once

namespace fast_io
{

//All compilers have supported bit_cast but most people are stick with GCC 10. We provide emulation currently.
template<typename To,typename From>
requires (sizeof(To)==sizeof(From) && std::is_trivially_copyable_v<To> && std::is_trivial_v<From>)
inline
#if __cpp_lib_bit_cast >= 201806L
constexpr
#endif
 To bit_cast(From const& src) noexcept
{
#if __cpp_lib_bit_cast >= 201806L
	return std::bit_cast<To>(src);
#else
	To dst;
#if defined(__has_builtin)
#if __has_builtin(__builtin_memcpy)
		__builtin_memcpy
#else
		std::memcpy
#endif
#else
		std::memcpy
#endif
	(std::addressof(dst), std::addressof(src), sizeof(To));
	return dst;
#endif
}

namespace details
{

template<std::unsigned_integral U>
requires (sizeof(U)==1||sizeof(U)==2||sizeof(U)==4||sizeof(U)==8)
inline constexpr U byte_swap_naive_impl(U a) noexcept
{
	if constexpr(sizeof(U)==8)
	{
		return  ((a & 0xff00000000000000ULL) >> 56) |
			((a & 0x00ff000000000000ULL) >> 40) |
			((a & 0x0000ff0000000000ULL) >> 24) |
			((a & 0x000000ff00000000ULL) >> 8)  |
			((a & 0x00000000ff000000ULL) >> 8)  |
			((a & 0x0000000000ff0000ULL) << 24) |
			((a & 0x000000000000ff00ULL) << 40) |
			((a & 0x00000000000000ffULL) << 56) ;
	}
	else if constexpr(sizeof(U)==4)
	{
		return  ((a & 0xff000000U) >> 24) |
			((a & 0x00ff0000U) >> 8) |
			((a & 0x0000ff00U) << 8) |
			((a & 0x000000ffU) << 24)  ;
	}
	else if constexpr(sizeof(U)==2)
	{
		return  ((a & 0xff00) >> 8) |
			((a & 0x00ff) << 8) ;
	}
	else
		return a;
}

template<std::unsigned_integral U>
requires (sizeof(U)==1||sizeof(U)==2||sizeof(U)==4||sizeof(U)==8)
inline constexpr U byte_swap(U a) noexcept
{
	if constexpr(sizeof(U)==1)
		return a;
	else
	{
#if (defined(__GNUG__) || defined(__clang__))
	if constexpr(sizeof(U)==8)
		return __builtin_bswap64(a);
	else if constexpr(sizeof(U)==4)
		return __builtin_bswap32(a);
	else
		return __builtin_bswap16(a);
#else

#if __cpp_lib_is_constant_evaluated>=201811L
	if (std::is_constant_evaluated())
	{
		return byte_swap_naive_impl(a);
	}
	else
#endif
	{
#if defined(_MSC_VER)
	if constexpr(sizeof(U)==8)
		return _byteswap_uint64(a);
	else if constexpr(sizeof(U)==4)
		return _byteswap_ulong(a);
	else
		return _byteswap_ushort(a);
#else
	return byte_swap_naive_impl(a);
#endif
	}
#endif
	}
}

template<std::unsigned_integral U>
inline constexpr U big_endian(U u) noexcept
{
	if constexpr(std::endian::little==std::endian::native)
		return byte_swap(u);
	else
		return u;
}

inline
#if defined(__has_builtin)
#if __has_builtin(__builtin_memcpy)
constexpr
#endif
#endif
void* my_memcpy(void* dest,void const* src,std::size_t count) noexcept
{
    return
#if defined(__has_builtin)
#if __has_builtin(__builtin_memcpy)
		__builtin_memcpy
#else
		std::memcpy
#endif
#else
		std::memcpy
#endif
        (dest,src,count);
}


inline
#if defined(__has_builtin)
#if __has_builtin(__builtin_memmove)
constexpr
#endif
#endif
void* my_memmove(void* dest,void const* src,std::size_t count) noexcept
{
    return
#if defined(__has_builtin)
#if __has_builtin(__builtin_memmove)
		__builtin_memmove
#else
		std::memmove
#endif
#else
		std::memmove
#endif
        (dest,src,count);
}

inline void* my_memset(void* dest, int ch, std::size_t count) noexcept
{
    return
#if defined(__has_builtin)
#if __has_builtin(__builtin_memset)
		__builtin_memset
#else
		std::memset
#endif
#else
		std::memset
#endif
        (dest,ch,count);
}



template<::fast_io::freestanding::input_iterator input_iter,::fast_io::freestanding::input_or_output_iterator output_iter>
inline constexpr output_iter non_overlapped_copy_n(input_iter first,std::size_t count,output_iter result)
{
#if __cpp_lib_is_constant_evaluated>=201811L
	if (std::is_constant_evaluated())
		return ::fast_io::freestanding::copy_n(first,count,result);
	else
#endif
	{
	using input_value_type = ::fast_io::freestanding::iter_value_t<input_iter>;
	using output_value_type = ::fast_io::freestanding::iter_value_t<output_iter>;
	if constexpr
	(::fast_io::freestanding::contiguous_iterator<input_iter>&&
	::fast_io::freestanding::contiguous_iterator<output_iter>&&
	std::is_trivially_copyable_v<input_value_type>&&
	std::is_trivially_copyable_v<output_value_type>&&
	(std::same_as<input_value_type,output_value_type>||
	(std::integral<input_value_type>&&std::integral<output_value_type>&&
	sizeof(input_value_type)==sizeof(output_value_type))))
	{
		if(count)	//to avoid nullptr UB
			my_memcpy(::fast_io::freestanding::to_address(result),::fast_io::freestanding::to_address(first),sizeof(::fast_io::freestanding::iter_value_t<input_iter>)*count);
		return result+=count;
	}
	else
		::fast_io::freestanding::copy_n(first,count,result);
	}
}

template<::fast_io::freestanding::input_iterator input_iter,::fast_io::freestanding::input_or_output_iterator output_iter>
inline constexpr output_iter non_overlapped_copy(input_iter first,input_iter last,output_iter result)
{
#if __cpp_lib_is_constant_evaluated>=201811L
	if (std::is_constant_evaluated())
		return ::fast_io::freestanding::copy(first,last,result);
	else
#endif
	{
	using input_value_type = ::fast_io::freestanding::iter_value_t<input_iter>;
	using output_value_type = ::fast_io::freestanding::iter_value_t<output_iter>;
	if constexpr
	(::fast_io::freestanding::contiguous_iterator<input_iter>&&
	::fast_io::freestanding::contiguous_iterator<output_iter>&&
	std::is_trivially_copyable_v<input_value_type>&&
	std::is_trivially_copyable_v<output_value_type>&&
	(std::same_as<input_value_type,output_value_type>||
	(std::integral<input_value_type>&&std::integral<output_value_type>&&
	sizeof(input_value_type)==sizeof(output_value_type))))
	{
		std::size_t count(last-first);
		if(count)	//to avoid nullptr UB
			my_memcpy(::fast_io::freestanding::to_address(result),::fast_io::freestanding::to_address(first),sizeof(::fast_io::freestanding::iter_value_t<input_iter>)*count);
		return result+=count;
	}
	else
	{
		return ::fast_io::freestanding::copy(first,last,result);
	}
	}
}

template<::fast_io::freestanding::input_iterator input_iter,::fast_io::freestanding::input_or_output_iterator output_iter>
inline constexpr output_iter my_copy_n(input_iter first,std::size_t count,output_iter result)
{
#if __cpp_lib_is_constant_evaluated>=201811L
	if (std::is_constant_evaluated())
		return ::fast_io::freestanding::copy_n(first,count,result);
	else
#endif
	{
	using input_value_type = ::fast_io::freestanding::iter_value_t<input_iter>;
	using output_value_type = ::fast_io::freestanding::iter_value_t<output_iter>;
	if constexpr
	(::fast_io::freestanding::contiguous_iterator<input_iter>&&
	::fast_io::freestanding::contiguous_iterator<output_iter>&&
	std::is_trivially_copyable_v<input_value_type>&&
	std::is_trivially_copyable_v<output_value_type>&&
	(std::same_as<input_value_type,output_value_type>||
	(std::integral<input_value_type>&&std::integral<output_value_type>&&
	sizeof(input_value_type)==sizeof(output_value_type))))
	{
		if(count)	//to avoid nullptr UB
			my_memmove(::fast_io::freestanding::to_address(result),::fast_io::freestanding::to_address(first),sizeof(::fast_io::freestanding::iter_value_t<input_iter>)*count);
		return result+=count;
	}
	else
	{
		return ::fast_io::freestanding::copy_n(first,count,result);
	}
	}
}

template<::fast_io::freestanding::input_iterator input_iter,::fast_io::freestanding::input_or_output_iterator output_iter>
inline constexpr output_iter my_copy(input_iter first,input_iter second,output_iter result)
{
	using input_value_type = ::fast_io::freestanding::iter_value_t<input_iter>;
	using output_value_type = ::fast_io::freestanding::iter_value_t<output_iter>;
	if constexpr(::fast_io::freestanding::contiguous_iterator<input_iter>&&::fast_io::freestanding::contiguous_iterator<output_iter>&&std::is_trivially_copyable_v<input_value_type>&&
	std::is_trivially_copyable_v<output_value_type>&&
	(std::same_as<input_value_type,output_value_type>||(std::integral<input_value_type>&&std::integral<output_value_type>&&sizeof(std::is_trivially_copyable_v<input_value_type>)==sizeof(std::is_trivially_copyable_v<output_value_type>))))
	{
		my_copy_n(first,second-first,result);
		return result+(second-first);
	}
	else
		return ::fast_io::freestanding::copy(first,second,result);
}

template<::fast_io::freestanding::input_or_output_iterator output_iter,std::integral count_type,typename T>
requires (std::is_trivially_copyable_v<T> && sizeof(T)<=sizeof(std::uintmax_t))
inline constexpr void my_fill_n(output_iter first,std::size_t count, T value)
{
#if __cpp_lib_is_constant_evaluated>=201811L
	if (std::is_constant_evaluated())
		::fast_io::freestanding::fill_n(first,count,value);
	else
#endif
	{
	using output_value_type = ::fast_io::freestanding::iter_value_t<output_iter>;
	if constexpr(::fast_io::freestanding::contiguous_iterator<output_iter>&&std::is_trivially_copyable_v<output_value_type>&&std::integral<output_value_type>&&sizeof(output_value_type)==1)
	{
		my_memset(::fast_io::freestanding::to_address(first),static_cast<int>(value),count);
		return first+count;
	}
	else
		::fast_io::freestanding::fill_n(first,count,value);
	}
}

template<::fast_io::freestanding::forward_iterator fwd_iter,typename T>
requires (std::is_trivially_copyable_v<T> && sizeof(T)<=sizeof(std::uintmax_t))
inline constexpr void my_fill(fwd_iter first,fwd_iter last,T value)
{
	using fwd_iter_value_type = ::fast_io::freestanding::iter_value_t<fwd_iter>;
	if constexpr(::fast_io::freestanding::contiguous_iterator<fwd_iter>&&std::is_trivially_copyable_v<fwd_iter_value_type>&&std::integral<fwd_iter_value_type>&&sizeof(fwd_iter_value_type)==1)
		::fast_io::freestanding::fill_n(first,last-first,value);
	else
		::fast_io::freestanding::fill(first,last,value);
}

template<typename T>
using my_make_signed_t=
#ifdef __SIZEOF_INT128__
std::conditional_t<
std::same_as<std::remove_cv_t<T>,__int128_t>||std::same_as<std::remove_cv_t<T>,__uint128_t>,
std::conditional_t<std::same_as<std::remove_cv_t<T>,__int128_t>,T,
std::conditional_t<std::same_as<T,__uint128_t volatile const>,__int128_t volatile const,
std::conditional_t<std::same_as<T,__uint128_t const>,__int128_t const,
std::conditional_t<std::same_as<T,__uint128_t volatile>,__int128_t volatile,
__int128_t>>>>,
std::make_signed_t<std::conditional_t<std::same_as<std::remove_cv_t<T>,__int128_t>||std::same_as<std::remove_cv_t<T>,__uint128_t>,int,T>>>;
#else
std::make_signed_t<T>;
#endif
template<typename T>
using my_make_unsigned_t=
#ifdef __SIZEOF_INT128__
std::conditional_t<
std::same_as<std::remove_cv_t<T>,__int128_t>||std::same_as<std::remove_cv_t<T>,__uint128_t>,
std::conditional_t<std::same_as<std::remove_cv_t<T>,__uint128_t>,T,
std::conditional_t<std::same_as<T,__int128_t volatile const>,__uint128_t volatile const,
std::conditional_t<std::same_as<T,__int128_t const>,__uint128_t const,
std::conditional_t<std::same_as<T,__int128_t volatile>,__uint128_t volatile,
__uint128_t>>>>,
std::make_unsigned_t<std::conditional_t<std::same_as<std::remove_cv_t<T>,__int128_t>||std::same_as<std::remove_cv_t<T>,__uint128_t>,int,T>>>;
#else
std::make_unsigned_t<T>;
#endif

template<typename T>
concept my_integral = 
std::integral<T>
#ifdef __SIZEOF_INT128__
||std::same_as<std::remove_cv_t<T>,__uint128_t>||std::same_as<std::remove_cv_t<T>,__int128_t>
#endif
;

template<typename T>
concept my_signed_integral = 
std::signed_integral<T>
#ifdef __SIZEOF_INT128__
||std::same_as<std::remove_cv_t<T>,__int128_t>
#endif
;
template<typename T>
concept my_unsigned_integral = my_integral<T>&&!my_signed_integral<T>;

template<std::integral char_type,std::size_t n>
requires(n!=0)
inline constexpr std::size_t string_literal_size(char_type const(&)[n])
{
	return n-1;
}

template<std::integral char_type,std::size_t n,::fast_io::freestanding::random_access_iterator output_iter>
requires(n!=0)
inline constexpr output_iter copy_string_literal(char_type const(&s)[n],output_iter result)
{
	details::non_overlapped_copy_n(s,n-1,result);
	return result+(n-1);
}

template<::fast_io::freestanding::input_or_output_iterator output_iter>
inline constexpr output_iter copy_scatter(basic_io_scatter_t<::fast_io::freestanding::iter_value_t<output_iter>> scatter,output_iter result)
{
	return details::non_overlapped_copy_n(scatter.base,scatter.len,result);
}

/*
Since many toolchains do not provide lock_guard. Let's implement it by ourselves based on libstdc++'s lock_guard
https://github.com/gcc-mirror/gcc/blob/53046f072c6e92aa4ba4594c992fe31d89e223ed/libstdc%2B%2B-v3/include/bits/std_mutex.h#L152
*/

template<typename mutex_type>
struct lock_guard
{
mutex_type& device;

explicit constexpr lock_guard(mutex_type& m) : device(m)
{ device.lock(); }

#if __cpp_constexpr >= 201907L
constexpr
#endif
~lock_guard()
{ device.unlock(); }

lock_guard(lock_guard const&) = delete;
lock_guard& operator=(lock_guard const&) = delete;

};


template<my_integral T>
inline constexpr T compile_time_pow(T base,std::size_t pow) noexcept
{
	T t=1;
	for(std::size_t i{};i!=pow;++i)
		t*=base;
	return t;
}

template<my_integral T,std::size_t pow>
inline constexpr auto compile_pow10() noexcept
{
	constexpr auto value{compile_time_pow(std::remove_cvref_t<T>(10),pow)};
	return value;
}

template<my_integral T,std::size_t pow>
inline constexpr auto compile_pow5() noexcept
{
	constexpr auto value{compile_time_pow(std::remove_cvref_t<T>(5),pow)};
	return value;
}

template<std::unsigned_integral uint_type>
inline constexpr auto power10_table_generator() noexcept
{
	constexpr std::size_t digits10(std::numeric_limits<uint_type>::digits10+1);
	::fast_io::freestanding::array<uint_type,digits10> array{};
	uint_type v{1};
	for(std::size_t i{1};i!=digits10;++i)
		array[i]=(v*=static_cast<uint_type>(10));
	return array;
}

template<std::unsigned_integral uint_type>
inline constexpr auto power10table{power10_table_generator<uint_type>()};

template<std::uint32_t base,bool ryu_mode=false,std::size_t mx_size=std::numeric_limits<std::size_t>::max(),my_unsigned_integral U>
inline constexpr std::uint32_t chars_len(U value) noexcept
{
#if 0
	if constexpr(base==10&&sizeof(U)<=8)
	{
		std::uint32_t t{static_cast<std::uint32_t>(std::bit_width(static_cast<std::remove_cvref_t<U>>(value|1)))*1233 >> 12};
		return t-(value<power10table<std::remove_cvref_t<U>>[t]) + 1;
	}
	else
#endif
	if constexpr(base==10&&sizeof(U)<=16)
	{
		if constexpr(15<sizeof(U))
		{
			using M = std::remove_cvref_t<U>;
			if constexpr(16<sizeof(M)||!ryu_mode)
			{
			if constexpr(39<=mx_size)
			if(compile_pow10<M,38>()<=value)
				return 39;
			if constexpr(38<=mx_size)
			if(compile_pow10<M,37>()<=value)
				return 38;
			if constexpr(37<=mx_size)
			if(compile_pow10<M,36>()<=value)
				return 37;
			}
			if constexpr(36<=mx_size)
			if(compile_pow10<M,35>()<=value)
				return 36;
			if constexpr(35<=mx_size)
			if(compile_pow10<M,34>()<=value)
				return 35;
			if constexpr(34<=mx_size)
			if(compile_pow10<M,33>()<=value)
				return 34;
			if constexpr(33<=mx_size)
			if(compile_pow10<M,32>()<=value)
				return 33;
			if constexpr(32<=mx_size)
			if(compile_pow10<M,31>()<=value)
				return 32;
			if constexpr(31<=mx_size)
			if(compile_pow10<M,30>()<=value)
				return 31;
			if constexpr(30<=mx_size)
			if(compile_pow10<M,29>()<=value)
				return 30;
			if constexpr(29<=mx_size)
			if(compile_pow10<M,28>()<=value)
				return 29;
			if constexpr(28<=mx_size)
			if(compile_pow10<M,27>()<=value)
				return 28;
			if constexpr(27<=mx_size)
			if(compile_pow10<M,26>()<=value)
				return 27;
			if constexpr(26<=mx_size)
			if(compile_pow10<M,25>()<=value)
				return 26;
			if constexpr(25<=mx_size)
			if(compile_pow10<M,24>()<=value)
				return 25;
			if constexpr(24<=mx_size)
			if(compile_pow10<M,23>()<=value)
				return 24;
			if constexpr(23<=mx_size)
			if(compile_pow10<M,22>()<=value)
				return 23;
			if constexpr(22<=mx_size)
			if(compile_pow10<M,21>()<=value)
				return 22;
			if constexpr(21<=mx_size)
			if(compile_pow10<M,20>()<=value)
				return 21;
		}
		if constexpr(7<sizeof(U))
		{
			if constexpr(8<sizeof(U)||!ryu_mode)
			{
			if constexpr(20<=mx_size)
			if(10000000000000000000ULL<=value)
				return 20;
			if constexpr(19<=mx_size)
			if(1000000000000000000ULL<=value)
				return 19;
			if constexpr(18<=mx_size)
			if(100000000000000000ULL<=value)
				return 18;
			}
			if constexpr(17<=mx_size)
			if(10000000000000000ULL<=value)
				return 17;
			if constexpr(16<=mx_size)
			if(1000000000000000ULL<=value)
				return 16;
			if constexpr(15<=mx_size)
			if(100000000000000ULL<=value)
				return 15;
			if constexpr(14<=mx_size)
			if(10000000000000ULL<=value)
				return 14;
			if constexpr(13<=mx_size)
			if(1000000000000ULL<=value)
				return 13;
			if constexpr(12<=mx_size)
			if(100000000000ULL<=value)
				return 12;
			if constexpr(11<=mx_size)
			if(10000000000ULL<=value)
				return 11;
		}
		if constexpr(3<sizeof(U))
		{
			if constexpr(4<sizeof(U)||!ryu_mode)
			{
			if constexpr(10<=mx_size)
			if(1000000000U<=value)
				return 10;
			}
			if constexpr(9<=mx_size)
			if(100000000U<=value)
				return 9;
			if constexpr(8<=mx_size)
			if(10000000U<=value)
				return 8;
			if constexpr(7<=mx_size)
			if(1000000U<=value)
				return 7;
			if constexpr(6<=mx_size)
			if(100000U<=value)
				return 6;
		}
		if constexpr(1<sizeof(U))
		{
			if constexpr(5<=mx_size)
			if(10000U<=value)
				return 5;
			if constexpr(4<=mx_size)
			if(1000U<=value)
				return 4;
		}
		if constexpr(3<=mx_size)
		if(100U<=value)
			return 3;
		if constexpr(2<=mx_size)
		if(10U<=value)
			return 2;
		if constexpr(1<=mx_size)
		return 1;
	}
	else
	{
		constexpr std::uint32_t base2(base  * base);
		constexpr std::uint32_t base3(base2 * base);
		constexpr std::uint32_t base4(base3 * base);
		for (std::uint32_t n(1);;n+=4)
		{
			if (value < base)
				return n;
			if (value < base2)
				return n + 1;
			if (value < base3)
				return n + 2;
			if (value < base4)
				return n + 3;
			value /= base4;
		}
	}
}

template<std::uint32_t base,std::size_t mx_size=std::numeric_limits<std::size_t>::max(),my_unsigned_integral U>
inline constexpr std::uint32_t chars_len_3_sep(U value) noexcept
{
	if constexpr(base==10&&sizeof(U)<=16)
	{
		if constexpr(15<sizeof(U))
		{
			using M = std::remove_cvref_t<U>;
			if constexpr(39<=mx_size)
			if(compile_pow10<M,38>()<=value)
				return 51;
			if constexpr(38<=mx_size)
			if(compile_pow10<M,37>()<=value)
				return 50;
			if constexpr(37<=mx_size)
			if(compile_pow10<M,36>()<=value)
				return 49;
			if constexpr(36<=mx_size)
			if(compile_pow10<M,35>()<=value)
				return 47;
			if constexpr(35<=mx_size)
			if(compile_pow10<M,34>()<=value)
				return 46;
			if constexpr(34<=mx_size)
			if(compile_pow10<M,33>()<=value)
				return 45;
			if constexpr(33<=mx_size)
			if(compile_pow10<M,32>()<=value)
				return 43;
			if constexpr(32<=mx_size)
			if(compile_pow10<M,31>()<=value)
				return 42;
			if constexpr(31<=mx_size)
			if(compile_pow10<M,30>()<=value)
				return 41;
			if constexpr(30<=mx_size)
			if(compile_pow10<M,29>()<=value)
				return 39;
			if constexpr(29<=mx_size)
			if(compile_pow10<M,28>()<=value)
				return 38;
			if constexpr(28<=mx_size)
			if(compile_pow10<M,27>()<=value)
				return 37;
			if constexpr(27<=mx_size)
			if(compile_pow10<M,26>()<=value)
				return 35;
			if constexpr(26<=mx_size)
			if(compile_pow10<M,25>()<=value)
				return 34;
			if constexpr(25<=mx_size)
			if(compile_pow10<M,24>()<=value)
				return 33;
			if constexpr(24<=mx_size)
			if(compile_pow10<M,23>()<=value)
				return 31;
			if constexpr(23<=mx_size)
			if(compile_pow10<M,22>()<=value)
				return 30;
			if constexpr(22<=mx_size)
			if(compile_pow10<M,21>()<=value)
				return 29;
			if constexpr(21<=mx_size)
			if(compile_pow10<M,20>()<=value)
				return 27;
		}
		if constexpr(7<sizeof(U))
		{
			if constexpr(20<=mx_size)
			if(10000000000000000000ULL<=value)
				return 26;
			if constexpr(19<=mx_size)
			if(1000000000000000000ULL<=value)
				return 25;
			if constexpr(18<=mx_size)
			if(100000000000000000ULL<=value)
				return 23;
			if constexpr(17<=mx_size)
			if(10000000000000000ULL<=value)
				return 22;
			if constexpr(16<=mx_size)
			if(1000000000000000ULL<=value)
				return 21;
			if constexpr(15<=mx_size)
			if(100000000000000ULL<=value)
				return 19;
			if constexpr(14<=mx_size)
			if(10000000000000ULL<=value)
				return 18;
			if constexpr(13<=mx_size)
			if(1000000000000ULL<=value)
				return 17;
			if constexpr(12<=mx_size)
			if(100000000000ULL<=value)
				return 15;
			if constexpr(11<=mx_size)
			if(10000000000ULL<=value)
				return 14;
		}
		if constexpr(3<sizeof(U))
		{
			if constexpr(10<=mx_size)
			if(1000000000U<=value)
				return 13;
			if constexpr(9<=mx_size)
			if(100000000U<=value)
				return 11;
			if constexpr(8<=mx_size)
			if(10000000U<=value)
				return 10;
			if constexpr(7<=mx_size)
			if(1000000U<=value)
				return 9;
			if constexpr(6<=mx_size)
			if(100000U<=value)
				return 7;
		}
		if constexpr(1<sizeof(U))
		{
			if constexpr(5<=mx_size)
			if(10000U<=value)
				return 6;
			if constexpr(4<=mx_size)
			if(1000U<=value)
				return 5;
		}
		if constexpr(3<=mx_size)
		if(100U<=value)
			return 3;
		if constexpr(2<=mx_size)
		if(10U<=value)
			return 2;
		if constexpr(1<=mx_size)
		return 1;
	}
	else
	{
		auto val{chars_len<base,false,mx_size>(value)};
		return val+(val-1)/3;
	}
}

template<my_integral T>
inline constexpr my_make_unsigned_t<T> cal_int_max() noexcept
{
	my_make_unsigned_t<T> n{};
	--n;
	if constexpr(my_signed_integral<T>)
		n>>=1;
	return n;
}
template<my_integral T>
inline constexpr T get_int_max() noexcept
{
	constexpr T v{static_cast<T>(cal_int_max<T>())};
	return v;
}
template<my_integral T>
inline constexpr auto get_int_max_unsigned() noexcept
{
	constexpr my_make_unsigned_t<std::remove_cvref_t<T>> v{cal_int_max<std::remove_cvref_t<T>>()};
	return v;
}
template<my_integral T,char8_t base = 10>
inline constexpr std::size_t cal_max_int_size() noexcept
{
	std::size_t i{};
	auto n(get_int_max_unsigned<T>());
	for(;n;++i)
		n/=base;
	return i;
}

static_assert(cal_max_int_size<std::uint64_t,10>()==20);
static_assert(cal_max_int_size<std::uint32_t,10>()==10);
template<typename char_type,std::size_t N>
inline constexpr basic_io_scatter_t<char_type> tsc(char_type const (&a)[N]) noexcept
{
	return {a,N-1};
}

}

}
