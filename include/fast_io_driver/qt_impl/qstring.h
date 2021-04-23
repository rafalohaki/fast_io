#pragma once

namespace fast_io
{

class qt_error
{
public:
	QString qstr;
	template<typename... Args>
	requires std::constructible_from<QString,Args...>
	qt_error(Args&& ...args):qstr(std::forward<Args>(args)...){}
};

template<typename... Args>
requires std::constructible_from<QString,Args...>
inline void throw_qt_error([[maybe_unused]] Args&& ...args)
{
#ifdef __cpp_exceptions
#if defined(_MSC_VER) && (!defined(_HAS_EXCEPTIONS) || _HAS_EXCEPTIONS == 0)
	fast_terminate();
#else
	throw qt_error(std::forward<Args>(args)...);
#endif
#else
	fast_terminate();
#endif
}

namespace manipulators
{
template<
encoding_scheme src_scheme=encoding_scheme::execution_charset,
encoding_scheme dst_scheme=encoding_scheme::execution_charset,
typename T
>
requires (std::same_as<std::remove_cvref_t<T>,QString>||
	std::same_as<std::remove_cvref_t<T>,QStringRef>||
	std::same_as<std::remove_cvref_t<T>,qt_error>
#if defined(QT_VERSION) && defined(QT_VERSION_CHECK)
#if (QT_VERSION >= QT_VERSION_CHECK(5,6,0))
	||std::same_as<std::remove_cvref_t<T>,QStringView>
#endif
#endif
	)
inline constexpr code_cvt_t<src_scheme,dst_scheme,char16_t> code_cvt(T const& hstr) noexcept
{
	using char16_may_alias_ptr
#if __has_cpp_attribute(gnu::may_alias)
	[[gnu::may_alias]]
#endif
	= char16_t const*;
	if constexpr(std::same_as<std::remove_cvref_t<T>,qt_error>)
		return {{reinterpret_cast<char16_may_alias_ptr>(hstr.qstr.data()),hstr.qstr.size()}};
	else
		return {{reinterpret_cast<char16_may_alias_ptr>(hstr.data()),hstr.size()}};
}
}

template<std::integral char_type,
typename T>
requires (sizeof(char_type)==sizeof(char16_t)||
	std::same_as<std::remove_cvref_t<T>,QString>||
	std::same_as<std::remove_cvref_t<T>,QStringRef>||
	std::same_as<std::remove_cvref_t<T>,qt_error>
#if defined(QT_VERSION) && defined(QT_VERSION_CHECK)
#if (QT_VERSION >= QT_VERSION_CHECK(5,6,0))
	||std::same_as<std::remove_cvref_t<T>,QStringView>
#endif
#endif
)
inline constexpr basic_io_scatter_t<char_type> print_scatter_define(print_scatter_type<char_type>,T const& hstr) noexcept
{
	using char16_may_alias_ptr
#if __has_cpp_attribute(gnu::may_alias)
	[[gnu::may_alias]]
#endif
	= char_type const*;
	if constexpr(std::same_as<std::remove_cvref_t<T>,qt_error>)
		return {{reinterpret_cast<char16_may_alias_ptr>(hstr.qstr.data()),hstr.qstr.size()}};
	else
		return {reinterpret_cast<char16_may_alias_ptr>(hstr.data()),hstr.size()};
}

}