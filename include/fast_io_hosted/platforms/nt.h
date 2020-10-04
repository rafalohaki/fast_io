#pragma once

namespace fast_io
{

namespace details::nt
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

inline constexpr nt_open_mode calculate_nt_open_mode(open_mode value,perms pm)
{
	nt_open_mode mode;
	bool generic_write{};
	bool generic_read{};
	if((value&open_mode::app)!=open_mode::none)
		mode.DesiredAccess|=4;		//FILE_APPEND_DATA
	else if((value&open_mode::out)!=open_mode::none)
	{
		mode.DesiredAccess|=0x120116;	//FILE_GENERIC_WRITE
		generic_write=true;
	}
	if((value&open_mode::in)!=open_mode::none)
	{
		mode.DesiredAccess|=0x120089;	//FILE_GENERIC_READ
		generic_read=true;
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


	if((value&open_mode::direct)!=open_mode::none)
		mode.CreateOptions |= 0x00000008;//FILE_NO_INTERMEDIATE_BUFFERING
	if((value&open_mode::sync)!=open_mode::none)
		mode.CreateOptions |= 0x00000002;//FILE_WRITE_THROUGH

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
		if(generic_read)
			mode.CreateOptions |= 0x00004000;		//FILE_OPEN_FOR_BACKUP_INTENT
		if(generic_write)
			mode.CreateOptions |= 0x00000400;		//FILE_OPEN_REMOTE_INSTANCE
	}
	if((value&open_mode::no_block)==open_mode::none)
		mode.CreateOptions|=0x00000020;	//FILE_SYNCHRONOUS_IO_NONALERT 0x00000020
	else
		mode.CreateOptions|=0x00000010;	//FILE_SYNCHRONOUS_IO_ALERT 0x00000010
	if((value&open_mode::random_access)==open_mode::none)
		mode.CreateOptions|=0x00000004;	//FILE_SEQUENTIAL_ONLY 0x00000004
	else
		mode.CreateOptions|=0x00000800;
	if((value&open_mode::no_recall)!=open_mode::none)
		mode.CreateOptions|=0x00400000;	//FILE_OPEN_NO_RECALL 0x00400000

	if((value&open_mode::posix_semantics)==open_mode::none)
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
	auto const status{win32::nt::nt_create_file(
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

template<std::integral char_type>
requires (std::same_as<char_type,char>||std::same_as<char_type,wchar_t>)
inline void* nt_create_file_directory_impl(void* directory,basic_cstring_view<char_type> filename,nt_open_mode const& mode)
{
	if constexpr(std::same_as<char_type,char>)
	{
		details::temp_unique_arr_ptr<wchar_t> buffer(filename.size());
		auto buffer_data_end=utf_code_convert(filename.data(),filename.data()+filename.size(),buffer.data());
		std::uint16_t const bytes(filename_bytes(buffer_data_end-buffer.data()));
		win32::nt::unicode_string relative_path{
			.Length=bytes,
			.MaximumLength=bytes,
			.Buffer=buffer.data()};
		return nt_create_file_common_impl(directory,std::addressof(relative_path),mode);
	}
	else
	{
		std::uint16_t const bytes(filename_bytes(filename.size()));
		win32::nt::unicode_string relative_path{
			.Length=bytes,
			.MaximumLength=bytes,
			.Buffer=const_cast<wchar_t*>(filename.c_str())};
		return nt_create_file_common_impl(directory,std::addressof(relative_path),mode);
	}
}

template<std::integral char_type>
requires (std::same_as<char_type,char>||std::same_as<char_type,wchar_t>)
inline void* nt_create_file_impl(basic_cstring_view<char_type> filename,nt_open_mode const& mode)
{
	wchar_t const* part_name{};
	win32::nt::rtl_relative_name_u relative_name{};
	win32::nt::unicode_string nt_name{};
	if constexpr(std::same_as<char_type,char>)
	{
		details::temp_unique_arr_ptr<wchar_t> buffer(filename.size()+1);
		nt_file_rtl_path(filename,buffer.data(),nt_name,part_name,relative_name);
	}
	else
	{
		nt_file_rtl_path(filename,nt_name,part_name,relative_name);
	}
	win32::nt::rtl_unicode_string_unique_ptr us_ptr{std::addressof(nt_name)};
	return nt_create_file_common_impl(nullptr,std::addressof(nt_name),mode);
}

inline std::size_t nt_read_impl(void* handle,void* begin,std::size_t size)
{
	if constexpr(4<sizeof(std::size_t))
		if(static_cast<std::size_t>(UINT32_MAX)<size)
			size=static_cast<std::size_t>(UINT32_MAX);
	win32::nt::io_status_block block{};
	auto const status{win32::nt::nt_read_file(handle,nullptr,nullptr,nullptr,
		std::addressof(block), begin, static_cast<std::uint32_t>(size), nullptr, nullptr)};
	if(status)
		throw_nt_error(status);
	return block.Information;
}

inline std::size_t nt_write_impl(void* handle,void const* begin,std::size_t size)
{
	if constexpr(4<sizeof(std::size_t))
		if(static_cast<std::size_t>(UINT32_MAX)<size)
			size=static_cast<std::size_t>(UINT32_MAX);
	win32::nt::io_status_block block{};
	auto const status{win32::nt::nt_write_file(handle,nullptr,nullptr,nullptr,
		std::addressof(block), begin, static_cast<std::uint32_t>(size), nullptr, nullptr)};
	if(status)
		throw_nt_error(status);
	return block.Information;
}

}

namespace details
{
extern "C" std::intptr_t _get_osfhandle(int fd) noexcept;
}

struct nt_at_entry
{
	using native_handle_type = void*;
	void* handle{reinterpret_cast<void*>(static_cast<std::uintptr_t>(-1))};
	explicit constexpr nt_at_entry() noexcept=default;
	explicit constexpr nt_at_entry(void* mhandle) noexcept:handle(mhandle){}
	nt_at_entry(posix_at_entry pate) noexcept:handle(reinterpret_cast<void*>(details::_get_osfhandle(pate.fd))){}
};

template<std::integral ch_type>
class basic_win32_io_observer;

template<std::integral ch_type>
class basic_nt_io_observer
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
	explicit constexpr operator bool() const noexcept
	{
		return handle;
	}
	explicit constexpr operator basic_win32_io_observer<char_type>() const noexcept
	{
		return basic_win32_io_observer<char_type>{handle};
	}
	inline native_handle_type release() noexcept
	{
		auto temp{handle};
		handle=reinterpret_cast<void*>(static_cast<std::uintptr_t>(-1));
		return temp;
	}
};

template<std::integral ch_type>
inline constexpr nt_at_entry at(basic_nt_io_observer<ch_type> niob) noexcept
{
	return nt_at_entry{niob.handle};
}

template<std::integral ch_type>
inline constexpr basic_nt_io_observer<ch_type> io_value_handle(basic_nt_io_observer<ch_type> other)
{
	return other;
}


template<std::integral ch_type,std::contiguous_iterator Iter>
inline Iter read(basic_nt_io_observer<ch_type> obs,Iter begin,Iter end)
{
	return begin+details::nt::nt_read_impl(obs.handle,std::to_address(begin),(end-begin)*sizeof(*begin))/sizeof(*begin);
}

template<std::integral ch_type,std::contiguous_iterator Iter>
inline Iter write(basic_nt_io_observer<ch_type> obs,Iter cbegin,Iter cend)
{
	return cbegin+details::nt::nt_write_impl(obs.handle,std::to_address(cbegin),(cend-cbegin)*sizeof(*cbegin))/sizeof(*cbegin);
}

template<std::integral ch_type>
inline constexpr void flush(basic_nt_io_observer<ch_type>) noexcept
{

}

/*
Let's first borrow win32 code before I finish it.
ReactOS provides implementation of how to do this
https://doxygen.reactos.org/da/d02/dll_2win32_2kernel32_2client_2file_2fileinfo_8c_source.html#l00327
*/

template<std::integral ch_type,typename T,std::integral U>
inline std::common_type_t<std::int64_t, std::size_t> seek(basic_nt_io_observer<ch_type> handle,seek_type_t<T>,U i=0,seekdir s=seekdir::cur)
{
	std::int64_t distance_to_move_high{};
	std::int64_t seekposition{seek_precondition<std::int64_t,T,ch_type>(i)};
	if(!win32::SetFilePointerEx(handle.native_handle(),seekposition,std::addressof(distance_to_move_high),static_cast<std::uint32_t>(s)))
		throw_win32_error();
	return distance_to_move_high;
}

template<std::integral ch_type,std::integral U>
inline auto seek(basic_nt_io_observer<ch_type> handle,U i=0,seekdir s=seekdir::cur)
{
	return seek(handle,seek_type<ch_type>,i,s);
}

template<std::integral ch_type>
class basic_nt_io_handle:public basic_nt_io_observer<ch_type>
{
public:
	using char_type = ch_type;
	using native_handle_type = void*;
	constexpr basic_nt_io_handle() noexcept = default;
	template<typename native_hd>
	requires std::same_as<native_handle_type,std::remove_cvref_t<native_hd>>
	constexpr basic_nt_io_handle(native_hd hd) noexcept:basic_nt_io_observer<ch_type>(hd){}
	void reset(native_handle_type newhandle=nullptr) noexcept
	{
		if(this->native_handle()!=reinterpret_cast<void*>(static_cast<std::uintptr_t>(-1)))[[likely]]
			win32::nt::nt_close(this->native_handle());
		this->native_handle()=newhandle;
	}
	void close()
	{
		if(this->native_handle()!=reinterpret_cast<void*>(static_cast<std::uintptr_t>(-1)))[[likely]]
		{
			auto status{win32::nt::nt_close(this->native_handle())};
			if(status)[[unlikely]]
				throw_nt_status(status);
			this->native_handle()=reinterpret_cast<void*>(static_cast<std::uintptr_t>(-1));
		}
	}
	basic_nt_io_handle(basic_nt_io_handle const&)=delete;//Todo copy with ZwDuplicateObject or NtDuplicateObject??
	basic_nt_io_handle& operator=(basic_nt_io_handle const&)=delete;

	constexpr basic_nt_io_handle(basic_nt_io_handle&& b) noexcept:
		basic_nt_io_handle<ch_type>(b.native_handle())
	{
		b.native_handle()=reinterpret_cast<void*>(static_cast<std::uintptr_t>(-1));
	}
	basic_nt_io_handle& operator=(basic_nt_io_handle&& b) noexcept
	{
		if(std::addressof(b)!=this)
		{
			if(this->native_handle()!=reinterpret_cast<void*>(static_cast<std::uintptr_t>(-1)))[[likely]]
				win32::nt::nt_close(this->native_handle());
			this->native_handle() = b.native_handle();
			b.native_handle()=reinterpret_cast<void*>(static_cast<std::uintptr_t>(-1));
		}
		return *this;
	}
};


template<std::integral ch_type>
class basic_nt_file:public basic_nt_io_handle<ch_type>
{
public:
	using char_type = ch_type;
	using native_handle_type = void*;
	constexpr basic_nt_file()=default;
	template<typename native_hd>
	requires std::same_as<native_handle_type,std::remove_cvref_t<native_hd>>
	constexpr basic_nt_file(native_hd hd):basic_nt_io_handle<ch_type>(hd){}
	basic_nt_file(cstring_view filename,open_mode om,perms pm=static_cast<perms>(436)):
		basic_nt_io_handle<ch_type>(details::nt::nt_create_file_impl(filename,details::nt::calculate_nt_open_mode(om,pm)))
	{
	}
	basic_nt_file(nt_at_entry nate,cstring_view filename,open_mode om,perms pm=static_cast<perms>(436)):
		basic_nt_io_handle<char_type>(details::nt::nt_create_file_directory_impl(nate.handle,filename,details::nt::calculate_nt_open_mode(om,pm)))
	{
	}
	basic_nt_file(wcstring_view filename,open_mode om,perms pm=static_cast<perms>(436)):
		basic_nt_io_handle<ch_type>(details::nt::nt_create_file_impl(filename,details::nt::calculate_nt_open_mode(om,pm)))
	{
	}
	basic_nt_file(nt_at_entry nate,wcstring_view filename,open_mode om,perms pm=static_cast<perms>(436)):
		basic_nt_io_handle<char_type>(details::nt::nt_create_file_directory_impl(nate.handle,filename,details::nt::calculate_nt_open_mode(om,pm)))
	{
	}
 	basic_nt_file(basic_nt_file const&)=default;
	basic_nt_file& operator=(basic_nt_file const&)=default;
	constexpr basic_nt_file(basic_nt_file&&) noexcept=default;
	basic_nt_file& operator=(basic_nt_file&&) noexcept=default;
	~basic_nt_file()
	{
		if(this->native_handle()!=reinterpret_cast<void*>(static_cast<std::uintptr_t>(-1)))[[likely]]
			win32::nt::nt_close(this->native_handle());
	}
};
using nt_io_observer=basic_nt_io_observer<char>;
using nt_io_handle=basic_nt_io_handle<char>;
using nt_file=basic_nt_file<char>;

using u8nt_io_observer=basic_nt_io_observer<char8_t>;
using u8nt_io_handle=basic_nt_io_handle<char8_t>;
using u8nt_file=basic_nt_file<char8_t>;

template<std::integral alias_char_type,std::integral char_type>
inline constexpr void const* print_alias_define(io_alias_t<alias_char_type>,basic_nt_io_observer<char_type> v)
{
	return v.handle;
}

}
