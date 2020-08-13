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
std::uint32_t ObjAttributes{0x00000040};
};

/*
https://docs.microsoft.com/en-us/windows/win32/secauthz/access-mask-format
*/

inline constexpr nt_open_mode calculate_nt_open_mode(open_mode value)
{
	value&=~open_mode::ate;
	nt_open_mode mode;
	if((value&open_mode::app)!=open_mode::none)
		mode.DesiredAccess|=4;		//FILE_APPEND_DATA
	else if((value&open_mode::out)!=open_mode::none)
		mode.DesiredAccess|=0x120116;	//FILE_GENERIC_WRITE
	if((value&open_mode::in)!=open_mode::none)
		mode.DesiredAccess|=0x120089;	//FILE_GENERIC_READ
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
		mode.FileAttributes|=0x4;							//FILE_ATTRIBUTE_SYSTEM
		set_normal={};
	}
	if((value&open_mode::offline)!=open_mode::none)
	{
		mode.FileAttributes|=0x1000;						//FILE_ATTRIBUTE_OFFLINE
		set_normal={};
	}
	if((value&open_mode::directory)!=open_mode::none)
	{
		mode.FileAttributes|=0x10;						//FILE_ATTRIBUTE_DIRECTORY
		set_normal={};
	}
	if(set_normal)[[likely]]
		mode.FileAttributes|=0x80;						//FILE_ATTRIBUTE_NORMAL


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

*/
	if((value&open_mode::excl)!=open_mode::none)
	{
		mode.CreateDisposition=1;		//FILE_OPEN
		if((value&open_mode::trunc)!=open_mode::none)
			throw_posix_error(EINVAL);
	}
	else if ((value&open_mode::trunc)!=open_mode::none)
	{
		if((value&open_mode::creat)!=open_mode::none)
			mode.CreateDisposition=2;	//FILE_CREATE
		else if((value&open_mode::in)!=open_mode::none)
			mode.CreateDisposition=0;	//FILE_SUPERSEDE
		else
			throw_posix_error(EINVAL);
	}
	else if((value&open_mode::in)==open_mode::none)
	{
		if((value&open_mode::app)!=open_mode::none)
			mode.CreateDisposition=3;	//FILE_OPEN_IF
		else
			mode.CreateDisposition=5; 	//FILE_OVERWRITE_IF
	}
	else
		mode.CreateDisposition=1;		//FILE_OPEN
	if((value&open_mode::directory)==open_mode::none)
		mode.CreateOptions|=0x00000040;	//FILE_NON_DIRECTORY_FILE 0x00000040
	else
		mode.CreateOptions|=0x00000001;	//FILE_DIRECTORY_FILE 0x00000001
	if((value&open_mode::no_block)==open_mode::none)
		mode.CreateOptions|=0x00000020;	//FILE_SYNCHRONOUS_IO_NONALERT 0x00000020
	else
		mode.CreateOptions|=0x00000010;	//FILE_SYNCHRONOUS_IO_ALERT 0x00000010
	if((value&open_mode::random_access)==open_mode::none)
		mode.CreateOptions|=0x00000004;	//FILE_SEQUENTIAL_ONLY 0x00000004
	else
		mode.CreateOptions|=0x00000800;
	return mode;
}

template<open_mode om>
struct nt_file_openmode_single
{
	inline static constexpr nt_open_mode mode = calculate_nt_open_mode(om);
};


inline void* nt_create_file_no_directory_detail(std::string_view filename,nt_open_mode mode)
{
	char16_t const* part_name{};
	win32::nt::rtl_relative_name_u relative_name{};
	win32::nt::unicode_string nt_name{};
	{
	details::temp_unique_arr_ptr<char16_t> buffer(filename.size()+1);
	*utf_code_convert(filename.data(),filename.data()+filename.size(),buffer.data())=0;
	if(!win32::nt::rtl_dos_path_name_to_nt_path_name_u(buffer.data(),std::addressof(nt_name),std::addressof(part_name),std::addressof(relative_name)))
		throw_nt_error(0xC0000039);
	}
	win32::nt::rtl_unicode_string_unique_ptr us_ptr{std::addressof(nt_name)};
	std::uint16_t const nt_part_name_bytes{static_cast<std::uint16_t>((nt_name.Buffer+static_cast<std::size_t>(nt_name.Length)/sizeof(char16_t)-part_name)*2)};
	win32::nt::unicode_string nt_part_name{
		.Length=nt_part_name_bytes,
		.MaximumLength=nt_part_name_bytes,
		.Buffer=const_cast<char16_t*>(part_name)};
	win32::nt::io_status_block block{};
	win32::nt::object_attributes obj{.Length=sizeof(win32::nt::object_attributes),
		.RootDirectory=nullptr,
		.ObjectName=std::addressof(nt_name),
		.Attributes=mode.ObjAttributes	//Todo
	};
	void* handle{};
	auto const status{win32::nt::nt_create_file(
	std::addressof(handle),mode.DesiredAccess,std::addressof(obj),std::addressof(block),nullptr,mode.FileAttributes,
	mode.ShareAccess,mode.CreateDisposition,mode.CreateOptions,nullptr,0)};
	if(status)
		throw_nt_error(status);
	return handle;
}

}

template<std::integral ch_type>
class basic_nt_io_observer
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
	inline constexpr native_handle_type release() noexcept
	{
		auto temp{handle};
		handle=nullptr;
		return temp;
	}
	inline constexpr void reset(native_handle_type newhandle=nullptr) noexcept
	{
		handle=newhandle;
	}
};

