#pragma once

namespace fast_io
{

template<std::integral char_type,std::floating_point fp_type,buffer_output_stream output,character_input_stream input>
inline constexpr bool scan_reserve_transmit(io_reserve_type_t<char_type,fp_type>,output& out,input& in)
{
	constexpr auto sgn{st_single([](char8_t ch)
	{
		return (ch==u8'-')|(ch==u8'+');
	})};
	constexpr auto nb{st_until([](char8_t ch)
	{
		return static_cast<char8_t>(9)<static_cast<char8_t>(ch-u8'0');
	})};
	return scan_transmit(out,in,sgn,nb,st_single([](char8_t ch)
	{
		return ch==u8'.';
	}),nb,st_single([](char8_t ch)
	{
		return (ch==u8'e')|(ch==u8'E');
	}),sgn,nb);
}

namespace details::ryu
{

template<std::floating_point floating_type>
inline constexpr typename floating_traits<floating_type>::mantissa_type me10_to_me2_revamp(typename floating_traits<floating_type>::mantissa_type m10,std::int32_t e10)
{
	using floating_trait = floating_traits<floating_type>;
	using mantissa_type = typename floating_trait::mantissa_type;
	using exponent_type = typename floating_trait::exponent_type;
	constexpr exponent_type real_bits{floating_trait::exponent_bits+floating_trait::mantissa_bits+1};
	constexpr exponent_type maximum_representable_e2{(1<<floating_trait::exponent_bits)-1};
	if((e10<floating_trait::minimum_exp)||(!m10))
		return {};
	if(floating_trait::maximum_exp<e10)
		return static_cast<mantissa_type>(static_cast<mantissa_type>(maximum_representable_e2) << floating_trait::mantissa_bits);
	bool trailing_zeros{};
	std::int32_t e2(static_cast<std::int32_t>(std::bit_width(m10))+e10-(2+floating_trait::mantissa_bits));
	mantissa_type m2{};
	if(e10<0)
	{
		auto const p5bme10(pow5bits(-e10));
		e2-=p5bme10;
		auto j{e2-e10+p5bme10-1+floating_trait::pow5_inv_bitcount};
#ifdef __SIZEOF_INT128__
		if constexpr(std::same_as<floating_type,long double>)
			m2=mul_shift_generic(m10, generic_compute_pow5_inv(-e10), j);
		else
#endif
		if constexpr(std::same_as<floating_type,float>)
			m2=mul_pow5_inv_div_pow2(m10,-e10,j);
		else
			m2=mul_shift(m10,pow5<floating_type,true>::inv_split[-e10],j);
		trailing_zeros=multiple_of_power_of_5(m10,-e10);
	}
	else
	{
		e2+=log2pow5(e10);
		auto j{e2-e10-pow5bits(e10)+floating_trait::pow5_bitcount};
#ifdef __SIZEOF_INT128__
		if constexpr(std::same_as<floating_type,long double>)
			m2=mul_shift_generic(m10, generic_compute_pow5(e10),j);
		else
#endif
		if constexpr(std::same_as<floating_type,float>)
			m2=mul_pow5_div_pow2(m10,e10,j);
		else
			m2=mul_shift(m10,pow5<floating_type,true>::split[e10],j);
		trailing_zeros = e2 < e10 || (e2 - e10 < static_cast<std::int32_t>(real_bits) && multiple_of_power_of_2(m10, e2 - e10));
	}
	std::int32_t ieee_e2(e2 + (floating_trait::bias-1) + std::bit_width(m2));
	if(ieee_e2<0)
		ieee_e2=0;
	if(static_cast<std::int32_t>(maximum_representable_e2)<=ieee_e2)[[unlikely]]
	{
		return static_cast<mantissa_type>(static_cast<mantissa_type>(maximum_representable_e2) << floating_trait::mantissa_bits);
	}
	std::int32_t shift((!ieee_e2?1:ieee_e2)-e2-(floating_trait::bias+floating_trait::mantissa_bits));
	trailing_zeros &= !(m2 & ((static_cast<mantissa_type>(1) << (shift - 1)) - 1));
	bool last_removed_bit((m2>>(shift-1))&1);
	bool round_up((last_removed_bit) && (!trailing_zeros || ((m2 >> shift) & 1)));
	mantissa_type ieee_m2((m2 >> shift) + round_up);
	if(std::same_as<floating_type,float>)
	{
		ieee_m2 &= ((static_cast<mantissa_type>(1) << floating_trait::mantissa_bits) - 1);
		if (ieee_m2 == 0 && round_up)
			++ieee_e2;
	}
	else
	{
		if(ieee_m2 == (static_cast<mantissa_type>(1) << (floating_trait::mantissa_bits + 1)))
			++ieee_e2;
		ieee_m2&=((static_cast<mantissa_type>(1) << floating_trait::mantissa_bits) - 1);
	}
	return (static_cast<mantissa_type>(ieee_e2) << floating_trait::mantissa_bits)|
		(((m2 >> shift) + round_up) & ((static_cast<mantissa_type>(1) << floating_trait::mantissa_bits) - 1));
}

template<bool end_test,std::contiguous_iterator Iter,std::floating_point fp_type>
inline constexpr Iter real_sto(Iter begin,Iter end,fp_type& t)
{
	using char_type = std::iter_value_t<Iter>;
	using floating_type = std::remove_cvref_t<fp_type>;
	using floating_trait = floating_traits<floating_type>;
	using mantissa_type = typename floating_trait::mantissa_type;
	using exponent_type = typename floating_trait::exponent_type;
	using unsigned_char_type = std::make_unsigned_t<char_type>;
	bool const signed_m{*begin==u8'-'};
	if(signed_m|*begin==u8'+')
		++begin;
	constexpr auto zero_lmd([](char8_t ch)
	{
		return ch!=u8'0';
	});
	begin=std::find_if(begin,end,zero_lmd);
	auto dot_it{end};
	if(begin!=end&&*begin==u8'.')
	{
		dot_it=begin;
		++begin;
		begin=std::find_if(begin,end,zero_lmd);
	}
//	::debug_println(dot_it," ",begin);
	auto i{begin};

	exponent_type m10digits{};
	mantissa_type m10{};
	bool need_verify{};
//	std::size_t extra_e10{};
	for(;i!=end;++i)
	{
		unsigned_char_type ch(*i);
		if(static_cast<unsigned_char_type>(9)<(ch-=u8'0'))[[unlikely]]
		{
			if(static_cast<unsigned_char_type>(*i)==u8'.')
			{
				if(dot_it!=end)
					throw_malformed_input();
				dot_it = i;
				continue;
			}
			else
				break;
		}
		if(floating_trait::digits10<m10digits)[[unlikely]]
		{
			if(dot_it==end)
			{
				auto first_none_zero_it{std::find_if(i,end,zero_lmd)};
//				extra_e10=first_none_zero_it-i;
				i=first_none_zero_it;
				if(i==end)
					break;
				else
				{
					if(*i==u8'.')
						i=(dot_it=i)+1;
					else if(
				static_cast<unsigned_char_type>(static_cast<unsigned_char_type>(*i)-static_cast<unsigned_char_type>(u8'1'))<
				static_cast<unsigned_char_type>(9)
					)
					{
						
						for(need_verify=true;i!=end&&
							static_cast<unsigned_char_type>(static_cast<unsigned_char_type>(*i)-u8'0')
							<static_cast<unsigned_char_type>(10);++i);
//							++extra_e10;
						if(i!=end&&*i==u8'.')
						{
							dot_it=i;
							++i;
						}
						else
							break;
					}
					else
						break;
				}
			}
			if((i=std::find_if(i,end,zero_lmd))==end)
				break;
			else
			{
				if(*i==u8'.')
					throw_malformed_input();
				else if(static_cast<unsigned_char_type>(static_cast<unsigned_char_type>(*i)-static_cast<unsigned_char_type>(u8'1'))<
				static_cast<unsigned_char_type>(9))
				{
					for(need_verify=true;i!=end&&static_cast<unsigned_char_type>(static_cast<unsigned_char_type>(*i)-u8'0')<static_cast<unsigned_char_type>(10);
					++i);
					if(i!=end&&*i==u8'.')
						throw_malformed_input();
				}
				else
					break;
			}
			break;
		}
		m10=m10*10+ch;
		++m10digits;
	}
	bool exp_negative{};
	std::ptrdiff_t e10{};
	{
		std::ptrdiff_t diff{i-begin};
		if(dot_it!=end)
			diff=dot_it-begin;
		if(diff<0)
			e10+=diff-m10digits+1;
		else
			e10+=diff-m10digits;
	}
	if(i!=end&&((*i==u8'e')|(*i==u8'E')))
	{
		++i;
		if(i==end)
		{
			if constexpr(end_test)
				throw_malformed_input();
			else
				return i;
		}
		if((*i==u8'+')|(exp_negative=(*i==u8'-')))[[likely]]
		{
			++i;
			if(i==end)
			{
				if constexpr(end_test)
					throw_malformed_input();
				else
					return i;
			}
		}
		if(static_cast<unsigned_char_type>(9)<static_cast<unsigned_char_type>(static_cast<unsigned_char_type>(*i)-static_cast<unsigned_char_type>(u8'0')))
			throw_malformed_input();
		i=std::find_if(i,end,zero_lmd);
		auto j{i};
		std::uint32_t ue10{};
		for(;j!=end;++j)
		{
			unsigned_char_type const ch(*j-u8'0');
			if(0x9<ch)[[unlikely]]
				break;
			ue10*=10;
			ue10+=ch;
		}
		if(6+j<i)[[unlikely]]
			throw_input_overflow_error();
		if(exp_negative)
			e10+=-static_cast<std::int32_t>(ue10);
		else
			e10+=ue10;
	}
	if constexpr(!end_test)
		if(i==end)
			return i;
	constexpr exponent_type real_bits{floating_trait::exponent_bits+floating_trait::mantissa_bits+1};
	auto fl{me10_to_me2_revamp<fp_type>(m10,e10)};

	if(need_verify)[[unlikely]]
	{
		auto cl{me10_to_me2_revamp<fp_type>(m10+1,e10)};
		if(fl==cl)
			t=bit_cast<fp_type>(((static_cast<mantissa_type>(signed_m)) << (real_bits-1))|fl);
		else
		{
#ifdef __cpp_exceptions
			throw fast_io_text_error("ryu to do with multiprecisions");
#else
			fast_terminate();
#endif
		}
	}
	else
	{
		t=bit_cast<fp_type>(((static_cast<mantissa_type>(signed_m)) << (real_bits-1)) | fl);
	}
	return i;
}
}

template<std::integral char_type,std::floating_point fp_type,bool end_test,std::contiguous_iterator Iter,typename T>
inline constexpr auto space_scan_reserve_define(io_reserve_type_t<char_type,fp_type,end_test>,Iter begin,Iter end,T& t)
{
	if constexpr(std::same_as<std::remove_cvref_t<fp_type>,long double>)
	{
		double d{};
		auto it{details::ryu::real_sto<end_test>(begin,end,d)};
		t=d;
		return it;
	}
	else
		return details::ryu::real_sto<end_test>(begin,end,t);
}


}