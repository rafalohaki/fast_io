#pragma once

namespace fast_io
{

namespace details
{

template<char8_t base,my_unsigned_integral T,std::contiguous_iterator Iter>
inline constexpr Iter scan_raw_unsigned_integer_impl(Iter i,Iter end,T& val,T& val_last)
{
	using unsigned_char_type = std::make_unsigned_t<std::iter_value_t<Iter>>;
	using unsigned_t = details::my_make_unsigned_t<std::remove_cvref_t<T>>;
	for(;i!=end;++i)
	{
		if constexpr(base <= 10)
		{
			unsigned_char_type ch(static_cast<unsigned_char_type>(*i)-static_cast<unsigned_char_type>(u8'0'));
			if(static_cast<unsigned_char_type>(base)<=ch)[[unlikely]]
				break;
			val=(val_last=val)*static_cast<unsigned_char_type>(base)+ch;
		}
		else
		{
			constexpr unsigned_char_type mns{base-10};
			unsigned_char_type const ch(static_cast<unsigned_char_type>(*i));
			unsigned_char_type ch1(ch-static_cast<unsigned_char_type>(u8'0'));
			unsigned_char_type ch2(ch-static_cast<unsigned_char_type>(u8'A'));
			unsigned_char_type ch3(ch-static_cast<unsigned_char_type>(u8'a'));
			if(ch2<mns)
				ch1=ch2+static_cast<unsigned_char_type>(10);
			else if(ch3<mns)
				ch1=ch3+static_cast<unsigned_char_type>(10);
			else if(static_cast<unsigned_char_type>(9)<ch1)[[unlikely]]
				break;
			val=(val_last=val)*static_cast<unsigned_char_type>(base)+ch1;
		}
	}
	return i;
}

template<char8_t base,my_integral T,std::contiguous_iterator Iter>
inline constexpr Iter scan_integer_impl(Iter begin,Iter end,T& t)
{
	using unsigned_char_type = std::make_unsigned_t<std::iter_value_t<Iter>>;
	using unsigned_t = details::my_make_unsigned_t<std::remove_cvref_t<T>>;
	unsigned_t val{},val_last{};
	if constexpr(my_unsigned_integral<T>)
	{
		for(;begin!=end&&*begin==u8'0';++begin);
		auto i{scan_raw_unsigned_integer_impl<base>(begin,end,val,val_last)};
		detect_overflow<base>(val,val_last,i-begin);
		t=val;
		return i;
	}
	else
	{
		bool sign{*begin==u8'-'};
		if(sign|(*begin==u8'+'))
			++begin;
		for(;begin!=end&&*begin==u8'0';++begin);
		auto i{scan_raw_unsigned_integer_impl<base>(begin,end,val,val_last)};
		detect_signed_overflow<base>(val,val_last,i-begin,sign);
		if(sign)
			t = 0-val;
		else
			t = val;
		return i;
	}
}

template<char8_t base,my_integral intg,buffer_output_stream output,character_input_stream input>
inline constexpr bool scn_int_res_impl(output& out,input& in)
{
	using namespace scan_transmitter;
	if constexpr(my_signed_integral<intg>)
		return scan_transmit(out,in,single_sign,until_none_digit<base>);
	else
		return scan_transmit(out,in,until_none_digit<base>);
}

}

template<details::my_integral intg,bool end_test,std::contiguous_iterator Iter,typename T>
inline constexpr auto space_scan_reserve_define(io_reserve_type_t<intg,end_test>,Iter begin,Iter end,T& t)
{
	return details::scan_integer_impl<10>(begin,end,t);
}

template<details::my_integral intg,buffer_output_stream output,character_input_stream input>
inline constexpr bool scan_reserve_transmit(io_reserve_type_t<intg>,output& out,input& in)
{
	return details::scn_int_res_impl<10,intg>(out,in);
}

template<details::my_integral intg>
inline constexpr std::size_t scan_reserve_size(io_reserve_type_t<intg>)
{
	if constexpr(details::my_unsigned_integral<intg>)
	{
		constexpr std::size_t sz{details::cal_max_int_size<details::my_make_unsigned_t<intg>>()+2};
		return sz;
	}
	else
	{
		constexpr std::size_t sz{details::cal_max_int_size<details::my_make_unsigned_t<intg>>()+3};
		return sz;
	}
}

template<char8_t base,bool uppercase,details::my_integral intg,bool end_test,std::contiguous_iterator Iter>
inline constexpr auto space_scan_reserve_define(io_reserve_type_t<manip::base_t<base,uppercase,intg>,end_test>,Iter begin,Iter end,auto t)
{
	return details::scan_integer_impl<base>(begin,end,t.reference);
}

template<char8_t base,bool uppercase,details::my_integral intg,buffer_output_stream output,character_input_stream input>
inline constexpr bool scan_reserve_transmit(io_reserve_type_t<manip::base_t<base,uppercase,intg>>,output& out,input& in)
{
	return details::scn_int_res_impl<base,intg>(out,in);
}

inline constexpr std::size_t scan_reserve_size(io_reserve_type_t<std::byte>)
{
	constexpr std::size_t sz{details::cal_max_int_size<details::my_make_unsigned_t<std::uint8_t>>()+2};
	return sz;
}

template<bool end_test,std::contiguous_iterator Iter,typename T>
inline constexpr auto space_scan_reserve_define(io_reserve_type_t<std::byte,end_test>,Iter begin,Iter end,T& t)
{
	char8_t val{};
	auto ret{details::scan_integer_impl<10>(begin,end,val)};
	t=std::byte(val);
	return ret;
}

template<buffer_output_stream output,character_input_stream input>
inline constexpr bool scan_reserve_transmit(io_reserve_type_t<std::byte>,output& out,input& in)
{
	return details::scn_int_res_impl<10,char8_t>(out,in);
}


template<char8_t base,bool uppercase,bool end_test,std::contiguous_iterator Iter>
inline constexpr auto space_scan_reserve_define(io_reserve_type_t<manip::base_t<base,uppercase,std::byte>,end_test>,Iter begin,Iter end,auto t)
{
	char8_t val{};
	auto ret{details::scan_integer_impl<base,char8_t>(begin,end)};
	t.reference=std::byte(val);
	return ret;
}

template<char8_t base,bool uppercase,buffer_output_stream output,character_input_stream input>
inline constexpr bool scan_reserve_transmit(io_reserve_type_t<manip::base_t<base,uppercase,std::byte>>,output& out,input& in)
{
	return details::scn_int_res_impl<base,char8_t>(out,in);
}

}