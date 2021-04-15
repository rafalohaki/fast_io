#pragma once

#if __STDC_HOSTED__==1 && (!defined(_GLIBCXX_HOSTED) || _GLIBCXX_HOSTED==1)
namespace fast_io::freestanding
{
using ::std::find_if;
using ::std::min;
using ::std::max;
using ::std::copy_n;
using ::std::copy;
using ::std::fill;
using ::std::fill_n;
}

#else
namespace fast_io::freestanding
{

template<input_iterator Iter,typename predicate>
inline constexpr Iter find_if(Iter first,Iter last,predicate q)
{
	for(;first!=last;++first)
		if(q(*first))
			return first;
	return last;
}

template<typename T>
constexpr T const& min(T const& a,T const& b)
{
	return (a<b)?a:b;
}

template<typename T>
constexpr T const& max(T const& a,T const& b)
{
	return (a<b)?b:a;
}

template<input_iterator input_iter,input_or_output_iterator output_iter>
inline constexpr output_iter copy_n(input_iter first,std::size_t count,output_iter result)
{
	for (std::size_t i{}; i != count; ++i)
	{
		*result = *++first;
		++result;
	}
}

template<::fast_io::freestanding::input_iterator input_iter,::fast_io::freestanding::input_or_output_iterator output_iter>
inline constexpr output_iter copy(input_iter first,input_iter last,output_iter result)
{
	for(;first!=last;)
	{
		*result=*first;
		++first;
		++result;
	}
}


template<::fast_io::freestanding::input_iterator fwd_iter,typename T>
inline constexpr void fill(fwd_iter first,fwd_iter last,T value)
{
	for(;first!=last;++first)
		*first=value;
}


template<::fast_io::freestanding::input_iterator fwd_iter,typename T>
inline constexpr void fill_n(fwd_iter first,std::size_t n,T value)
{
	for(std::size_t i{};i!=n;++i)
		*first=value;
}


}

#endif

namespace fast_io::freestanding
{

template<input_iterator Iter,std::integral T>
//requires (std::is_trivially_copyable_v<T>&&sizeof(T)<=sizeof(std::uintmax_t))
requires (std::is_trivially_copyable_v<T>)
inline constexpr Iter find(Iter first,Iter last,T t)
{
	for(;first!=last;++first)
		if(*first==t)
			return first;
	return last;
}

}