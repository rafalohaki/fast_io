﻿#pragma once

namespace fast_io
{

/*
Referenced from Windows Internal Version 7
*/
enum class nt_family
{
alpc,	//Alpc: Advanced Local Procedure Calls
cc,	//Cc: Common Cache
cm,	//Cm: Configuration manager
dbg,	//Dbg: kernel debug support
dbgk,	//Dbgk: Debugging Framework for user mode
em,	//Em: Errata manager
etw,	//Event: Event Tracing for Windows
ex,	//Executive support routines
fs_rtl,	//FsRtl: File System Runtime Library
hv,	//Hv: Hive library
hvl,	//Hvl: Hypervisor Library
io,	//Io: I/O manager
kd,	//Kd: Kernel debugger
ke,	//Ke: Kernel
kse,	//Kse: Kernel Shim Engine
lsa,	//Lsa: Local Security Authority
mm,	//Mm: Memory manager
nt,	//Nt: NT system services (accessible from user mode through system calls)
ob,	//Ob: Object manager
pf,	//Pf: Prefetcher
po,	//Po: Power manager
po_fx,	//PoFx: Power framework
pp,	//Pp: PnP manager
ppm,	//Ppm: Processor power manager
ps,	//Ps: Process support
rtl,	//Rtl: Run-time library
se,	//Se: Security Reference Monitor
sm,	//Sm: Store Manager
tm,	//Tm: Transaction
ttm,	//Ttm: Terminal timeout manager
vf,	//Vf: Driver verifier
vsl,	//Vsl: Virtual secure mode library
wdi,	//Wdi: Windows Diagnostic Infrastructure
wfp,	//Wfp: Windows FingerPrint
whea,	//Whea: Windows Hardware Error Architecture
wmi,	//Wmi: Windows Management Instrumentation
zw	//Zw: Mirror entry point for system services (beginning with Nt) that sets previous access mode to kernel, which eliminates parameter validation, because Nt system services validate parameters only if previous access mode is user
};

namespace win32::nt::details
{

struct nt_open_mode
{
std::uint32_t DesiredAccess{};
std::uint32_t FileAttributes{};
std::uint32_t ShareAccess{1|2};
std::uint32_t CreateDisposition{};
std::uint32_t CreateOptions{};
std::uint32_t ObjAttributes{};
};

/*
https://docs.microsoft.com/en-us/windows/win32/secauthz/access-mask-format
*/

inline constexpr nt_open_mode calculate_nt_open_mode(open_mode_perms ompm) noexcept
{
	open_mode value{ompm.om};
	perms pm{ompm.pm};
	nt_open_mode mode;
	bool generic_write{};
	if((value&open_mode::app)!=open_mode::none)
		mode.DesiredAccess|=4;		//FILE_APPEND_DATA
	else if((value&open_mode::out)!=open_mode::none)
	{
		mode.DesiredAccess|=0x120116;	//FILE_GENERIC_WRITE
		generic_write=true;
	}
	if(((value&open_mode::in)!=open_mode::none)||((value&open_mode::app)!=open_mode::none))
	{
		mode.DesiredAccess|=0x120089;	//FILE_GENERIC_READ
		if((value&open_mode::out)!=open_mode::none&&((value&open_mode::app)!=open_mode::none&&(value&open_mode::trunc)!=open_mode::none))
		{
			mode.DesiredAccess|=0x120116;
			generic_write=true;
		}
	}
/*

https://doxygen.reactos.org/d6/d0e/ndk_2iotypes_8h.html
#define 	FILE_SUPERSEDE   0x00000000
#define 	FILE_OPEN   0x00000001
#define 	FILE_CREATE   0x00000002
#define 	FILE_OPEN_IF   0x00000003
#define 	FILE_OVERWRITE   0x00000004
#define 	FILE_OVERWRITE_IF   0x00000005

https://docs.microsoft.com/en-us/windows-hardware/drivers/ddi/ntifs/nf-ntifs-ntcreatefile
CreateDisposition value	Action if file exists	Action if file does not exist
FILE_SUPERSEDE	Replace the file.	Create the file. 0x00000000
FILE_OPEN	Open the file.	Return an error. 0x00000001
FILE_CREATE	Return an error.	Create the file. 0x00000002
FILE_OPEN_IF	Open the file.	Create the file. 0x00000003
FILE_OVERWRITE	Open the file, and overwrite it.	Return an error. 0x00000004
FILE_OVERWRITE_IF	Open the file, and overwrite it.	Create the file. 0x00000005


https://doxygen.reactos.org/dd/d83/dll_2win32_2kernel32_2client_2file_2create_8c_source.html
Line 88:

win32		=>	nt
CREATE_NEW	=>	FILE_CREATE		(0x00000002)
CREATE_ALWAYS	=>	FILE_OVERWRITE_IF	(0x00000005)
OPEN_EXISITNG	=>	FILE_OPEN		(0x00000001)
OPEN_ALWAYS	=>	FILE_OPEN_IF		(0x00000003)
TRUNCATING_EXISITING=>	FILE_OVERWRITE		(0x00000004)

File access
mode string	Meaning	Explanation	Action if file
already exists	Action if file
does not exist
"r"	read	Open a file for reading	read from start	failure to open
"w"	write	Create a file for writing	destroy contents	create new
"a"	append	Append to a file	write to end	create new
"r+"	read extended	Open a file for read/write	read from start	error
"w+"	write extended	Create a file for read/write	destroy contents	create new
"a+"	append extended	Open a file for read/write	write to end	create new
*/
	if ((value&open_mode::trunc)!=open_mode::none)
	{
		if((value&open_mode::excl)!=open_mode::none)
			mode.CreateDisposition=0x00000002;// CREATE_NEW	=>	FILE_CREATE		(0x00000002)
		else
			mode.CreateDisposition=0x00000005;// CREATE_ALWAYS	=>	FILE_OVERWRITE_IF	(0x00000005)
	}
	else if((value&open_mode::in)==open_mode::none)
	{
		if((value&open_mode::app)!=open_mode::none)
			mode.CreateDisposition=0x00000003;//OPEN_ALWAYS	=>	FILE_OPEN_IF		(0x00000003)
		else if((value&open_mode::out)!=open_mode::none)
		{
			if((value&open_mode::excl)!=open_mode::none)
				mode.CreateDisposition=0x00000002;// CREATE_NEW	=>	FILE_CREATE		(0x00000002)
			else
				mode.CreateDisposition=0x00000005;// CREATE_ALWAYS	=>	FILE_OVERWRITE_IF	(0x00000005)
		}
	}
	else if((value&open_mode::app)!=open_mode::none)
		mode.CreateDisposition=0x00000003;//OPEN_ALWAYS		=>	FILE_OPEN_IF		(0x00000003)
	else
		mode.CreateDisposition=0x00000001;//OPEN_EXISTING	=>	FILE_OPEN		(0x00000001)

	if((value&open_mode::creat)!=open_mode::none)
	{
		if((value&open_mode::excl)!=open_mode::none)
			mode.CreateDisposition=0x00000002;//CREATE_NEW	=>	FILE_CREATE		(0x00000002)
		else
			mode.CreateDisposition=0x00000003;//OPEN_ALWAYS	=>	FILE_OPEN_IF		(0x00000003)
	}
	if((value&open_mode::direct)!=open_mode::none)
		mode.CreateOptions |= 0x00000008;//FILE_NO_INTERMEDIATE_BUFFERING
	if((value&open_mode::sync)!=open_mode::none)
		mode.CreateOptions |= 0x00000002;//FILE_WRITE_THROUGH
	if((value&open_mode::follow)!=open_mode::none)
		mode.CreateOptions |= 0x00200000; //FILE_FLAG_OPEN_REPARSE_POINT => FILE_OPEN_REPARSE_POINT (0x00200000)
	bool set_normal{true};
	if((value&open_mode::archive)!=open_mode::none)
	{
		mode.FileAttributes|=0x20;		//FILE_ATTRIBUTE_ARCHIVE
		set_normal={};
	}
	if((value&open_mode::encrypted)!=open_mode::none)
	{
		mode.FileAttributes|=0x4000;		//FILE_ATTRIBUTE_ENCRYPTED
		set_normal={};
	}
	if((value&open_mode::hidden)!=open_mode::none)
	{
		mode.FileAttributes|=0x2;			//FILE_ATTRIBUTE_HIDDEN
		set_normal={};
	}
	if((value&open_mode::compressed)!=open_mode::none)
	{
		mode.FileAttributes|=0x800;		//FILE_ATTRIBUTE_COMPRESSED
		set_normal={};
	}
	if((value&open_mode::system)!=open_mode::none)
	{
		mode.FileAttributes|=0x4;						//FILE_ATTRIBUTE_SYSTEM
		set_normal={};
	}
	if((value&open_mode::offline)!=open_mode::none)
	{
		mode.FileAttributes|=0x1000;						//FILE_ATTRIBUTE_OFFLINE
		set_normal={};
	}
	if(set_normal)[[likely]]
		mode.FileAttributes|=0x80;						//FILE_ATTRIBUTE_NORMAL

	if((value&open_mode::directory)==open_mode::none)
		mode.CreateOptions|=0x00000040;	//FILE_NON_DIRECTORY_FILE 0x00000040
	else
	{
		if(mode.CreateDisposition==0)
		{
			mode.DesiredAccess|=0x120089;	//FILE_GENERIC_READ
			mode.CreateDisposition=0x00000001;	//OPEN_EXISTING
		}
		mode.CreateOptions |= 0x00004000;		//FILE_OPEN_FOR_BACKUP_INTENT
		mode.CreateOptions |= 0x00000001;		//FILE_DIRECTORY_FILE
		if(generic_write)
			mode.CreateOptions |= 0x00000400;		//FILE_OPEN_REMOTE_INSTANCE
		if((value&open_mode::creat)!=open_mode::none)
			mode.DesiredAccess |= 0x120116|0x120089;	//GENERIC_READ | GENERIC_WRITE
	}
	if((value&open_mode::no_block)==open_mode::none)
		mode.CreateOptions|=0x00000020;	//FILE_SYNCHRONOUS_IO_NONALERT 0x00000020
	else
		mode.CreateOptions|=0x00000010;	//FILE_SYNCHRONOUS_IO_ALERT 0x00000010
	
	if((value&open_mode::random_access)==open_mode::none)
	{
		if((value&open_mode::directory)==open_mode::none)
			mode.CreateOptions|=0x00000004;	//FILE_SEQUENTIAL_ONLY 0x00000004
	}
	else
		mode.CreateOptions|=0x00000800;
	if((value&open_mode::no_recall)!=open_mode::none)
		mode.CreateOptions|=0x00400000;	//FILE_OPEN_NO_RECALL 0x00400000
	if((value&open_mode::case_insensitive)!=open_mode::none)
		mode.ObjAttributes|=0x00000040;	//OBJ_CASE_INSENSITIVE
	if((value&open_mode::inherit)!=open_mode::none)
		mode.ObjAttributes|=0x00000002;	//OBJ_INHERIT

	if((value&open_mode::session_aware)!=open_mode::none)
		mode.CreateOptions|=0x00040000;	//FILE_SESSION_AWARE
	if((value&open_mode::temporary)!=open_mode::none)
	{
		mode.CreateOptions|=0x00001000;					//FILE_DELETE_ON_CLOSE
		mode.FileAttributes|=0x100;						//FILE_ATTRIBUTE_TEMPORARY??
		mode.DesiredAccess|=0x00010000;					//DELETE
	}
	if((pm&perms::owner_write)==perms::none)
		mode.FileAttributes|=0x00000001;  //FILE_ATTRIBUTE_READONLY
	return mode;
}

template<bool zw>
inline void* nt_create_file_common(void* directory, win32::nt::unicode_string* relative_path, nt_open_mode const& mode)
{
	win32::security_attributes sec_attr{sizeof(win32::security_attributes),nullptr,true};
	win32::nt::object_attributes obj{.Length=sizeof(win32::nt::object_attributes),
		.RootDirectory=directory,
		.ObjectName=relative_path,
		.Attributes=mode.ObjAttributes,
		.SecurityDescriptor=mode.ObjAttributes&0x00000002?__builtin_addressof(sec_attr):nullptr,
		.SecurityQualityOfService=nullptr
	};
	void* handle;
	win32::nt::io_status_block block;
	auto const status{win32::nt::nt_create_file<zw>(
	__builtin_addressof(handle),mode.DesiredAccess,__builtin_addressof(obj),__builtin_addressof(block),nullptr,mode.FileAttributes,
	mode.ShareAccess,mode.CreateDisposition,mode.CreateOptions,nullptr,0u)};
	if(status)
		throw_nt_error(status);
	return handle;
}

template<bool zw>
struct nt_create_callback
{
	nt_open_mode const& mode;
#if __has_cpp_attribute(gnu::always_inline)
[[gnu::always_inline]]
#elif __has_cpp_attribute(msvc::forceinline)
[[msvc::forceinline]]
#endif
	void* operator()(void* directory_handle,win32::nt::unicode_string* relative_path) const
	{
		return nt_create_file_common<zw>(directory_handle,relative_path,mode);	//get rid of this pointer
	}
};

template<bool zw>
inline void* nt_family_create_file_impl(wchar_t const* filename_cstr,open_mode_perms ompm)
{
	return ::fast_io::win32::nt::details::nt_call_invoke_without_directory_handle_impl(
				filename_cstr,
				nt_create_callback<zw>{::fast_io::win32::nt::details::calculate_nt_open_mode(ompm)});
}

template<bool zw>
struct nt_family_open_file_parameter
{
	open_mode_perms ompm{};
#if __has_cpp_attribute(gnu::always_inline)
[[gnu::always_inline]]
#elif __has_cpp_attribute(msvc::forceinline)
[[msvc::forceinline]]
#endif
	inline void* operator()(wchar_t const* filename_cstr)
	{
		return nt_family_create_file_impl<zw>(filename_cstr,ompm);
	}
};

template<bool zw,typename T>
requires (::fast_io::constructible_to_os_c_str<T>)
inline void* nt_create_file_impl(T const& t,open_mode_perms ompm)
{
	return nt_api_common(t,nt_family_open_file_parameter<zw>{ompm});
}

template<bool zw,bool kernel>
inline void* nt_family_create_file_at_impl(void* directory_handle,wchar_t const* filename_c_str,std::size_t filename_c_str_len,open_mode_perms md)
{
	if constexpr(kernel)
	{
		return ::fast_io::win32::nt::details::nt_call_kernel_callback(directory_handle,filename_c_str,filename_c_str_len,
			nt_create_callback<zw>{::fast_io::win32::nt::details::calculate_nt_open_mode(md)});
	}
	else
	{
		return ::fast_io::win32::nt::details::nt_call_callback(directory_handle,filename_c_str,filename_c_str_len,
			nt_create_callback<zw>{::fast_io::win32::nt::details::calculate_nt_open_mode(md)});
	}
}

template<bool zw>
inline void* nt_family_create_file_fs_dirent_impl(void* directory_handle,wchar_t const* filename_c_str,std::size_t filename_c_str_len,open_mode_perms md)
{
	return ::fast_io::win32::nt::details::nt_call_kernel_fs_dirent_callback(directory_handle,filename_c_str,filename_c_str_len,
		nt_create_callback<zw>{::fast_io::win32::nt::details::calculate_nt_open_mode(md)});
}

template<bool zw>
inline void* nt_family_create_file_kernel_impl(wchar_t const* filename_cstr,std::size_t filename_c_str_len,open_mode_perms ompm)
{
	return ::fast_io::win32::nt::details::nt_call_kernel_nodir_callback(
				filename_cstr,filename_c_str_len,
				nt_create_callback<zw>{::fast_io::win32::nt::details::calculate_nt_open_mode(ompm)});
}

template<bool zw>
struct nt_family_open_file_kernel_parameter
{
	open_mode_perms ompm{};
#if __has_cpp_attribute(gnu::always_inline)
[[gnu::always_inline]]
#elif __has_cpp_attribute(msvc::forceinline)
[[msvc::forceinline]]
#endif
	inline void* operator()(wchar_t const* filename_cstr,std::size_t filename_c_str_len)
	{
		return nt_family_create_file_kernel_impl<zw>(filename_cstr,filename_c_str_len,ompm);
	}
};

template<bool zw,typename T>
requires (::fast_io::constructible_to_os_c_str<T>)
inline void* nt_create_file_kernel_impl(T const& t,open_mode_perms op)
{
	return nt_api_common(t,nt_family_open_file_kernel_parameter<zw>{op});
}

template<bool zw,bool kernel>
struct nt_family_open_file_at_parameter
{
	void* directory_handle{};
	open_mode_perms ompm{};
#if __has_cpp_attribute(gnu::always_inline)
[[gnu::always_inline]]
#elif __has_cpp_attribute(msvc::forceinline)
[[msvc::forceinline]]
#endif
	inline void* operator()(wchar_t const* filename_cstr,std::size_t filename_c_str_len)
	{
		return nt_family_create_file_at_impl<zw,kernel>(directory_handle,filename_cstr,filename_c_str_len,ompm);
	}
};

template<bool zw,bool kernel=false,typename T>
requires (::fast_io::constructible_to_os_c_str<T>)
inline void* nt_create_file_at_impl(void* directory_handle,T const& t,open_mode_perms op)
{
	return nt_api_common(t,nt_family_open_file_at_parameter<zw,kernel>{directory_handle,op});
}

template<bool zw>
inline std::size_t nt_read_impl(void* __restrict handle,void* __restrict begin,std::size_t size)
{
	if constexpr(4<sizeof(std::size_t))
		if(static_cast<std::size_t>(UINT32_MAX)<size)
			size=static_cast<std::size_t>(UINT32_MAX);
	win32::nt::io_status_block block;	//some poeple in zwclose7 forum said we do not need to initialize io_status_block
	auto const status{win32::nt::nt_read_file<zw>(handle,nullptr,nullptr,nullptr,
		__builtin_addressof(block), begin, static_cast<std::uint32_t>(size), nullptr, nullptr)};
	if(status)
		throw_nt_error(status);
	return block.Information;
}

template<bool zw>
inline std::size_t nt_write_impl(void* __restrict handle,void const* __restrict begin,std::size_t size)
{
	if constexpr(4<sizeof(std::size_t))		//above the size of std::uint32_t, unfortunately, we cannot guarantee the atomicity of syscall
	{
		std::size_t written{};
		for(;size;)
		{
			std::uint32_t to_write_this_round{UINT32_MAX};
			if(size<static_cast<std::size_t>(UINT32_MAX))
				to_write_this_round=static_cast<std::uint32_t>(size);
			win32::nt::io_status_block block; //some poeple in zwclose7 forum said we do not need to initialize io_status_block
			auto const status{win32::nt::nt_write_file<zw>(handle,nullptr,nullptr,nullptr,
				__builtin_addressof(block), begin, to_write_this_round, nullptr, nullptr)};
			if(status)
				throw_nt_error(status);
			std::uint32_t number_of_bytes_written{static_cast<std::uint32_t>(block.Information)};
			written+=number_of_bytes_written;
			if(number_of_bytes_written<to_write_this_round)
				break;
			size-=to_write_this_round;
		}
		return written;
	}
	else
	{
		win32::nt::io_status_block block;	//some poeple in zwclose7 forum said we do not need to initialize io_status_block
		auto const status{win32::nt::nt_write_file<zw>(handle,nullptr,nullptr,nullptr,
			__builtin_addressof(block), begin, static_cast<std::uint32_t>(size), nullptr, nullptr)};
		if(status)
			throw_nt_error(status);
		return block.Information;
	}
}

template<bool zw>
struct nt_file_lock_guard
{
	void* handle{};
	explicit constexpr nt_file_lock_guard(void* h):handle(h){}
	nt_file_lock_guard(nt_file_lock_guard const&)=delete;
	nt_file_lock_guard& operator=(nt_file_lock_guard const&)=delete;
	~nt_file_lock_guard()
	{
		if(handle)
		{
			win32::nt::io_status_block block;
			std::int64_t byte_offset{};
			std::int64_t length{INT64_MAX};
			win32::nt::nt_unlock_file<zw>(handle,__builtin_addressof(block),__builtin_addressof(byte_offset),__builtin_addressof(length),0);//ignore error
		}
	}
};

template<bool zw>
inline bool nt_lock_entire_file(void* __restrict handle) noexcept
{
	win32::nt::io_status_block block;
	std::int64_t byte_offset{};
	std::int64_t length{INT64_MAX};
	std::uint32_t status{win32::nt::nt_lock_file<zw>(handle,nullptr,nullptr,nullptr,__builtin_addressof(block),
		__builtin_addressof(byte_offset),__builtin_addressof(length),0,0,1/*exclusive lock*/)};
	return status;
}

template<bool zw>
inline io_scatter_status_t nt_scatter_read_impl(void* __restrict handle,io_scatter_t const* scatters,std::size_t n)
{
	nt_file_lock_guard<zw> gd{
		nt_lock_entire_file<zw>(handle)?
		nullptr:
		handle
	};
	std::size_t total_size{};
	for(std::size_t i{};i!=n;++i)
	{
		std::size_t pos_in_span{nt_read_impl<zw>(handle,const_cast<void*>(scatters[i].base),scatters[i].len)};
		total_size+=pos_in_span;
		if(pos_in_span<scatters[i].len)[[unlikely]]
			return {total_size,i,pos_in_span};
	}
	return {total_size,n,0};
}

template<bool zw>
inline io_scatter_status_t nt_scatter_write_impl(void* __restrict handle,io_scatter_t const* scatters,std::size_t n)
{
	nt_file_lock_guard<zw> gd{
		nt_lock_entire_file<zw>(handle)?
		nullptr:
		handle
	};
	std::size_t total_size{};
	for(std::size_t i{};i!=n;++i)
	{
		std::size_t written{nt_write_impl<zw>(handle,scatters[i].base,scatters[i].len)};
		total_size+=written;
		if(scatters[i].len<written)[[unlikely]]
			return {total_size,i,written};
	}
	return {total_size,n,0};
}

}
namespace details
{
#if !defined(__WINE__)
inline void* my_get_osfile_handle(int fd) noexcept;
#endif
}

struct nt_at_entry
{
	using native_handle_type = void*;
	void* handle{};
	explicit constexpr nt_at_entry() noexcept=default;
	explicit constexpr nt_at_entry(void* mhandle) noexcept:handle(mhandle){}
#if !defined(__WINE__)
	nt_at_entry(posix_at_entry pate) noexcept:handle(details::my_get_osfile_handle(pate.fd)){}
#endif
};

#if __has_cpp_attribute(gnu::always_inline)
[[gnu::always_inline]]
#elif __has_cpp_attribute(msvc::forceinline)
[[msvc::forceinline]]
#endif
inline nt_at_entry nt_at_fdcwd() noexcept
{
	constexpr std::intptr_t value{-3};	//use -3 as at_fdwcd handle
	return nt_at_entry{bit_cast<void*>(value)};
}

#if !defined(__CYGWIN__) && !defined(__WINE__)
#if __has_cpp_attribute(gnu::always_inline)
[[gnu::always_inline]]
#elif __has_cpp_attribute(msvc::forceinline)
[[msvc::forceinline]]
#endif
inline nt_at_entry at_fdcwd() noexcept
{
	return nt_at_fdcwd();
}
#endif

template<nt_family family>
struct nt_family_at_entry:nt_at_entry
{
	using nt_at_entry::native_handle_type;
	explicit constexpr nt_family_at_entry() noexcept=default;
	explicit constexpr nt_family_at_entry(void* mhandle) noexcept:nt_at_entry{mhandle}{}
};

struct nt_fs_dirent
{
	void* handle{};
	wcstring_view filename{};
	explicit constexpr nt_fs_dirent() noexcept = default;
	explicit constexpr nt_fs_dirent(void* mhandle,wcstring_view mfilename) noexcept:handle(mhandle),filename(mfilename){}
};

using zw_at_entry=nt_family_at_entry<nt_family::zw>;

template<win32_family family,std::integral ch_type>
class basic_win32_family_io_observer;

template<nt_family family,std::integral ch_type>
requires (family==nt_family::nt||family==nt_family::zw)
class basic_nt_family_io_observer
{
public:
	using native_handle_type = void*;
	using char_type = ch_type;
	native_handle_type handle{};
	constexpr auto& native_handle() noexcept
	{
		return handle;
	}
	constexpr auto& native_handle() const noexcept
	{
		return handle;
	}
	explicit constexpr operator bool() const noexcept
	{
		return handle;
	}
	template<win32_family family2>
	explicit operator basic_win32_family_io_observer<family2,char_type>() const noexcept
	{
		return basic_win32_family_io_observer<family2,char_type>{reinterpret_cast<void*>(handle)};
	}
	template<nt_family family2>
	constexpr operator basic_nt_family_io_observer<family2,char_type>() const noexcept
	{
		return basic_nt_family_io_observer<family2,char_type>{handle};
	}
	constexpr native_handle_type release() noexcept
	{
		auto temp{handle};
		handle=nullptr;
		return temp;
	}
};

#if __cpp_lib_three_way_comparison >= 201907L

template<nt_family family,std::integral ch_type>
inline constexpr bool operator==(basic_nt_family_io_observer<family,ch_type> a,basic_nt_family_io_observer<family,ch_type> b) noexcept
{
	return a.handle==b.handle;
}

template<nt_family family,std::integral ch_type>
inline constexpr auto operator<=>(basic_nt_family_io_observer<family,ch_type> a,basic_nt_family_io_observer<family,ch_type> b) noexcept
{
	return a.handle<=>b.handle;
}

#endif

template<nt_family family,std::integral ch_type>
inline constexpr auto at(basic_nt_family_io_observer<family,ch_type> niob) noexcept
{
	if constexpr(family==nt_family::nt)
		return nt_at_entry{niob.handle};
	else
		return nt_family_at_entry<family>{niob.handle};
}

template<nt_family family,std::integral ch_type>
inline constexpr basic_nt_family_io_observer<family,ch_type> io_value_handle(basic_nt_family_io_observer<family,ch_type> other) noexcept
{
	return other;
}

template<nt_family family,std::integral ch_type,::fast_io::freestanding::contiguous_iterator Iter>
[[nodiscard]] inline Iter read(basic_nt_family_io_observer<family,ch_type> obs,Iter begin,Iter end)
{
	return begin+win32::nt::details::nt_read_impl<family==nt_family::zw>(obs.handle,::fast_io::freestanding::to_address(begin),static_cast<std::size_t>(end-begin)*sizeof(*begin))/sizeof(*begin);
}

template<nt_family family,std::integral ch_type,::fast_io::freestanding::contiguous_iterator Iter>
inline Iter write(basic_nt_family_io_observer<family,ch_type> obs,Iter cbegin,Iter cend)
{
	return cbegin+win32::nt::details::nt_write_impl<family==nt_family::zw>(obs.handle,::fast_io::freestanding::to_address(cbegin),static_cast<std::size_t>(cend-cbegin)*sizeof(*cbegin))/sizeof(*cbegin);
}

template<nt_family family,std::integral ch_type>
inline io_scatter_status_t scatter_read(basic_nt_family_io_observer<family,ch_type> obs,io_scatters_t sp)
{
	return win32::nt::details::nt_scatter_read_impl<family==nt_family::zw>(obs.handle,sp.base,sp.len);
}

template<nt_family family,std::integral ch_type>
inline io_scatter_status_t scatter_write(basic_nt_family_io_observer<family,ch_type> obs,io_scatters_t sp)
{
	return win32::nt::details::nt_scatter_write_impl<family==nt_family::zw>(obs.handle,sp.base,sp.len);
}

namespace win32::nt::details
{

template<bool zw>
inline void nt_flush_impl(void* handle)
{
	win32::nt::io_status_block block;
	std::uint32_t status{win32::nt::nt_flush_buffers_file<zw>(handle,__builtin_addressof(block))};
	if(status)
		throw_nt_error(status);
}

template<bool zw>
inline void nt_data_sync_impl(void* handle,data_sync_flags flags [[maybe_unused]])
{
#if _WIN32_WINNT >= 0x0602 || WINVER >= 0x0602
/*
NtFlushBuffersFileEx and ZwFlushBuffersFileEx are only provided since windows 8
*/
	win32::nt::io_status_block block;
	std::uint32_t status{win32::nt::nt_flush_buffers_file_ex<zw>(handle,static_cast<std::uint32_t>(flags),nullptr,0u,__builtin_addressof(block))};
	if(status)
		throw_nt_error(status);
#else
	nt_flush_impl<zw>(handle);
#endif
}

}

template<nt_family family,std::integral ch_type>
inline void flush(basic_nt_family_io_observer<family,ch_type> ntiob)
{
	win32::nt::details::nt_flush_impl<family==nt_family::zw>(ntiob.handle);
}

template<nt_family family,std::integral ch_type>
inline void data_sync(basic_nt_family_io_observer<family,ch_type> ntiob,data_sync_flags flags)
{
	win32::nt::details::nt_data_sync_impl<family==nt_family::zw>(ntiob.handle,flags);
}

/*
Let's first borrow win32 code before I finish it.
ReactOS provides implementation of how to do this
https://doxygen.reactos.org/da/d02/dll_2win32_2kernel32_2client_2file_2fileinfo_8c_source.html#l00327
*/

namespace win32::nt::details
{

template<bool zw>
inline std::uint64_t nt_seek64_impl(void* __restrict handle,std::int64_t offset,seekdir s)
{
	std::uint64_t file_position{static_cast<std::uint64_t>(offset)};
	win32::nt::io_status_block block{};
	switch(s)
	{
	case seekdir::cur:
	{
		std::uint64_t fps{};
		auto status{win32::nt::nt_query_information_file<zw>(handle,
			__builtin_addressof(block),
			__builtin_addressof(fps),
			sizeof(std::uint64_t),
			win32::nt::file_information_class::FilePositionInformation)};
		if(status)
			throw_nt_error(status);
		file_position+=fps;
	}
	break;
	case seekdir::end:
	{
		win32::nt::file_standard_information fsi;
		auto status{win32::nt::nt_query_information_file<zw>(handle,
			__builtin_addressof(block),
			__builtin_addressof(fsi),
			sizeof(win32::nt::file_standard_information),
			win32::nt::file_information_class::FileStandardInformation)};
		if(status)
			throw_nt_error(status);
		file_position+=fsi.end_of_file;
	}
	break;
	default:
	[[fallthrough]];
	}
	if(static_cast<std::uint64_t>(std::numeric_limits<std::int64_t>::max())<file_position)
		file_position=0;
	auto status{win32::nt::nt_set_information_file<zw>(handle,
		__builtin_addressof(block),
		__builtin_addressof(file_position),
		sizeof(std::uint64_t),
		win32::nt::file_information_class::FilePositionInformation)};
	if(status)
		throw_nt_error(status);
	return file_position;
}

template<bool zw>
inline std::uintmax_t nt_seek_impl(void* __restrict handle,std::intmax_t offset,seekdir s)
{
	return static_cast<std::uintmax_t>(nt_seek64_impl<zw>(handle,static_cast<std::int64_t>(offset),s));
}

template<bool zw>
inline void* nt_dup_impl(void* handle)
{
	void* current_process{reinterpret_cast<void*>(static_cast<intptr_t>(-1))};
	void* new_handle{};
	auto status{win32::nt::nt_duplicate_object<zw>(current_process,handle,current_process,__builtin_addressof(new_handle),0,0x00000002L,2)};
	if(status)
		throw_nt_error(status);
	return new_handle;
}

template<bool zw>
inline void* nt_dup2_impl(void* handle,void* newhandle)
{
	auto temp{nt_dup_impl<zw>(handle)};
	if(newhandle)[[likely]]
		win32::nt::nt_close<zw>(newhandle);
	return temp;
}

template<bool zw>
inline void nt_truncate_impl(void* handle,std::uintmax_t newfilesizem)
{
	std::uint64_t newfilesize{static_cast<std::uint64_t>(newfilesizem)};
	win32::nt::io_status_block block{};
	auto status{win32::nt::nt_set_information_file<zw>(handle,
		__builtin_addressof(block),
		__builtin_addressof(newfilesize),
		sizeof(std::uint64_t),
		win32::nt::file_information_class::FileEndOfFileInformation)};
	if(status)
		throw_nt_error(status);
}

}

template<nt_family family,std::integral ch_type>
inline std::uintmax_t seek(basic_nt_family_io_observer<family,ch_type> handle,std::intmax_t offset=0,seekdir s=seekdir::cur)
{
	return win32::nt::details::nt_seek_impl<family==nt_family::zw>(handle.handle,offset,s);
}

template<nt_family family,std::integral ch_type>
inline void truncate(basic_nt_family_io_observer<family,ch_type> handle,std::uintmax_t newfilesize)
{
	win32::nt::details::nt_truncate_impl<family==nt_family::zw>(handle.handle,newfilesize);
}

template<nt_family family,std::integral ch_type>
class basic_nt_family_io_handle:public basic_nt_family_io_observer<family,ch_type>
{
public:
	using char_type = ch_type;
	using native_handle_type = void*;
	explicit constexpr basic_nt_family_io_handle() noexcept = default;
	template<typename native_hd>
	requires std::same_as<native_handle_type,std::remove_cvref_t<native_hd>>
	explicit constexpr basic_nt_family_io_handle(native_hd hd) noexcept:basic_nt_family_io_observer<family,ch_type>{hd}{}
	constexpr basic_nt_family_io_handle(decltype(nullptr)) noexcept = delete;
	void reset(native_handle_type newhandle=nullptr) noexcept
	{
		if(this->handle)[[likely]]
			win32::nt::nt_close<family==nt_family::zw>(this->handle);
		this->handle=newhandle;
	}
	void close()
	{
		if(this->handle)[[likely]]
		{
			auto status{win32::nt::nt_close<family==nt_family::zw>(this->handle)};
			this->handle=nullptr;//POSIX standard says we should never call close(2) again even close syscall fails
			if(status)[[unlikely]]
				throw_nt_error(status);
		}
	}
	basic_nt_family_io_handle(basic_nt_family_io_handle const& other):basic_nt_family_io_observer<family,ch_type>(win32::nt::details::nt_dup_impl<family==nt_family::zw>(other.handle)){}
	basic_nt_family_io_handle& operator=(basic_nt_family_io_handle const& other)
	{
		this->handle=win32::nt::details::nt_dup2_impl<family==nt_family::zw>(other.handle,this->handle);
		return *this;
	}
	constexpr basic_nt_family_io_handle(basic_nt_family_io_handle&& b) noexcept:
		basic_nt_family_io_observer<family,ch_type>{b.handle}
	{
		b.handle=nullptr;
	}
	basic_nt_family_io_handle& operator=(basic_nt_family_io_handle&& b) noexcept
	{
		if(__builtin_addressof(b)!=this)
		{
			if(this->handle)[[likely]]
				win32::nt::nt_close<family==nt_family::zw>(this->handle);
			this->handle = b.handle;
			b.handle=nullptr;
		}
		return *this;
	}
};

template<nt_family family>
struct
#if __has_cpp_attribute(gnu::trivial_abi)
[[gnu::trivial_abi]]
#endif
nt_family_file_factory
{
	using native_handle_type=void*;
	void* handle{};
	explicit constexpr nt_family_file_factory(void* hd) noexcept:handle(hd){};
	nt_family_file_factory(nt_family_file_factory const&)=delete;
	nt_family_file_factory& operator=(nt_family_file_factory const&)=delete;
	~nt_family_file_factory()
	{
		if(handle)[[likely]]
			win32::nt::nt_close<family==nt_family::zw>(this->handle);
	}
};

using nt_file_factory = nt_family_file_factory<nt_family::nt>;
using zw_file_factory = nt_family_file_factory<nt_family::zw>;

template<nt_family family,std::integral ch_type>
class basic_nt_family_file:public basic_nt_family_io_handle<family,ch_type>
{
public:
	using char_type = ch_type;
	using native_handle_type = void*;
	using file_factory_type = nt_family_file_factory<family>;
	constexpr basic_nt_family_file()=default;
	template<typename native_hd>
	requires std::same_as<native_handle_type,std::remove_cvref_t<native_hd>>
	explicit constexpr basic_nt_family_file(native_hd hd) noexcept:basic_nt_family_io_handle<family,ch_type>{hd}{}
	constexpr basic_nt_family_file(decltype(nullptr)) noexcept = delete;
	explicit constexpr basic_nt_family_file(nt_family_file_factory<family>&& hd) noexcept:basic_nt_family_io_handle<family,ch_type>{hd}
	{
		hd.handle=nullptr;
	}
	explicit basic_nt_family_file(io_dup_t,basic_nt_family_io_observer<family,ch_type> wiob):
		basic_nt_family_io_handle<family,ch_type>{win32::nt::details::nt_dup_impl<family==nt_family::zw>(wiob.handle)}
	{}
	explicit basic_nt_family_file(nt_fs_dirent fsdirent,open_mode om,perms pm=static_cast<perms>(436)):
		basic_nt_family_io_handle<family,char_type>{win32::nt::details::nt_family_create_file_fs_dirent_impl<family==nt_family::zw>(fsdirent.handle,fsdirent.filename.c_str(),fsdirent.filename.size(),{om,pm})}{}
	template<::fast_io::constructible_to_os_c_str T>
	explicit basic_nt_family_file(T const& t,open_mode om,perms pm=static_cast<perms>(436)):basic_nt_family_io_handle<family,ch_type>{::fast_io::win32::nt::details::nt_create_file_impl<family==nt_family::zw>(t,{om,pm})}{}
	template<::fast_io::constructible_to_os_c_str T>
	explicit basic_nt_family_file(nt_at_entry ent,T const& t,open_mode om,perms pm=static_cast<perms>(436)):basic_nt_family_io_handle<family,ch_type>{::fast_io::win32::nt::details::nt_create_file_at_impl<family==nt_family::zw>(ent.handle,t,{om,pm})}{}

