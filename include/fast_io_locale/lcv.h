#pragma once

namespace fast_io
{
namespace manip
{
template<std::integral ch_type,typename T>
struct lcv
{
	using manip_tag = manip_tag_t;
	basic_lconv_storage<ch_type> const& storage;
	T reference;
};
}

namespace details
{

template<std::integral char_type>
inline constexpr auto process_lcv_grouping(std::basic_string_view<char_type> const& grouping,
	char_type* str_first,char_type* str_last,
	char_type* buffer_iter,char_type seperator)
{
	using unsigned_char_type =my_make_unsigned_t<char_type>;
	auto gp_cendm1{grouping.cend()-1};
	for(auto iter(grouping.cbegin());;)
	{
		unsigned_char_type gp{static_cast<unsigned_char_type>(*iter)};
		if(gp)[[likely]]
		{
			if(str_last-gp<str_first)[[unlikely]]
				break;
			non_overlapped_copy_n(str_last-=gp,gp,buffer_iter-=gp);
			*--buffer_iter=seperator;
		}
		if(iter!=gp_cendm1)[[likely]]
			++iter;
	}
	if(std::size_t const sz(str_last-str_first);sz)
		non_overlapped_copy_n(str_first,sz,buffer_iter-=sz);
	else
		++buffer_iter;
	return buffer_iter;
}

template<typename Func>
constexpr std::size_t cal_lcv_size_base(Func func)
{
	constexpr std::size_t sz{(func()+2)<<1};
	return sz;
}

template<my_integral int_type,char8_t base>
constexpr std::size_t cal_lcv_integer_output_size()
{
	return cal_lcv_size_base([]()
	{
		return cal_max_int_size<my_make_unsigned_t<int_type>,base>();
	});
}

template<std::floating_point fp_type,manip::floating_formats fm>
constexpr std::size_t cal_lcv_floating_len()
{
	return cal_lcv_size_base([]()
	{
		return cal_floating_len<fp_type,fm>();
	});
}

template<std::integral char_type,char8_t base,bool uppercase,std::contiguous_iterator caiter,typename T,my_integral int_type,typename U>
inline constexpr auto process_lcv_integer_output_cold(caiter outiter,T const& storage,int_type value,U grouping)
{
	std::array<char_type,cal_max_int_size<my_make_unsigned_t<int_type>,base>()> str;
	auto str_iter(details::process_integer_output<base,uppercase,true>(str.data(),value));
	constexpr std::size_t buffer_size{cal_lcv_integer_output_size<int_type,base>()};
	auto buffer_iter{process_lcv_grouping(grouping,str.data(),str_iter,outiter+buffer_size,storage.thousands_sep)};
	if constexpr(my_signed_integral<int_type>)
	{
		if(value<0)
			*--buffer_iter=u8'-';
	}
	return details::my_copy(buffer_iter,outiter+buffer_size,outiter);
}

template<std::integral char_type,char8_t base,bool uppercase,std::contiguous_iterator caiter,typename T,my_integral int_type>
inline constexpr auto process_lcv_integer_output(caiter outiter,T const& storage,int_type value)
{
	auto grouping{storage.grouping()};
	if(grouping.empty())[[likely]]
		return process_integer_output<base,uppercase>(outiter,value);
	return process_lcv_integer_output_cold<char_type,base,uppercase>(outiter,storage,value,grouping);
}

template<std::size_t buffer_size,std::integral char_type,std::contiguous_iterator caiter,typename T,typename Func>
inline constexpr auto process_lcv_floating_output(caiter outiter,T const& storage,Func fun)
{
	auto grouping{storage.grouping()};
	if(grouping.empty())
		return fun(details::compile_time_floating_value<true,char_type,0>{storage.decimal_point},outiter);
	std::array<char_type,buffer_size> array;
	auto buffer_end{fun(details::compile_time_floating_v<u8'.'>,array.data())};
	auto buffer_start{array.data()};
	bool const negative{*buffer_start==u8'-'};
	if(negative)
		++buffer_start;
	auto dcm_piter{std::find(buffer_start,buffer_end,u8'.')};
	if(dcm_piter==buffer_end)
	{
		auto e_piter{std::find(buffer_start,buffer_end,u8'e')};
		if(e_piter!=buffer_end)
			dcm_piter=e_piter;
	}
	else
		*dcm_piter=storage.decimal_point;
	constexpr std::size_t preallocated_size(cal_lcv_size_base([]
	{
		return buffer_size;
	}));
	auto buffer_iter{process_lcv_grouping(grouping,buffer_start,dcm_piter,outiter+preallocated_size,storage.thousands_sep)};
	if(negative)
		*--buffer_iter=u8'-';
	return details::my_copy(dcm_piter,buffer_end,details::my_copy(buffer_iter,outiter+preallocated_size,outiter));
}

}
template<std::integral ch_type,typename T>
inline constexpr manip::lcv<ch_type,T&> lcv(basic_lconv_storage<ch_type> const& t,T&& f){return {t,f};}

template<std::integral ch_type,details::my_integral int_type>
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<manip::lcv<ch_type,int_type&>>)
{
	return details::cal_lcv_integer_output_size<int_type,10>();
}

template<std::contiguous_iterator caiter,std::integral char_type,details::my_integral int_type>
inline constexpr caiter print_reserve_define(io_reserve_type_t<manip::lcv<char_type,int_type&>>,caiter outiter,auto ref)
{
	return details::process_lcv_integer_output<char_type,10,false>(outiter,ref.storage,ref.reference);
}

template<std::integral ch_type,char8_t base,bool uppercase,details::my_integral int_type>
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<manip::lcv<ch_type,manip::base_t<base,uppercase,int_type>&>>)
{
	return details::cal_lcv_integer_output_size<int_type,base>();
}

template<std::contiguous_iterator caiter,std::integral char_type,char8_t base,bool uppercase,details::my_integral int_type>
inline constexpr caiter print_reserve_define(io_reserve_type_t<manip::lcv<char_type,manip::base_t<base,uppercase,int_type>&>>,caiter outiter,auto ref)
{
	return details::process_lcv_integer_output<char_type,base,uppercase>(outiter,ref.storage,ref.reference.reference);
}


template<std::integral char_type,std::floating_point fp_type>
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<manip::lcv<char_type,fp_type&>>)
{
	return details::cal_lcv_floating_len<fp_type,manip::floating_formats::general>();
}

template<std::contiguous_iterator caiter,std::integral char_type,std::floating_point fp_type>
inline constexpr caiter print_reserve_define(io_reserve_type_t<manip::lcv<char_type,fp_type&>>,caiter outiter,auto ref)
{
	return details::process_lcv_floating_output<details::cal_floating_len<fp_type,manip::floating_formats::general>(),char_type>(outiter,ref.storage,[value=ref.reference](auto dec,auto iter)
	{
		if constexpr(std::same_as<std::remove_cvref_t<fp_type>,long double>&&sizeof(long double)!=16)
			return details::ryu::output_shortest<false,0,true>(dec,iter,static_cast<double>(value));
		else
			return details::ryu::output_shortest<false,0,true>(dec,iter,value);
	});
}

}