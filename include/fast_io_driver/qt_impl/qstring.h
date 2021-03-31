#pragma once

namespace fast_io
{
#if defined(QT_VERSION) && defined(QT_VERSION_CHECK)
#if (QT_VERSION >= QT_VERSION_CHECK(5,6,0))
namespace mnp
{
template<
encoding_scheme src_scheme=encoding_scheme::execution_charset,
encoding_scheme dst_scheme=encoding_scheme::execution_charset>
inline constexpr code_cvt_t<src_scheme,dst_scheme,char16_t> code_cvt(QStringView hstr) noexcept
{
	using char16_may_alias_ptr
#if __has_cpp_attribute(gnu::may_alias)
	[[gnu::may_alias]]
#endif
	= char16_t const*;
	return {{reinterpret_cast<char16_may_alias_ptr>(hstr.c_str()),hstr.size()}};
}
}

template<std::integral char_type,
typename T>
requires (sizeof(char_type)==sizeof(char16_t)||
	std::same_as<std::remove_cvref_t<T>,QStringView>>)
inline constexpr basic_io_scatter_t<char_type> print_scatter_define(print_scatter_type<char_type>,QStringView hstr) noexcept
{
	using char16_may_alias_ptr
#if __has_cpp_attribute(gnu::may_alias)
	[[gnu::may_alias]]
#endif
	= char_type const*;
	return {reinterpret_cast<char16_may_alias_ptr>(hstr.c_str()),hstr.size()};
}

#endif
#endif

namespace mnp
{
template<
encoding_scheme src_scheme=encoding_scheme::execution_charset,
encoding_scheme dst_scheme=encoding_scheme::execution_charset,
typename T
>
requires (std::same_as<std::remove_cvref_t<T>,QString>||
	std::same_as<std::remove_cvref_t<T>,QStringRef>)
inline constexpr code_cvt_t<src_scheme,dst_scheme,char16_t> code_cvt(T const& hstr) noexcept
{
	using char16_may_alias_ptr
#if __has_cpp_attribute(gnu::may_alias)
	[[gnu::may_alias]]
#endif
	= char16_t const*;
	return {{reinterpret_cast<char16_may_alias_ptr>(hstr.c_str()),hstr.size()}};
}
}

template<std::integral char_type,
typename T>
requires (sizeof(char_type)==sizeof(char16_t)||
	std::same_as<std::remove_cvref_t<T>,QString>||
	std::same_as<std::remove_cvref_t<T>,QStringRef>)
inline constexpr basic_io_scatter_t<char_type> print_scatter_define(print_scatter_type<char_type>,T const& hstr) noexcept
{
	using char16_may_alias_ptr
#if __has_cpp_attribute(gnu::may_alias)
	[[gnu::may_alias]]
#endif
	= char_type const*;
	return {reinterpret_cast<char16_may_alias_ptr>(hstr.c_str()),hstr.size()};
}

}