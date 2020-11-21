#pragma once

#include"utf_util_table.h"
#ifdef __SSE__
#include <emmintrin.h>
#include <immintrin.h>
#include <xmmintrin.h>
#endif
/*
CppCon 2018: Bob Steagall “Fast Conversion From UTF-8 with C++, DFAs, and SSE Intrinsics”

https://www.youtube.com/watch?v=5FQ87-Ecb-A
https://github.com/BobSteagall/utf_utils/blob/master/src/utf_utils.cpp

*/

namespace fast_io
{

template<std::integral T>
requires ((std::unsigned_integral<T>&&sizeof(T)==2)||sizeof(T)==1)
inline constexpr std::size_t utf_get_code_units(char32_t cdpt, T* pDst)
/*
 noexcept
Has not done. Cannot be noexcept
*/
{
	if constexpr(sizeof(T)==2)
	{
		if (cdpt < 0x10000)
		{
			*pDst = static_cast<T>(cdpt);
			return 1;
		}
		else
		{
			*pDst = static_cast<T>(0xD7C0 + (cdpt >> 10));
			pDst[1] = static_cast<T>(0xDC00 + (cdpt & 0x3FF));
			return 2;
		}
	}
	else
	{
		if (cdpt <= 0x7F)[[likely]]
		{
			*pDst = static_cast<T>(cdpt);
			return 1;
		}
		else if (cdpt <= 0x7FF)
		{
			*pDst = static_cast<T>(0xC0 | ((cdpt >> 6) & 0x1F));
			pDst[1] = static_cast<T>(0x80 | (cdpt        & 0x3F));
			return 2;
		}
		else if (cdpt <= 0xFFFF)
		{
			*pDst = static_cast<T>(0xE0 | ((cdpt >> 12) & 0x0F));
			pDst[1] = static_cast<T>(0x80 | ((cdpt >> 6)  & 0x3F));
			pDst[2] = static_cast<T>(0x80 | (cdpt         & 0x3F));
			return 3;
		}
		else if (cdpt <= 0x10FFFF)
		{
			*pDst= static_cast<T>(0xF0 | ((cdpt >> 18) & 0x07));
			pDst[1] = static_cast<T>(0x80 | ((cdpt >> 12) & 0x3F));
			pDst[2] = static_cast<T>(0x80 | ((cdpt >> 6)  & 0x3F));
			pDst[3] = static_cast<T>(0x80 | (cdpt         & 0x3F));
			return 4;
		}
#ifdef __cpp_exceptions
		throw fast_io_text_error("illegal utf32 code unit");
#else
		fast_terminate();
#endif
	}

}


namespace details::utf
{
#ifdef __SSE__
template<std::integral T,std::integral U>
requires (sizeof(T)==1)&&(sizeof(U)==2||sizeof(U)==4)
inline void convert_ascii_with_sse(T*& pSrc, U*& pDst) noexcept
{
	uint16_t mask;
	if constexpr(sizeof(U)==2)
	{
    		__m128i     chunk, half;
		chunk = _mm_loadu_si128((__m128i const*) pSrc);     //- Load the register with 8-bit bytes
		mask  = _mm_movemask_epi8(chunk);                   //- Determine which octets have high bit set

		half = _mm_unpacklo_epi8(chunk, _mm_set1_epi8(0));  //- Unpack lower half into 16-bit words
		_mm_storeu_si128((__m128i*) pDst, half);            //- Write to memory

		half = _mm_unpackhi_epi8(chunk, _mm_set1_epi8(0));  //- Unpack upper half into 16-bit words
		_mm_storeu_si128((__m128i*) (pDst + 8), half);      //- Write to memory

	}
	else
	{
		__m128i     chunk, half, qrtr, zero;
		zero  = _mm_set1_epi8(0);                           //- Zero out the interleave register
		chunk = _mm_loadu_si128((__m128i const*) pSrc);     //- Load a register with 8-bit bytes
		mask  = _mm_movemask_epi8(chunk);                   //- Determine which octets have high bit set

		half = _mm_unpacklo_epi8(chunk, zero);              //- Unpack bytes 0-7 into 16-bit words
		qrtr = _mm_unpacklo_epi16(half, zero);              //- Unpack words 0-3 into 32-bit dwords
		_mm_storeu_si128((__m128i*) pDst, qrtr);            //- Write to memory
		qrtr = _mm_unpackhi_epi16(half, zero);              //- Unpack words 4-7 into 32-bit dwords
		_mm_storeu_si128((__m128i*) (pDst + 4), qrtr);      //- Write to memory

		half = _mm_unpackhi_epi8(chunk, zero);              //- Unpack bytes 8-15 into 16-bit words
		qrtr = _mm_unpacklo_epi16(half, zero);              //- Unpack words 8-11 into 32-bit dwords
		_mm_storeu_si128((__m128i*) (pDst + 8), qrtr);      //- Write to memory
		qrtr = _mm_unpackhi_epi16(half, zero);              //- Unpack words 12-15 into 32-bit dwords
		_mm_storeu_si128((__m128i*) (pDst + 12), qrtr);     //- Write to memory
	}
	auto const incr{std::countr_zero(mask)};
	pSrc += incr;
	pDst += incr;
}
#endif


template<std::input_iterator input>
constexpr inline uint32_t advance_with_big_table(input& pSrc, input pSrcEnd, char32_t& cdpt) noexcept
{
	char8_t const* info{first_unit_info[static_cast<char8_t>(*pSrc)]};
	cdpt = *info;                                //- From it, get the initial code point value
	std::int32_t curr{info[1]};                                 //- From it, get the second state
	for(++pSrc;12<curr;)
	{
		if (pSrc < pSrcEnd)[[likely]]
		{
			char8_t const unit(*pSrc);
			++pSrc;                                 //- Cache the current code unit
			cdpt = (cdpt << 6) | (unit & 0x3F);             //- Adjust code point with continuation bits
			curr = transitions[curr + octet_category[unit]];
			//- Look up the next state
		}
		else
			return 12;
	}
	return curr;
}

template<bool stream=false,std::contiguous_iterator from_iter,std::contiguous_iterator to_iter>
requires (
sizeof(std::iter_value_t<from_iter>)<=4&&sizeof(std::iter_value_t<to_iter>)<=4&&sizeof(std::iter_value_t<from_iter>)!=sizeof(std::iter_value_t<to_iter>)
&&((sizeof(std::iter_value_t<from_iter>)==1&&std::integral<std::iter_value_t<from_iter>>)||std::unsigned_integral<std::iter_value_t<from_iter>>)
&&((sizeof(std::iter_value_t<to_iter>)==1&&std::integral<std::iter_value_t<to_iter>>)||std::unsigned_integral<std::iter_value_t<to_iter>>))
inline constexpr to_iter utf_code_convert_details(from_iter& p_src_begin_iter,from_iter p_src_end_iter,to_iter p_dst_iter)
{
	auto p_src{std::to_address(p_src_begin_iter)};
	auto p_src_end{std::to_address(p_src_end_iter)};
	auto p_dst{std::to_address(p_dst_iter)};
	if constexpr(sizeof(std::iter_value_t<from_iter>)<sizeof(std::iter_value_t<to_iter>))
	{
		char32_t cdpt;
		if constexpr(sizeof(std::iter_value_t<from_iter>)==1)
		{
#ifdef __SSE__
#if __cpp_lib_is_constant_evaluated>=201811L
		if (!std::is_constant_evaluated())
		{
#endif
		while (p_src + sizeof(__m128i)< p_src_end)
		{
			if (static_cast<char8_t>(*p_src) < 0x80)
			{
				details::utf::convert_ascii_with_sse(p_src, p_dst);
			}
			else
			{
				if (details::utf::advance_with_big_table(p_src, p_src_end, cdpt) != 12)[[likely]]
				{
					if constexpr(sizeof(std::iter_value_t<to_iter>)==4)
					{
						*p_dst=cdpt;
						++p_dst;
					}
					else
						p_dst+=utf_get_code_units(cdpt, p_dst);
				}
				else
				{
#if 0
					if constexpr(sizeof(std::iter_value_t<to_iter>)==4)
					{
						*p_dst=0xFFFD;
						++p_dst;
					}
					else
						p_dst+=utf_get_code_units(cdpt, p_dst);
#else
#ifdef __cpp_exceptions
					throw fast_io_text_error("illegal utf8");
#else
					fast_terminate();
#endif
#endif
				}
			}
		}

#if __cpp_lib_is_constant_evaluated>=201811L
		}
#endif
#endif
		}
		constexpr std::size_t offset(stream?4:0);
		while (p_src + offset < p_src_end)
		{
			if (static_cast<char8_t>(*p_src) < 0x80)
			{
				*p_dst = *p_src;
				++p_dst;
				++p_src;
			}
			else
			{
				if (details::utf::advance_with_big_table(p_src, p_src_end, cdpt) != 12)[[likely]]
				{
					if constexpr(sizeof(std::iter_value_t<to_iter>)==4)
					{
						*p_dst=cdpt;
						++p_dst;
					}
					else
						p_dst+=utf_get_code_units(cdpt, p_dst);
				}
				else
#ifdef __cpp_exceptions
					throw fast_io_text_error("illegal utf8");
#else
					fast_terminate();
#endif
			}
		}
		if constexpr(stream)
		{
			while (p_src < p_src_end)
			{
				if (static_cast<char8_t>(*p_src) < 0x80)
				{
					*p_dst = *p_src;
					++p_dst;
					++p_src;
				}
				else
				{
					auto newp_src{p_src};
					if (details::utf::advance_with_big_table(newp_src, p_src_end, cdpt) != 12)[[likely]]
					{
						if constexpr(sizeof(std::iter_value_t<to_iter>)==4)
						{
							*p_dst=cdpt;
							++p_dst;
						}
						else
							p_dst+=utf_get_code_units(cdpt, p_dst);
						p_src=newp_src;
					}
					else
						break;
				}
			}
		}
	}
	else
	{
		for(;p_src!=p_src_end;++p_src)
			p_dst+=utf_get_code_units(*p_src, p_dst);
	}
	if constexpr(stream)
		p_src_begin_iter=p_src-std::to_address(p_src_begin_iter)+p_src_begin_iter;
	return p_dst-std::to_address(p_dst_iter) + p_dst_iter;
}

}

/*

Todo in the future, detect EBCDIC compiler for code_cvt

Assume little endian first until I create a good interface
*/

template<std::contiguous_iterator from_iter,std::contiguous_iterator to_iter>
inline constexpr to_iter utf_code_convert(from_iter p_src_begin_iter,from_iter p_src_end_iter,to_iter p_dst_iter)
{
	return details::utf::utf_code_convert_details<false>(p_src_begin_iter,p_src_end_iter,p_dst_iter);
}

template<std::integral char_type>
inline constexpr manip::code_cvt<basic_io_scatter_t<char_type>> code_cvt(basic_io_scatter_t<char_type> b) noexcept
{
	return {b};
}


template<std::integral char_type,std::size_t n>
inline constexpr manip::code_cvt<basic_io_scatter_t<char_type>> code_cvt(char_type const(&s)[n]) noexcept
{
	return {{s,n-1}};
}

template<std::integral char_type>
inline constexpr manip::code_cvt<basic_io_scatter_t<char_type>> code_cvt(std::basic_string_view<char_type> sv) noexcept
{
	return {{sv.data(),sv.size()}};
}

template<output_stream output,std::integral ch_type>
inline constexpr void print_define(output out,manip::code_cvt<basic_io_scatter_t<ch_type>> view)
{
	constexpr std::size_t coff{sizeof(typename output::char_type)<sizeof(ch_type)?2:0};
	reserve_write(out,view.reference.len<<coff,[&](auto ptr)
	{
		auto refbegin{view.reference.base};
		return details::utf::utf_code_convert_details<false>(refbegin,view.reference.base+view.reference.len,ptr);
	});
}

}
