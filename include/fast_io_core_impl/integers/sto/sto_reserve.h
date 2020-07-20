#pragma once

namespace fast_io
{

namespace details
{

template<char8_t base,my_unsigned_integral T,std::contiguous_iterator Iter>
inline constexpr Iter scan_raw_unsigned_integer_impl(Iter begin,Iter end,T& val,T& val_last)
{
	using unsigned_char_type = std::make_unsigned_t<std::iter_value_t<Iter>>;
	using unsigned_t = details::my_make_unsigned_t<std::remove_cvref_t<T>>;
	auto i{begin};
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
			unsigned_char_type ch1(static_cast<unsigned_char_type>(*i)-static_cast<unsigned_char_type>(u8'0'));
			unsigned_char_type ch2(static_cast<unsigned_char_type>(*i)-static_cast<unsigned_char_type>(u8'A'));
			unsigned_char_type ch3(static_cast<unsigned_char_type>(*i)-static_cast<unsigned_char_type>(u8'a'));
			if(ch1<static_cast<unsigned_char_type>(10))
				val=(val_last=val)*static_cast<unsigned_char_type>(base)+ch1;
			else if(ch2<mns)
				val=(val_last=val)*static_cast<unsigned_char_type>(base)+(ch2+static_cast<unsigned_char_type>(10));
			else if(ch3<mns)
				val=(val_last=val)*static_cast<unsigned_char_type>(base)+(ch3+static_cast<unsigned_char_type>(10));
			else[[unlikely]]
				break;
		}
	}
	return i;
}

template<char8_t base,my_unsigned_integral T>
inline constexpr void detect_overflow(T const t1,T const t2,std::size_t length)
{
	constexpr std::size_t max_size{cal_max_int_size<T,base>()};
	constexpr std::remove_cvref_t<T> mx_val(std::numeric_limits<std::remove_cvref_t<T>>::max()/static_cast<std::remove_cvref_t<T>>(base));
	if(max_size<=length)[[unlikely]]
	{
		if((max_size<length)|(t1<base)|(mx_val<t2))[[unlikely]]
#ifdef __cpp_exceptions
			throw fast_io_text_error("unsigned overflow");
#else
			fast_terminate();
#endif
	}
}

template<char8_t base,my_unsigned_integral T>
inline constexpr void detect_signed_overflow(T const t1,T const t2,std::size_t length,bool sign)
{
	constexpr std::size_t max_size{cal_max_int_size<T,base>()};
	constexpr std::remove_cvref_t<T> mx_val(std::numeric_limits<std::remove_cvref_t<T>>::max()/static_cast<std::remove_cvref_t<T>>(base));
	if(max_size<=length)[[unlikely]]
	{
		if((max_size<length)|(t1<base)|(mx_val<t2)|(static_cast<T>(get_int_max_unsigned<T>()+sign)<t1))[[unlikely]]
#ifdef __cpp_exceptions
			throw fast_io_text_error("signed overflow");
#else
			fast_terminate();
#endif
	}
}

template<char8_t base,my_integral T,std::contiguous_iterator Iter>
inline constexpr Iter scan_integer_impl(Iter begin,Iter end,T& t)
{
	using unsigned_char_type = std::make_unsigned_t<std::iter_value_t<Iter>>;
	using unsigned_t = details::my_make_unsigned_t<std::remove_cvref_t<T>>;
	unsigned_t val{},val_last{};
	if constexpr(my_unsigned_integral<T>)
	{
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
		auto i{scan_raw_unsigned_integer_impl<base>(begin,end,val,val_last)};
		detect_signed_overflow<base>(val,val_last,i-begin,sign);
		if(sign)
			t=-static_cast<T>(val);
		else
			t=static_cast<T>(val);
		return i;
	}
}

template<char8_t base,my_integral intg,dynamic_buffer_output_stream output,character_input_stream input>
inline constexpr bool scn_int_res_impl(output& out,input& in)
{
	auto gen{igenerator(in)};
	auto bg{begin(gen)},ed{end(gen)};
	if(bg==ed)
		return false;
	using unsigned_char_type = std::make_unsigned_t<decltype(*bg)>;
	if constexpr(my_signed_integral<intg>)
	{
		if((*bg==u8'-')|(*bg==u8'+'))
		{
			put(out,*bg)
			++bg;
		}
	}
	for(;bg!=ed;++bg)
	{
		if constexpr(9<base)
		{
			if(static_cast<unsigned_char_type>(base)<=static_cast<unsigned_char_type>(static_cast<unsigned_char_type>(*bg)-static_cast<unsigned_char_type>(u8'0')))
				break;
		}
		else
		{
			constexpr unsigned_char_type mns{base-10};
			unsigned_char_type ch1(static_cast<unsigned_char_type>(*bg)-static_cast<unsigned_char_type>(u8'0'));
			unsigned_char_type ch2(static_cast<unsigned_char_type>(*bg)-static_cast<unsigned_char_type>(u8'A'));
			unsigned_char_type ch3(static_cast<unsigned_char_type>(*bg)-static_cast<unsigned_char_type>(u8'a'));
			if(static_cast<unsigned_char_type>(10)<=ch1&mns<=ch2&mns<=ch3)
				break;
		}
		put(out,*bg);
	}
	return true;
}

}

template<details::my_integral intg,std::contiguous_iterator Iter,typename T>
inline constexpr Iter space_scan_reserve_define(io_reserve_type_t<intg>,Iter begin,Iter end,T& t)
{
	return details::scan_integer_impl<10>(begin,end,t);
}

template<details::my_integral intg,dynamic_buffer_output_stream output,character_input_stream input>
inline constexpr bool scan_reserve_transmit(io_reserve_type_t<intg>,output& out,input& in)
{
	return details::scn_int_res_impl<10,intg>(out,in);
}



template<char8_t base,bool uppercase,details::my_integral intg,std::contiguous_iterator Iter>
inline constexpr Iter space_scan_reserve_define(io_reserve_type_t<manip::base_t<base,uppercase,intg>>,Iter begin,Iter end,auto t)
{
	return details::scan_integer_impl<base>(begin,end,t.reference);
}

template<char8_t base,bool uppercase,details::my_integral intg,dynamic_buffer_output_stream output,character_input_stream input>
inline constexpr bool scan_reserve_transmit(io_reserve_type_t<manip::base_t<base,uppercase,intg>>,output& out,input& in)
{
	return details::scn_int_res_impl<base,intg>(out,in);
}



}