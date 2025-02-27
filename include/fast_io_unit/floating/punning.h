﻿#pragma once

namespace fast_io::details
{

template<typename flt>
struct iec559_traits;


#if defined(__SIZEOF_FLOAT16__) || defined(__FLOAT16__)
template<>
struct iec559_traits<__float16>
{
	using mantissa_type = std::uint16_t;
	inline static constexpr std::size_t mbits{10};
	inline static constexpr std::size_t ebits{5};
	inline static constexpr std::uint32_t m10digits{4};
	inline static constexpr std::uint32_t m2hexdigits{3};
	inline static constexpr std::uint32_t e10digits{2};
	inline static constexpr std::uint32_t e2hexdigits{2};
	inline static constexpr std::uint32_t e10max{7};
};
#endif

template<>
struct iec559_traits<float>
{
	using mantissa_type = std::uint32_t;
	inline static constexpr std::size_t mbits{23};
	inline static constexpr std::size_t ebits{8};
	inline static constexpr std::uint32_t m10digits{9};
	inline static constexpr std::uint32_t m2hexdigits{6};
	inline static constexpr std::uint32_t e10digits{2};
	inline static constexpr std::uint32_t e2hexdigits{3};
	inline static constexpr std::uint32_t e10max{38};
};

template<>
struct iec559_traits<double>
{
	using mantissa_type = std::uint64_t;
	inline static constexpr std::size_t mbits{52};
	inline static constexpr std::size_t ebits{11};
	inline static constexpr std::uint32_t m10digits{17};
	inline static constexpr std::uint32_t m2hexdigits{13};
	inline static constexpr std::uint32_t e10digits{3};
	inline static constexpr std::uint32_t e2hexdigits{4};
	inline static constexpr std::uint32_t e10max{308};
};

#if defined(__SIZEOF_INT128__)

#if defined(__SIZEOF_FLOAT128__) || defined(__FLOAT128__)
template<>
struct iec559_traits<__float128>
{
	using mantissa_type = __uint128_t;
	inline static constexpr std::size_t mbits{112};
	inline static constexpr std::size_t ebits{15};
	inline static constexpr std::uint32_t m10digits{37};
	inline static constexpr std::uint32_t m2hexdigits{28};
	inline static constexpr std::uint32_t e10digits{4};
	inline static constexpr std::uint32_t e2hexdigits{5};
	inline static constexpr std::uint32_t e10max{4966};
};
#endif

#endif

template<my_unsigned_integral T>
#if defined(_MSC_VER) && !defined(__clang__)
#if __has_cpp_attribute(msvc::forceinline)
[[msvc::forceinline]]
#endif
#endif
inline constexpr int my_countr_zero_unchecked(T x) noexcept	//contract: t cannot be zero
{
//referenced from libstdc++
#if defined(__GNUC__) || defined(__clang__)
	constexpr auto Nd = std::numeric_limits<char>::digits*sizeof(T);
	constexpr auto Nd_ull = std::numeric_limits<unsigned long long>::digits;
	constexpr auto Nd_ul = std::numeric_limits<unsigned long>::digits;
	constexpr auto Nd_u = std::numeric_limits<unsigned>::digits;
	if constexpr (Nd <= Nd_u)
		return __builtin_ctz(x);
	else if constexpr (Nd <= Nd_ul)
		return __builtin_ctzl(x);
	else if constexpr (Nd <= Nd_ull)
		return __builtin_ctzll(x);
	else
	{
		static_assert(Nd <= (2 * Nd_ull),
				"Maximum supported integer size is 128-bit");
		constexpr auto max_ull = std::numeric_limits<unsigned long long>::max();
		unsigned long long low = x & max_ull;
		if (low != 0)
			return __builtin_ctzll(low);
		unsigned long long high = x >> Nd_ull;
		return __builtin_ctzll(high) + Nd_ull;
	}
#else
	return std::countr_zero(x);
#endif
}

template<std::floating_point flt>
struct iec559_rep
{
	using mantissa_type = typename iec559_traits<flt>::mantissa_type;
	mantissa_type m;
	std::int32_t e;
};

template<bool uppercase,::fast_io::freestanding::random_access_iterator Iter>
inline constexpr Iter prsv_fp_nan_impl(Iter iter,bool nan) noexcept
{
	using char_type = ::fast_io::freestanding::iter_value_t<Iter>;
	if(nan)
	{
		if constexpr(uppercase)
		{
			if constexpr(std::same_as<char_type,char>)
				return copy_string_literal("NAN",iter);
			else if constexpr(std::same_as<char_type,wchar_t>)
				return copy_string_literal(L"NAN",iter);
			else if constexpr(std::same_as<char_type,char16_t>)
				return copy_string_literal(u"NAN",iter);
			else if constexpr(std::same_as<char_type,char32_t>)
				return copy_string_literal(U"NAN",iter);
			else
				return copy_string_literal(u8"NAN",iter);
		}
		else
		{
			if constexpr(std::same_as<char_type,char>)
				return copy_string_literal("nan",iter);
			else if constexpr(std::same_as<char_type,wchar_t>)
				return copy_string_literal(L"nan",iter);
			else if constexpr(std::same_as<char_type,char16_t>)
				return copy_string_literal(u"nan",iter);
			else if constexpr(std::same_as<char_type,char32_t>)
				return copy_string_literal(U"nan",iter);
			else
				return copy_string_literal(u8"nan",iter);
		}
	}
	else
	{
		if constexpr(uppercase)
		{
			if constexpr(std::same_as<char_type,char>)
				return copy_string_literal("INF",iter);
			else if constexpr(std::same_as<char_type,wchar_t>)
				return copy_string_literal(L"INF",iter);
			else if constexpr(std::same_as<char_type,char16_t>)
				return copy_string_literal(u"INF",iter);
			else if constexpr(std::same_as<char_type,char32_t>)
				return copy_string_literal(U"INF",iter);
			else
				return copy_string_literal(u8"INF",iter);
		}
		else
		{
			if constexpr(std::same_as<char_type,char>)
				return copy_string_literal("inf",iter);
			else if constexpr(std::same_as<char_type,wchar_t>)
				return copy_string_literal(L"inf",iter);
			else if constexpr(std::same_as<char_type,char16_t>)
				return copy_string_literal(u"inf",iter);
			else if constexpr(std::same_as<char_type,char32_t>)
				return copy_string_literal(U"inf",iter);
			else
				return copy_string_literal(u8"inf",iter);
		}
	}
}

template<bool uppercase,::fast_io::freestanding::random_access_iterator Iter>
inline constexpr Iter prsv_fp_hex_0(Iter iter) noexcept
{
	using char_type = ::fast_io::freestanding::iter_value_t<Iter>;
	if constexpr(uppercase)
	{
		if constexpr(std::same_as<char_type,char>)
			return copy_string_literal("0P+0",iter);
		else if constexpr(std::same_as<char_type,wchar_t>)
			return copy_string_literal(L"0P+0",iter);
		else if constexpr(std::same_as<char_type,char16_t>)
			return copy_string_literal(u"0P+0",iter);
		else if constexpr(std::same_as<char_type,char32_t>)
			return copy_string_literal(U"0P+0",iter);
		else
			return copy_string_literal(u8"0P+0",iter);
	}
	else
	{
		if constexpr(std::same_as<char_type,char>)
			return copy_string_literal("0p+0",iter);
		else if constexpr(std::same_as<char_type,wchar_t>)
			return copy_string_literal(L"0p+0",iter);
		else if constexpr(std::same_as<char_type,char16_t>)
			return copy_string_literal(u"0p+0",iter);
		else if constexpr(std::same_as<char_type,char32_t>)
			return copy_string_literal(U"0p+0",iter);
		else
			return copy_string_literal(u8"0p+0",iter);
	}
}

template<bool comma=false,::fast_io::freestanding::random_access_iterator Iter>
inline constexpr Iter prsv_fp_hex1d(Iter iter) noexcept
{
	using char_type = ::fast_io::freestanding::iter_value_t<Iter>;
	if constexpr(comma)
	{
		if constexpr(std::same_as<char_type,char>)
			return copy_string_literal("1,",iter);
		else if constexpr(std::same_as<char_type,wchar_t>)
			return copy_string_literal(L"1,",iter);
		else if constexpr(std::same_as<char_type,char16_t>)
			return copy_string_literal(u"1,",iter);
		else if constexpr(std::same_as<char_type,char32_t>)
			return copy_string_literal(U"1,",iter);
		else
			return copy_string_literal(u8"1,",iter);
	}
	else
	{
		if constexpr(std::same_as<char_type,char>)
			return copy_string_literal("1.",iter);
		else if constexpr(std::same_as<char_type,wchar_t>)
			return copy_string_literal(L"1.",iter);
		else if constexpr(std::same_as<char_type,char16_t>)
			return copy_string_literal(u"1.",iter);
		else if constexpr(std::same_as<char_type,char32_t>)
			return copy_string_literal(U"1.",iter);
		else
			return copy_string_literal(u8"1.",iter);
	}
}

template<bool comma=false,::fast_io::freestanding::random_access_iterator Iter>
inline constexpr Iter prsv_fp_hex0d(Iter iter) noexcept
{
	using char_type = ::fast_io::freestanding::iter_value_t<Iter>;
	if constexpr(comma)
	{
		if constexpr(std::same_as<char_type,char>)
			return copy_string_literal("0,",iter);
		else if constexpr(std::same_as<char_type,wchar_t>)
			return copy_string_literal(L"0,",iter);
		else if constexpr(std::same_as<char_type,char16_t>)
			return copy_string_literal(u"0,",iter);
		else if constexpr(std::same_as<char_type,char32_t>)
			return copy_string_literal(U"0,",iter);
		else
			return copy_string_literal(u8"0,",iter);
	}
	else
	{
		if constexpr(std::same_as<char_type,char>)
			return copy_string_literal("0.",iter);
		else if constexpr(std::same_as<char_type,wchar_t>)
			return copy_string_literal(L"0.",iter);
		else if constexpr(std::same_as<char_type,char16_t>)
			return copy_string_literal(u"0.",iter);
		else if constexpr(std::same_as<char_type,char32_t>)
			return copy_string_literal(U"0.",iter);
		else
			return copy_string_literal(u8"0.",iter);
	}
}

template<bool uppercase,::fast_io::freestanding::random_access_iterator Iter>
inline constexpr Iter prsv_fp_hex0p0(Iter iter) noexcept
{
	using char_type = ::fast_io::freestanding::iter_value_t<Iter>;
	if constexpr(uppercase)
	{
		if constexpr(std::same_as<char_type,char>)
			return copy_string_literal("0P+0",iter);
		else if constexpr(std::same_as<char_type,wchar_t>)
			return copy_string_literal(L"0P+0",iter);
		else if constexpr(std::same_as<char_type,char16_t>)
			return copy_string_literal(u"0P+0",iter);
		else if constexpr(std::same_as<char_type,char32_t>)
			return copy_string_literal(U"0P+0",iter);
		else
			return copy_string_literal(u8"0P+0",iter);
	}
	else
	{
		if constexpr(std::same_as<char_type,char>)
			return copy_string_literal("0p+0",iter);
		else if constexpr(std::same_as<char_type,wchar_t>)
			return copy_string_literal(L"0p+0",iter);
		else if constexpr(std::same_as<char_type,char16_t>)
			return copy_string_literal(u"0p+0",iter);
		else if constexpr(std::same_as<char_type,char32_t>)
			return copy_string_literal(U"0p+0",iter);
		else
			return copy_string_literal(u8"0p+0",iter);
	}
}

template<bool uppercase,::fast_io::freestanding::random_access_iterator Iter>
inline constexpr Iter prsv_fp_dece0(Iter iter) noexcept
{
	using char_type = ::fast_io::freestanding::iter_value_t<Iter>;
	if constexpr(uppercase)
	{
		if constexpr(std::same_as<char_type,char>)
			return copy_string_literal("0E+0",iter);
		else if constexpr(std::same_as<char_type,wchar_t>)
			return copy_string_literal(L"0E+0",iter);
		else if constexpr(std::same_as<char_type,char16_t>)
			return copy_string_literal(u"0E+0",iter);
		else if constexpr(std::same_as<char_type,char32_t>)
			return copy_string_literal(U"0E+0",iter);
		else
			return copy_string_literal(u8"0E+0",iter);
	}
	else
	{
		if constexpr(std::same_as<char_type,char>)
			return copy_string_literal("0e+0",iter);
		else if constexpr(std::same_as<char_type,wchar_t>)
			return copy_string_literal(L"0e+0",iter);
		else if constexpr(std::same_as<char_type,char16_t>)
			return copy_string_literal(u"0e+0",iter);
		else if constexpr(std::same_as<char_type,char32_t>)
			return copy_string_literal(U"0e+0",iter);
		else
			return copy_string_literal(u8"0e+0",iter);
	}
}

template<typename flt>
struct punning_result
{
	typename iec559_traits<flt>::mantissa_type mantissa;
	std::uint32_t exponent;
	bool sign;
};

struct
#if __has_cpp_attribute(gnu::packed)
[[gnu::packed]]
#endif
float80_result
{
	std::uint64_t mantissa;
	std::uint16_t exponent;
};

template<typename flt>
#if __has_cpp_attribute(gnu::always_inline)
[[gnu::always_inline]]
#elif __has_cpp_attribute(msvc::forceinline)
[[msvc::forceinline]]
#endif
inline constexpr punning_result<flt> get_punned_result(flt f) noexcept
{
	using trait = iec559_traits<flt>;
	using mantissa_type = typename trait::mantissa_type;
	constexpr std::size_t mbits{trait::mbits};
	constexpr std::size_t ebits{trait::ebits};
	constexpr std::size_t total_bits{mbits+ebits};
	constexpr mantissa_type mantissa_mask{(static_cast<mantissa_type>(1)<<mbits)-1};
	constexpr mantissa_type exponent_mask{(static_cast<mantissa_type>(1)<<ebits)-1};

	auto unwrap =
#if defined(__has_builtin)
#if __has_builtin(__builtin_bit_cast)
	__builtin_bit_cast(mantissa_type,f)
#else
	bit_cast<mantissa_type>(f)
#endif
#elif defined(_MSC_VER) && __cpp_lib_bit_cast >= 201806L
	__builtin_bit_cast(mantissa_type,f)
#else
	bit_cast<mantissa_type>(f)
#endif
	;
	return {unwrap&mantissa_mask,static_cast<std::uint32_t>((unwrap>>mbits)&exponent_mask),static_cast<bool>((unwrap>>total_bits)&1u)};
}

template<bool showpos,::fast_io::freestanding::random_access_iterator Iter>
inline constexpr Iter print_rsv_fp_sign_impl(Iter iter,bool sign) noexcept
{
	using char_type = ::fast_io::freestanding::iter_value_t<Iter>;
	if constexpr(showpos)
	{
		*iter=sign?char_literal_v<u8'-',char_type>:char_literal_v<u8'+',char_type>;
		++iter;
	}
	else
	{
		if(sign)
		{
			*iter=char_literal_v<u8'-',char_type>;
			++iter;
		}
	}
	return iter;
}

template<::fast_io::freestanding::random_access_iterator Iter,my_unsigned_integral U>
#if __has_cpp_attribute(gnu::hot)
[[gnu::hot]]
#endif
inline constexpr Iter prt_rsv_hundred_flt_impl(Iter iter,U u) noexcept
{
	using char_type = ::fast_io::freestanding::iter_value_t<Iter>;
	constexpr auto tb{::fast_io::details::get_shared_inline_constexpr_base_table<char_type,10,false>().element};
	return non_overlapped_copy_n(tb[u].element,2,iter);
}

template<std::size_t mxdigits,bool indent,::fast_io::freestanding::random_access_iterator Iter,my_unsigned_integral U>
inline constexpr Iter prt_rsv_exponent_impl(Iter iter,U u) noexcept
{
	using char_type = ::fast_io::freestanding::iter_value_t<Iter>;
	using unsigned_char_type = std::make_unsigned_t<char_type>;
	if constexpr(mxdigits==0)
		return iter;
	else if constexpr(mxdigits==1)
	{
		*iter=static_cast<char_type>(char_literal_v<u8'0',char_type>+static_cast<unsigned_char_type>(u));
		++iter;
		return iter;
	}
	else
	{
		constexpr U ten{10u};
		constexpr U hundred{100u};
		constexpr U thousand{1000u};
		if constexpr(mxdigits==2)
		{
			if constexpr(indent)
			{
				return prt_rsv_hundred_flt_impl(iter,u);
			}
			else
			{
				if(u>=ten)
					return prt_rsv_hundred_flt_impl(iter,u);
				else
				{
					*iter=static_cast<char_type>(char_literal_v<u8'0',char_type>+static_cast<unsigned_char_type>(u));
					++iter;
					return iter;
				}
			}
		}
		else if constexpr(mxdigits==3)
		{
			if constexpr(indent)
			{
				if(u>=hundred)
				{
					U div100{u/hundred};
					U mod100{u%hundred};
					*iter=static_cast<char_type>(char_literal_v<u8'0',char_type>+static_cast<unsigned_char_type>(div100));
					++iter;
					u=mod100;
				}
				return prt_rsv_hundred_flt_impl(iter,u);
			}
			else
			{
				if(u>=hundred)
				{
					U div100{u/hundred};
					U mod100{u%hundred};
					*iter=static_cast<char_type>(char_literal_v<u8'0',char_type>+static_cast<unsigned_char_type>(div100));
					++iter;
					return prt_rsv_hundred_flt_impl(iter,mod100);
				}
				else if(u<ten)
				{
					*iter=static_cast<char_type>(char_literal_v<u8'0',char_type>+static_cast<unsigned_char_type>(u));
					++iter;
					return iter;
				}
				return prt_rsv_hundred_flt_impl(iter,u);
			}
		}
		else if constexpr(mxdigits==4)
		{
			if constexpr(indent)
			{
				if(u<hundred)
					return prt_rsv_hundred_flt_impl(iter,u);
				std::size_t sz(3);
				if(u>=thousand)
					sz=4;
				print_reserve_integral_main_impl<10,false>(iter+=sz,u,sz);
			}
			else
			{
				std::size_t sz(1);
				if(u>=thousand)
					sz=4;
				else if(u>=hundred)
					sz=3;
				else if(u>=ten)
					sz=2;
				print_reserve_integral_main_impl<10,false>(iter+=sz,u,sz);
			}
			return iter;
		}
		else if constexpr(mxdigits==5)
		{		
			constexpr U tenthousand{10000u};
			if constexpr(indent)
			{
				if(u<hundred)
					return prt_rsv_hundred_flt_impl(iter,u);
				std::size_t sz(3);
				if(u>=tenthousand)
					sz=5;
				else if(u>=thousand)
					sz=4;
				print_reserve_integral_main_impl<10,false>(iter+=sz,u,sz);
			}
			else
			{
				std::size_t sz(1);
				if(u>=tenthousand)
					sz=5;
				else if(u>=thousand)
					sz=4;
				else if(u>=hundred)
					sz=3;
				else if(u>=ten)
					sz=2;
				print_reserve_integral_main_impl<10,false>(iter+=sz,u,sz);
			}
			return iter;
		}
		else
		{
			if constexpr(indent)
			{
				if(u<hundred)
					return prt_rsv_hundred_flt_impl(iter,u);
			}
			std::size_t sz{chars_len<10,false>(u)};
			auto temp{iter+sz};
			print_reserve_integral_main_impl<10,false>(temp,u,sz);
			return temp;
		}
	}
}

}
