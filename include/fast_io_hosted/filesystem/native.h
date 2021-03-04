#pragma once

#if _BSD_SOURCE || _SVID_SOURCE || (_POSIX_C_SOURCE >= 200809L || _XOPEN_SOURCE >= 700)
#include"posix.h"
#include"posix_at.h"
#elif _WIN32
#include"nt.h"
#endif

namespace fast_io
{
#if !defined(__NEWLIB__) && !defined(__MSDOS__) && (_BSD_SOURCE || _SVID_SOURCE || (_POSIX_C_SOURCE >= 200809L || _XOPEN_SOURCE >= 700) || _WIN32)

template<std::integral char_type>
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<char_type,directory_entry>,
	directory_entry ent) noexcept
{
	if constexpr(std::same_as<char_type,typename directory_entry::native_char_type>)
		return native_filename(ent).size();
	else if constexpr(std::same_as<char_type,char8_t>)
		return filename(ent).size();
	else
		return details::cal_full_reserve_size<
			sizeof(typename directory_entry::native_char_type),
			sizeof(char_type)>(native_filename(ent).size());
}

inline u8cstring_view extension(directory_entry ent) noexcept
{
	auto fnm{filename(ent)};
	auto pos{fnm.rfind(u8'.')};
	if(pos==static_cast<std::size_t>(-1))
		return {};
	if(pos==0)
		return {};
	if(2<fnm.size()&&pos==1&&fnm.front()==u8'.')
		return {};
	return u8cstring_view(null_terminated,fnm.data()+pos,fnm.data()+fnm.size());
}

inline std::u8string_view stem(directory_entry ent) noexcept
{
	std::u8string_view fnm{filename(ent)};
	auto pos{fnm.rfind(u8'.')};
	if(pos==static_cast<std::size_t>(-1))
		return fnm;
	if(pos==0)
		return fnm;
	if(2<fnm.size()&&pos==1&&fnm.front()==u8'.')
		return fnm;
	return fnm.substr(0,pos);
}

template<std::integral char_type>
requires ((std::same_as<char_type,char8_t>)||(std::same_as<char_type,directory_entry::native_char_type>))
inline basic_io_scatter_t<char_type> print_scatter_define(print_scatter_type_t<char_type>,directory_entry pth)
{
	if constexpr(std::same_as<char_type,char8_t>)
	{
		auto name{filename(pth)};
		return {name.data(),name.size()};
	}
	else
	{
		auto name{native_filename(pth)};
		return {name.data(),name.size()};
	}
}

template<std::random_access_iterator Iter>
inline constexpr Iter print_reserve_define(io_reserve_type_t<std::iter_value_t<Iter>,directory_entry>,
	Iter iter,directory_entry ent) noexcept
{
	using char_type = std::iter_value_t<Iter>;
	if constexpr(std::same_as<char_type,typename directory_entry::native_char_type>)
	{
		auto nfnm{native_filename(ent)};
		return details::non_overlapped_copy_n(nfnm.data(),nfnm.size(),iter);
	}
	else if constexpr(std::same_as<char_type,char8_t>)
	{
		auto fnm{filename(ent)};
		return details::non_overlapped_copy_n(fnm.data(),fnm.size(),iter);
	}
	else
	{
		auto fnm{filename(ent)};
		if constexpr(std::is_pointer_v<Iter>)
			return details::codecvt::general_code_cvt_full<encoding_scheme::utf>(fnm.data(),fnm.data()+fnm.size(),iter);
		else
			return iter+(details::codecvt::general_code_cvt_full<encoding_scheme::utf>(fnm.data(),fnm.data()+fnm.size(),std::to_address(iter))-std::to_address(iter));
	}
}

#if __cpp_lib_filesystem >= 201703L

inline std::filesystem::path to_path(directory_entry de)
{
	auto nfm{native_filename(de)};
	std::basic_string_view<native_char_type> bsv{nfm};
	return std::filesystem::path(bsv);
}

template<std::integral to_char_type>
inline std::size_t print_reserve_size(io_reserve_type_t<to_char_type,std::filesystem::path>,std::filesystem::path const& pth) noexcept
{
	if constexpr(std::same_as<to_char_type,std::filesystem::path::value_type>)
		return pth.native().size();
	else
		return details::cal_full_reserve_size<sizeof(std::filesystem::path::value_type),sizeof(to_char_type)>(pth.native().size());
}

template<std::contiguous_iterator Iter>
inline Iter print_reserve_define(io_reserve_type_t<std::iter_value_t<Iter>,std::filesystem::path>,Iter iter,std::filesystem::path const& pth) noexcept
{
	if constexpr(std::same_as<std::iter_value_t<Iter>,std::filesystem::path::value_type>)
		return details::non_overlapped_copy_n(pth.native().data(),pth.native().size(),iter);
	else if constexpr(std::is_pointer_v<Iter>)
		return details::codecvt::general_code_cvt_full<encoding_scheme::utf>(pth.native().data(),pth.native().data()+pth.native().size(),iter);
	else
		return iter+(details::codecvt::general_code_cvt_full<encoding_scheme::utf>(pth.native().data(),pth.native().data()+pth.native().size(),std::to_address(iter))-std::to_address(iter));
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
