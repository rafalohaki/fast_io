#pragma once

namespace fast_io
{

/*
Referenced from Windows Internal Version 7
*/
enum class nt_family
{
alpc,	//Alpc: Advanced Local Proedure Calls
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

inline constexpr nt_open_mode calculate_nt_open_mode(open_mode value,perms pm) noexcept
{
	nt_open_mode mode;
	bool generic_write{};
	if((value&open_mode::app)!=open_mode::none)
		mode.DesiredAccess|=4;		//FILE_APPEND_DATA
	else if((value&open_mode::out)!=open_mode::none)
	{
		mode.DesiredAccess|=0x120116;	//FILE_GENERIC_WRITE
		generic_write=true;
	}
	if(((value&open_mode::in)!=open_mode::none)|((value&open_mode::app)!=open_mode::none))
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
	if((value&open_mode::case_insensitive)==open_mode::none)
		mode.ObjAttributes|=0x00000040;	//OBJ_CASE_INSENSITIVE
	if((value&open_mode::inherit)!=open_mode::none)
		mode.ObjAttributes|=0x00000002;	//OBJ_INHERIT

	if((value&open_mode::session_aware)!=open_mode::none)
		mode.CreateOptions|=0x00040000;	//FILE_SESSION_AWARE
	if((value&open_mode::temporary)!=open_mode::none)
	{
		mode.CreateOptions|=0x00001000;					//FILE_DELETE_ON_CLOSE
		mode.FileAttributes|=0x100;				//FILE_ATTRIBUTE_TEMPORARY??
	}
	if((pm&perms::owner_write)==perms::none)
		mode.FileAttributes|=0x00000001;  //FILE_ATTRIBUTE_READONLY
	return mode;
}
template<open_mode om,perms pm=static_cast<perms>(436)>
struct nt_file_openmode
{
	inline static constexpr nt_open_mode mode = calculate_nt_open_mode(om,pm);
};

inline void nt_file_rtl_path(wcstring_view filename,win32::nt::unicode_string& nt_name,wchar_t const*& part_name,win32::nt::rtl_relative_name_u& relative_name)
{
	if(!win32::nt::rtl_dos_path_name_to_nt_path_name_u(filename.c_str(),std::addressof(nt_name),std::addressof(part_name),std::addressof(relative_name)))
		throw_nt_error(0xC0000039);
}

inline void nt_file_rtl_path(cstring_view filename,wchar_t* buffer_data,win32::nt::unicode_string& nt_name,wchar_t const*& part_name,win32::nt::rtl_relative_name_u& relative_name)
{
	*utf_code_convert(filename.data(),filename.data()+filename.size(),buffer_data)=0;
	if(!win32::nt::rtl_dos_path_name_to_nt_path_name_u(buffer_data,std::addressof(nt_name),std::addressof(part_name),std::addressof(relative_name)))
		throw_nt_error(0xC0000039);
}

template<bool zw>
inline void* nt_create_file_common_impl(void* directory,win32::nt::unicode_string* relative_path,nt_open_mode const& mode)
{
	win32::security_attributes sec_attr{sizeof(win32::security_attributes),nullptr,true};
	win32::nt::object_attributes obj{.Length=sizeof(win32::nt::object_attributes),
		.RootDirectory=directory,
		.ObjectName=relative_path,
		.Attributes=mode.ObjAttributes,
		.SecurityDescriptor=mode.ObjAttributes&0x00000002?std::addressof(sec_attr):nullptr,
	};
	void* handle{};
	win32::nt::io_status_block block{};
	auto const status{win32::nt::nt_create_file<zw>(
	std::addressof(handle),mode.DesiredAccess,std::addressof(obj),std::addressof(block),nullptr,mode.FileAttributes,
	mode.ShareAccess,mode.CreateDisposition,mode.CreateOptions,nullptr,0)};
	if(status)
		throw_nt_error(status);
	return handle;
}

inline std::uint16_t filename_bytes(std::size_t sz)
{
	sz<<=1;
	if constexpr(sizeof(sz)<=sizeof(std::uint16_t))
		return static_cast<std::uint16_t>(sz);
	if(static_cast<std::size_t>(std::numeric_limits<std::uint16_t>::max())<sz)
		throw_nt_error(0xC0000106);
	return static_cast<std::uint16_t>(sz);
}

template<bool zw,std::integral char_type>
requires (std::same_as<char_type,char>||std::same_as<char_type,wchar_t>)
inline void* nt_create_file_directory_impl(void* directory,basic_cstring_view<char_type> filename,nt_open_mode const& mode)
{
	if constexpr(std::same_as<char_type,char>)
	{
		std::unique_ptr<wchar_t[]> buffer(new wchar_t[filename.size()]);
		auto buffer_data_end=utf_code_convert(filename.data(),filename.data()+filename.size(),buffer.get());
		std::uint16_t const bytes(filename_bytes(buffer_data_end-buffer.get()));
		win32::nt::unicode_string relative_path{
			.Length=bytes,
			.MaximumLength=bytes,
			.Buffer=buffer.get()};
		return nt_create_file_common_impl<zw>(directory,std::addressof(relative_path),mode);
	}
	else
	{
		std::uint16_t const bytes(filename_bytes(filename.size()));
		win32::nt::unicode_string relative_path{
			.Length=bytes,
			.MaximumLength=bytes,
			.Buffer=const_cast<wchar_t*>(filename.c_str())};
		return nt_create_file_common_impl<zw>(directory,std::addressof(relative_path),mode);
	}
}

template<bool zw,std::integral char_type>
requires (std::same_as<char_type,char>||std::same_as<char_type,wchar_t>)
inline void* nt_create_file_impl(basic_cstring_view<char_type> filename,nt_open_mode const& mode)
{
	wchar_t const* part_name{};
	win32::nt::rtl_relative_name_u relative_name{};
	win32::nt::unicode_string nt_name{};
	if constexpr(std::same_as<char_type,char>)
	{
		std::unique_ptr<wchar_t[]> buffer(new wchar_t[filename.size()+1]);
		nt_file_rtl_path(filename,buffer.get(),nt_name,part_name,relative_name);
	}
	else
	{
		nt_file_rtl_path(filename,nt_name,part_name,relative_name);
	}
	win32::nt::rtl_unicode_string_unique_ptr us_ptr{std::addressof(nt_name)};
	return nt_create_file_common_impl<zw>(nullptr,std::addressof(nt_name),mode);
}

template<bool zw>
inline std::size_t nt_read_impl(void* __restrict handle,void* __restrict begin,std::size_t size)
{
	if constexpr(4<sizeof(std::size_t))
		if(static_cast<std::size_t>(UINT32_MAX)<size)
			size=static_cast<std::size_t>(UINT32_MAX);
	win32::nt::io_status_block block{};
	auto const status{win32::nt::nt_read_file<zw>(handle,nullptr,nullptr,nullptr,
		std::addressof(block), begin, static_cast<std::uint32_t>(size), nullptr, nullptr)};
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
			win32::nt::io_status_block block{};
			auto const status{win32::nt::nt_write_file<zw>(handle,nullptr,nullptr,nullptr,
				std::addressof(block), begin, to_write_this_round, nullptr, nullptr)};
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
		win32::nt::io_status_block block{};
		auto const status{win32::nt::nt_write_file<zw>(handle,nullptr,nullptr,nullptr,
			std::addressof(block), begin, static_cast<std::uint32_t>(size), nullptr, nullptr)};
		if(status)
			throw_nt_error(status);
		return block.Information;
	}
}

}

