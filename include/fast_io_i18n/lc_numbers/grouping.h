#pragma once

namespace fast_io
{

template<std::integral char_type,details::my_integral T>
inline constexpr std::size_t print_reserve_size(basic_lc_all<char_type> const* __restrict all,T t) noexcept
{
	constexpr std::size_t static_size{print_reserve_size(io_reserve_type<char_type,T>)};
	constexpr std::size_t static_sizem1{static_size-1};
	return static_size+static_sizem1*all->numeric.thousands_sep.len;
}

namespace details
{

#if 0

template<std::random_access_iterator Iter,details::my_integral T>
constexpr Iter print_lc_grouping_unhappy_path_impl(basic_lc_all<std::iter_value_t<Iter>> const* __restrict all,Iter iter,T t) noexcept
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
		return print_lc_grouping_unhappy_path_impl(seperator,iter,abs_value);
	}
	else
	{
		return print_lc_grouping_unhappy_unsigned_path_impl(seperator,iter,static_cast<unsigned_type>(t));
	}

	if(all->numeric.thousands_sep.len==1)
	{

	}
	else
	{

	}
	return iter;
}
#endif

template<std::random_access_iterator Iter,my_unsigned_integral T>
requires (sizeof(T)>1)
constexpr void output_unsigned_with_3_seperator_len(std::iter_value_t<Iter> seperator_ch,Iter iter,T value) noexcept
{
	using char_type = std::iter_value_t<Iter>;
#ifdef FAST_IO_OPTIMIZE_TIME
	constexpr auto table3(jiaendu::static_tables<char_type>::table3.data());
#elif !defined(FAST_IO_OPTIMIZE_SIZE)
	constexpr auto table(shared_static_base_table<char_type,10,false>::table.data());
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
				*--iter=remained+'0';
			else if constexpr(std::same_as<char_type,wchar_t>)
				*--iter=remained+L'0';
			else
				*--iter=remained+u8'0';
		}
#else
		auto low3digits{value%1000u};
		value/=1000u;
		auto low2digits{low3digits%100u};
		auto highdigit{low3digits/100u};
		non_overlapped_copy_n(table[low2digits].data(),2,iter-=2);
		if constexpr(std::same_as<char_type,char>)
			*--iter=highdigit+'0';
		else if constexpr(std::same_as<char_type,wchar_t>)
			*--iter=highdigit+L'0';
		else
			*--iter=highdigit+u8'0';
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
			*--iter=value+'0';
		else if constexpr(std::same_as<char_type,wchar_t>)
			*--iter=value+L'0';
		else
			*--iter=value+u8'0';
	}
#elif defined(FAST_IO_OPTIMIZE_SIZE)
	for(;value;)
	{
		auto remained{value%10u};
		value/=10u;
		if constexpr(std::same_as<char_type,char>)
			*--iter=remained+'0';
		else if constexpr(std::same_as<char_type,wchar_t>)
			*--iter=remained+L'0';
		else
			*--iter=remained+u8'0';
	}
#else
	if(value>=100u)
	{
		auto low2digits{value%100u};
		auto highdigit{value/100u};
		non_overlapped_copy_n(table[low2digits].data(),2,iter-=2);
		if constexpr(std::same_as<char_type,char>)
			*--iter=highdigit+'0';
		else if constexpr(std::same_as<char_type,wchar_t>)
			*--iter=highdigit+L'0';
		else
			*--iter=highdigit+u8'0';
	}
	else if(value>=10u)
		non_overlapped_copy_n(table[value].data(),2,iter-=2);
	else
	{
		if constexpr(std::same_as<char_type,char>)
			*--iter=value+'0';
		else if constexpr(std::same_as<char_type,wchar_t>)
			*--iter=value+L'0';
		else
			*--iter=value+u8'0';
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
inline constexpr Iter print_reserve_define(basic_lc_all<std::iter_value_t<Iter>> const* __restrict all,Iter iter,T t,std::size_t) noexcept
{
	using char_type = std::iter_value_t<Iter>;
#if 0
//To finish grouping
	if((all->numeric.grouping.len==0)|(all->numeric.thousands_sep.len==0))
		return print_reserve_define(io_reserve_type<char_type,std::remove_cvref_t<T>>,iter,t);
	else if((all->numeric.grouping.len==1&&*all->numeric.grouping.base==3)&(all->numeric.thousands_sep.len==1))
		return details::print_lc_grouping_3_path_impl(*all->numeric.thousands_sep.base,iter,t);
	else
		return details::print_lc_grouping_unhappy_path_impl(all,iter,t);
#else
	if((all->numeric.grouping.len==1&&*all->numeric.grouping.base==3)&(all->numeric.thousands_sep.len==1))
		return details::print_lc_grouping_3_path_impl(*all->numeric.thousands_sep.base,iter,t);
	else
		return print_reserve_define(io_reserve_type<char_type,std::remove_cvref_t<T>>,iter,t);
#endif
}

}