template<std::integral ch_type,std::contiguous_iterator Iter>
inline Iter read(basic_nt_io_observer<ch_type> obs,Iter begin,Iter end)
{
	std::size_t to_read((end-begin)*sizeof(*begin));
	if constexpr(4<sizeof(std::size_t))
		if(static_cast<std::size_t>(UINT32_MAX)<to_read)
			to_read=static_cast<std::size_t>(UINT32_MAX);
	win32::nt::io_status_block block{};
	auto const status{win32::nt::nt_read_file(obs.handle,nullptr,nullptr,nullptr,
		std::addressof(block), std::to_address(begin), static_cast<std::uint32_t>(to_read), nullptr, nullptr)};
	if(status)
		throw_nt_error(status);
	return begin+block.Information/sizeof(*begin);
}

template<std::integral ch_type,std::contiguous_iterator Iter>
inline Iter write(basic_nt_io_observer<ch_type> obs,Iter cbegin,Iter cend)
{
	std::size_t to_write((cend-cbegin)*sizeof(*cbegin));
	if constexpr(4<sizeof(std::size_t))
		if(static_cast<std::size_t>(UINT32_MAX)<to_write)
			to_write=static_cast<std::size_t>(UINT32_MAX);
	win32::nt::io_status_block block{};
	auto const status{win32::nt::nt_write_file(obs.handle,nullptr,nullptr,nullptr,
		std::addressof(block), std::to_address(cbegin), static_cast<std::uint32_t>(to_write), nullptr, nullptr)};
	if(status)
		throw_nt_error(status);
	return cbegin+block.Information/sizeof(*cbegin);
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
	constexpr basic_nt_io_handle(native_handle_type hd) noexcept:basic_nt_io_observer<ch_type>(hd){}
	void reset(native_handle_type newhandle=nullptr) noexcept
	{
		if(this->native_handle())[[likely]]
			win32::nt::nt_close(this->native_handle());
		this->native_handle()=newhandle;
	}
	void close()
	{
		if(this->native_handle())[[likely]]
		{
			auto status{win32::nt::nt_close(this->native_handle())};
			if(status)[[unlikely]]
				throw_nt_status(status);
			this->native_handle()=nullptr;
		}
	}
	basic_nt_io_handle(basic_nt_io_handle const&)=delete;//Todo copy with ZwDuplicateObject or NtDuplicateObject??
	basic_nt_io_handle& operator=(basic_nt_io_handle const&)=delete;

	constexpr basic_nt_io_handle(basic_nt_io_handle&& b) noexcept:
		basic_nt_io_handle<ch_type>(b.native_handle())
	{
		b.native_handle()=nullptr;
	}
	basic_nt_io_handle& operator=(basic_nt_io_handle&& b) noexcept
	{
		if(std::addressof(b)!=this)
		{
			if(this->native_handle())[[likely]]
				win32::nt::nt_close(this->native_handle());
			this->native_handle() = b.native_handle();
			b.native_handle()=nullptr;
		}
		return *this;
	}
};


template<std::integral ch_type>
class basic_nt_file:public basic_nt_io_handle<ch_type>
{
	void seek_end_local()
	{
		basic_nt_file<char> local{this->native_handle()};
		seek(*this,0,seekdir::end);
		local.release();
	};
public:
	using char_type = ch_type;
	using native_handle_type = void*;
	constexpr basic_nt_file()=default;
	template<typename native_hd>
	requires std::same_as<native_handle_type,std::remove_cvref_t<native_hd>>
	constexpr basic_nt_file(native_hd hd):basic_nt_io_handle<ch_type>(hd){}
	template<open_mode om>
	basic_nt_file(std::string_view filename,open_interface_t<om>):
		basic_nt_io_handle<ch_type>(details::nt::nt_create_file_no_directory_detail(filename,details::nt::nt_file_openmode_single<om>::mode))
	{
		if constexpr ((om&open_mode::ate)!=open_mode::none)
			seek_end_local();
	}
	basic_nt_file(std::string_view filename,open_mode om):
		basic_nt_io_handle<ch_type>(details::nt::nt_create_file_no_directory_detail(filename,details::nt::calculate_nt_open_mode(om)))
	{
		if((om&open_mode::ate)!=open_mode::none)
			seek_end_local();
	}
	basic_nt_file(std::string_view filename,std::string_view mode):basic_nt_file(filename,fast_io::from_c_mode(mode)){}
	basic_nt_file(basic_nt_file const&)=default;
	basic_nt_file& operator=(basic_nt_file const&)=default;
	constexpr basic_nt_file(basic_nt_file&&) noexcept=default;
	basic_nt_file& operator=(basic_nt_file&&) noexcept=default;
	~basic_nt_file()
	{
		if(this->native_handle())[[likely]]
			win32::nt::nt_close(this->native_handle());
	}
};
using nt_io_observer=basic_nt_io_observer<char>;
using nt_io_handle=basic_nt_io_handle<char>;
using nt_file=basic_nt_file<char>;

using u8nt_io_observer=basic_nt_io_observer<char8_t>;
using u8nt_io_handle=basic_nt_io_handle<char8_t>;
using u8nt_file=basic_nt_file<char8_t>;

template<std::integral char_type>
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<basic_nt_io_observer<char_type>>)
{
	return print_reserve_size(io_reserve_type<void*>);
}

template<std::integral char_type,std::contiguous_iterator caiter,typename U>
inline constexpr caiter print_reserve_define(io_reserve_type_t<basic_nt_io_observer<char_type>>,caiter iter,U&& v)
{
	return print_reserve_define(io_reserve_type<void*>,iter,v.handle);
}

}