namespace details
{
inline std::intptr_t my_get_osfhandle(int) noexcept;
}

struct nt_at_entry
{
	using native_handle_type = void*;
	void* handle{reinterpret_cast<void*>(static_cast<std::uintptr_t>(-1))};
	explicit constexpr nt_at_entry() noexcept=default;
	explicit constexpr nt_at_entry(void* mhandle) noexcept:handle(mhandle){}
	nt_at_entry(posix_at_entry pate) noexcept:handle(reinterpret_cast<void*>(details::my_get_osfhandle(pate.fd))){}
};

template<nt_family family>
struct nt_family_at_entry:nt_at_entry
{
	using nt_at_entry::native_handle_type;
	explicit constexpr nt_family_at_entry() noexcept=default;
	explicit constexpr nt_family_at_entry(void* mhandle) noexcept:nt_at_entry{mhandle}{}
};

using zw_at_entry=nt_family_at_entry<nt_family::zw>;

template<std::integral ch_type>
class basic_win32_io_observer;

template<nt_family family,std::integral ch_type>
requires (family==nt_family::nt||family==nt_family::zw)
class basic_nt_family_io_observer
{
public:
	using native_handle_type = void*;
	using char_type = ch_type;
	native_handle_type handle{reinterpret_cast<void*>(static_cast<std::uintptr_t>(-1))};
	constexpr auto& native_handle() noexcept
	{
		return handle;
	}
	constexpr auto& native_handle() const noexcept
	{
		return handle;
	}
	explicit operator bool() const noexcept
	{
		return handle!=reinterpret_cast<void*>(static_cast<std::uintptr_t>(-1));
	}
	explicit operator basic_win32_io_observer<char_type>() const noexcept
	{
		return basic_win32_io_observer<char_type>{reinterpret_cast<void*>(handle)};
	}
	template<nt_family family2>
	explicit constexpr operator basic_nt_family_io_observer<family2,char_type>() const noexcept
	{
		return basic_nt_family_io_observer<family2,char_type>{handle};
	}
	constexpr native_handle_type release() noexcept
	{
		auto temp{handle};
		handle=reinterpret_cast<void*>(static_cast<std::uintptr_t>(-1));
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

template<nt_family family,std::integral ch_type,std::contiguous_iterator Iter>
[[nodiscard]] inline Iter read(basic_nt_family_io_observer<family,ch_type> obs,Iter begin,Iter end)
{
	return begin+win32::nt::details::nt_read_impl<family==nt_family::zw>(obs.handle,std::to_address(begin),(end-begin)*sizeof(*begin))/sizeof(*begin);
}

template<nt_family family,std::integral ch_type,std::contiguous_iterator Iter>
inline Iter write(basic_nt_family_io_observer<family,ch_type> obs,Iter cbegin,Iter cend)
{
	return cbegin+win32::nt::details::nt_write_impl<family==nt_family::zw>(obs.handle,std::to_address(cbegin),(cend-cbegin)*sizeof(*cbegin))/sizeof(*cbegin);
}

template<nt_family family,std::integral ch_type>
inline constexpr void flush(basic_nt_family_io_observer<family,ch_type>) noexcept
{

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
			std::addressof(block),
			std::addressof(fps),
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
			std::addressof(block),
			std::addressof(fsi),
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
		std::addressof(block),
		std::addressof(file_position),
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
	auto status{win32::nt::nt_duplicate_object<zw>(current_process,handle,current_process,std::addressof(new_handle),0,0x00000002L,2)};
	if(status)
		throw_nt_error(status);
	return new_handle;
}

template<bool zw>
inline void* nt_dup2_impl(void* handle,void* newhandle)
{
	auto temp{nt_dup_impl<zw>(handle)};
	if(newhandle!=reinterpret_cast<void*>(static_cast<std::uintptr_t>(-1)))[[likely]]
		win32::nt::nt_close<zw>(newhandle);
	return temp;
}

template<bool zw>
inline void nt_truncate_impl(void* handle,std::uintmax_t newfilesizem)
{
	std::uint64_t newfilesize{static_cast<std::uint64_t>(newfilesizem)};
	win32::nt::io_status_block block{};
	auto status{win32::nt::nt_set_information_file<zw>(handle,
		std::addressof(block),
		std::addressof(newfilesize),
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
	explicit constexpr basic_nt_family_io_handle(native_hd hd) noexcept:basic_nt_family_io_observer<family,ch_type>(hd){}
	void reset(native_handle_type newhandle=reinterpret_cast<void*>(static_cast<std::uintptr_t>(-1))) noexcept
	{
		if(this->native_handle()!=static_cast<std::uintptr_t>(-1))[[likely]]
			win32::nt::nt_close<family==nt_family::zw>(this->native_handle());
		this->native_handle()=newhandle;
	}
	void close()
	{
		if(this->native_handle()!=reinterpret_cast<void*>(static_cast<std::uintptr_t>(-1)))[[likely]]
		{
			auto status{win32::nt::nt_close<family==nt_family::zw>(this->native_handle())};
			if(status)[[unlikely]]
				throw_nt_error(status);
			this->native_handle()=reinterpret_cast<void*>(static_cast<std::uintptr_t>(-1));
		}
	}
	basic_nt_family_io_handle(basic_nt_family_io_handle const& other):basic_nt_family_io_observer<family,ch_type>(win32::nt::details::nt_dup_impl<family==nt_family::zw>(other.handle)){}
	basic_nt_family_io_handle& operator=(basic_nt_family_io_handle const& other)
	{
		this->handle=win32::nt::details::nt_dup2_impl<family==nt_family::zw>(other.handle,this->handle);
		return *this;
	}
	constexpr basic_nt_family_io_handle(basic_nt_family_io_handle&& b) noexcept:
		basic_nt_family_io_observer<family,ch_type>(b.release()){}
	basic_nt_family_io_handle& operator=(basic_nt_family_io_handle&& b) noexcept
	{
		if(std::addressof(b)!=this)
		{
			if(this->native_handle()!=reinterpret_cast<void*>(static_cast<std::uintptr_t>(-1)))[[likely]]
				win32::nt::nt_close<family==nt_family::zw>(this->native_handle());
			this->native_handle() = b.native_handle();
			b.native_handle()=reinterpret_cast<void*>(static_cast<std::uintptr_t>(-1));
		}
		return *this;
	}
};

template<nt_family family,std::integral ch_type>
class basic_nt_family_file:public basic_nt_family_io_handle<family,ch_type>
{
public:
	using char_type = ch_type;
	using native_handle_type = void*;
	constexpr basic_nt_family_file()=default;
	template<typename native_hd>
	requires std::same_as<native_handle_type,std::remove_cvref_t<native_hd>>
	explicit constexpr basic_nt_family_file(native_hd hd):basic_nt_family_io_handle<family,ch_type>(hd){}
	explicit basic_nt_family_file(io_dup_t,basic_nt_family_io_observer<family,ch_type> wiob):
		basic_nt_family_io_handle<family,ch_type>(win32::nt::details::nt_dup_impl<family==nt_family::zw>(wiob.handle))
	{}
	explicit basic_nt_family_file(cstring_view filename,open_mode om,perms pm=static_cast<perms>(436)):
		basic_nt_family_io_handle<family,ch_type>(win32::nt::details::nt_create_file_impl<family==nt_family::zw>(filename,win32::nt::details::calculate_nt_open_mode(om,pm)))
	{
	}
	explicit basic_nt_family_file(nt_at_entry nate,cstring_view filename,open_mode om,perms pm=static_cast<perms>(436)):
		basic_nt_family_io_handle<family,char_type>(win32::nt::details::nt_create_file_directory_impl<family==nt_family::zw>(nate.handle,filename,win32::nt::details::calculate_nt_open_mode(om,pm)))
	{
	}
	explicit basic_nt_family_file(wcstring_view filename,open_mode om,perms pm=static_cast<perms>(436)):
		basic_nt_family_io_handle<family,ch_type>(win32::nt::details::nt_create_file_impl<family==nt_family::zw>(filename,win32::nt::details::calculate_nt_open_mode(om,pm)))
	{
	}
	explicit basic_nt_family_file(nt_at_entry nate,wcstring_view filename,open_mode om,perms pm=static_cast<perms>(436)):
		basic_nt_family_io_handle<family,char_type>(win32::nt::details::nt_create_file_directory_impl<family==nt_family::zw>(nate.handle,filename,win32::nt::details::calculate_nt_open_mode(om,pm)))
	{
	}
 	basic_nt_family_file(basic_nt_family_file const&)=default;
	basic_nt_family_file& operator=(basic_nt_family_file const&)=default;
	constexpr basic_nt_family_file(basic_nt_family_file&&) noexcept=default;
	basic_nt_family_file& operator=(basic_nt_family_file&&) noexcept=default;
	~basic_nt_family_file()
	{
		if(this->native_handle()!=reinterpret_cast<void*>(static_cast<std::uintptr_t>(-1)))[[likely]]
			win32::nt::nt_close<family==nt_family::zw>(this->native_handle());
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

inline constexpr std::uint32_t nt_stdin_number(static_cast<std::uint32_t>(-10));
inline constexpr std::uint32_t nt_stdout_number(static_cast<std::uint32_t>(-11));
inline constexpr std::uint32_t nt_stderr_number(static_cast<std::uint32_t>(-12));

template<std::integral char_type=char>
inline basic_nt_io_observer<char_type> nt_stdin() noexcept
{
	return {fast_io::win32::GetStdHandle(-10)};
}

template<std::integral char_type=char>
inline basic_nt_io_observer<char_type> nt_stdout() noexcept
{
	return {fast_io::win32::GetStdHandle(-11)};
}

template<std::integral char_type=char>
inline basic_nt_io_observer<char_type> nt_stderr() noexcept
{
	return {fast_io::win32::GetStdHandle(-12)};
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

inline constexpr std::uint32_t zw_stdin_number(static_cast<std::uint32_t>(-10));
inline constexpr std::uint32_t zw_stdout_number(static_cast<std::uint32_t>(-11));
inline constexpr std::uint32_t zw_stderr_number(static_cast<std::uint32_t>(-12));

template<std::integral char_type=char>
inline basic_zw_io_observer<char_type> zw_stdin() noexcept
{
	return {fast_io::win32::GetStdHandle(-10)};
}

template<std::integral char_type=char>
inline basic_zw_io_observer<char_type> zw_stdout() noexcept
{
	return {fast_io::win32::GetStdHandle(-11)};
}

template<std::integral char_type=char>
inline basic_zw_io_observer<char_type> zw_stderr() noexcept
{
	return {fast_io::win32::GetStdHandle(-12)};
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
