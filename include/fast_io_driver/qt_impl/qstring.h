#pragma once

namespace fast_io
{

class qt_error
{
public:
	QString qstr;
	template<typename... Args>
	requires std::constructible_from<QString,Args...>
	qt_error(Args&& ...args):qstr(::fast_io::freestanding::forward<Args>(args)...){}

	QChar* data() noexcept
	{
		return qstr.data();
	}
	QChar const* data() const noexcept
	{
		return qstr.data();
	}
	std::size_t size() const noexcept
	{
		return static_cast<std::size_t>(qstr.size());
	}
};

namespace details
{

template<typename T>
concept qt_qstring_view_like_impl = std::same_as<std::remove_cvref_t<T>,QString>||std::same_as<std::remove_cvref_t<T>,qt_error>;

template<typename T>
concept qt_convertible_to_qstring_impl = requires(T const& e)
{
	{e.toString()}->std::same_as<QString>;
};

}

template<typename... Args>
requires std::constructible_from<QString,Args...>
inline void throw_qt_error([[maybe_unused]] Args&& ...args)
{
#ifdef __cpp_exceptions
#if defined(_MSC_VER) && (!defined(_HAS_EXCEPTIONS) || _HAS_EXCEPTIONS == 0)
	fast_terminate();
#else
	throw qt_error(::fast_io::freestanding::forward<Args>(args)...);
#endif
#else
	fast_terminate();
#endif
}

template<std::integral char_type,typename T>
requires (details::qt_qstring_view_like_impl<T>)
constexpr std::conditional_t<sizeof(char_type)==sizeof(char16_t),basic_io_scatter_t<char_type>,manipulators::code_cvt_t<encoding_scheme::execution_charset,encoding_scheme::execution_charset,char16_t>> status_io_print_forward(io_alias_type_t<char_type>,T const& hstr) noexcept
{
	if constexpr(sizeof(char_type)==sizeof(char16_t))
	{
		using general_utf16_may_alias_ptr
#if __has_cpp_attribute(gnu::may_alias)
		[[gnu::may_alias]]
#endif
		= char_type const*;
		return {reinterpret_cast<general_utf16_may_alias_ptr>(hstr.data()),static_cast<std::size_t>(hstr.size())};
	}
	else
	{
		using char16_may_alias_ptr
#if __has_cpp_attribute(gnu::may_alias)
		[[gnu::may_alias]]
#endif
		= char16_t const*;
		return {{reinterpret_cast<char16_may_alias_ptr>(hstr.data()),static_cast<std::size_t>(hstr.size())}};
	}
}

template<typename T>
requires (details::qt_convertible_to_qstring_impl<T>)
inline QString print_alias_define(io_alias_t,T const& hstr) noexcept
{
	return {hstr.toString()};
}

}
