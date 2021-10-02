#pragma once

namespace fast_io
{

template<std::integral char_type,typename T>
requires (std::same_as<std::remove_cvref_t<T>,std::filesystem::path>)
constexpr std::conditional_t<sizeof(char_type)==sizeof(std::filesystem::path::value_type),basic_io_scatter_t<char_type>,manipulators::code_cvt_t<encoding_scheme::execution_charset,encoding_scheme::execution_charset,std::filesystem::path::value_type>> status_io_print_forward(io_alias_type_t<char_type>,T const& hstr) noexcept
{
	auto const& native{hstr.native()};
	if constexpr(sizeof(char_type)==sizeof(std::filesystem::path::value_type))
	{
		using char_type_may_alias_ptr
#if __has_cpp_attribute(gnu::may_alias)
		[[gnu::may_alias]]
#endif
		= char_type const*;
		return {reinterpret_cast<char_type_may_alias_ptr>(native.c_str()),native.size()};
	}
	else
		return {{native.data(),native.size()}};
}

}
