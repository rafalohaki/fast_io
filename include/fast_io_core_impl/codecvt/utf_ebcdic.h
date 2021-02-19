#pragma once

namespace fast_io::details
{
/*
Referenced from UTF-EBCDIC
Unicode Technical Report #16
UTF-EBCDIC
EBCDIC-Friendly Unicode (or UCS) Transformation Format
1999-06-24
https://www.unicode.org/reports/tr16/tr16-5.html
*/
inline constexpr char8_t bm_i8_to_ebcdic[256]{0x0,0x1,0x2,0x3,0x37,0x2D,0x2E,0x2F,0x16,0x5,0x15,0xB,0xC,0xD,0xE,0xF,0x10,0x11,0x12,0x13,0x3C,0x3D,0x32,0x26,0x18,0x19,0x3F,0x27,0x1C,0x1D,0x1E,0x1F,0x40,0x5A,0x7F,0x7B,0x5B,0x6C,0x50,0x7D,0x4D,0x5D,0x5C,0x4E,0x6B,0x60,0x4B,0x61,0xF0,0xF1,0xF2,0xF3,0xF4,0xF5,0xF6,0xF7,0xF8,0xF9,0x7A,0x5E,0x4C,0x7E,0x6E,0x6F,0x7C,0xC1,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,0xC8,0xC9,0xD1,0xD2,0xD3,0xD4,0xD5,0xD6,0xD7,0xD8,0xD9,0xE2,0xE3,0xE4,0xE5,0xE6,0xE7,0xE8,0xE9,0xAD,0xE0,0xBD,0x5F,0x6D,0x79,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x91,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0xA2,0xA3,0xA4,0xA5,0xA6,0xA7,0xA8,0xA9,0xC0,0x4F,0xD0,0xA1,0x7,0x20,0x21,0x22,0x23,0x24,0x25,0x6,0x17,0x28,0x29,0x2A,0x2B,0x2C,0x9,0xA,0x1B,0x30,0x31,0x1A,0x33,0x34,0x35,0x36,0x8,0x38,0x39,0x3A,0x3B,0x4,0x14,0x3E,0xFF,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6A,0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77,0x78,0x80,0x8A,0x8B,0x8C,0x8D,0x8E,0x8F,0x90,0x9A,0x9B,0x9C,0x9D,0x9E,0x9F,0xA0,0xAA,0xAB,0xAC,0xAE,0xAF,0xB0,0xB1,0xB2,0xB3,0xB4,0xB5,0xB6,0xB7,0xB8,0xB9,0xBA,0xBB,0xBC,0xBE,0xBF,0xCA,0xCB,0xCC,0xCD,0xCE,0xCF,0xDA,0xDB,0xDC,0xDD,0xDE,0xDF,0xE1,0xEA,0xEB,0xEC,0xED,0xEE,0xEF,0xFA,0xFB,0xFC,0xFD,0xFE};
inline constexpr char8_t bm_ebcdic_to_i18[256]{0x0,0x1,0x2,0x3,0x9C,0x9,0x86,0x7F,0x97,0x8D,0x8E,0xB,0xC,0xD,0xE,0xF,0x10,0x11,0x12,0x13,0x9D,0xA,0x8,0x87,0x18,0x19,0x92,0x8F,0x1C,0x1D,0x1E,0x1F,0x80,0x81,0x82,0x83,0x84,0x85,0x17,0x1B,0x88,0x89,0x8A,0x8B,0x8C,0x5,0x6,0x7,0x90,0x91,0x16,0x93,0x94,0x95,0x96,0x4,0x98,0x99,0x9A,0x9B,0x14,0x15,0x9E,0x1A,0x20,0xA0,0xA1,0xA2,0xA3,0xA4,0xA5,0xA6,0xA7,0xA8,0xA9,0x2E,0x3C,0x28,0x2B,0x7C,0x26,0xAA,0xAB,0xAC,0xAD,0xAE,0xAF,0xB0,0xB1,0xB2,0x21,0x24,0x2A,0x29,0x3B,0x5E,0x2D,0x2F,0xB3,0xB4,0xB5,0xB6,0xB7,0xB8,0xB9,0xBA,0xBB,0x2C,0x25,0x5F,0x3E,0x3F,0xBC,0xBD,0xBE,0xBF,0xC0,0xC1,0xC2,0xC3,0xC4,0x60,0x3A,0x23,0x40,0x27,0x3D,0x22,0xC5,0x61,0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0xC6,0xC7,0xC8,0xC9,0xCA,0xCB,0xCC,0x6A,0x6B,0x6C,0x6D,0x6E,0x6F,0x70,0x71,0x72,0xCD,0xCE,0xCF,0xD0,0xD1,0xD2,0xD3,0x7E,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7A,0xD4,0xD5,0xD6,0x5B,0xD7,0xD8,0xD9,0xDA,0xDB,0xDC,0xDD,0xDE,0xDF,0xE0,0xE1,0xE2,0xE3,0xE4,0xE5,0x5D,0xE6,0xE7,0x7B,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0xE8,0xE9,0xEA,0xEB,0xEC,0xED,0x7D,0x4A,0x4B,0x4C,0x4D,0x4E,0x4F,0x50,0x51,0x52,0xEE,0xEF,0xF0,0xF1,0xF2,0xF3,0x5C,0xF4,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5A,0xF5,0xF6,0xF7,0xF8,0xF9,0xFA,0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0xFB,0xFC,0xFD,0xFE,0xFF,0x9F};
inline constexpr char8_t utfebcdic_shadow_flags[256]{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,9,9,9,9,9,9,9,9,9,9,1,1,1,1,1,1,9,9,9,9,9,9,9,9,9,1,1,1,1,1,1,1,1,9,9,9,9,9,9,9,9,9,1,1,1,1,1,9,9,9,9,2,2,2,2,2,1,1,1,1,1,1,1,2,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,1,1,1,1,1,1,1,1,1,2,2,2,1,2,2,2,2,2,2,2,2,2,3,3,3,3,3,3,1,3,3,1,1,1,1,1,1,1,1,1,1,3,3,3,3,3,3,1,1,1,1,1,1,1,1,1,1,3,3,4,4,4,4,1,4,1,1,1,1,1,1,1,1,4,4,4,5,5,5,1,1,1,1,1,1,1,1,1,1,5,6,6,7,7,0};

template<std::integral T>
requires (sizeof(T)==1)
inline constexpr std::size_t get_utf_ebcdic_invalid_code_units(T* dst) noexcept
{
	*dst=0xEE;
	dst[1]=0x73;
	dst[2]=0x73;
	dst[3]=0x71;
	return 4;
}

template<std::integral T>
requires (sizeof(T)==1)
inline constexpr std::size_t get_utf_ebcdic_code_units(char32_t code,T* dst) noexcept
{
	if(code<0xA0)
	{
		*dst=static_cast<T>(bm_i8_to_ebcdic[code]);
		return 1;
	}
	else if(code<0x400)
	{
		*dst=static_cast<T>(bm_i8_to_ebcdic[static_cast<char32_t>(0b11000000)|(code>>5)]);
		dst[1]=static_cast<T>(bm_i8_to_ebcdic[static_cast<char32_t>(0b10100000)|(code&static_cast<char32_t>(0b11111))]);
		return 2;
	}
	else if(code<0x4000)
	{
		*dst=static_cast<T>(bm_i8_to_ebcdic[static_cast<char32_t>(0b11100000)|(code>>10)]);
		dst[1]=static_cast<T>(bm_i8_to_ebcdic[static_cast<char32_t>(0b10100000)|((code>>5)&static_cast<char32_t>(0b11111))]);
		dst[2]=static_cast<T>(bm_i8_to_ebcdic[static_cast<char32_t>(0b10100000)|(code&static_cast<char32_t>(0b11111))]);
		return 3;
	}
	else if(code<0x40000)
	{
		*dst=static_cast<T>(bm_i8_to_ebcdic[static_cast<char32_t>(0b11111000)|(code>>15)]);
		dst[1]=static_cast<T>(bm_i8_to_ebcdic[static_cast<char32_t>(0b10100000)|((code>>10)&static_cast<char32_t>(0b11111))]);
		dst[2]=static_cast<T>(bm_i8_to_ebcdic[static_cast<char32_t>(0b10100000)|((code>>5)&static_cast<char32_t>(0b11111))]);
		dst[3]=static_cast<T>(bm_i8_to_ebcdic[static_cast<char32_t>(0b10100000)|(code&static_cast<char32_t>(0b11111))]);
		return 4;
	}
	else if(code<0x110000)
	{
		*dst=static_cast<T>(bm_i8_to_ebcdic[static_cast<char32_t>(0b11111000)|(code>>20)]);
		dst[1]=static_cast<T>(bm_i8_to_ebcdic[static_cast<char32_t>(0b10100000)|((code>>15)&static_cast<char32_t>(0b11111))]);
		dst[2]=static_cast<T>(bm_i8_to_ebcdic[static_cast<char32_t>(0b10100000)|((code>>10)&static_cast<char32_t>(0b11111))]);
		dst[3]=static_cast<T>(bm_i8_to_ebcdic[static_cast<char32_t>(0b10100000)|((code>>5)&static_cast<char32_t>(0b11111))]);
		dst[4]=static_cast<T>(bm_i8_to_ebcdic[static_cast<char32_t>(0b10100000)|(code&static_cast<char32_t>(0b11111))]);
		return 5;
	}
	return get_utf_ebcdic_invalid_code_units(dst);
}

struct from_ebcdic_result
{
	char32_t code;
	char8_t len;
};

template<std::integral T>
requires (sizeof(T)==1)
inline constexpr from_ebcdic_result utf_ebcdic_advance(T* src_first,std::size_t sz) noexcept
{
	char8_t src0(static_cast<char8_t>(*src_first));
	char8_t shadow{utfebcdic_shadow_flags[src0]};
	if(shadow<2)
		return {static_cast<char32_t>(bm_ebcdic_to_i18[src0]),1};
	if(shadow==9)
		return {0xFFFD,1};
	if(sz<shadow)
		return {0,static_cast<char8_t>(-1)};
	char32_t res{};
	for(char8_t i{1};i!=shadow;++i)
	{
		char8_t code(static_cast<char8_t>(src_first[i]));
		if((code>>5)!=0b101)[[unlikely]]
			return {0xFFFD,i};
		res=(res<<6)|(bm_ebcdic_to_i18[code&0b11111]);
	}
	char8_t src0i18{bm_ebcdic_to_i18[src0]};
	switch(shadow)
	{
	case 2:
		return {((static_cast<char32_t>(src0i18)&0b11111)<<5)|res,2};
	case 3:
		return {((static_cast<char32_t>(src0i18)&0b1111)<<10)|res,3};
	case 4:
		return {((static_cast<char32_t>(src0i18)&0b111)<<15)|res,4};
	case 5:
		return {((static_cast<char32_t>(src0i18)&0b1)<<20)|res,5};
	default:
		return {0xFFFD,shadow};
	};
}

}