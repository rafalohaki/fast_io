#pragma once

namespace fast_io
{

namespace details
{

template<constructible_to_path path_type>
inline constexpr auto to_its_cstring_view(path_type&& pth) noexcept
{
	if constexpr(std::constructible_from<cstring_view,path_type>)
	{
		return cstring_view(pth);
	}
	else if constexpr(std::constructible_from<u8cstring_view,path_type>)
	{
		return u8cstring_view(pth);
	}
	else if constexpr(std::constructible_from<u16cstring_view,path_type>)
	{
		return u16cstring_view(pth);
	}
	else if constexpr(std::constructible_from<u32cstring_view,path_type>)
	{
		return u32cstring_view(pth);
	}
	else
		return wcstring_view(pth);
}

enum class posix_api_22
{
renameat,
linkat
};
enum class posix_api_12
{
symlinkat
};
enum class posix_api_1x
{
faccessat,
utimensat,
fchmodat,
fchownat,
fstatat,
mkdirat,
mknodat,
unlinkat
};

#if defined(__CYGWIN__)
extern int renameat(int olddirfd,char const* oldpath,int newdirfd, char const* newpath) noexcept asm("renameat");
extern int linkat(int olddirfd,char const* oldpath,int newdirfd, char const* newpath,int flags) noexcept asm("linkat");
extern int symlinkat(char const* oldpath, int newdirfd, char const *newpath) noexcept asm("symlinkat");
extern int fchmodat(int dirfd, char const *pathname, mode_t mode, int flags) noexcept asm("fchmodat");
extern int utimensat(int dirfd, char const *pathname,struct timespec const times[2], int flags) noexcept asm("utimensat");
extern int fchownat(int dirfd, char const *pathname,uid_t owner, gid_t group, int flags) noexcept asm("fchownat");
extern int fstatat(int dirfd, char const *pathname, struct stat *buf,int flags) noexcept asm("fstatat");
extern int mkdirat(int dirfd, char const* pathname, mode_t mode) noexcept asm("mkdirat");
extern int mknodat(int dirfd, char const* pathname, mode_t mode, dev_t dev) noexcept asm("mknodat");
extern int unlinkat(int dirfd, char const*pathname, int flags) noexcept asm("unlinkat");
extern int readlinkat(int dirfd, char const *pathname,char *buf, size_t bufsiz) noexcept asm("readlinkat");
#endif

inline void posix_renameat_impl(int olddirfd,char const* oldpath,
	int newdirfd,char const* newpath)
{
	system_call_throw_error(
#if defined(__linux__) && defined(__NR_renameat)
	system_call<__NR_renameat,int>
#else
	renameat
#endif
	(olddirfd,oldpath,newdirfd,newpath));
}

inline void posix_linkat_impl(int olddirfd,char const* oldpath,
	int newdirfd,char const* newpath,int flags)
{
	system_call_throw_error(
#if defined(__linux__)
	system_call<__NR_linkat,int>
#else
	linkat
#endif
	(olddirfd,oldpath,newdirfd,newpath,flags));
}

template<posix_api_22 dsp,typename... Args>
inline auto posix22_api_dispatcher(int olddirfd,char const* oldpath,
	int newdirfd,char const* newpath,Args... args)
{
	if constexpr(dsp==posix_api_22::renameat)
	{
		static_assert(sizeof...(Args)==0);
		posix_renameat_impl(olddirfd,oldpath,newdirfd,newpath);
	}
	else if constexpr(dsp==posix_api_22::linkat)
	{
		posix_linkat_impl(olddirfd,oldpath,newdirfd,newpath,args...);
	}
}
template<posix_api_12 dsp>
inline auto posix12_api_dispatcher(char const* oldpath,
	int newdirfd,char const* newpath)
{
	if constexpr(dsp==posix_api_12::symlinkat)
	{
		system_call_throw_error(
#if defined(__linux__)
		system_call<__NR_symlinkat,int>
#else
		symlinkat
#endif
		(oldpath,newdirfd,newpath));
	}
}

inline void posix_fchownat_impl(int dirfd, const char *pathname, uintmax_t owner, uintmax_t group, int flags)
{
	if constexpr(sizeof(uintmax_t)>sizeof(uid_t))
	{
		constexpr std::uintmax_t mx{std::numeric_limits<uid_t>::max()};
		if(static_cast<std::uintmax_t>(owner)>mx)
			throw_posix_error(EOVERFLOW);
	}
	if constexpr(sizeof(uintmax_t)>sizeof(gid_t))
	{
		constexpr std::uintmax_t mx{std::numeric_limits<gid_t>::max()};
		if(static_cast<std::uintmax_t>(owner)>mx)
			throw_posix_error(EOVERFLOW);
	}
	system_call_throw_error(
#if defined(__linux__)
	system_call<__NR_fchownat,int>
#else
	fchownat
#endif
	(dirfd,pathname,owner,group,flags));
}

inline void posix_fchmodat_impl(int dirfd, const char *pathname, mode_t mode, int flags)
{
	system_call_throw_error(
#if defined(__linux__)
	system_call<__NR_fchmodat,int>
#else
	fchmodat
#endif
	(dirfd,pathname,mode,flags));
}

inline posix_file_status posix_fstatat_impl(int dirfd, const char *pathname, int flags)
{
#if defined(__linux__)
	struct stat64 buf;
#else
	struct stat buf;
#endif
	system_call_throw_error(
#if defined(__linux__)
	system_call<
#if defined(__NR_newfstatat)
	__NR_newfstatat
#else
	__NR_fstatat64
#endif
	,int>
#else
	fstatat
#endif
	(dirfd,pathname,std::addressof(buf),flags));
	return struct_stat_to_posix_file_status(buf);
}

inline void posix_mkdirat_impl(int dirfd, const char *pathname, mode_t mode)
{
	system_call_throw_error(
#if defined(__linux__)
	system_call<
	__NR_mkdirat
	,int>
#else
	mkdirat
#endif
	(dirfd,pathname,mode));
}

inline void posix_mknodat_impl(int dirfd, const char *pathname, mode_t mode,std::uintmax_t dev)
{
#if defined(__DARWIN_C_LEVEL)
	throw_posix_error(ENOTSUP);
#else
	if constexpr(sizeof(std::uintmax_t)>sizeof(dev_t))
	{
		constexpr std::uintmax_t mx{std::numeric_limits<dev_t>::max()};
		if(static_cast<std::uintmax_t>(dev)>mx)
			throw_posix_error(EOVERFLOW);
	}
	system_call_throw_error(
#if defined(__linux__)
	system_call<
#if __NR_mknodat
	__NR_mknodat
#endif
	,int>
#else
	mknodat
#endif
	(dirfd,pathname,mode,dev));
#endif
}

inline void posix_unlinkat_impl(int dirfd,char const* path,int flags)
{
	system_call_throw_error(
#if defined(__linux__)
	system_call<__NR_unlinkat,int>
#else
	unlinkat
#endif
	(dirfd,path,flags));
}

namespace details
{
inline constexpr struct timespec unix_timestamp_to_struct_timespec64(unix_timestamp stmp) noexcept
{
	constexpr uintiso_t mul_factor{uintiso_subseconds_per_second/1000000000u};
	return {static_cast<std::time_t>(stmp.seconds),static_cast<long>(static_cast<long unsigned>((stmp.subseconds)/mul_factor))};
}

inline constexpr struct timespec unix_timestamp_to_struct_timespec64(unix_timestamp_option opt) noexcept
{
	switch(opt.flags)
	{
	case utime_flags::now:
		return {.tv_sec=0,.tv_nsec=UTIME_NOW};
	case utime_flags::omit:
		return {.tv_sec=0,.tv_nsec=UTIME_OMIT};
	default:
		return unix_timestamp_to_struct_timespec64(opt.timestamp);
	}
}

}

inline void posix_utimensat_impl(int dirfd,char const* path,
unix_timestamp_option creation_time,
unix_timestamp_option last_access_time,
unix_timestamp_option last_modification_time,
int flags)
{
	if(creation_time.flags!=utime_flags::omit)
		throw_posix_error(EINVAL);
	struct timespec ts[2]{
		details::unix_timestamp_to_struct_timespec64(last_access_time),
		details::unix_timestamp_to_struct_timespec64(last_modification_time),
	};
	struct timespec* tsptr{ts};
	system_call_throw_error(
#if defined(__linux__)

#if defined(__NR_utimensat64)
	system_call<__NR_utimensat64,int>
#else
	system_call<__NR_utimensat,int>
#endif

#else
	utimensat
#endif
	(dirfd,path,tsptr,flags));
}

template<posix_api_1x dsp,typename... Args>
inline auto posix1x_api_dispatcher(int dirfd,char const* path,Args... args)
{
	if constexpr(dsp==posix_api_1x::faccessat)
		posix_faccessat_impl(dirfd,path,args...);
	else if constexpr(dsp==posix_api_1x::fchownat)
		posix_fchownat_impl(dirfd,path,args...);
	else if constexpr(dsp==posix_api_1x::fchmodat)
		posix_fchmodat_impl(dirfd,path,args...);
	else if constexpr(dsp==posix_api_1x::fstatat)
		posix_fstatat_impl(dirfd,path,args...);
	else if constexpr(dsp==posix_api_1x::mkdirat)
		posix_mkdirat_impl(dirfd,path,args...);
	else if constexpr(dsp==posix_api_1x::mknodat)
		posix_mkdirat_impl(dirfd,path,args...);
	else if constexpr(dsp==posix_api_1x::unlinkat)
		posix_unlinkat_impl(dirfd,path,args...);
	else if constexpr(dsp==posix_api_1x::utimensat)
		posix_utimensat_impl(dirfd,path,args...);
}

template<posix_api_22 dsp,std::integral char_type2>
inline auto posix_deal_with22(int olddirfd,char const* oldpath,
	int newdirfd,basic_cstring_view<char_type2> newpath)
{
	if constexpr(sizeof(char_type2)==1)
	{
		return posix22_api_dispatcher<dsp>(olddirfd,oldpath,newdirfd,reinterpret_cast<char const*>(newpath.c_str()));
	}
	else
	{
		posix_path_dealer newpdealer(newpath.data(),newpath.size());
		return posix22_api_dispatcher<dsp>(olddirfd,oldpath,newdirfd,newpdealer.c_str());
	}
}

template<posix_api_22 dsp,std::integral char_type1,std::integral char_type2,typename... Args>
inline auto posix_deal_with22(int olddirfd,basic_cstring_view<char_type1> oldpath,
	int newdirfd,basic_cstring_view<char_type2> newpath,Args... args)
{
	if constexpr(sizeof(char_type1)==1&&sizeof(char_type2)==1)
	{
		return posix22_api_dispatcher<dsp>(olddirfd,reinterpret_cast<char const*>(oldpath.c_str()),
		newdirfd,reinterpret_cast<char const*>(newpath.c_str()));
	}
	else if constexpr(sizeof(char_type1)==1&&sizeof(char_type2)!=1)
	{

		posix_path_dealer dealer(newpath.data(),newpath.size());
		return posix22_api_dispatcher<dsp>(olddirfd,reinterpret_cast<char const*>(oldpath.c_str()),
		newdirfd,dealer.c_str());
	}
	else if constexpr(sizeof(char_type1)!=1&&sizeof(char_type2)==1)
	{
		posix_path_dealer opdealer(oldpath.data(),oldpath.size());
		return posix22_api_dispatcher<dsp>(olddirfd,opdealer.c_str(),
		newdirfd,reinterpret_cast<char const*>(newpath.c_str()));
	}
	else
	{
		posix_path_dealer opdealer(oldpath.data(),oldpath.size());
		posix_path_dealer newpdealer(newpath.data(),newpath.size());
		return posix22_api_dispatcher<dsp>(olddirfd,opdealer.c_str(),
		newdirfd,newpdealer.c_str());
	}
}

template<posix_api_12 dsp,std::integral char_type1,std::integral char_type2>
inline auto posix_deal_with12(
	basic_cstring_view<char_type1> oldpath,
	int newdirfd,
	basic_cstring_view<char_type2> newpath)
{
	if constexpr(sizeof(char_type1)==1&&sizeof(char_type2)==1)
	{
		return posix12_api_dispatcher<dsp>(reinterpret_cast<char const*>(oldpath.c_str()),
		newdirfd,reinterpret_cast<char const*>(newpath.c_str()));
	}
	else if constexpr(sizeof(char_type1)==1&&sizeof(char_type2)!=1)
	{

		posix_path_dealer dealer(newpath.data(),newpath.size());
		return posix12_api_dispatcher<dsp>(reinterpret_cast<char const*>(oldpath.c_str()),
		newdirfd,dealer.c_str());
	}
	else if constexpr(sizeof(char_type1)!=1&&sizeof(char_type2)==1)
	{
		posix_path_dealer opdealer(oldpath.data(),oldpath.size());
		return posix12_api_dispatcher<dsp>(opdealer.c_str(),
		newdirfd,reinterpret_cast<char const*>(newpath.c_str()));
	}
	else
	{
		posix_path_dealer opdealer(oldpath.data(),oldpath.size());
		posix_path_dealer newpdealer(newpath.data(),newpath.size());
		return posix12_api_dispatcher<dsp>(opdealer.c_str(),
		newdirfd,newpdealer.c_str());
	}
}


template<posix_api_1x dsp,std::integral char_type1,typename... Args>
inline auto posix_deal_with1x(
	int dirfd,
	basic_cstring_view<char_type1> path,
	Args... args)
{
	if constexpr(sizeof(char_type1)==1)
	{
		return posix1x_api_dispatcher<dsp>(dirfd,reinterpret_cast<char const*>(path.c_str()),
		args...);
	}
	else
	{
		posix_path_dealer dealer(path.data(),path.size());
		return posix1x_api_dispatcher<dsp>(dirfd,dealer.c_str(),args...);
	}
}

}

template<constructible_to_path old_path_type,
	constructible_to_path new_path_type>
inline void posix_renameat(native_at_entry oldent,
	old_path_type&& oldpath,
	native_at_entry newent,
	new_path_type&& newpath)
{
	details::posix_deal_with22<details::posix_api_22::renameat>(
		oldent.fd,details::to_its_cstring_view(oldpath),
		newent.fd,details::to_its_cstring_view(newpath));
}

template<constructible_to_path new_path_type>
inline void posix_renameat(posix_fs_dirent fs_dirent,
	native_at_entry newent,
	new_path_type&& newpath)
{
	details::posix_deal_with22<details::posix_api_22::renameat>(
		fs_dirent.fd,fs_dirent.filename,
		newent.fd,details::to_its_cstring_view(newpath));
}

template<constructible_to_path old_path_type,
	constructible_to_path new_path_type>
inline void posix_symlinkat(old_path_type&& oldpath,
	native_at_entry newent,
	new_path_type&& newpath)
{
	details::posix_deal_with12<details::posix_api_12::symlinkat>(
		details::to_its_cstring_view(oldpath),
		newent.fd,details::to_its_cstring_view(newpath));
}

enum class at_flags
{
eaccess=
#ifdef AT_EACCESS
AT_EACCESS
#else
0
#endif
,
symlink_nofollow=
#ifdef AT_SYMLINK_NOFOLLOW
AT_SYMLINK_NOFOLLOW
#else
0
#endif
,
no_automount=
#ifdef AT_NO_AUTOMOUNT
AT_NO_AUTOMOUNT
#else
0
#endif
,
removedir=
#ifdef AT_REMOVEDIR
AT_REMOVEDIR
#else
0
#endif
,
empty_path=
#ifdef AT_EMPTY_PATH
AT_EMPTY_PATH
#else
0
#endif
};

constexpr at_flags operator&(at_flags x, at_flags y) noexcept
{
using utype = typename std::underlying_type<at_flags>::type;
return static_cast<at_flags>(static_cast<utype>(x) & static_cast<utype>(y));
}

constexpr at_flags operator|(at_flags x, at_flags y) noexcept
{
using utype = typename std::underlying_type<at_flags>::type;
return static_cast<at_flags>(static_cast<utype>(x) | static_cast<utype>(y));
}

constexpr at_flags operator^(at_flags x, at_flags y) noexcept
{
using utype = typename std::underlying_type<at_flags>::type;
return static_cast<at_flags>(static_cast<utype>(x) ^ static_cast<utype>(y));
}

constexpr at_flags operator~(at_flags x) noexcept
{
using utype = typename std::underlying_type<at_flags>::type;
return static_cast<at_flags>(~static_cast<utype>(x));
}

inline constexpr at_flags& operator&=(at_flags& x, at_flags y) noexcept{return x=x&y;}

inline constexpr at_flags& operator|=(at_flags& x, at_flags y) noexcept{return x=x|y;}

inline constexpr at_flags& operator^=(at_flags& x, at_flags y) noexcept{return x=x^y;}

template<constructible_to_path path_type>
inline void posix_faccessat(native_at_entry ent,path_type&& path,
	access_how mode,at_flags flags=at_flags::symlink_nofollow)
{
	details::posix_deal_with1x<details::posix_api_1x::faccessat>(ent.fd,details::to_its_cstring_view(path),static_cast<int>(mode),static_cast<int>(flags));
}

template<constructible_to_path path_type>
inline void posix_fchmodat(native_at_entry ent,path_type&& path,
	perms mode,at_flags flags=at_flags::symlink_nofollow)
{
	details::posix_deal_with1x<details::posix_api_1x::fchmodat>(ent.fd,details::to_its_cstring_view(path),static_cast<int>(mode),static_cast<int>(flags));
}

template<constructible_to_path path_type>
inline void posix_fchownat(native_at_entry ent,path_type&& path,std::uintmax_t owner,std::uintmax_t group,at_flags flags=at_flags::symlink_nofollow)
{
	details::posix_deal_with1x<details::posix_api_1x::fchownat>(ent.fd,details::to_its_cstring_view(path),owner,group,static_cast<int>(flags));
}

template<constructible_to_path path_type>
inline posix_file_status posix_fstatat(native_at_entry ent,path_type&& path,at_flags flags=at_flags::symlink_nofollow)
{
	return details::posix_deal_with1x<details::posix_api_1x::fstatat>(ent.fd,details::to_its_cstring_view(path),static_cast<int>(flags));
}

template<constructible_to_path path_type>
inline void posix_mkdirat(native_at_entry ent,path_type&& path,perms perm=static_cast<perms>(436))
{
	return details::posix_deal_with1x<details::posix_api_1x::mkdirat>(ent.fd,details::to_its_cstring_view(path),static_cast<mode_t>(perm));
}

template<constructible_to_path path_type>
inline void posix_mknodat(native_at_entry ent,path_type&& path,perms perm,std::uintmax_t dev)
{
	return details::posix_deal_with1x<details::posix_api_1x::mknodat>(ent.fd,details::to_its_cstring_view(path),static_cast<mode_t>(perm),dev);
}

template<constructible_to_path path_type>
inline void posix_unlinkat(native_at_entry ent,path_type&& path,at_flags flags={})
{
	details::posix_deal_with1x<details::posix_api_1x::unlinkat>(ent.fd,details::to_its_cstring_view(path),static_cast<int>(flags));
}

template<constructible_to_path old_path_type,constructible_to_path new_path_type>
inline void posix_linkat(native_at_entry oldent,old_path_type&& oldpath,native_at_entry newent,new_path_type&& newpath,at_flags flags=at_flags::symlink_nofollow)
{
	details::posix_deal_with22<details::posix_api_22::linkat>(oldent.fd,details::to_its_cstring_view(oldpath),
	newent.fd,details::to_its_cstring_view(newpath),static_cast<int>(flags));
}

template<constructible_to_path path_type>
inline void posix_utimensat(native_at_entry ent,path_type&& path,
	unix_timestamp_option creation_time,
	unix_timestamp_option last_access_time,
	unix_timestamp_option last_modification_time,
	at_flags flags=at_flags::symlink_nofollow)
{
	details::posix_deal_with1x<details::posix_api_1x::utimensat>(ent.fd,details::to_its_cstring_view(path),
	creation_time,last_access_time,last_modification_time,static_cast<int>(flags));
}


template<std::integral ch_type>
struct basic_posix_readlinkat_t
{
	using char_type = ch_type;
	int dirfd{-1};
	std::conditional_t<sizeof(ch_type)==1,char_type const*,basic_cstring_view<ch_type>> path;
};

template<constructible_to_path path_type>
inline constexpr auto posix_readlinkat(
	posix_at_entry ent,
	path_type&& path) noexcept
{
	auto strvw{details::to_its_cstring_view(path)};
	using value_type = typename std::remove_cvref_t<decltype(strvw)>::value_type;
	if constexpr(sizeof(value_type)==1)
		return basic_posix_readlinkat_t<value_type>{ent.fd,strvw.c_str()};
	else
		return basic_posix_readlinkat_t<value_type>{ent.fd,strvw};
}

namespace details
{

inline constexpr std::size_t read_linkbuffer_size() noexcept
{
#if defined(PATH_MAX)
	if constexpr(PATH_MAX<4096)
		return 4096;
	else
		return PATH_MAX;
#else
	return 4096;
#endif
}

inline std::size_t posix_readlinkat_common_impl(int dirfd,char const* pathname,char* buffer)
{
	constexpr std::size_t buffer_size{read_linkbuffer_size()};
	std::ptrdiff_t bytes{
#if defined(__linux__)
	system_call<
#if __NR_readlinkat
	__NR_readlinkat
#endif
	,int>
#else
	readlinkat
#endif
	(dirfd,pathname,buffer,buffer_size)
	};
	system_call_throw_error(bytes);
	return static_cast<std::size_t>(bytes);
}

template<std::integral path_char_type>
inline std::size_t read_linkat_impl_phase2(char* dst,basic_posix_readlinkat_t<path_char_type> rlkat)
{
	if constexpr(sizeof(path_char_type)==1)
	{
		return posix_readlinkat_common_impl(rlkat.dirfd,reinterpret_cast<char const*>(rlkat.path),dst);
	}
	else
	{
		posix_path_dealer dealer(rlkat.path.data(),rlkat.path.size());
		return posix_readlinkat_common_impl(rlkat.dirfd,dealer.c_str(),dst);
	}
}

template<
std::integral to_char_type,
std::integral path_char_type>
inline to_char_type* read_linkat_impl_phase1(to_char_type* dst,basic_posix_readlinkat_t<path_char_type> rlkat)
{
	if constexpr(sizeof(path_char_type)==1)
	{
		return dst+read_linkat_impl_phase2(reinterpret_cast<char*>(dst),rlkat);
	}
	else
	{
		constexpr std::size_t buffer_size{read_linkbuffer_size()};
		local_operator_new_array_ptr<char8_t> dynamic_buffer(buffer_size);
		std::size_t bytes{read_linkat_impl_phase2(reinterpret_cast<char*>(dynamic_buffer.ptr),rlkat)};
		return codecvt::general_code_cvt_full(dynamic_buffer.ptr,dynamic_buffer.ptr+bytes,dst);
	}
}

}


template<std::integral char_type,std::integral path_char_type>
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<char_type,basic_posix_readlinkat_t<path_char_type>>,
	basic_posix_readlinkat_t<path_char_type>) noexcept
{
	if constexpr(sizeof(char_type)==1)
		return details::read_linkbuffer_size();
	else
	{
		constexpr std::size_t sz{details::read_linkbuffer_size()};
		constexpr std::size_t decorated_size{details::cal_decorated_reserve_size<1,sizeof(char_type)>(sz)};
		return decorated_size;
	}
}

template<std::contiguous_iterator Iter,std::integral char_type>
inline constexpr Iter print_reserve_define(io_reserve_type_t<std::iter_value_t<Iter>,
	basic_posix_readlinkat_t<char_type>>,
	Iter iter,
	basic_posix_readlinkat_t<char_type> rlkat)
{
	return details::read_linkat_impl_phase1(iter,rlkat);
}


}