#pragma once

#include"manip.h"
#include"twodigits/shared_base_table.h"
#include"twodigits/base.h"
#include"optimize_size/impl.h"
#include"jiaendu/jiaendu.h"
#include"sto/sto_overhual.h"
#include"append_nine_digits.h"

namespace fast_io
{

namespace details
{
template<char8_t base,bool uppercase,bool ignore_sign=false,std::random_access_iterator Iter,my_integral int_type>
constexpr Iter process_integer_output(Iter iter,int_type i) noexcept
{
	using char_type = std::iter_value_t<Iter>;
	if (std::is_constant_evaluated())
	{
		namespace algo_decision = fast_io::details::optimize_size;
		if constexpr(my_unsigned_integral<int_type>)
			return iter+algo_decision::output_unsigned<base>(iter,static_cast<std::remove_cvref_t<int_type>>(i));
		else
		{
			auto abs_value{static_cast<details::my_make_unsigned_t<std::remove_cvref_t<int_type>>>(i)};
			bool const negative(i<0);
			if(negative)
			{
				abs_value = 0u - abs_value;
				if constexpr(!ignore_sign)
				{
					if constexpr(std::same_as<char_type,char8_t>)
						*iter='-';
					else if constexpr(std::same_as<char_type,wchar_t>)
						*iter=L'-';
					else
						*iter=u8'-';
					++iter;
				}
			}
			return iter+algo_decision::output_unsigned<base>(iter,abs_value);
		}
	}
	else
	{
		if constexpr(base==10)
		{
			namespace algo_decision = 
#ifdef FAST_IO_OPTIMIZE_SIZE
				details::optimize_size;
#elif defined(FAST_IO_OPTIMIZE_TIME)
				details::jiaendu;//Jiaendu is objectively the fastest algorithm since it avoids division. There is no point this isn't the fastest
#else
				details::twodigits;
#endif
			if constexpr(my_unsigned_integral<int_type>)
				return iter+algo_decision::output_unsigned(iter,static_cast<std::remove_cvref_t<int_type>>(i));
			else
			{
				auto abs_value{static_cast<details::my_make_unsigned_t<std::remove_cvref_t<int_type>>>(i)};
				bool const negative(i<0);
				if(negative)
				{
					abs_value = 0 - abs_value;
					if constexpr(!ignore_sign)
					{
						if constexpr(std::same_as<char_type,char8_t>)
							*iter='-';
						else if constexpr(std::same_as<char_type,wchar_t>)
							*iter=L'-';
						else
							*iter=u8'-';
						++iter;
					}
				}
				return iter+algo_decision::output_unsigned(iter,abs_value);
			}
		}
		else
		{
			namespace algo_decision = 
#ifdef FAST_IO_OPTIMIZE_SIZE
				details::optimize_size;
#else
				details::twodigits;
#endif
			if constexpr(my_unsigned_integral<int_type>)
				return iter+algo_decision::output_unsigned<base,uppercase>(iter,static_cast<std::remove_cvref_t<int_type>>(i));
			else
			{
				auto abs_value{static_cast<details::my_make_unsigned_t<std::remove_cvref_t<int_type>>>(i)};
				bool const negative(i<0);
				if(negative)
				{
					abs_value = 0 - abs_value;
					if constexpr(!ignore_sign)
					{
						if constexpr(std::same_as<char_type,char8_t>)
							*iter='-';
						else if constexpr(std::same_as<char_type,wchar_t>)
							*iter=L'-';
						else
							*iter=u8'-';
						++iter;
					}
				}
				return iter+algo_decision::output_unsigned<base,uppercase>(iter,abs_value);
			}
		}
	}
}


}

template<std::integral char_type,details::my_integral int_type>
constexpr std::size_t print_reserve_size(io_reserve_type_t<char_type,int_type>) noexcept
{
	if constexpr(std::same_as<std::remove_cvref_t<int_type>,bool>)
		return 1;
	else if constexpr(details::my_unsigned_integral<int_type>)
		return details::cal_max_int_size<int_type>();
	else
		return details::cal_max_int_size<details::my_make_unsigned_t<int_type>>()+1;
}

template<std::integral char_type,std::random_access_iterator caiter,details::my_integral int_type,typename U>
constexpr caiter print_reserve_define(io_reserve_type_t<char_type,int_type>,caiter iter,U i) noexcept
{
	if constexpr(std::same_as<std::remove_cvref_t<int_type>,bool>)
	{
		if constexpr(std::same_as<char_type,char>)
			*iter=static_cast<char>(i)+'0';
		else if constexpr(std::same_as<char_type,wchar_t>)
			*iter=static_cast<wchar_t>(i)+L'0';
		else
			*iter=static_cast<char8_t>(i)+u8'0';
		return ++iter;
	}
	else
		return details::process_integer_output<10,false>(iter,i);
}

template<std::integral char_type,char8_t base,bool uppercase,details::my_integral int_type>
constexpr std::size_t print_reserve_size(io_reserve_type_t<char_type,manip::base_t<base,uppercase,int_type>>) noexcept
{
	if constexpr(details::my_unsigned_integral<int_type>)
		return details::cal_max_int_size<int_type,base>();
	else
		return details::cal_max_int_size<details::my_make_unsigned_t<int_type>,base>()+1;
}

template<std::integral char_type,std::random_access_iterator caiter,char8_t base,bool uppercase,details::my_integral int_type,typename P>
constexpr caiter print_reserve_define(io_reserve_type_t<char_type,manip::base_t<base,uppercase,int_type>>,caiter iter,P ref) noexcept
{
	return details::process_integer_output<base,uppercase>(iter,ref.reference);
}


template<std::integral char_type>
constexpr std::size_t print_reserve_size(io_reserve_type_t<char_type,std::byte>) noexcept
{
	return details::cal_max_int_size<details::my_make_unsigned_t<std::uint8_t>>();
}

template<std::integral char_type,std::random_access_iterator caiter,typename U>
constexpr caiter print_reserve_define(io_reserve_type_t<char_type,std::byte>,caiter iter,U i) noexcept
{
	return details::process_integer_output<10,false>(iter,std::to_integer<std::uint8_t>(i));
}

template<std::integral char_type,char8_t base,bool uppercase>
constexpr std::size_t print_reserve_size(io_reserve_type_t<char_type,manip::base_t<base,uppercase,std::byte>>) noexcept
{
	return details::cal_max_int_size<std::uint8_t,base>();
}

template<std::integral char_type,std::random_access_iterator caiter,char8_t base,bool uppercase,typename P>
constexpr caiter print_reserve_define(io_reserve_type_t<char_type,manip::base_t<base,uppercase,std::byte>>,caiter iter,P ref) noexcept
{
	return details::process_integer_output<base,uppercase>(iter,std::to_integer<std::uint8_t>(ref.reference));
}

namespace details
{


template<std::random_access_iterator Iter,my_unsigned_integral U>
inline constexpr void output_unsigned_with_size(Iter str,U value,std::size_t len) noexcept
{
#ifdef FAST_IO_OPTIMIZE_SIZE
	optimize_size::with_length::output_unsigned(str,value,len);
#else
	twodigits::with_length::output_unsigned(str,value,len);
#endif
}

template<std::size_t mx_size,std::random_access_iterator Iter>
inline constexpr Iter output_unsigned_serialize_size(std::size_t val,Iter iter) noexcept
{
	using char_type = std::iter_value_t<Iter>;
	if constexpr(mx_size==1)
	{
		if constexpr(exec_charset_is_ebcdic<char_type>())
			*iter=val+0xF0;
		else
			*iter=val+u8'0';
		++iter;
		return iter;
	}
#ifdef FAST_IO_OPTIMIZE_SIZE
	else
		return optimize_size::output_unsigned(iter,val);
#elif defined(FAST_IO_OPTIMIZE_TIME)
	else if constexpr(mx_size==2)
	{
		if(val<10)
			return output_unsigned_serialize_size<1>(val,iter);
		else
			return non_overlapped_copy_n(jiaendu::static_tables<char_type>::table2.data(),2,iter);
	}
	else
		return jiaendu::output_unsigned(std::to_address(iter),val);
#else
	else if constexpr(mx_size==2)
	{
		if(val<10)
			return output_unsigned_serialize_size<1>(val,iter);
		else
			return non_overlapped_copy_n(details::shared_static_base_table<char_type,10,false,false>::table.data(),2,iter);
	}
	else
		return twodigits::output_unsigned(iter,val);
#endif
}
}
}

#include"pointer.h"
#include"representation.h"
#include"chrono.h"