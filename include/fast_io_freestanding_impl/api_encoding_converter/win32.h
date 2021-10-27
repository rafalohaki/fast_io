#pragma once

namespace fast_io
{

namespace details
{
template<win32_family family,std::integral char_type,typename Func>
inline auto win32_family_api_common_code_cvt_impl(char_type const* filename_c_str,std::size_t filename_c_str_len,Func callback)
{
	win32_family_api_encoding_converter<family> converter(filename_c_str,filename_c_str_len);
	return callback(converter.native_c_str());
}
}

template<win32_family family,typename T,typename Func>
requires (::fast_io::constructible_to_os_c_str<T>)
inline auto win32_family_api_common(T const& t,Func callback)
{
	using family_char_type = std::conditional_t<family==win32_family::wide_nt,wchar_t,char>;
	using family_char_type_const_may_alias_ptr
#if __has_cpp_attribute(gnu::may_alias)
	[[gnu::may_alias]]
#endif
	= family_char_type const*;
	if constexpr(::std::is_array_v<T>)
	{
		using cstr_char_type = std::remove_extent_t<T>;
		auto p{t};
		if constexpr(sizeof(cstr_char_type)==sizeof(family_char_type))
		{
			return callback(reinterpret_cast<family_char_type_const_may_alias_ptr>(p));
		}
		else
		{
			return ::fast_io::details::win32_family_api_common_code_cvt_impl<family>(p,::fast_io::details::cal_array_size(t),callback);
		}
	}
	else
	{
		using cstr_char_type = std::remove_pointer_t<decltype(t.c_str())>;
		if constexpr(sizeof(cstr_char_type)==sizeof(family_char_type_const_may_alias_ptr))
		{
			return ::fast_io::details::win32_family_api_common_code_cvt_impl<family>(reinterpret_cast<family_char_type_const_may_alias_ptr>(t.c_str()),callback);
		}
		else
		{
#if __STDC_HOSTED__==1 && (!defined(_GLIBCXX_HOSTED) || _GLIBCXX_HOSTED==1) && __has_include(<ranges>)
			if constexpr(::std::ranges::contiguous_range<std::remove_cvref_t<T>>)
			{
				return ::fast_io::details::win32_family_api_common_code_cvt_impl<family>(::std::ranges::data(t),::std::ranges::size(t),callback);
			}
			else
#endif
			{
				auto ptr{t.c_str()};
				return ::fast_io::details::win32_family_api_common_code_cvt_impl<family>(ptr,::fast_io::cstr_len(ptr),callback);
			}
		}
	}
}

}
