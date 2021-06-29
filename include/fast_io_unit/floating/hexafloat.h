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
inline constexpr Iter print_rsvhexafloat_define_impl(Iter iter,flt f) noexcept
{
	using char_type = ::fast_io::freestanding::iter_value_t<Iter>;
	using trait = iec559_traits<flt>;
	using mantissa_type = typename trait::mantissa_type;
	mantissa_type const unwrap		//80bit long double does not work but 128 bit __float128 works
	{
		bit_cast<mantissa_type>(f)
	};
	constexpr std::size_t mbits{trait::mbits};
	constexpr std::size_t ebits{trait::ebits};
	constexpr std::size_t total_bits{mbits+ebits};
	constexpr std::uint32_t bias{(static_cast<std::uint32_t>(1<<ebits)>>1)-1};
	bool sign{static_cast<bool>((unwrap>>total_bits)&1u)};
	constexpr mantissa_type mantissa_mask{(static_cast<mantissa_type>(1)<<mbits)-1};
	mantissa_type mantissa{unwrap&mantissa_mask};
	constexpr mantissa_type exponent_mask{(static_cast<mantissa_type>(1)<<ebits)-1};
	std::uint32_t exponent{static_cast<std::uint32_t>((unwrap>>mbits)&exponent_mask)};
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
inline constexpr std::size_t print_rsvhexafloat_size_cache
{
print_integer_reserved_size_cache<16,showbase,true,mantissa_type>+
6+
print_integer_reserved_size_cache<10,true,true,std::int32_t>
};

}

template<std::integral char_type,manipulators::scalar_flags flags,details::my_floating_point flt>
requires (flags.base==10&&flags.floating==manipulators::floating_format::hexafloat)
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<char_type,manipulators::scalar_manip_t<flags,flt>>) noexcept
{
	using trait = details::iec559_traits<flt>;
	return details::print_rsvhexafloat_size_cache<flags.showbase,typename trait::mantissa_type>;
}

template<freestanding::random_access_iterator Iter,manipulators::scalar_flags flags,details::my_floating_point flt>
requires (flags.base==10&&flags.floating==manipulators::floating_format::hexafloat)
inline constexpr Iter print_reserve_define(io_reserve_type_t<freestanding::iter_value_t<Iter>,manipulators::scalar_manip_t<flags,flt>>,Iter iter,manipulators::scalar_manip_t<flags,flt> f) noexcept
{
	return details::print_rsvhexafloat_define_impl<flags.showbase,flags.uppercase_showbase,flags.showpos,flags.uppercase,flags.uppercase_e,flags.comma>(iter,f.reference);
}

}