	template<::fast_io::constructible_to_os_c_str T>
	explicit basic_nt_family_file(io_kernel_t,T const& t,open_mode om,perms pm=static_cast<perms>(436)):basic_nt_family_io_handle<family,ch_type>{::fast_io::win32::nt::details::nt_create_file_kernel_impl<family==nt_family::zw>(t,{om,pm})}{}
	template<::fast_io::constructible_to_os_c_str T>
	explicit basic_nt_family_file(io_kernel_t,nt_at_entry ent,T const& t,open_mode om,perms pm=static_cast<perms>(436)):basic_nt_family_io_handle<family,ch_type>{::fast_io::win32::nt::details::nt_create_file_at_impl<family==nt_family::zw,true>(ent.handle,t,{om,pm})}{}
 
 	basic_nt_family_file(basic_nt_family_file const&)=default;
	basic_nt_family_file& operator=(basic_nt_family_file const&)=default;
	constexpr basic_nt_family_file(basic_nt_family_file&&) noexcept=default;
	basic_nt_family_file& operator=(basic_nt_family_file&&) noexcept=default;
	~basic_nt_family_file()
	{
		if(this->handle)[[likely]]
			win32::nt::nt_close<family==nt_family::zw>(this->handle);
	}
};

template<std::integral char_type>
using basic_nt_io_observer=basic_nt_family_io_observer<nt_family::nt,char_type>;

template<std::integral char_type>
using basic_nt_io_handle=basic_nt_family_io_handle<nt_family::nt,char_type>;

template<std::integral char_type>
using basic_nt_file=basic_nt_family_file<nt_family::nt,char_type>;

using nt_io_observer=basic_nt_io_observer<char>;
using nt_io_handle=basic_nt_io_handle<char>;
using nt_file=basic_nt_file<char>;

using wnt_io_observer=basic_nt_io_observer<wchar_t>;
using wnt_io_handle=basic_nt_io_handle<wchar_t>;
using wnt_file=basic_nt_file<wchar_t>;

using u8nt_io_observer=basic_nt_io_observer<char8_t>;
using u8nt_io_handle=basic_nt_io_handle<char8_t>;
using u8nt_file=basic_nt_file<char8_t>;

using u16nt_io_observer=basic_nt_io_observer<char16_t>;
using u16nt_io_handle=basic_nt_io_handle<char16_t>;
using u16nt_file=basic_nt_file<char16_t>;

using u32nt_io_observer=basic_nt_io_observer<char32_t>;
using u32nt_io_handle=basic_nt_io_handle<char32_t>;
using u32nt_file=basic_nt_file<char32_t>;

template<std::integral char_type=char>
inline basic_nt_io_observer<char_type> nt_stdin() noexcept
{
	return {fast_io::win32::GetStdHandle(static_cast<std::uint32_t>(-10))};
}

template<std::integral char_type=char>
inline basic_nt_io_observer<char_type> nt_stdout() noexcept
{
	return {fast_io::win32::GetStdHandle(static_cast<std::uint32_t>(-11))};
}

template<std::integral char_type=char>
inline basic_nt_io_observer<char_type> nt_stderr() noexcept
{
	return {fast_io::win32::GetStdHandle(static_cast<std::uint32_t>(-12))};
}


template<std::integral char_type>
using basic_zw_io_observer=basic_nt_family_io_observer<nt_family::zw,char_type>;

template<std::integral char_type>
using basic_zw_io_handle=basic_nt_family_io_handle<nt_family::zw,char_type>;

template<std::integral char_type>
using basic_zw_file=basic_nt_family_file<nt_family::zw,char_type>;

using zw_io_observer=basic_zw_io_observer<char>;
using zw_io_handle=basic_zw_io_handle<char>;
using zw_file=basic_zw_file<char>;

using wzw_io_observer=basic_zw_io_observer<wchar_t>;
using wzw_io_handle=basic_zw_io_handle<wchar_t>;
using wzw_file=basic_zw_file<wchar_t>;

using u8zw_io_observer=basic_zw_io_observer<char8_t>;
using u8zw_io_handle=basic_zw_io_handle<char8_t>;
using u8zw_file=basic_zw_file<char8_t>;

using u16zw_io_observer=basic_zw_io_observer<char16_t>;
using u16zw_io_handle=basic_zw_io_handle<char16_t>;
using u16zw_file=basic_zw_file<char16_t>;

using u32zw_io_observer=basic_zw_io_observer<char32_t>;
using u32zw_io_handle=basic_zw_io_handle<char32_t>;
using u32zw_file=basic_zw_file<char32_t>;

template<std::integral char_type=char>
inline basic_zw_io_observer<char_type> zw_stdin() noexcept
{
	return {fast_io::win32::GetStdHandle(static_cast<std::uint32_t>(-10))};
}

template<std::integral char_type=char>
inline basic_zw_io_observer<char_type> zw_stdout() noexcept
{
	return {fast_io::win32::GetStdHandle(static_cast<std::uint32_t>(-11))};
}

template<std::integral char_type=char>
inline basic_zw_io_observer<char_type> zw_stderr() noexcept
{
	return {fast_io::win32::GetStdHandle(static_cast<std::uint32_t>(-12))};
}

#if 0
template<std::integral char_type=char>
inline basic_nt_io_observer<char_type> native_stdin() noexcept
{
	return {fast_io::win32::GetStdHandle(-10)};
}
template<std::integral char_type=char>
inline basic_nt_io_observer<char_type> native_stdout() noexcept
{
	return {fast_io::win32::GetStdHandle(-11)};
}
template<std::integral char_type=char>
inline basic_nt_io_observer<char_type> native_stderr() noexcept
{
	return {fast_io::win32::GetStdHandle(-12)};
}
#endif
}
