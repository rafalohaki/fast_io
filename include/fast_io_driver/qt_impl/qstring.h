#pragma once

namespace fast_io
{
namespace manipulators
{

template<typename T>
requires std::same_as<std::remove_cvref_t<T>,QStringView>
inline constexpr cdcvt_t<QStringView> cdcvt(T t) noexcept
{
	return {t};
}

template<typename T>
requires (std::same_as<std::remove_cvref_t<T>,QString>||
	std::same_as<std::remove_cvref_t<T>,QStringRef>)
inline constexpr cdcvt_t<QStringView> cdcvt(T&& t) noexcept
{
	return {t};
}

}

namespace details
{
template<std::integral char_type>
inline constexpr std::size_t print_reserve_size_qstring_view_impl(QStringView vw) noexcept
{
	if constexpr(exec_charset_is_ebcdic<char_type>())
		fast_terminate();
	if constexpr(sizeof(qsizetype)==sizeof(std::size_t))
	{
		if constexpr(sizeof(char_type)==1)
		{
			if((SIZE_MAX>>2)<vw.size())
				fast_terminate();
			return static_cast<std::size_t>(vw.size())<<2;
		}
		else
			return static_cast<std::size_t>(vw.size());
	}
	else
	{

		if constexpr(sizeof(char_type)==1)
		{
			if((SIZE_MAX>>2)<vw.size())
				fast_terminate();
			return static_cast<std::size_t>(vw.size());
		}
		else
		{
			if(SIZE_MAX<vw.size())
				fast_terminate();
			return static_cast<std::size_t>(vw.size());
		}
	}
}

template<std::random_access_iterator Iter>
inline constexpr Iter print_reserve_define_qstring_view_impl(Iter iter,QStringView vw)
{
	using char_type = std::iter_value_t<Iter>;
	char_type* ptr(std::to_address(iter));
	using may_alias_char_type_ptr
#if __has_cpp_attribute(gnu::may_alias)
	[[gnu::may_alias]]
#endif
	= char16_t const*;
	may_alias_char_type_ptr newptr{reinterpret_cast<may_alias_char_type_ptr>(vw.data())};
	std::size_t sz{static_cast<std::size_t>(vw.size())};
	char_type* ptr_after{utf::utf_code_convert_details<false>(newptr,newptr+sz,ptr)};
	return iter+(ptr_after-ptr);
}

}

template<std::integral char_type>
requires (std::same_as<char_type,char>||
	(std::same_as<char_type,wchar_t>&&sizeof(wchar_t)!=sizeof(char16_t))||
	std::same_as<char_type,char8_t>||
	std::same_as<char_type,char32_t>)
inline constexpr std::size_t print_reserve_size(
	io_reserve_type_t<char_type,manipulators::cdcvt_t<QStringView>>,manipulators::cdcvt_t<QStringView> cdcvt) noexcept
{
	return details::print_reserve_size_qstring_view_impl<char_type>(cdcvt.reference);
}

template<std::random_access_iterator Iter>
requires (std::same_as<std::iter_value_t<Iter>,char>||
	(std::same_as<std::iter_value_t<Iter>,wchar_t>&&sizeof(wchar_t)!=sizeof(char16_t))||
	std::same_as<std::iter_value_t<Iter>,char8_t>||
	std::same_as<std::iter_value_t<Iter>,char32_t>)
inline constexpr Iter print_reserve_define(
	io_reserve_type_t<std::iter_value_t<Iter>,manipulators::cdcvt_t<QStringView>>,
	Iter iter,
	manipulators::cdcvt_t<QStringView> cdcvt)
{
	return details::print_reserve_define_qstring_view_impl(iter,cdcvt.reference);
}

template<std::integral char_type>
requires ((std::same_as<char_type,wchar_t>&&sizeof(wchar_t)==sizeof(char16_t))
	||std::same_as<char_type,char16_t>)
inline constexpr basic_io_scatter_t<char_type> print_scatter_define(print_scatter_type_t<char_type>,manipulators::cdcvt_t<QStringView> cdcvt)
{
	using may_alias_char_type_ptr
#if __has_cpp_attribute(gnu::may_alias)
	[[gnu::may_alias]]
#endif
	= char_type const*;
	may_alias_char_type_ptr
	 newptr{reinterpret_cast<may_alias_char_type_ptr>(cdcvt.reference.data())};
	if constexpr(sizeof(qsizetype)>sizeof(std::size_t))
	{
		if(cdcvt.reference.size()>SIZE_MAX)
			fast_terminate();
	}
	std::size_t sz{static_cast<std::size_t>(cdcvt.reference.size())};
	return {newptr,sz};
}

}