#pragma once

namespace fast_io
{

namespace details
{
template<std::integral char_type,manipulators::scalar_flags flags,typename T>
inline constexpr std::size_t lc_print_reserve_size_int_cal(basic_lc_all<char_type> const* __restrict all) noexcept
{
	constexpr std::size_t static_size{print_integer_reserved_size_cache<flags.base,flags.showbase,flags.showpos,std::remove_cv_t<T>>};
	constexpr std::size_t static_sizem1{static_size-1};
	return static_size+static_sizem1*all->numeric.thousands_sep.len;
}
#if 0
inline constexpr std::size_t grouping_char_lens(std::size_t const* __restrict grouping_base,std::size_t grouping_len,std::size_t digits) noexcept
{
	std::size_t sum{};
	std::size_t i{};
	for(;i!=grouping_len;++i)
	{
		auto e{grouping_base[i]};
		if((e==0)|(digits-sum<=e))
			break;
		sum+=e;
	}
	if(i==grouping_len)
		return i+digits+(digits-sum-1)/grouping_base[i-1];
	return digits+i;
}
#endif
template<std::integral char_type,std::size_t base,bool uppercase,my_unsigned_integral T>
inline constexpr char_type to_char_single_digit(T t) noexcept
{
	if constexpr(base<=10)
	{
		return static_cast<char_type>(t+char_literal_v<u8'0',char_type>);
	}
	else
	{
		constexpr T ten{static_cast<T>(10u)};
		if(t<ten)
			return static_cast<char_type>(t+char_literal_v<u8'0',char_type>);
		else
		{
			if constexpr(is_ebcdic<char_type>)
			{
				if constexpr(base<=19)
				{
					if constexpr(uppercase)
						return static_cast<char_type>((0xC1-10)+t);
					else
						return static_cast<char_type>((0x81-10)+t);
				}
				else if constexpr(base<=28)
				{
					if(t<19u)
					{
						if constexpr(uppercase)
							return static_cast<char_type>((0xC1-10u)+t);
						else
							return static_cast<char_type>((0x81-10u)+t);
					}
					else
					{
						if constexpr(uppercase)
							return static_cast<char_type>((0xD1-19u)+t);
						else
							return static_cast<char_type>((0x91-19u)+t);
					}
				}
				else
				{
					if(t<19u)
					{
						if constexpr(uppercase)
							return static_cast<char_type>((0xC1-10u)+t);
						else
							return static_cast<char_type>((0x81-10u)+t);
					}
					else if(t<28u)
					{
						if constexpr(uppercase)
							return static_cast<char_type>((0xD1-19u)+t);
						else
							return static_cast<char_type>((0x91-19u)+t);
					}
					else
					{
						if constexpr(uppercase)
							return static_cast<char_type>((0xE2-28u)+t);
						else
							return static_cast<char_type>((0xA2-28u)+t);
					}
				}
			}
			else
			{
				t-=ten;
				return static_cast<char_type>(t+char_literal_v<(uppercase?u8'A':u8'a'),char_type>);
			}
		}
	}
}

template<bool full,std::size_t base,bool uppercase,::fast_io::freestanding::random_access_iterator Iter,my_unsigned_integral T>
constexpr Iter lc_grouping_single_sep_impl(basic_io_scatter_t<std::size_t> const& grouping,Iter iter,T t) noexcept
{
	std::size_t const* grouping_base{grouping.base};
	std::size_t const grouping_len{grouping.len};
	using char_type = ::fast_io::freestanding::iter_value_t<Iter>;
	std::size_t i{};
	constexpr T c0{static_cast<T>(base)};
	constexpr std::size_t full_len{cal_max_int_size<T,base>()};
	std::size_t full_i{};
	for(;i!=grouping_len;++i)
	{
		auto e{grouping_base[i]};
		if(e==0)
			break;
		for(std::size_t j{};j!=e;++j)
		{
			*--iter=to_char_single_digit<char_type,base,uppercase>(static_cast<T>(t%c0));
			t/=c0;
			if constexpr(full)
			{
				if((++full_i)==full_len)
					return iter;
			}
			else
			{
				if(t==0)
					return iter;
			}
		}
		*--iter=char_literal_v<u8',',char_type>;
	}
	if(i!=grouping_len)
	{
		for(;;)
		{
			*--iter=to_char_single_digit<char_type,base,uppercase>(static_cast<T>(t%c0));
			t/=c0;
			if constexpr(full)
			{
				if((++full_i)==full_len)
					return iter;
			}
			else
			{
				if(t==0)
					return iter;
			}
		}
	}
	else
	{
		for(std::size_t e{grouping_base[i-1]};;*--iter=char_literal_v<u8',',char_type>)
		{
			for(std::size_t j{};j!=e;++j)
			{
				*--iter=to_char_single_digit<char_type,base,uppercase>(static_cast<T>(t%c0));
				t/=c0;
				if constexpr(full)
				{
					if((++full_i)==full_len)
						return iter;
				}
				else
				{
					if(t==0)
						return iter;
				}
			}
		}
	}
}

template<std::integral char_type,::fast_io::freestanding::random_access_iterator Iter>
constexpr Iter grouping_mul_sep_print_sep_impl(basic_io_scatter_t<char_type> const& thousands_sep,
	char_type const* first,char_type const* last,
	Iter outit) noexcept
{
	if(thousands_sep.len==1)[[likely]]
	{
		auto const sep_ch{*thousands_sep.base};
		if(sep_ch==char_literal_v<u8',',char_type>)
		{
			return non_overlapped_copy(first,last,outit);
		}
		else
		{
			for(;first!=last;++first)
			{
				if(*first==char_literal_v<u8',',char_type>)
					*outit=sep_ch;
				else
				{
					*outit=*first;
					++outit;
				}
			}
		}
	}
	else
	{
		for(;first!=last;++first)
		{
			if(*first==char_literal_v<u8',',char_type>)
				outit=copy_scatter(thousands_sep,outit);
			else
			{
				*outit=*first;
				++outit;
			}
		}
	}
	return outit;
}

template<bool full,std::size_t base,bool uppercase,::fast_io::freestanding::random_access_iterator Iter,my_unsigned_integral T>
constexpr Iter grouping_mul_sep_impl(basic_lc_all<::fast_io::freestanding::iter_value_t<Iter>> const* __restrict all,Iter iter,T t) noexcept
{
	using char_type = ::fast_io::freestanding::iter_value_t<Iter>;
	constexpr std::size_t array_len{print_reserve_size(io_reserve_type<char_type,T>)*2u-1u};
	char_type array[array_len];

	auto const ed{array+array_len};
	auto first{lc_grouping_single_sep_impl<full,base,uppercase>(all->numeric.grouping,ed,t)};

	return grouping_mul_sep_print_sep_impl(all->numeric.thousands_sep,first,ed,iter);
}

template<bool full,std::size_t base,bool uppercase,::fast_io::freestanding::random_access_iterator Iter,my_unsigned_integral T>
constexpr Iter grouping_sep_impl(basic_lc_all<::fast_io::freestanding::iter_value_t<Iter>> const* __restrict all,Iter iter,T t) noexcept
{
#if 0
	if(all->numeric.thousands_sep.len==1)
	{
		auto sep{*all->numeric.thousands_sep.base};
		std::size_t digits{chars_len<base>(t)};
		auto grouping{all->numeric.grouping};
		std::size_t const len{grouping_char_lens(grouping,digits)};
		lc_grouping_single_sep_impl(grouping,sep,iter+len,t);
		return iter+len;
	}
	else
#endif
		return grouping_mul_sep_impl<full,base,uppercase>(all,iter,t);
}

template<std::size_t base,bool uppercase,::fast_io::freestanding::random_access_iterator Iter,my_unsigned_integral T>
requires (sizeof(T)>1)
constexpr void lc_print_unsigned_with_3_seperator_len(::fast_io::freestanding::iter_value_t<Iter> seperator_ch,Iter iter,T value,std::size_t size) noexcept
{
	using char_type = ::fast_io::freestanding::iter_value_t<Iter>;
#ifndef __OPTIMIZE_SIZE__
	constexpr auto table(get_shared_inline_constexpr_base_table<char_type,base,uppercase>().element);
#endif
	constexpr std::uint_least32_t cpow1{static_cast<std::uint_least32_t>(base)};
#if !defined(__OPTIMIZE_SIZE__)
	constexpr std::uint_least32_t cpow2{static_cast<std::uint_least32_t>(cpow1*cpow1)};
	constexpr std::uint_least32_t cpow3{static_cast<std::uint_least32_t>(cpow2*cpow1)};
#endif
	for(;3u<size;*--iter=seperator_ch)
	{
#if defined(__OPTIMIZE_SIZE__)
		for(std::size_t i{};i!=3u;++i)
		{
			T remained{static_cast<T>(value%cpow1)};
			value/=cpow1;
			*--iter=to_char_single_digit<char_type,base,uppercase>(remained);
		}
#else
		T low3digits{static_cast<T>(value%cpow3)};
		value/=cpow3;
		T low2digits{static_cast<T>(low3digits%cpow2)};
		T highdigit{static_cast<T>(low3digits/cpow2)};
		non_overlapped_copy_n(table[low2digits].element,2u,iter-=2u);
		*--iter=to_char_single_digit<char_type,base,uppercase>(highdigit);
#endif
		size-=3u;
	}
#if defined(__OPTIMIZE_SIZE__)
	for(;size;--size)
	{
		T remained{static_cast<T>(value%cpow1)};
		value/=cpow1;
		*--iter=to_char_single_digit<char_type,base,uppercase>(remained);
	}
#else
	if(size==3)
	{
		T low2digits{static_cast<T>(value%cpow2)};
		T highdigit{static_cast<T>(value/cpow2)};
		non_overlapped_copy_n(table[low2digits].element,2u,iter-=2u);
		*--iter=to_char_single_digit<char_type,base,uppercase>(highdigit);
	}
	else if(size==2)
		non_overlapped_copy_n(table[value].element,2u,iter-=2u);
	else
	{
		*--iter=to_char_single_digit<char_type,base,uppercase>(value);
	}
#endif
}

template<bool full,std::size_t base,bool uppercase,::fast_io::freestanding::random_access_iterator Iter,my_unsigned_integral int_type>
inline constexpr Iter print_lc_grouping_3_path_impl(::fast_io::freestanding::iter_value_t<Iter> seperator,Iter iter,int_type t) noexcept
{
	if constexpr(full)
	{
		constexpr std::size_t size{cal_max_int_size<int_type,base>()};
		constexpr std::size_t offset_size{size+(size-1)/3};
		if constexpr(sizeof(int_type)<=sizeof(unsigned))
			lc_print_unsigned_with_3_seperator_len<base,uppercase>(seperator,iter+=offset_size,static_cast<unsigned>(t),size);
		else
			lc_print_unsigned_with_3_seperator_len<base,uppercase>(seperator,iter+=offset_size,t,size);
	}
	else
	{
		std::size_t const size{chars_len<base>(t)};
		std::size_t const offset_size{size+(size-1u)/3u};
		if constexpr(sizeof(int_type)<=sizeof(unsigned))
			lc_print_unsigned_with_3_seperator_len<base,uppercase>(seperator,iter+=offset_size,static_cast<unsigned>(t),size);
		else
			lc_print_unsigned_with_3_seperator_len<base,uppercase>(seperator,iter+=offset_size,t,size);
	}
	return iter;
}

template<bool full,std::size_t base,bool uppercase,::fast_io::freestanding::random_access_iterator Iter,my_unsigned_integral intg>
inline constexpr Iter lc_print_reserve_integral_withfull_main_impl(basic_lc_all<::fast_io::freestanding::iter_value_t<Iter>> const* __restrict all,Iter first,intg t)
{
	if((all->numeric.grouping.len==0)|(all->numeric.thousands_sep.len==0))
		return print_reserve_integral_withfull_main_impl<full,base,uppercase>(first,t);
	else if((all->numeric.grouping.len==1&&*all->numeric.grouping.base==3)&(all->numeric.thousands_sep.len==1))
		return print_lc_grouping_3_path_impl<full,base,uppercase>(*(all->numeric.thousands_sep.base),first,t);
	else
		return grouping_sep_impl<full,base,uppercase>(all,first,t);
}

template<std::size_t base,
	bool showbase=false,
	bool uppercase_showbase=false,
	bool showpos=false,
	bool uppercase=false,
	bool full=false,
	typename int_type,::fast_io::freestanding::random_access_iterator Iter>
inline constexpr Iter lc_print_reserve_integral_define(basic_lc_all<::fast_io::freestanding::iter_value_t<Iter>> const* __restrict all,Iter first,int_type t) noexcept
{
	if constexpr(base<=10&&uppercase)
	{
		return print_reserve_integral_define<base,showbase,uppercase_showbase,showpos,false,full>(first,t);//prevent duplications
	}
	else
	{
		static_assert((2<=base)&&(base<=36));
		using char_type = ::fast_io::freestanding::iter_value_t<Iter>;
		using unsigned_type = ::fast_io::details::my_make_unsigned_t<int_type>;
		unsigned_type u{static_cast<unsigned_type>(t)};
		if constexpr(showpos)
		{
			if constexpr(::fast_io::details::my_unsigned_integral<int_type>)
			{
				*first=char_literal_v<u8'+',char_type>;
			}
			else
			{
				if(t<0)
				{
					*first=char_literal_v<u8'-',char_type>;
					constexpr unsigned_type zero{};
					u=zero-u;
				}
				else
				{
					*first=char_literal_v<u8'+',char_type>;
				}
			}
			++first;
		}
		else
		{
			if constexpr(::fast_io::details::my_signed_integral<int_type>)
			{
				if(t<0)
				{
					*first=char_literal_v<u8'-',char_type>;
					++first;
					constexpr unsigned_type zero{};
					u=zero-u;
				}
			}
		}
		if constexpr(showbase&&(base!=10))
			first=print_reserve_show_base_impl<base,uppercase_showbase>(first);
		return lc_print_reserve_integral_withfull_main_impl<full,base,uppercase>(all,first,u);
	}
}

}
template<std::integral char_type,::fast_io::manipulators::scalar_flags flags,typename T>
requires ((details::my_integral<T>||std::same_as<std::remove_cv_t<T>,std::byte>)&&!flags.alphabet&&!std::same_as<std::remove_cv_t<T>,bool>)
inline constexpr std::size_t print_reserve_size(basic_lc_all<char_type> const* __restrict all,manipulators::scalar_manip_t<flags,T>) noexcept
{
	if constexpr(std::same_as<std::remove_cv_t<T>,std::byte>)
		return ::fast_io::details::lc_print_reserve_size_int_cal<char_type,flags,char8_t>(all);
	else
		return ::fast_io::details::lc_print_reserve_size_int_cal<char_type,flags,std::remove_cv_t<T>>(all);
}

template<::fast_io::freestanding::random_access_iterator Iter,::fast_io::manipulators::scalar_flags flags,typename T>
inline constexpr Iter print_reserve_define(basic_lc_all<::fast_io::freestanding::iter_value_t<Iter>> const* __restrict all,Iter iter,manipulators::scalar_manip_t<flags,T> t) noexcept
{
	if constexpr(std::same_as<std::remove_cv_t<T>,std::byte>)
		return details::lc_print_reserve_integral_define<flags.base,flags.showbase,flags.uppercase_showbase,flags.showpos,flags.uppercase,flags.full>(iter,static_cast<char8_t>(t.reference));
	else
		return details::lc_print_reserve_integral_define<flags.base,flags.showbase,flags.uppercase_showbase,flags.showpos,flags.uppercase,flags.full>(all,iter,t.reference);
}

}
