#pragma once

namespace fast_io::details
{

template<bool upper_case,bool endian_reverse,::fast_io::freestanding::forward_iterator Iter>
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
			if constexpr(std::same_as<char_type,char>)
				high+='0';
			else if constexpr(std::same_as<char_type,wchar_t>)
				high+=L'0';
			else
				high+=u8'0';
		}
		else
		{
			if constexpr(upper_case)
			{
			if constexpr(std::same_as<char_type,char>)
				high+='A';
			else if constexpr(std::same_as<char_type,wchar_t>)
				high+=L'A';
			else
				high+=u8'A';
			}
			else
			{
			if constexpr(std::same_as<char_type,char>)
				high+='a';
			else if constexpr(std::same_as<char_type,wchar_t>)
				high+=L'a';
			else
				high+=u8'a';
			}
			high-=10u;
		}
		char8_t low{static_cast<char8_t>(e&static_cast<char8_t>(0xF))};
		if(low<10)
		{
			if constexpr(std::same_as<char_type,char>)
				low+='0';
			else if constexpr(std::same_as<char_type,wchar_t>)
				low+=L'0';
			else
				low+=u8'0';
		}
		else
		{
			if constexpr(upper_case)
			{
			if constexpr(std::same_as<char_type,char>)
				low+='A';
			else if constexpr(std::same_as<char_type,wchar_t>)
				low+=L'A';
			else
				low+=u8'A';
			}
			else
			{
			if constexpr(std::same_as<char_type,char>)
				low+='a';
			else if constexpr(std::same_as<char_type,wchar_t>)
				low+=L'a';
			else
				low+=u8'a';
			}
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


template<bool upper_case,bool endian_reverse,std::integral T,::fast_io::freestanding::random_access_iterator Iter>
inline constexpr Iter crypto_hash_main_reserve_define_common_impl(T const* first,T const* last,Iter iter)
{
	constexpr std::size_t offset{sizeof(T)*2};
	for(;first!=last;++first)
	{
		auto e{*first};
		if constexpr(!endian_reverse)
			e=big_endian(e);
		optimize_size::output_unsigned_dummy<offset,16,upper_case>(iter,e);
		iter+=offset;
	}
	return iter;
}

}