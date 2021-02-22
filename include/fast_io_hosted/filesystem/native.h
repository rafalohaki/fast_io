#pragma once

#include"naive_vector.h"
#if _BSD_SOURCE || _SVID_SOURCE || (_POSIX_C_SOURCE >= 200809L || _XOPEN_SOURCE >= 700)
#include"posix.h"
#include"posix_at.h"
#elif _WIN32
#include"nt.h"
#endif

namespace fast_io
{
#if !defined(__NEWLIB__) && !defined(__MSDOS__) && (_BSD_SOURCE || _SVID_SOURCE || (_POSIX_C_SOURCE >= 200809L || _XOPEN_SOURCE >= 700) || _WIN32)

#if 0
template<std::integral char_type>
requires std::same_as<char_type,char>||std::same_as<char_type,native_char_type>
inline constexpr auto print_alias_define(io_alias_type_t<char_type>,directory_entry dir) noexcept
{
	auto nfm{filename(dir)};
	if constexpr(std::same_as<char_type,native_char_type>)
		return basic_io_scatter_t<native_char_type>{nfm.data(),nfm.size()};
	else
		return manip::code_cvt<basic_io_scatter_t<native_char_type>>{{nfm.data(),nfm.size()}};
}
#endif

#if __cpp_lib_filesystem >= 201703L

inline std::filesystem::path to_path(directory_entry de)
{
	auto nfm{filename(de)};
	std::basic_string_view<native_char_type> bsv{nfm};
	return std::filesystem::path(bsv);
}

template<std::integral to_char_type>
inline std::size_t print_reserve_size(io_reserve_type_t<to_char_type,std::filesystem::path>,std::filesystem::path const& pth) noexcept
{
	return details::cal_full_reserve_size<sizeof(std::filesystem::path::value_type),sizeof(to_char_type)>(pth.native().size());
}

template<std::contiguous_iterator Iter>
inline Iter print_reserve_define(io_reserve_type_t<std::iter_value_t<Iter>,std::filesystem::path>,Iter iter,std::filesystem::path const& pth) noexcept
{
	if constexpr(std::is_pointer_v<Iter>)
		return details::codecvt::general_code_cvt_full<encoding_scheme::utf>(pth.native().data(),pth.native().data()+pth.native().size(),iter);
	else
		return iter+(details::codecvt::general_code_cvt_full<encoding_scheme::utf>(pth.native().data(),pth.native().data()+pth.native().size(),std::to_address(iter))-std::to_address(iter));
}

template<std::integral char_type>
requires ((std::same_as<std::filesystem::path::value_type,char>&&std::same_as<char_type,char>&&std::same_as<char_type,char8_t>)||
((std::same_as<std::filesystem::path::value_type,wchar_t>&&sizeof(wchar_t)==sizeof(char16_t))&&std::same_as<char_type,wchar_t>&&std::same_as<char_type,char16_t>))
inline basic_io_scatter_t<char_type> print_scatter_define(print_scatter_type_t<char_type>,std::filesystem::path const& pth)
{
	if constexpr(std::same_as<std::filesystem::path::value_type,char_type>)
		return basic_io_scatter_t<char_type>{pth.native().data(),pth.native().size()};
	else
	{
		using char_type_may_alias_ptr
#if __has_cpp_attribute(gnu::may_alias)
		[[gnu::may_alias]]
#endif
		= char_type const*;
		char_type_may_alias_ptr aliasptr(reinterpret_cast<char_type_may_alias_ptr>(pth.native().data()));
		return basic_io_scatter_t<char_type>{aliasptr,pth.native().size()};
	}
}

#endif

#endif
}
