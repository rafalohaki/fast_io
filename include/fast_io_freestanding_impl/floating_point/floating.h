#pragma once
#include"jiaendu_fp.h"
#include"floating_integer_part.h"
#include"ryu/ryu.h"
//#include"grisu_exact/grisu_exact.h"
//#include"grisu_exact/grisu_exact_api.h"

namespace fast_io
{

namespace details
{

template<std::floating_point fp_type,manip::floating_formats fm>
constexpr std::size_t cal_floating_len()
{
	if constexpr(std::same_as<std::remove_cvref_t<fp_type>,long double>&&8<sizeof(long double))
	{
		if constexpr(fm==manip::floating_formats::general||fm==manip::floating_formats::scientific)
			return 60;
		else if constexpr(fm==manip::floating_formats::fixed)
			return 5000;//??Is that enough? To verify
	}
	else
	{
	if constexpr(fm==manip::floating_formats::general||fm==manip::floating_formats::scientific)
		return 30;
	else if constexpr(fm==manip::floating_formats::fixed)
		return 350;
	}
}
}
/*
template<character_input_stream input,std::floating_point T>
inline constexpr void space_scan_define(input& in,T &t)
{
	auto igen{igenerator(in)};
	if constexpr(std::same_as<std::remove_cvref_t<T>,long double>)
		t=static_cast<std::remove_cvref_t<T>>(details::ryu::input_floating<u8'.',double>(begin(igen),end(igen)));
	else
		t=details::ryu::input_floating<u8'.',std::remove_cvref_t<T>>(begin(igen),end(igen));
}

template<char32_t dec,character_input_stream input,std::floating_point T>
inline constexpr void space_scan_define(input& in,manip::decimal_point<T&,dec> t)
{
	auto igen{igenerator(in)};
	if constexpr(std::same_as<std::remove_cvref_t<T>,long double>)
		t.value=static_cast<std::remove_cvref_t<T>>(details::ryu::input_floating<dec,double>(begin(igen),end(igen)));
	else
		t.value=details::ryu::input_floating<dec,std::remove_cvref_t<T>>(begin(igen),end(igen));
}
*/
template<manip::floating_formats fm,bool uppercase,std::floating_point T,char32_t dec>
inline constexpr std::size_t print_reserve_size
	(io_reserve_type_t<manip::decimal_point<manip::floating_manip_precision<fm,uppercase,T const>,dec>>)
{
	if constexpr(fm==manip::floating_formats::general||fm==manip::floating_formats::fixed)
		return 2048;
	else if constexpr(fm==manip::floating_formats::scientific)
		return 1024;
}

template<std::random_access_iterator raiter,manip::floating_formats fm,std::floating_point T,bool uppercase,char32_t dec,typename U>
requires (dec<std::numeric_limits<std::iter_value_t<raiter>>::max())
inline raiter print_reserve_define(io_reserve_type_t<manip::decimal_point<manip::floating_manip_precision<fm,uppercase,T const>,dec>>,raiter start,U a)
{
	if constexpr(fm==manip::floating_formats::fixed)
	{
		if(1024<a.value.precision)
#ifdef __cpp_exceptions
			throw fast_io_text_error("precision too large");
#else
			fast_terminate();
#endif
		return details::ryu::output_fixed<dec,false,uppercase>(start,static_cast<double>(a.value.reference),a.value.precision);
	}
	else if constexpr(fm==manip::floating_formats::scientific)
	{
		if(512<a.value.precision)
#ifdef __cpp_exceptions
			throw fast_io_text_error("precision too large");
#else
			fast_terminate();
#endif
		return details::ryu::output_fixed<dec,true,uppercase>(start,static_cast<double>(a.value.reference),a.value.precision);
	}
	else if constexpr(fm==manip::floating_formats::general)
	{
		if(1024<a.value.precision)
#ifdef __cpp_exceptions
			throw fast_io_text_error("precision too large");
#else
			fast_terminate();
#endif
		auto fixed_iter{details::ryu::output_fixed<dec,false,uppercase>(start,static_cast<double>(a.value.reference),a.value.precision)};
		std::array<std::iter_value_t<raiter>,512> scientific;
		auto scientific_it{details::ryu::output_fixed<dec,true,uppercase>(scientific.data(),static_cast<double>(a.value.reference),a.value.precision)};
		if(scientific_it-scientific.data()<fixed_iter-start)[[unlikely]]
		{
			details::my_copy(scientific.data(),scientific_it,start);
			return start+(scientific_it-scientific.data());
		}
		return fixed_iter;
	}
}

template<std::floating_point T>
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<T>)
{
	return details::cal_floating_len<T,manip::floating_formats::general>();
}

