#pragma once

namespace fast_io
{

template<std::integral char_type,typename T>
constexpr std::conditional_t<sizeof(char_type)==sizeof(char16_t),basic_io_scatter_t<char_type>,manipulators::code_cvt_t<encoding_scheme::execution_charset,encoding_scheme::execution_charset,char16_t>> status_io_print_forward(io_alias_type_t<char_type>,hstring const& hstr) noexcept
{
	if constexpr(sizeof(char_type)==sizeof(char16_t))
	{
		using general_utf16_may_alias_ptr
#if __has_cpp_attribute(gnu::may_alias)
		[[gnu::may_alias]]
#endif
		= char_type const*;
		return {reinterpret_cast<general_utf16_may_alias_ptr>(hstr.c_str()),hstr.size()};
	}
	else
	{
		using char16_may_alias_ptr
#if __has_cpp_attribute(gnu::may_alias)
		[[gnu::may_alias]]
#endif
		= char16_t const*;
		return {{reinterpret_cast<char16_may_alias_ptr>(hstr.c_str()),hstr.size()}};
	}
}

}