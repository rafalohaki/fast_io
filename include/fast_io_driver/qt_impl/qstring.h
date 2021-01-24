#pragma once

namespace fast_io
{

namespace details
{
struct qt_scatter
{
	using char16_ptr_mayalias
#if __has_cpp_attribute(gnu::may_alias)
	[[gnu::may_alias]]
#endif
	= char16_t const*;
	char16_ptr_mayalias base{};
	std::size_t len{};
};
}

namespace manipulators
{
#if defined(QT_VERSION) && defined(QT_VERSION_CHECK)
#if (QT_VERSION <= QT_VERSION_CHECK(5,6,0))
template<typename T>
requires std::same_as<std::remove_cvref_t<T>,QStringView>
inline constexpr cdcvt_t<::fast_io::details::qt_scatter> cdcvt(T t) noexcept
{
	if constexpr(sizeof(qsizetype)>sizeof(std::size_t))
	{
		if(t.size()>SIZE_MAX)
			fast_terminate();
	}
	return {reinterpret_cast<::fast_io::details::qt_scatter::char16_ptr_mayalias>(t.data()),static_cast<std::size_t>(t.size())};
}

#endif
#endif

template<typename T>
requires (std::same_as<std::remove_cvref_t<T>,QString>||
	std::same_as<std::remove_cvref_t<T>,QStringRef>)
inline constexpr cdcvt_t<::fast_io::details::qt_scatter> cdcvt(T&& t) noexcept
{
	if constexpr(sizeof(qsizetype)>sizeof(std::size_t))
	{
		if(t.size()>SIZE_MAX)
			fast_terminate();
	}
	return {reinterpret_cast<::fast_io::details::qt_scatter::char16_ptr_mayalias>(t.data()),static_cast<std::size_t>(t.size())};
}
}

namespace details
{
template<std::integral char_type>
inline constexpr std::size_t print_reserve_size_qt_scatter_impl(qt_scatter vw) noexcept
{
	if constexpr(exec_charset_is_ebcdic<char_type>())
		fast_terminate();
	if constexpr(sizeof(char_type)==1)
	{
		if((SIZE_MAX>>2)<vw.len)
			fast_terminate();
		return static_cast<std::size_t>(vw.len);
	}
	else
	{
		if(SIZE_MAX<vw.len)
			fast_terminate();
		return static_cast<std::size_t>(vw.len);
	}
}

template<std::random_access_iterator Iter>
inline constexpr Iter print_reserve_define_qt_scatter_impl(Iter iter,qt_scatter vw)
{
	using char_type = std::iter_value_t<Iter>;
	char_type* ptr(std::to_address(iter));
	char_type* ptr_after{utf::utf_code_convert_details<false>(vw.base,vw.base+vw.len,ptr)};
	return iter+(ptr_after-ptr);
}

}

template<std::integral char_type>
requires (std::same_as<char_type,char>||
	(std::same_as<char_type,wchar_t>&&sizeof(wchar_t)!=sizeof(char16_t))||
	std::same_as<char_type,char8_t>||
	std::same_as<char_type,char32_t>)
inline constexpr std::size_t print_reserve_size(
	io_reserve_type_t<char_type,manipulators::cdcvt_t<details::qt_scatter>>,manipulators::cdcvt_t<details::qt_scatter> cdcvt) noexcept
{
	return details::print_reserve_size_qt_scatter_impl<char_type>(cdcvt.reference);
}

template<std::random_access_iterator Iter>
requires (std::same_as<std::iter_value_t<Iter>,char>||
	(std::same_as<std::iter_value_t<Iter>,wchar_t>&&sizeof(wchar_t)!=sizeof(char16_t))||
	std::same_as<std::iter_value_t<Iter>,char8_t>||
	std::same_as<std::iter_value_t<Iter>,char32_t>)
inline constexpr Iter print_reserve_define(
	io_reserve_type_t<std::iter_value_t<Iter>,manipulators::cdcvt_t<details::qt_scatter>>,
	Iter iter,
	manipulators::cdcvt_t<details::qt_scatter> cdcvt)
{
	return details::print_reserve_define_qt_scatter_impl(iter,cdcvt.reference);
}

template<std::integral char_type>
requires ((std::same_as<char_type,wchar_t>&&sizeof(wchar_t)==sizeof(char16_t))
	||std::same_as<char_type,char16_t>)
inline constexpr basic_io_scatter_t<char_type> print_scatter_define(print_scatter_type_t<char_type>,manipulators::cdcvt_t<details::qt_scatter> cdcvt)
{
	using may_alias_char_type_ptr
#if __has_cpp_attribute(gnu::may_alias)
	[[gnu::may_alias]]
#endif
	= char_type const*;
	may_alias_char_type_ptr
	 newptr{reinterpret_cast<may_alias_char_type_ptr>(cdcvt.reference.base)};
	return {newptr,cdcvt.reference.len};
}

}