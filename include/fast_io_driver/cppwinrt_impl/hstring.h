#pragma once

namespace fast_io
{

namespace manipulators
{
template<
encoding_scheme src_scheme=encoding_scheme::execution_charset,
encoding_scheme dst_scheme=encoding_scheme::execution_charset>
inline constexpr code_cvt_t<src_scheme,dst_scheme,char16_t> code_cvt(winrt::hstring const& hstr) noexcept
{
	using char16_may_alias_ptr
#if __has_cpp_attribute(gnu::may_alias)
	[[gnu::may_alias]]
#endif
	= char16_t const*;
	return {{reinterpret_cast<char16_may_alias_ptr>(hstr.c_str()),hstr.size()}};
}
}

template<std::integral char_type>
requires (sizeof(char_type)==sizeof(char16_t))
inline constexpr basic_io_scatter_t<char_type> print_scatter_define(
		print_scatter_type_t<char_type>,winrt::hstring const& hstr) noexcept
{
	using char_type_may_alias_ptr
#if __has_cpp_attribute(gnu::may_alias)
	[[gnu::may_alias]]
#endif
	= char_type const*;
	return {reinterpret_cast<char_type_may_alias_ptr>(hstr.c_str()),hstr.size()};
}

}