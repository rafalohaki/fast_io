#pragma once

namespace fast_io
{
/*
Allow us to change this in the future without breaking ABI if uintmax_t becomes uint128_t in the future.
*/
using uintiso_t = std::uintmax_t;
using intiso_t = std::intmax_t;

namespace details
{

template<std::random_access_iterator Iter>
inline constexpr Iter output_iso8601_subseconds(Iter iter,uintiso_t subseconds) noexcept
{
	using char_type = std::iter_value_t<Iter>;
	if constexpr(std::same_as<char_type,char>)
		*iter='.';
	else if constexpr(std::same_as<char_type,wchar_t>)
		*iter=L'.';
	else
		*iter=u8'.';
	++iter;
	constexpr std::size_t digitsm1(std::numeric_limits<uintiso_t>::digits10-1);
	std::size_t sz(digitsm1);
	for(;subseconds%10==0;--sz)
		subseconds/=10;
	with_length_output_unsigned(iter,subseconds,sz);
	return iter+sz;
}

}
}

#include"iso3166.h"
#include"iso6709.h"
#include"iso8601.h"