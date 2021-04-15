#pragma once

namespace fast_io
{
namespace details
{
template<std::integral char_type,::fast_io::freestanding::forward_iterator Iter>
inline constexpr Iter print_reserve_parse_code_impl(Iter iter,parse_code code) noexcept
{
	if constexpr(std::same_as<::fast_io::freestanding::iter_value_t<Iter>,char>)
	{
	switch(code)
	{
	case parse_code::ok:
		return copy_string_literal("ok",iter);
	case parse_code::end_of_file:
		return copy_string_literal("end_of_file",iter);
	case parse_code::partial:
		return copy_string_literal("partial",iter);
	case parse_code::invalid:
		return copy_string_literal("invalid",iter);
	case parse_code::overflow:
		return copy_string_literal("overflow",iter);
	default:
		return copy_string_literal("unknown",iter);
	}
	}
	else if constexpr(std::same_as<::fast_io::freestanding::iter_value_t<Iter>,wchar_t>)
	{
	switch(code)
	{
	case parse_code::ok:
		return copy_string_literal(L"ok",iter);
	case parse_code::end_of_file:
		return copy_string_literal(L"end_of_file",iter);
	case parse_code::partial:
		return copy_string_literal(L"partial",iter);
	case parse_code::invalid:
		return copy_string_literal(L"invalid",iter);
	case parse_code::overflow:
		return copy_string_literal(L"overflow",iter);
	default:
		return copy_string_literal(L"unknown",iter);
	}
	}
	else if constexpr(std::same_as<::fast_io::freestanding::iter_value_t<Iter>,char16_t>)
	{
	switch(code)
	{
	case parse_code::ok:
		return copy_string_literal(u"ok",iter);
	case parse_code::end_of_file:
		return copy_string_literal(u"end_of_file",iter);
	case parse_code::partial:
		return copy_string_literal(u"partial",iter);
	case parse_code::invalid:
		return copy_string_literal(u"invalid",iter);
	case parse_code::overflow:
		return copy_string_literal(u"overflow",iter);
	default:
		return copy_string_literal(u"unknown",iter);
	}
	}
	else
	{
	switch(code)
	{
	case parse_code::ok:
		return copy_string_literal(u8"ok",iter);
	case parse_code::end_of_file:
		return copy_string_literal(u8"end_of_file",iter);
	case parse_code::partial:
		return copy_string_literal(u8"partial",iter);
	case parse_code::invalid:
		return copy_string_literal(u8"invalid",iter);
	case parse_code::overflow:
		return copy_string_literal(u8"overflow",iter);
	default:
		return copy_string_literal(u8"unknown",iter);
	}
	}
}


}

template<std::integral char_type>
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<char_type,parse_code>) noexcept
{
	return 16;
}

template<std::integral char_type,::fast_io::freestanding::forward_iterator Iter>
inline constexpr Iter print_reserve_define(io_reserve_type_t<char_type,parse_code>,Iter iter,parse_code code) noexcept
{
	return details::print_reserve_parse_code_impl(iter,code);
}
inline void throw_parse_code([[maybe_unused]] parse_code code)
{
#ifdef __cpp_exceptions
#if defined(_MSC_VER) && (!defined(_HAS_EXCEPTIONS) || _HAS_EXCEPTIONS == 0)
	fast_terminate();
#else
	throw code;
#endif
#else
	fast_terminate();
#endif
}

}