#pragma once

#if (!defined(__NEWLIB__)||defined(__CYGWIN__)) && !defined(_WIN32) && !defined(__MSDOS__)
#include"posix.h"
#include"posix_at.h"
#endif

#if defined(_WIN32) || defined(__CYGWIN__)
#include"nt.h"
#endif

namespace fast_io
{
#if (!defined(__NEWLIB__)||defined(__CYGWIN__)) && !defined(__MSDOS__)

#if 0

#if defined(_WIN32) || defined(__CYGWIN__)
inline std::filesystem::path to_path(nt_directory_entry de)
{
	auto nfm{native_filename(de)};
	::fast_io::freestanding::basic_string_view<wchar_t> bsv{nfm};
	return std::filesystem::path(bsv);
}
#endif

#ifndef _WIN32
inline std::filesystem::path to_path(posix_directory_entry de)
{
	auto nfm{native_filename(de)};
	::fast_io::freestanding::basic_string_view<char> bsv{nfm};
	return std::filesystem::path(bsv);
}
#endif

template<std::integral to_char_type>
inline std::size_t print_reserve_size(io_reserve_type_t<to_char_type,std::filesystem::path>,std::filesystem::path const& pth) noexcept
{
	if constexpr(std::same_as<to_char_type,std::filesystem::path::value_type>)
		return pth.native().size();
	else
		return details::cal_full_reserve_size<sizeof(std::filesystem::path::value_type),sizeof(to_char_type)>(pth.native().size());
}

template<::fast_io::freestanding::contiguous_iterator Iter>
inline Iter print_reserve_define(io_reserve_type_t<::fast_io::freestanding::iter_value_t<Iter>,std::filesystem::path>,Iter iter,std::filesystem::path const& pth) noexcept
{
	if constexpr(std::same_as<::fast_io::freestanding::iter_value_t<Iter>,std::filesystem::path::value_type>)
		return details::non_overlapped_copy_n(pth.native().data(),pth.native().size(),iter);
	else if constexpr(std::is_pointer_v<Iter>)
		return details::codecvt::general_code_cvt_full<encoding_scheme::utf>(pth.native().data(),pth.native().data()+pth.native().size(),iter);
	else
		return iter+(details::codecvt::general_code_cvt_full<encoding_scheme::utf>(pth.native().data(),pth.native().data()+pth.native().size(),::fast_io::freestanding::to_address(iter))-::fast_io::freestanding::to_address(iter));
}

template<std::integral char_type>
requires ((std::same_as<std::filesystem::path::value_type,char>&&(std::same_as<char_type,char>||std::same_as<char_type,char8_t>))||
((std::same_as<std::filesystem::path::value_type,wchar_t>&&sizeof(wchar_t)==sizeof(char16_t))&&(std::same_as<char_type,wchar_t>||std::same_as<char_type,char16_t>)))
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
