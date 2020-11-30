#pragma once

#if _BSD_SOURCE || _SVID_SOURCE || (_POSIX_C_SOURCE >= 200809L || _XOPEN_SOURCE >= 700)
#include"posix.h"
#include"posix_at.h"
#elif _WIN32
#include"nt.h"
#endif


namespace fast_io
{
#if !defined(__NEWLIB__) && !defined(__MSDOS__)
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

#if __cpp_lib_filesystem >= 201703L

inline std::filesystem::path to_path(directory_entry de)
{
	auto nfm{filename(de)};
	std::basic_string_view<native_char_type> bsv{nfm};
	return std::filesystem::path(bsv);
}

template<std::integral char_type>
requires std::same_as<char_type,char>||std::same_as<char_type,native_char_type>
inline auto print_alias_define(io_alias_type_t<char_type>,std::filesystem::path const& pth)
{
	if constexpr(std::same_as<char_type,native_char_type>)
		return basic_io_scatter_t<native_char_type>{pth.native().data(),pth.native().size()};
	else
		return manip::code_cvt<basic_io_scatter_t<native_char_type>>{{pth.native().data(),pth.native().size()}};
}

#endif

#endif
}
