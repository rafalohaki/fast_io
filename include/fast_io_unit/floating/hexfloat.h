#pragma once

namespace fast_io
{

namespace details
{

template<
bool showbase,
bool showbase_uppercase,
bool showpos,
bool uppercase,
bool uppercase_e,
bool comma,
typename flt,::fast_io::freestanding::random_access_iterator Iter>
inline constexpr Iter print_rsvhexfloat_define_impl(Iter iter,flt f) noexcept
{
	using char_type = ::fast_io::freestanding::iter_value_t<Iter>;
	using trait = iec559_traits<flt>;
	using mantissa_type = typename trait::mantissa_type;
	constexpr std::size_t mbits{trait::mbits};
	constexpr std::size_t ebits{trait::ebits};
	constexpr std::uint32_t bias{(static_cast<std::uint32_t>(1<<ebits)>>1)-1};
	constexpr mantissa_type exponent_mask{(static_cast<mantissa_type>(1)<<ebits)-1};
	auto [mantissa,exponent,sign] = get_punned_result(f);
	constexpr std::uint32_t exponent_mask_u32{static_cast<std::uint32_t>(exponent_mask)};
	constexpr std::int32_t minus_bias{-static_cast<std::int32_t>(bias)};
	constexpr std::uint32_t makeup_bits{((mbits / 4 + 1) * 4 - mbits) % 4};
	if constexpr(showpos)
	{
		*iter=sign?sign_ch<u8'-',char_type>:sign_ch<u8'+',char_type>;
		++iter;
	}
	else
	{
		if(sign)
		{
			*iter=sign_ch<u8'-',char_type>;
			++iter;
		}
	}
	if(exponent==exponent_mask_u32)
		return prsv_fp_nan_impl<uppercase>(iter,mantissa);
	if constexpr(showbase)
		iter=print_reserve_show_base_impl<16,showbase_uppercase>(iter);
	if(!mantissa&&!exponent)
		return prsv_fp_hex0p0<uppercase>(iter);
	std::int32_t e2{static_cast<std::int32_t>(exponent)+minus_bias};
	if(mantissa)
	{
		std::uint32_t digits{static_cast<std::uint32_t>(my_countr_zero_unchecked(mantissa))+makeup_bits};
		constexpr std::uint32_t mdigits_d4{static_cast<std::uint32_t>((mbits+makeup_bits)>>2)};
		std::uint32_t digits_d4{digits>>2};
		std::uint32_t digits_d4m4{digits_d4<<2};
		mantissa>>=(digits_d4m4-makeup_bits);
		std::uint32_t len{mdigits_d4-digits_d4};
		if(exponent==0)
		{
			++e2;
			iter=prsv_fp_hex0d<comma>(iter);
		}
		else
			iter=prsv_fp_hex1d<comma>(iter);
		print_reserve_integral_main_impl<16,uppercase>(iter+=len,mantissa,len);
	}
	else
	{
		*iter=sign_ch<u8'1',char_type>;
		++iter;
	}
	*iter=sign_ch<uppercase_e?u8'P':u8'p',char_type>;
	++iter;
	iter=print_reserve_integral_define<10,false,false,true,false,false>(iter,e2);
	return iter;
}


template<
bool showbase,
typename mantissa_type>
inline constexpr std::size_t print_rsvhexfloat_size_cache
{
print_integer_reserved_size_cache<16,showbase,true,mantissa_type>+
6+
print_integer_reserved_size_cache<10,true,true,std::int32_t>
};

}

template<std::integral char_type,manipulators::scalar_flags flags,details::my_floating_point flt>
requires (flags.base==10&&flags.floating==manipulators::floating_format::hexfloat)
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<char_type,manipulators::scalar_manip_t<flags,flt>>) noexcept
{
	using trait = details::iec559_traits<flt>;
	if constexpr(std::same_as<std::remove_cvref_t<flt>,long double>
#if defined(__SIZEOF_FLOAT128__) || defined(__FLOAT128__)
	||std::same_as<std::remove_cvref_t<flt>,__float128>
#endif
	)
	{
#if (defined(__SIZEOF_FLOAT128__) || defined(__FLOAT128__)) && defined(__SIZEOF_INT128__)
		if constexpr(sizeof(flt)>sizeof(double))
			return details::print_rsvhexfloat_size_cache<flags.showbase,__uint128_t>;
		else
#endif
			return details::print_rsvhexfloat_size_cache<flags.showbase,typename details::iec559_traits<double>::mantissa_type>;
	}
	else
		return details::print_rsvhexfloat_size_cache<flags.showbase,typename trait::mantissa_type>;
}

template<freestanding::random_access_iterator Iter,manipulators::scalar_flags flags,details::my_floating_point flt>
requires (flags.base==10&&flags.floating==manipulators::floating_format::hexfloat)
inline constexpr Iter print_reserve_define(io_reserve_type_t<freestanding::iter_value_t<Iter>,manipulators::scalar_manip_t<flags,flt>>,Iter iter,manipulators::scalar_manip_t<flags,flt> f) noexcept
{

	if constexpr(std::same_as<std::remove_cvref_t<flt>,long double>
#if defined(__SIZEOF_FLOAT128__) || defined(__FLOAT128__)
	||std::same_as<std::remove_cvref_t<flt>,__float128>
#endif
	)
	{
#if (defined(__SIZEOF_FLOAT128__) || defined(__FLOAT128__)) && defined(__SIZEOF_INT128__)
		if constexpr(sizeof(flt)>sizeof(double))
			return details::print_rsvhexfloat_define_impl<flags.showbase,flags.uppercase_showbase,flags.showpos,flags.uppercase,flags.uppercase_e,flags.comma>(iter,static_cast<__float128>(f.reference));
		else
#endif
			return details::print_rsvhexfloat_define_impl<flags.showbase,flags.uppercase_showbase,flags.showpos,flags.uppercase,flags.uppercase_e,flags.comma>(iter,static_cast<double>(f.reference));
	}
	else
		return details::print_rsvhexfloat_define_impl<flags.showbase,flags.uppercase_showbase,flags.showpos,flags.uppercase,flags.uppercase_e,flags.comma>(iter,f.reference);

}

}
