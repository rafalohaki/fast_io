#pragma once

namespace fast_io
{

enum class nt_at_flags:std::uint_least32_t
{
eaccess=static_cast<std::uint_least32_t>(1),
symlink_nofollow=static_cast<std::uint_least32_t>(1)<<1,
no_automount=static_cast<std::uint_least32_t>(1)<<2,
removedir=static_cast<std::uint_least32_t>(1)<<3,
empty_path=static_cast<std::uint_least32_t>(1)<<4
};


constexpr nt_at_flags operator&(nt_at_flags x, nt_at_flags y) noexcept
{
using utype = typename std::underlying_type<nt_at_flags>::type;
return static_cast<nt_at_flags>(static_cast<utype>(x) & static_cast<utype>(y));
}

constexpr nt_at_flags operator|(nt_at_flags x, nt_at_flags y) noexcept
{
using utype = typename std::underlying_type<nt_at_flags>::type;
return static_cast<nt_at_flags>(static_cast<utype>(x) | static_cast<utype>(y));
}

constexpr nt_at_flags operator^(nt_at_flags x, nt_at_flags y) noexcept
{
using utype = typename std::underlying_type<nt_at_flags>::type;
return static_cast<nt_at_flags>(static_cast<utype>(x) ^ static_cast<utype>(y));
}

constexpr nt_at_flags operator~(nt_at_flags x) noexcept
{
using utype = typename std::underlying_type<nt_at_flags>::type;
return static_cast<nt_at_flags>(~static_cast<utype>(x));
}

inline constexpr nt_at_flags& operator&=(nt_at_flags& x, nt_at_flags y) noexcept{return x=x&y;}

inline constexpr nt_at_flags& operator|=(nt_at_flags& x, nt_at_flags y) noexcept{return x=x|y;}

inline constexpr nt_at_flags& operator^=(nt_at_flags& x, nt_at_flags y) noexcept{return x=x^y;}

namespace win32::nt::details
{

inline constexpr nt_open_mode calculate_nt_delete_flag(nt_at_flags flags) noexcept
{
	nt_open_mode mode{
	.DesiredAccess=0x00010000,	//FILE_GENERIC_READ
	.FileAttributes=0x80,		//FILE_READ_ATTRIBUTES
	.ShareAccess=0x00000007,	//FILE_SHARE_DELETE
	.CreateDisposition=0x00000001, /*OPEN_EXISTING	=>	FILE_OPEN*/
	.CreateOptions=0x00001000		/*FILE_DELETE_ON_CLOSE*/
	};
	if((flags&nt_at_flags::symlink_nofollow)!=nt_at_flags::symlink_nofollow)
	{
		mode.CreateOptions |= 0x00200000;//FILE_FLAG_OPEN_REPARSE_POINT => FILE_OPEN_REPARSE_POINT (0x00200000)
	}
	if((flags&nt_at_flags::removedir)==nt_at_flags::removedir)
	{
		mode.CreateOptions |= 0x00004000;		//FILE_OPEN_FOR_BACKUP_INTENT
		mode.CreateOptions |= 0x00000001;		//FILE_DIRECTORY_FILE
	}
	else
	{
		mode.CreateOptions |= 0x00000040;		//FILE_NON_DIRECTORY_FILE 0x00000040
	}
	return mode;
}

template<bool zw>
inline void nt_unlinkat_impl(void* dirhd,wchar_t const* path_c_str,std::size_t path_size,nt_at_flags flags)
{
	auto status{nt_close<zw>(nt_call_callback(dirhd,path_c_str,path_size,nt_create_callback<zw>{calculate_nt_delete_flag(flags)}))};
	if(status)
		throw_nt_error(status);
}

template<bool zw,::fast_io::details::posix_api_1x dsp,typename... Args>
inline auto nt1x_api_dispatcher(void* dir_handle,wchar_t const* path_c_str,std::size_t path_size,Args... args)
{
#if 0
	if constexpr(dsp==::fast_io::details::posix_api_1x::faccessat)
		nt_faccessat_impl<zw>(dir_handle,path_c_str,path_size,args...);
	else if constexpr(dsp==::fast_io::details::posix_api_1x::fchownat)
		nt_fchownat_impl<zw>(dir_handle,path_c_str,path_size,args...);
	else if constexpr(dsp==::fast_io::details::posix_api_1x::fchmodat)
		nt_fchmodat_impl<zw>(dir_handle,path_c_str,path_size,args...);
	else if constexpr(dsp==::fast_io::details::posix_api_1x::fstatat)
		nt_fstatat_impl<zw>(dir_handle,path_c_str,path_size,args...);
	else if constexpr(dsp==::fast_io::details::posix_api_1x::mkdirat)
		nt_mkdirat_impl<zw>(dir_handle,path_c_str,path_size,args...);
	else if constexpr(dsp==::fast_io::details::posix_api_1x::mknodat)
		nt_mkdirat_impl<zw>(dir_handle,path_c_str,path_size,args...);
	else
#endif
	if constexpr(dsp==::fast_io::details::posix_api_1x::unlinkat)
		nt_unlinkat_impl<zw>(dir_handle,path_c_str,path_size,args...);
#if 0
	else if constexpr(dsp==::fast_io::details::posix_api_1x::unlinkat::utimensat)
		nt_utimensat_impl<zw>(dir_handle,path_c_str,path_size,args...);
#endif
}

template<nt_family family,::fast_io::details::posix_api_1x dsp,std::integral char_type,typename... Args>
inline auto nt_deal_with1x(
	void* dir_handle,
	char_type const* path_c_str,
	std::size_t path_size,
	Args... args)
{
	using wchar_t_may_alias_const_ptr
#if __has_cpp_attribute(gnu::may_alias)
	[[gnu::may_alias]]
#endif
	= wchar_t const*;
	if constexpr(sizeof(char_type)==2)
	{
		return nt1x_api_dispatcher<family==nt_family::zw,dsp>(dir_handle,reinterpret_cast<wchar_t_may_alias_const_ptr>(path_c_str),path_size,args...);
	}
	else
	{
		nt_api_encoding_converter converter(path_c_str,path_size);
		return nt1x_api_dispatcher<family==nt_family::zw,dsp>(dir_handle,converter.native_c_str(),converter.size(),args...);
	}
}

}

template<nt_family family,constructible_to_path path_type>
requires (family==nt_family::nt||family==nt_family::zw)
inline void nt_family_unlinkat(nt_at_entry ent,path_type&& path,nt_at_flags flags={})
{
	auto vw{::fast_io::details::to_its_cstring_view(path)};
	::fast_io::win32::nt::details::nt_deal_with1x<family,details::posix_api_1x::unlinkat>(ent.handle,vw.c_str(),vw.size(),flags);
}

template<constructible_to_path path_type>
inline void nt_unlinkat(nt_at_entry ent,path_type&& path,nt_at_flags flags={})
{
	auto vw{::fast_io::details::to_its_cstring_view(path)};
	::fast_io::win32::nt::details::nt_deal_with1x<nt_family::nt,details::posix_api_1x::unlinkat>(ent.handle,vw.c_str(),vw.size(),flags);
}

template<constructible_to_path path_type>
inline void zw_unlinkat(nt_at_entry ent,path_type&& path,nt_at_flags flags={})
{
	auto vw{::fast_io::details::to_its_cstring_view(path)};
	::fast_io::win32::nt::details::nt_deal_with1x<nt_family::zw,details::posix_api_1x::unlinkat>(ent.handle,vw.c_str(),vw.size(),flags);
}

#if !defined(__CYGWIN__) && !defined(__WINE__)
using native_at_flags = nt_at_flags;
template<constructible_to_path path_type>
inline void native_unlinkat(nt_at_entry ent,path_type&& path,native_at_flags flags={})
{
	auto vw{::fast_io::details::to_its_cstring_view(path)};
	::fast_io::win32::nt::details::nt_deal_with1x<nt_family::nt,::fast_io::details::posix_api_1x::unlinkat>(ent.handle,vw.c_str(),vw.size(),flags);
}
#endif
}
