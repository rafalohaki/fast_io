#pragma once

namespace fast_io
{

template<std::integral char_type,details::my_integral T>
inline constexpr std::size_t print_reserve_size(basic_lc_all<char_type> const* __restrict all,T) noexcept
{
	constexpr std::size_t static_size{print_reserve_size(io_reserve_type<char_type,T>)};
	constexpr std::size_t static_sizem1{static_size-1};
	return static_size+static_sizem1*all->numeric.thousands_sep.len;
}

namespace details
{

inline constexpr std::size_t grouping_char_lens(::fast_io::basic_io_scatter_t<std::size_t> grouping,std::size_t digits) noexcept
{
	std::size_t sum{};
	std::size_t i{};
	for(;i!=grouping.len;++i)
	{
		auto e{grouping.base[i]};
		if((e==0)|(digits-sum<=e))
			break;
		sum+=e;
	}
	if(i==grouping.len)
		return i+digits+(digits-sum-1)/grouping.base[i-1];
	return digits+i;
}

template<std::random_access_iterator Iter,details::my_unsigned_integral T>
constexpr Iter grouping_single_sep_impl(fast_io::basic_io_scatter_t<std::size_t> grouping,std::iter_value_t<Iter> sep,Iter iter,T t) noexcept
{
	using char_type = std::iter_value_t<Iter>;
	std::size_t i{};
	for(;i!=grouping.len;++i)
	{
		auto e{grouping.base[i]};
		if(e==0)
			break;
		for(std::size_t j{};j!=e;++j)
		{
			*--iter=t%10u;
			if constexpr(std::same_as<char_type,char>)
				*iter+='0';
			else if constexpr(std::same_as<char_type,wchar_t>)
				*iter+=L'0';
			else
				*iter+=u8'0';
			t/=10u;
			if(t==0)
				return iter;
		}
		*--iter=sep;
	}
	if(i!=grouping.len)
	{
		for(;;)
		{
			*--iter=t%10u;
			if constexpr(std::same_as<char_type,char>)
				*iter+='0';
			else if constexpr(std::same_as<char_type,wchar_t>)
				*iter+=L'0';
			else
				*iter+=u8'0';
			t/=10u;
			if(t==0)
				return iter;
		}
	}
	else
	{
		for(std::size_t e{grouping.base[i-1]};;*--iter=sep)
		{
			for(std::size_t j{};j!=e;++j)
			{
				*--iter=t%10u;
				if constexpr(std::same_as<char_type,char>)
					*iter+='0';
				else if constexpr(std::same_as<char_type,wchar_t>)
					*iter+=L'0';
				else
					*iter+=u8'0';
				t/=10u;
				if(t==0)
					return iter;
			}
		}
	}
}

template<std::random_access_iterator Iter,details::my_unsigned_integral T>
constexpr Iter grouping_mul_sep_impl(basic_lc_all<std::iter_value_t<Iter>> const* __restrict all,Iter iter,T t) noexcept
{
	using char_type = std::iter_value_t<Iter>;
	std::array<char_type,print_reserve_size(io_reserve_type<char_type,T>)*2-1> array;
	char_type sep;
	if constexpr(std::same_as<char_type,char>)
		sep=',';
	else if constexpr(std::same_as<char_type,wchar_t>)
		sep=L',';
	else
		sep=u8',';	
	auto i{grouping_single_sep_impl(all->numeric.grouping,sep,array.data()+array.size(),t)};
	for(auto ed{array.data()+array.size()};i!=ed;++i)
	{
		if(*i==sep)
			iter=non_overlapped_copy_n(all->numeric.thousands_sep.base,all->numeric.thousands_sep.len,iter);
		else
		{
			*iter=*i;
			++iter;
		}
	}
	return iter;
}

template<std::random_access_iterator Iter,details::my_unsigned_integral T>
constexpr Iter grouping_sep_impl(basic_lc_all<std::iter_value_t<Iter>> const* __restrict all,Iter iter,T t) noexcept
{
	if(all->numeric.thousands_sep.len==1)
	{
		auto sep{*all->numeric.thousands_sep.base};
		std::size_t digits{details::chars_len<10>(t)};
		auto grouping{all->numeric.grouping};
		std::size_t const len{grouping_char_lens(grouping,digits)};
		grouping_single_sep_impl(grouping,sep,iter+len,t);
		return iter+len;
	}
	else
		return grouping_mul_sep_impl(all,iter,t);
}

template<std::random_access_iterator Iter,details::my_integral T>
constexpr Iter print_lc_grouping_unhappy_path_impl(basic_lc_all<std::iter_value_t<Iter>> const* __restrict all,Iter iter,T t) noexcept
{
	using int_type = T;
	using char_type = std::iter_value_t<Iter>;
	using unsigned_type = details::my_make_unsigned_t<std::remove_cvref_t<int_type>>;
	if constexpr(my_signed_integral<int_type>)
	{
		unsigned_type abs_value(t);
		bool const negative(t<0);
		if(negative)
		{
			abs_value = 0 - abs_value;
			if constexpr(std::same_as<char,char_type>)
				*iter='-';
			else if constexpr(std::same_as<wchar_t,char_type>)
				*iter=L'-';
			else
				*iter=u8'-';
			++iter;
		}
		return grouping_sep_impl(all,iter,abs_value);
	}
	else
	{
		return grouping_sep_impl(all,iter,t);
	}
}

template<std::random_access_iterator Iter,my_unsigned_integral T>
requires (sizeof(T)>1)
constexpr void output_unsigned_with_3_seperator_len(std::iter_value_t<Iter> seperator_ch,Iter iter,T value) noexcept
{
	using char_type = std::iter_value_t<Iter>;
#ifdef FAST_IO_OPTIMIZE_TIME
	constexpr auto table3(jiaendu::static_tables<char_type>::table3.data());
#elif !defined(FAST_IO_OPTIMIZE_SIZE)
	constexpr auto table(get_shared_inline_constexpr_base_table<char_type,10,false>().data());
#endif
	for(;value>=1000u;*--iter=seperator_ch)
	{
#ifdef FAST_IO_OPTIMIZE_TIME
		auto low3digits{value%1000u};
		value/=1000u;
		non_overlapped_copy_n(table3[low3digits].data(),3,iter-=3);
#elif defined(FAST_IO_OPTIMIZE_SIZE)
		for(std::size_t i{};i!=3;++i)
		{
			auto remained{value%10u};
			value/=10;
			if constexpr(std::same_as<char_type,char>)
				*--iter=static_cast<char_type>(remained+'0');
			else if constexpr(std::same_as<char_type,wchar_t>)
				*--iter=static_cast<char_type>(remained+L'0');
			else
				*--iter=static_cast<char_type>(remained+u8'0');
		}
#else
		auto low3digits{value%1000u};
		value/=1000u;
		auto low2digits{low3digits%100u};
		auto highdigit{low3digits/100u};
		non_overlapped_copy_n(table[low2digits].data(),2,iter-=2);
		if constexpr(std::same_as<char_type,char>)
			*--iter=static_cast<char_type>(highdigit+'0');
		else if constexpr(std::same_as<char_type,wchar_t>)
			*--iter=static_cast<char_type>(highdigit+L'0');
		else
			*--iter=static_cast<char_type>(highdigit+u8'0');
#endif
	}
#ifdef FAST_IO_OPTIMIZE_TIME
	if(value>=100u)
	{
		non_overlapped_copy_n(table3[value].data(),3,iter-3);
	}
	else if(value>=10u)
	{
		non_overlapped_copy_n(jiaendu::static_tables<char_type>::table2[value].data(),2,iter-2);
	}
	else
	{
		if constexpr(std::same_as<char_type,char>)
			*--iter=static_cast<char_type>(value+'0');
		else if constexpr(std::same_as<char_type,wchar_t>)
			*--iter=static_cast<char_type>(value+L'0');
		else
			*--iter=static_cast<char_type>(value+u8'0');
	}
#elif defined(FAST_IO_OPTIMIZE_SIZE)
	for(;value;)
	{
		auto remained{value%10u};
		value/=10u;
		if constexpr(std::same_as<char_type,char>)
			*--iter=static_cast<char_type>(remained+'0');
		else if constexpr(std::same_as<char_type,wchar_t>)
			*--iter=static_cast<char_type>(remained+L'0');
		else
			*--iter=static_cast<char_type>(remained+u8'0');
	}
#else
	if(value>=100u)
	{
		auto low2digits{value%100u};
		auto highdigit{value/100u};
		non_overlapped_copy_n(table[low2digits].data(),2,iter-=2);
		if constexpr(std::same_as<char_type,char>)
			*--iter=static_cast<char_type>(highdigit+'0');
		else if constexpr(std::same_as<char_type,wchar_t>)
			*--iter=static_cast<char_type>(highdigit+L'0');
		else
			*--iter=static_cast<char_type>(highdigit+u8'0');
	}
	else if(value>=10u)
		non_overlapped_copy_n(table[value].data(),2,iter-=2);
	else
	{
		if constexpr(std::same_as<char_type,char>)
			*--iter=static_cast<char_type>(value+'0');
		else if constexpr(std::same_as<char_type,wchar_t>)
			*--iter=static_cast<char_type>(value+L'0');
		else
			*--iter=static_cast<char_type>(value+u8'0');
	}
#endif
}

template<std::random_access_iterator Iter,my_unsigned_integral T>
requires (sizeof(T)>1)
constexpr Iter output_unsigned_with_3_seperator(std::iter_value_t<Iter> seperator_ch,Iter iter,T value) noexcept
{
	std::uint32_t const len{chars_len_3_sep<10>(value)};
	auto result{iter+len};
	if constexpr(sizeof(T)<=sizeof(std::size_t))
		output_unsigned_with_3_seperator_len(seperator_ch,result,static_cast<std::size_t>(value));
	else
		output_unsigned_with_3_seperator_len(seperator_ch,result,value);
	return result;
}

template<std::random_access_iterator Iter,details::my_integral int_type>
inline constexpr Iter print_lc_grouping_3_path_impl(std::iter_value_t<Iter> seperator,Iter iter,int_type t) noexcept
{
	using char_type = std::iter_value_t<Iter>;
	using unsigned_type = details::my_make_unsigned_t<std::remove_cvref_t<int_type>>;
	if constexpr(my_signed_integral<int_type>)
	{
		unsigned_type abs_value(t);
		bool const negative(t<0);
		if(negative)
		{
			abs_value = 0 - abs_value;
			if constexpr(std::same_as<char,char_type>)
				*iter='-';
			else if constexpr(std::same_as<wchar_t,char_type>)
				*iter=L'-';
			else
				*iter=u8'-';
			++iter;
		}
		return print_lc_grouping_3_path_impl(seperator,iter,abs_value);
	}
	else
	{
		if constexpr(sizeof(int_type)==1) //3 seperator will not affect this
			return print_reserve_define(io_reserve_type<char_type,unsigned_type>,iter,t);
		else
			return output_unsigned_with_3_seperator(seperator,iter,static_cast<unsigned_type>(t));
	}
}


}

template<std::random_access_iterator Iter,details::my_integral T>
inline constexpr Iter print_reserve_define(basic_lc_all<std::iter_value_t<Iter>> const* __restrict all,Iter iter,T t) noexcept
{
	using char_type = std::iter_value_t<Iter>;
//To finish grouping
	if((all->numeric.grouping.len==0)|(all->numeric.thousands_sep.len==0))
		return print_reserve_define(io_reserve_type<char_type,std::remove_cvref_t<T>>,iter,t);
	else if((all->numeric.grouping.len==1&&*all->numeric.grouping.base==3)&(all->numeric.thousands_sep.len==1))
		return details::print_lc_grouping_3_path_impl(*all->numeric.thousands_sep.base,iter,t);
	else
		return details::print_lc_grouping_unhappy_path_impl(all,iter,t);
}

namespace details
{
template<std::integral char_type>
inline constexpr std::size_t print_reserve_size_grouping_timestamp_impl(basic_lc_all<char_type> const* __restrict all)
{
	constexpr std::size_t static_size{print_reserve_size(io_reserve_type<char_type,intiso_t>)};
	constexpr std::size_t static_sizem1{static_size-1};
	return static_size+static_sizem1*all->numeric.thousands_sep.len+all->numeric.decimal_point.len+std::numeric_limits<uintiso_t>::digits10;
}

template<std::random_access_iterator Iter>
inline constexpr Iter print_reserve_define_grouping_timestamp_impl(basic_lc_all<std::iter_value_t<Iter>> const* __restrict all,Iter iter,unix_timestamp timestamp)
{
	iter=print_reserve_define(all,iter,timestamp.seconds);
	if(timestamp.subseconds)
	{
		if(all->numeric.decimal_point.len==1)
		{
			*iter=all->numeric.decimal_point.base[0];
			++iter;
		}
		else
			iter=non_overlapped_copy_n(all->numeric.decimal_point.base,all->numeric.decimal_point.len,iter);
		iter=output_iso8601_subseconds_main(iter,timestamp.subseconds);
	}
	return iter;
}
}

template<std::integral char_type,intiso_t off_to_epoch>
inline constexpr std::size_t print_reserve_size(basic_lc_all<char_type> const* __restrict all,basic_timestamp<off_to_epoch>) noexcept
{
	return details::print_reserve_size_grouping_timestamp_impl(all);
}

template<std::random_access_iterator Iter,intiso_t off_to_epoch>
inline constexpr Iter print_reserve_define(basic_lc_all<std::iter_value_t<Iter>> const* __restrict all,Iter iter,basic_timestamp<off_to_epoch> ts) noexcept
{
	if constexpr(off_to_epoch==0)
	{
		return details::print_reserve_define_grouping_timestamp_impl(all,iter,ts);
	}
	else
	{
		return details::print_reserve_define_grouping_timestamp_impl(all,iter,{ts.seconds,ts.subseconds});
	}
}


}