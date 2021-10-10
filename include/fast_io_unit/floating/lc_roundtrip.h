#pragma once

namespace fast_io::details
{
//let the compiler pick the best calling convention.
template<::fast_io::freestanding::random_access_iterator Iter,my_unsigned_integral U>
inline constexpr Iter lc_print_rsv_fp_general_scientific_common_decay_impl(::fast_io::freestanding::iter_value_t<Iter> const* decimal_point_base,std::size_t decimal_point_len,Iter iter,U m10,std::uint32_t m10len) noexcept
{
	auto res{iter+m10len+decimal_point_len};
	print_reserve_integral_main_impl<10,false>(res,m10,m10len);
	*iter=iter[decimal_point_len];
	if(decimal_point_len==1)[[likely]]
		*iter=*decimal_point_base;
	else
		iter=non_overlapped_copy_n(decimal_point_base,decimal_point_len,iter);
	return res;
}

template<::fast_io::freestanding::random_access_iterator Iter,my_unsigned_integral U>
inline constexpr Iter lc_print_rsv_fp_general_scientific_common_impl(basic_io_scatter_t<::fast_io::freestanding::iter_value_t<Iter>> const& decimal_point_ref,Iter iter,U m10,std::uint32_t m10len) noexcept
{
	basic_io_scatter_t<::fast_io::freestanding::iter_value_t<Iter>> decimal_point{decimal_point_ref};
	return lc_print_rsv_fp_general_scientific_common_decay_impl(decimal_point.base,decimal_point.len,iter,m10,m10len);
}

template<bool comma,::fast_io::freestanding::random_access_iterator Iter,my_unsigned_integral U>
inline constexpr Iter lc_print_rsv_fp_general_common_impl(basic_io_scatter_t<::fast_io::freestanding::iter_value_t<Iter>> const& decimal_point_ref,Iter iter,U m10,std::uint32_t m10len) noexcept
{
	using char_type = ::fast_io::freestanding::iter_value_t<Iter>;
	using unsigned_char_type = std::make_unsigned_t<char_type>;
	if(m10len==1)[[unlikely]]
	{
		*iter=static_cast<char_type>(static_cast<unsigned_char_type>(m10)+char_literal_v<u8'0',char_type>);
		++iter;
		return iter;
	}
	else
		return lc_print_rsv_fp_general_scientific_common_impl<comma>(decimal_point_ref,iter,m10,m10len);
}

template<
typename flt,
bool uppercase_e,
::fast_io::manipulators::floating_format mt,
::fast_io::freestanding::random_access_iterator Iter>
inline constexpr Iter lc_print_rsv_fp_decision_impl(basic_lc_all<::fast_io::freestanding::iter_value_t<Iter>> const* all,Iter iter,typename iec559_traits<flt>::mantissa_type m10,std::int32_t e10) noexcept
{
	using char_type = ::fast_io::freestanding::iter_value_t<Iter>;
	using unsigned_char_type = std::make_unsigned_t<char_type>;
	if constexpr(mt==::fast_io::manipulators::floating_format::scientific)
	{
		if(m10<10u)[[unlikely]]
		{
			*iter=static_cast<char_type>(static_cast<unsigned_char_type>(m10)+char_literal_v<u8'0',char_type>);
			++iter;
		}
		else
		{
			std::uint32_t sz{static_cast<std::uint32_t>(chars_len<10,true>(m10)-1u)};
			e10+=static_cast<std::int32_t>(sz);
			iter=lc_print_rsv_fp_general_scientific_common_impl(all->numeric.decimal_point,iter,m10,sz);
		}
		return print_rsv_fp_e_impl<flt,uppercase_e>(iter,e10);
	}
	else	//general
	{
		std::int32_t olength{static_cast<std::int32_t>(chars_len<10,true>(m10))};
		std::int32_t const real_exp{static_cast<std::int32_t>(e10 + olength - 1)};
		std::uint32_t fixed_length{},this_case{};
		if(olength<=real_exp)
		{
			fixed_length=static_cast<std::uint32_t>(real_exp+1);
			this_case=1;
		}
		else if(0<=real_exp&&real_exp<olength)
		{
			fixed_length=static_cast<std::uint32_t>(olength+2);
			if(olength==real_exp+1)
				--fixed_length;
			this_case=2;
		}
		else
			fixed_length=static_cast<std::uint32_t>(static_cast<std::uint32_t>(-real_exp)+static_cast<std::uint32_t>(olength)+1u);
		std::uint32_t scientific_length{static_cast<std::uint32_t>(olength==1?olength+3:olength+5)};
		auto const& numeric_ref{all->numeric};
		if(scientific_length<fixed_length)
		{
			//scientific decision
			iter=lc_print_rsv_fp_general_common_impl(numeric_ref.decimal_point,iter,m10,static_cast<std::uint32_t>(olength));
			return print_rsv_fp_e_impl<flt,uppercase_e>(iter,real_exp);
		}
		//fixed decision
		auto thousands_sep{numeric_ref.thousands_sep};
		auto grouping{numeric_ref.grouping};
		if(((grouping.len==0u)|(thousand_sep.len==0u)))	//no grouping. for case like "C" or "POSIX"
		{
			switch(this_case)
			{
				case 1:
					print_reserve_integral_main_impl<10,false>(iter+=olength,m10,static_cast<std::uint32_t>(olength));
					return fill_zeros_impl(iter,static_cast<std::uint32_t>(real_exp+1-olength));
				case 2:
				{
					auto eposition(real_exp+1);
					if(olength==eposition)
						print_reserve_integral_main_impl<10,false>(iter+=olength,m10,static_cast<std::uint32_t>(olength));
					else
					{
						auto tmp{iter};
						auto decimal_point{numeric_ref.decimal_point};
						auto decimal_point_base{decimal_point.base};
						std::size_t const decimal_point_len{decimal_point.len};
						print_reserve_integral_main_impl<10,false>(iter+=olength+decimal_point_len,m10,static_cast<std::uint32_t>(olength));
						my_copy_n(tmp+decimal_point_len,static_cast<std::uint32_t>(eposition),tmp);
						if(decimal_point_len==1u)[[likely]]
							tmp[eposition]=*decimal_point_base;
						else
						{
							non_overlapped_copy_n(decimal_point_base,decimal_point_len,tmp+eposition);
						}
					}
					return iter;
				}
				default:
				{
					*iter=char_literal_v<u8'0',char_type>;
					auto decimal_point{numeric_ref.decimal_point};
					auto decimal_point_base{decimal_point.base};
					std::size_t const decimal_point_len{decimal_point.len};
					if(decimal_point_len==1u)
					{
						*iter=*decimal_point_base;
						++iter;
					}
					else
					{
						iter=non_overlapped_copy_n(decimal_point_base,decimal_point_len,iter);
					}
					iter=fill_zeros_impl(iter,static_cast<std::uint32_t>(-real_exp-1));
					print_reserve_integral_main_impl<10,false>(iter+=olength,m10,static_cast<std::uint32_t>(olength));
					return iter;
				}
			}
		}
		else if((grouping.len==1&&*grouping.base==3)&(thousands_sep.len==1)) //three seps which are used in most locales
		{
			switch(this_case)
			{
				case 1:
				{
					print_reserve_integral_main_impl<10,false>(iter+=olength,m10,static_cast<std::uint32_t>(olength));
					return fill_zeros_impl(iter,static_cast<std::uint32_t>(real_exp+1-olength));
				}
				case 2:
				{
					auto eposition(real_exp+1);
					if(olength==eposition)
						print_reserve_integral_main_impl<10,false>(iter+=olength,m10,static_cast<std::uint32_t>(olength));
					else
					{
						auto tmp{iter};
						auto decimal_point{numeric_ref.decimal_point};
						auto decimal_point_base{decimal_point.base};
						std::size_t const decimal_point_len{decimal_point.len};
						print_reserve_integral_main_impl<10,false>(iter+=olength+decimal_point_len,m10,static_cast<std::uint32_t>(olength));
						my_copy_n(tmp+decimal_point_len,static_cast<std::uint32_t>(eposition),tmp);
						if(decimal_point_len==1u)[[likely]]
							tmp[eposition]=*decimal_point_base;
						else
						{
							non_overlapped_copy_n(decimal_point_base,decimal_point_len,tmp+eposition);
						}
					}
					return iter;
				}
				default:
				{
					*iter=char_literal_v<u8'0',char_type>;
					auto decimal_point{numeric_ref.decimal_point};
					auto decimal_point_base{decimal_point.base};
					std::size_t const decimal_point_len{decimal_point.len};
					if(decimal_point_len==1u)
					{
						*iter=*decimal_point_base;
						++iter;
					}
					else
					{
						iter=non_overlapped_copy_n(decimal_point_base,decimal_point_len,iter);
					}
					iter=fill_zeros_impl(iter,static_cast<std::uint32_t>(-real_exp-1));
					print_reserve_integral_main_impl<10,false>(iter+=olength,m10,static_cast<std::uint32_t>(olength));
					return iter;
				}
			}
		}
		else
		{
			
		}
	}
}

template<
bool showpos,
bool uppercase,
bool uppercase_e,
::fast_io::manipulators::floating_format mt,
typename flt,::fast_io::freestanding::random_access_iterator Iter>
inline constexpr Iter lc_print_rsvflt_define_impl(basic_lc_all<::fast_io::freestanding::iter_value_t<Iter>> const* all,Iter iter,flt f) noexcept
{
	if constexpr(::fast_io::manipulators::floating_format::fixed==mt&&uppercase_e)
	{
		return lc_print_rsvflt_define_impl<showpos,uppercase,false,mt>(all,iter,f);
	}
	else
	{
		using char_type = ::fast_io::freestanding::iter_value_t<Iter>;
		using trait = iec559_traits<flt>;
		using mantissa_type = typename trait::mantissa_type;
		constexpr std::size_t ebits{trait::ebits};
		constexpr mantissa_type exponent_mask{(static_cast<mantissa_type>(1)<<ebits)-1};
		constexpr std::uint32_t exponent_mask_u32{static_cast<std::uint32_t>(exponent_mask)};
		auto [mantissa,exponent,sign] = get_punned_result(f);
		iter=print_rsv_fp_sign_impl<showpos>(iter,sign);
		if(exponent==exponent_mask_u32)
			return prsv_fp_nan_impl<uppercase>(iter,mantissa);
		if(!mantissa&&!exponent)
		{
			if constexpr(mt!=::fast_io::manipulators::floating_format::scientific)
			{
				*iter=char_literal_v<u8'0',char_type>;
				++iter;
				return iter;
			}
			else
				return prsv_fp_dece0<uppercase>(iter);
		}
		auto [m10,e10] = dragonbox_impl<flt>(mantissa,static_cast<std::int32_t>(exponent));
		if constexpr(mt==::fast_io::manipulators::floating_format::fixed)
			return lc_print_rsv_fp_fixed_decision_impl<flt>(all,iter,m10,e10);
		else
			return lc_print_rsv_fp_decision_impl<flt,uppercase_e,mt>(all,iter,m10,e10);
	}
}

}