template<std::random_access_iterator raiter,std::floating_point T,typename U>
inline raiter print_reserve_define(io_reserve_type_t<T>,raiter start,U a)
{
	if constexpr(std::same_as<std::remove_cvref_t<T>,long double>&&sizeof(long double)!=16)
		return details::ryu::output_shortest<false,0,true>(details::compile_time_floating_v<u8'.'>,start,static_cast<double>(a));	
	else
		return details::ryu::output_shortest<false,0,true>(details::compile_time_floating_v<u8'.'>,start,a);
}

template<manip::floating_formats fm,bool uppercase,std::floating_point T,char32_t dec>
inline constexpr std::size_t print_reserve_size
	(io_reserve_type_t<manip::decimal_point<manip::floating_manip<fm,uppercase,T const>,dec>>)
{
	return details::cal_floating_len<T,fm>();
}

template<std::random_access_iterator raiter,manip::floating_formats fm,std::floating_point T,bool uppercase,char32_t dec,typename U>
requires (dec<std::numeric_limits<std::iter_value_t<raiter>>::max())
inline raiter print_reserve_define(io_reserve_type_t<manip::decimal_point<manip::floating_manip<fm,uppercase,T const>,dec>>,raiter start,U a)
{
	if constexpr(std::same_as<std::remove_cvref_t<T>,long double>&&sizeof(long double)!=16)
	{
	if constexpr(fm==manip::floating_formats::general)
		return details::ryu::output_shortest<uppercase,0,true>(details::compile_time_floating_v<dec>,start,static_cast<double>(a.value.reference));
	else if constexpr(fm==manip::floating_formats::fixed)
		return details::ryu::output_shortest<false,1,true>(details::compile_time_floating_v<dec>,start,static_cast<double>(a.value.reference));
	else if constexpr(fm==manip::floating_formats::scientific)
		return details::ryu::output_shortest<uppercase,2,true>(details::compile_time_floating_v<dec>,start,static_cast<double>(a.value.reference));
	}
	else
	{
	if constexpr(fm==manip::floating_formats::general)
		return details::ryu::output_shortest<uppercase,0,true>(details::compile_time_floating_v<dec>,start,a.value.reference);
	else if constexpr(fm==manip::floating_formats::fixed)
		return details::ryu::output_shortest<false,1,true>(details::compile_time_floating_v<dec>,start,a.value.reference);
	else if constexpr(fm==manip::floating_formats::scientific)
		return details::ryu::output_shortest<uppercase,2,true>(details::compile_time_floating_v<dec>,start,a.value.reference);
	}
}

template<std::floating_point T,char32_t dec>
inline constexpr std::size_t print_reserve_size
	(io_reserve_type_t<manip::decimal_point<T&,dec>>)
{
	return details::cal_floating_len<T,manip::floating_formats::general>();
}

template<std::random_access_iterator raiter,std::floating_point T,char32_t dec,typename U>
requires (dec<std::numeric_limits<std::iter_value_t<raiter>>::max())
inline raiter print_reserve_define(io_reserve_type_t<manip::decimal_point<T&,dec>>,raiter start,U a)
{
	if constexpr(std::same_as<std::remove_cvref_t<T>,long double>&&sizeof(long double)!=16)
	{
	return details::ryu::output_shortest<false,0,true>(details::compile_time_floating_v<dec>,start,static_cast<double>(a.value));
	}
	else
	{
	return details::ryu::output_shortest<false,0,true>(details::compile_time_floating_v<dec>,start,a.value);
	}
}

}
