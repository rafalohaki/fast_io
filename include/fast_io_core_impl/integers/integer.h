#pragma once
#include"util.h"
#include"manip.h"
#include"twodigits/shared_base_table.h"
#include"twodigits/base.h"
#include"optimize_size/impl.h"
#include"jiaendu/jiaendu.h"
#include"sto/sto.h"
#include"sto/sto_reserve.h"
#include"sto/scan_context.h"
#include"append_nine_digits.h"

namespace fast_io
{

namespace details
{
template<char8_t base,bool uppercase,bool ignore_sign=false,std::contiguous_iterator Iter,my_integral int_type>
constexpr Iter process_integer_output(Iter iter,int_type i) noexcept
{
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


}

#include"pointer.h"
#include"representation.h"
#include"chrono.h"