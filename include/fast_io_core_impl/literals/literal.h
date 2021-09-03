#pragma once

namespace fast_io
{

template<std::integral ch_type>
#if __has_cpp_attribute(gnu::always_inline)
[[gnu::always_inline]]
#elif __has_cpp_attribute(msvc::forceinline)
[[gnu::forceinline]]
#endif
inline constexpr ch_type char_literal(char8_t ch) noexcept
{
#if 'A' != u8'A' || L'A' !=u8'A'
#include"none_ascii.h"
#endif
	{
		using unsigned_t = ::std::make_unsigned_t<ch_type>;
		return static_cast<unsigned_t>(ch);
	}
}

struct literal_ir
{
	char8_t const* buffer{};
	std::size_t n{};
	template<std::size_t ns>
	inline constexpr literal_ir(char8_t const (&literal)[ns]):buffer(literal),n(ns-1)
	{}
};

namespace details
{

template<std::integral ch_type,std::size_t n>
requires (n!=0)
inline constexpr ::fast_io::freestanding::array<ch_type,n-1> literal_ir_to_char_array_impl(char8_t const* lit) noexcept
{
	constexpr std::size_t nm1{n-1};
	::fast_io::freestanding::array<ch_type,nm1> buffer;
#if 'A' != u8'A' || L'A' !=u8'A'
	if constexpr(::fast_io::details::is_ebcdic<ch_type>)
	{
		for(std::size_t i{};i!=nm1;++i)
			buffer[i]=char_literal(lit[i]);
	}
	else
#endif
	{
		using unsigned_t = ::std::make_unsigned_t<ch_type>;
		for(std::size_t i{};i!=nm1;++i)
			buffer[i]=static_cast<ch_type>(static_cast<unsigned_t>(lit[i]));
	}
	return buffer;
}

}

template<std::integral ch_type,literal_ir lir>
struct literal_irs
{
	using char_type = ch_type;
	::fast_io::freestanding::array<char_type,lir.n> value{::fast_io::details::literal_ir_to_char_array_impl<char_type,lir.n>(lir.buffer)};
};

template<std::integral ch_type,literal_ir lir>
inline constexpr literal_irs<ch_type,lir> literal_values{};

template<std::integral ch_type,literal_ir lir>
inline constexpr auto& literal_ref{literal_values<ch_type,lir>};

}
