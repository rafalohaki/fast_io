#pragma once

namespace fast_io::details
{

template<bool uppercase,bool endian_reverse,::fast_io::freestanding::forward_iterator Iter>
inline constexpr Iter crypto_hash_print_reserve_define_common_impl(char unsigned const* first,char unsigned const* last,Iter iter)
{
	using char_type = ::fast_io::freestanding::iter_value_t<Iter>;
/*
http://astrodigital.org/digital/ebcdic.html
EBCDIC table shows 'A' - 'I' are contiguous even for EBCDIC.
So we do not need to tweak more things around. Just plainly do addition
*/
	for(;first!=last;++first)
	{
		char8_t e{static_cast<char8_t>(*first)};
		char8_t high{static_cast<char8_t>(e>>static_cast<char8_t>(4u))};
		if(high<10)
		{
			high+=char_literal_v<u8'0',char_type>;
		}
		else
		{
			high+=char_literal_v<(uppercase?u8'A':u8'a'),char_type>;
			high-=10u;
		}
		char8_t low{static_cast<char8_t>(e&static_cast<char8_t>(0xF))};
		if(low<10)
		{
			low+=char_literal_v<u8'0',char_type>;
		}
		else
		{
			low+=char_literal_v<(uppercase?u8'A':u8'a'),char_type>;
			low-=10u;
		}
		if constexpr(endian_reverse)
		{
			*iter=low;
			++iter;
			*iter=high;
			++iter;
		}
		else
		{
			*iter=high;
			++iter;
			*iter=low;
			++iter;
		}
	}
	return iter;
}


template<bool uppercase,bool endian_reverse,std::integral T,::fast_io::freestanding::random_access_iterator Iter>
inline constexpr Iter crypto_hash_main_reserve_define_common_impl(T const* first,T const* last,Iter iter)
{
	constexpr std::size_t offset{sizeof(T)*2};
	for(;first!=last;++first)
	{
		auto e{*first};
		if constexpr(!endian_reverse)
			e=big_endian(e);
		print_reserve_integral_main_impl<16,uppercase>(iter+=offset,e,offset);
//		optimize_size::output_unsigned_dummy<offset,16,upper_case>(iter,e);
//		iter+=offset;
	}
	return iter;
}

}