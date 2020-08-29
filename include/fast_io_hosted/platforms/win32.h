#pragma once

namespace fast_io
{
namespace details
{
inline void* create_win32_temp_file()
{
	std::array<wchar_t,512> arr;
	std::uint32_t temp_path_size{win32::GetTempPathW(300,arr.data())};
	if(temp_path_size==0)
		throw_win32_error();
	for(std::size_t i{};i!=128;++i)
	{
		std::array<char,512> buffer1;
		secure_clear_guard<char> guard(buffer1.data(),buffer1.size());
		if(!win32::SystemFunction036(buffer1.data(),buffer1.size()))[[unlikely]]
			continue;
		fast_io::sha256 sha;
		fast_io::hash_processor processor(sha);
		write(processor,buffer1.begin(),buffer1.end());
		processor.do_final();
		ospan osp(arr);
		obuffer_set_curr(osp,arr.data()+temp_path_size);
		print(osp,sha,L".tmp");
		put(osp,0);
		void* handle = win32::CreateFileW(arr.data(),
		0x40000000|0x80000000,	//GENERIC_READ|GENERIC_WRITE
		0,		//Prevents other processes from opening a file or device if they request delete, read, or write access.
		nullptr,
		1,		//CREATE_NEW
		0x100|0x04000000|0x01000000|0x08000000,	//FILE_ATTRIBUTE_TEMPORARY|FILE_FLAG_DELETE_ON_CLOSE|FILE_FLAG_POSIX_SEMANTICS|FILE_FLAG_SEQUENTIAL_SCAN
		nullptr);
		if(handle==(void*) (std::intptr_t)-1)[[unlikely]]
		{
			auto last_error{win32::GetLastError()};
			if(last_error==80)	//ERROR_FILE_EXISTS
				continue;
			throw_win32_error();
		}
		return handle;
	}
	throw_win32_error();
	return nullptr;
}

template<typename... Args>
requires (sizeof...(Args)==4)
inline auto create_io_completion_port(Args&&... args)
{
	auto ptr{fast_io::win32::CreateIoCompletionPort(std::forward<Args>(args)...)};
	if(ptr==nullptr)[[unlikely]]
		throw_win32_error();
	return ptr;
}

template<bool inherit=false>
inline void* create_file_a_impld(wchar_t const* lpFileName,
std::uint32_t dwDesiredAccess,
std::uint32_t dwShareMode,
std::uint32_t dwCreationDisposition,
std::uint32_t dwFlagsAndAttributes)
{
	if constexpr(inherit)
	{
		win32::security_attributes sec_attr
		{
			.nLength=sizeof(win32::security_attributes),
			.bInheritHandle = true
		};
		auto handle(win32::CreateFileW(lpFileName,
		dwDesiredAccess,
		dwShareMode,
		std::addressof(sec_attr),
		dwCreationDisposition,
		dwFlagsAndAttributes,
		nullptr));
		if(handle==((void*) (std::intptr_t)-1))
			throw_win32_error();
		return handle;
	}
	else
	{
		auto handle(win32::CreateFileW(lpFileName,
		dwDesiredAccess,
		dwShareMode,
		nullptr,
		dwCreationDisposition,
		dwFlagsAndAttributes,
		nullptr));
		if(handle==((void*) (std::intptr_t)-1))
			throw_win32_error();
		return handle;
	}
}

template<bool inherit=false,typename... Args>
requires (sizeof...(Args)==4)
inline void* create_file_a_impl(std::string_view path,Args&& ...args)
{
	if(path.size()<511)[[likely]]
	{
		std::array<wchar_t,512> buffer;
		*utf_code_convert(path.data(),path.data()+path.size(),buffer.data())=0;
		return create_file_a_impld<inherit>(buffer.data(),std::forward<Args>(args)...);
	}
	else
	{
		details::temp_unique_arr_ptr<wchar_t> buffer(path.size()+1);
		*utf_code_convert(path.data(),path.data()+path.size(),buffer.ptr)=0;
		return create_file_a_impld<inherit>(buffer.ptr,std::forward<Args>(args)...);
	}
}

struct win32_open_mode
{
std::uint32_t dwDesiredAccess{};
std::uint32_t dwShareMode{1|2};//FILE_SHARE_READ|FILE_SHARE_WRITE
std::uint32_t dwCreationDisposition{};	//depends on EXCL
std::uint32_t dwFlagsAndAttributes{};//=128|0x10000000;//FILE_ATTRIBUTE_NORMAL|FILE_FLAG_RANDOM_ACCESS
};

inline constexpr win32_open_mode calculate_win32_open_mode(open_mode value)
{
	value&=~open_mode::ate;
	win32_open_mode mode;
	if((value&open_mode::app)!=open_mode::none)
		mode.dwDesiredAccess|=4;//FILE_APPEND_DATA
	else if((value&open_mode::out)!=open_mode::none)
		mode.dwDesiredAccess|=0x40000000;//GENERIC_WRITE
	if((value&open_mode::in)!=open_mode::none)
	{
		mode.dwDesiredAccess|=0x80000000;//GENERIC_READ
		if((value&open_mode::out)!=open_mode::none&&((value&open_mode::app)!=open_mode::none&&(value&open_mode::trunc)!=open_mode::none))
			mode.dwDesiredAccess|=0x40000000;//GENERIC_WRITE
	}
/*
Referenced partially from ReactOS
https://github.com/changloong/msvcrt/blob/master/io/wopen.c



https://docs.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-createfilew

CREATE_ALWAYS
2
Creates a new file, always.
If the specified file exists and is writable, the function overwrites the file, the function succeeds, and last-error code is set to ERROR_ALREADY_EXISTS (183).

If the specified file does not exist and is a valid path, a new file is created, the function succeeds, and the last-error code is set to zero.

For more information, see the Remarks section of this topic.

CREATE_NEW
1
Creates a new file, only if it does not already exist.
If the specified file exists, the function fails and the last-error code is set to ERROR_FILE_EXISTS (80).

If the specified file does not exist and is a valid path to a writable location, a new file is created.

OPEN_ALWAYS
4
Opens a file, always.
If the specified file exists, the function succeeds and the last-error code is set to ERROR_ALREADY_EXISTS (183).

If the specified file does not exist and is a valid path to a writable location, the function creates a file and the last-error code is set to zero.

OPEN_EXISTING
3
Opens a file or device, only if it exists.
If the specified file or device does not exist, the function fails and the last-error code is set to ERROR_FILE_NOT_FOUND (2).

For more information about devices, see the Remarks section.

TRUNCATE_EXISTING
5
Opens a file and truncates it so that its size is zero bytes, only if it exists.
If the specified file does not exist, the function fails and the last-error code is set to ERROR_FILE_NOT_FOUND (2).

The calling process must open the file with the GENERIC_WRITE bit set as part of the dwDesiredAccess parameter.



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
			mode.dwCreationDisposition=1;// CREATE_NEW
		else
			mode.dwCreationDisposition=2;// CREATE_ALWAYS
	}
	else if((value&open_mode::in)==open_mode::none)
	{
		if((value&open_mode::app)!=open_mode::none)
			mode.dwCreationDisposition=4;//OPEN_ALWAYS
		else if((value&open_mode::out)!=open_mode::none)
		{
			if((value&open_mode::excl)!=open_mode::none)
				mode.dwCreationDisposition=1;// CREATE_NEW
			else
				mode.dwCreationDisposition=2;// CREATE_ALWAYS
		}
	}
	else if((value&open_mode::app)!=open_mode::none)
		mode.dwCreationDisposition=4;//OPEN_ALWAYS
	else
		mode.dwCreationDisposition=3;//OPEN_EXISTING
	if((value&open_mode::direct)!=open_mode::none)
		mode.dwFlagsAndAttributes|=0x20000000;//FILE_FLAG_NO_BUFFERING
	if((value&open_mode::sync)!=open_mode::none)
		mode.dwFlagsAndAttributes|=0x80000000;//FILE_FLAG_WRITE_THROUGH
	if((value&open_mode::no_block)!=open_mode::none)
		mode.dwFlagsAndAttributes|=0x40000000;//FILE_FLAG_OVERLAPPED
	if((value&open_mode::follow)!=open_mode::none)
		mode.dwFlagsAndAttributes|=0x00200000;	//FILE_FLAG_OPEN_REPARSE_POINT
	if((value&open_mode::directory)!=open_mode::none)
		mode.dwFlagsAndAttributes|=0x02000000;	//FILE_FLAG_BACKUP_SEMANTICS
	bool set_normal{true};
	if((value&open_mode::archive)!=open_mode::none)
	{
		mode.dwFlagsAndAttributes|=0x20;		//FILE_ATTRIBUTE_ARCHIVE
		set_normal={};
	}
	if((value&open_mode::encrypted)!=open_mode::none)
	{
		mode.dwFlagsAndAttributes|=0x4000;		//FILE_ATTRIBUTE_ENCRYPTED
		set_normal={};
	}
	if((value&open_mode::hidden)!=open_mode::none)
	{
		mode.dwFlagsAndAttributes|=0x2;			//FILE_ATTRIBUTE_HIDDEN
		set_normal={};
	}
	if((value&open_mode::compressed)!=open_mode::none)
	{
		mode.dwFlagsAndAttributes|=0x800;		//FILE_ATTRIBUTE_COMPRESSED
		set_normal={};
	}
	if((value&open_mode::system)!=open_mode::none)
	{
		mode.dwFlagsAndAttributes|=0x4;							//FILE_ATTRIBUTE_SYSTEM
		set_normal={};
	}
	if((value&open_mode::offline)!=open_mode::none)
	{
		mode.dwFlagsAndAttributes|=0x1000;						//FILE_ATTRIBUTE_OFFLINE
		set_normal={};
	}
	if((value&open_mode::directory)!=open_mode::none)
	{
		mode.dwFlagsAndAttributes|=0x10;						//FILE_ATTRIBUTE_DIRECTORY
		set_normal={};
	}
	if(set_normal)[[likely]]
		mode.dwFlagsAndAttributes|=0x80;						//FILE_ATTRIBUTE_NORMAL
	if((value&open_mode::random_access)==open_mode::none)
		mode.dwFlagsAndAttributes|=0x10000000;		//FILE_FLAG_SEQUENTIAL_SCAN
	else
		mode.dwFlagsAndAttributes|=0x08000000;		//FILE_FLAG_RANDOM_ACCESS
	if((value&open_mode::no_recall)!=open_mode::none)
		mode.dwFlagsAndAttributes|=0x00100000;					//FILE_FLAG_OPEN_NO_RECALL
	if((value&open_mode::posix_semantics)!=open_mode::none)
		mode.dwFlagsAndAttributes|=0x01000000;					//FILE_FLAG_POSIX_SEMANTICS
	if((value&open_mode::session_aware)!=open_mode::none)
		mode.dwFlagsAndAttributes|=0x00800000;					//FILE_FLAG_SESSION_AWARE
	if((value&open_mode::temporary)!=open_mode::none)
	{
		mode.dwFlagsAndAttributes|=0x04000000;					//FILE_FLAG_DELETE_ON_CLOSE
		mode.dwFlagsAndAttributes|=0x100;					//FILE_ATTRIBUTE_TEMPORARY
	}
	return mode;
}

inline constexpr std::uint32_t dw_flag_attribute_with_perms(std::uint32_t dw_flags_and_attributes,perms pm)
{
	if((pm&perms::owner_write)==perms::none)
		return dw_flags_and_attributes|1;//dw_flags_and_attributes|FILE_ATTRIBUTE_READONLY
	return dw_flags_and_attributes;
}

inline constexpr win32_open_mode calculate_win32_open_mode_with_perms(open_mode om,perms pm)
{
	auto m(calculate_win32_open_mode(om));
	m.dwFlagsAndAttributes=dw_flag_attribute_with_perms(m.dwFlagsAndAttributes,pm);
	return m;
}

template<open_mode om,perms pm>
struct win32_file_openmode
{
	inline static constexpr win32_open_mode mode = calculate_win32_open_mode_with_perms(om,pm);
};

template<open_mode om>
struct win32_file_openmode_single
{
	inline static constexpr win32_open_mode mode = calculate_win32_open_mode(om);
};
}
template<std::integral ch_type>
class basic_win32_io_observer
{
public:
	using native_handle_type = void*;
	using char_type = ch_type;
	using async_scheduler_type = basic_win32_io_observer<char>;
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
	explicit constexpr operator basic_nt_io_observer<char_type>() const noexcept
	{
		return basic_nt_io_observer<char_type>{handle};
	}
	inline native_handle_type release() noexcept
	{
		auto temp{handle};
		handle=reinterpret_cast<void*>(static_cast<std::uintptr_t>(-1));
		return temp;
	}
	inline constexpr void reset(native_handle_type newhandle=reinterpret_cast<void*>(static_cast<std::uintptr_t>(-1))) noexcept
	{
		handle=newhandle;
	}
};

template<std::integral ch_type>
class basic_win32_io_handle:public basic_win32_io_observer<ch_type>
{
public:
	using native_handle_type = void*;
	using char_type = ch_type;
	using async_scheduler_type = basic_win32_io_observer<char>;
	constexpr basic_win32_io_handle() noexcept =default;
	constexpr basic_win32_io_handle(native_handle_type handle) noexcept:
		basic_win32_io_observer<ch_type>{handle}{}
	basic_win32_io_handle(basic_win32_io_handle const& other)
	{
		auto const current_process(win32::GetCurrentProcess());
		if(!win32::DuplicateHandle(current_process,other.native_handle(),current_process,std::addressof(this->native_handle()), 0, true, 2/*DUPLICATE_SAME_ACCESS*/))
			throw_win32_error();
	}
	basic_win32_io_handle& operator=(basic_win32_io_handle const& other)
	{
		auto const current_process(win32::GetCurrentProcess());
		void* new_handle{};
		if(!win32::DuplicateHandle(current_process,other.native_handle(),current_process,std::addressof(new_handle), 0, true, 2/*DUPLICATE_SAME_ACCESS*/))
			throw_win32_error();
		if(*this)[[likely]]
			fast_io::win32::CloseHandle(this->native_handle());
		this->native_handle()=new_handle;
		return *this;
	}
	basic_win32_io_handle(basic_win32_io_handle&& b) noexcept:
		basic_win32_io_observer<ch_type>(b.native_handle())
	{
		b.native_handle()=reinterpret_cast<void*>(static_cast<std::uintptr_t>(-1));
	}
	basic_win32_io_handle& operator=(basic_win32_io_handle&& b) noexcept
	{
		if(std::addressof(b)!=this)
		{
			if(*this)[[likely]]
				fast_io::win32::CloseHandle(this->native_handle());
			this->native_handle() = b.native_handle();
			b.native_handle()=reinterpret_cast<void*>(static_cast<std::uintptr_t>(-1));
		}
		return *this;
	}
	void reset(native_handle_type newhandle=reinterpret_cast<void*>(static_cast<std::uintptr_t>(-1))) noexcept
	{
		if(*this)[[likely]]
			fast_io::win32::CloseHandle(this->native_handle());
		this->native_handle()=newhandle;
	}
	void close()
	{
		if(*this)[[likely]]
		{
			if(!fast_io::win32::CloseHandle(this->native_handle()))[[unlikely]]
				throw_win32_error();
			this->native_handle()=nullptr;
		}
	}
};

template<std::integral ch_type>
inline constexpr auto redirect_handle(basic_win32_io_observer<ch_type> hd)
{
	return hd.native_handle();
}

namespace win32::details
{

inline std::size_t read_impl(void* handle,void* begin,std::size_t to_read)
{
	std::uint32_t number_of_bytes_read{};
	if constexpr(4<sizeof(std::size_t))
		if(static_cast<std::size_t>(UINT32_MAX)<to_read)
			to_read=static_cast<std::size_t>(UINT32_MAX);
	if(!win32::ReadFile(handle,begin,static_cast<std::uint32_t>(to_read),std::addressof(number_of_bytes_read),nullptr))
	{
		auto err(win32::GetLastError());
		if(err==109)
			return 0;
		throw_win32_error(err);
	}
	return number_of_bytes_read;
}

inline std::size_t write_impl(void* handle,void const* cbegin,std::size_t to_write)
{
	std::uint32_t number_of_bytes_written{};
	if constexpr(4<sizeof(std::size_t))
		if(static_cast<std::size_t>(UINT32_MAX)<to_write)
			to_write=static_cast<std::size_t>(UINT32_MAX);
	if(!win32::WriteFile(handle,cbegin,static_cast<std::uint32_t>(to_write),std::addressof(number_of_bytes_written),nullptr))
		throw_win32_error();
	return number_of_bytes_written;
}

inline std::common_type_t<std::size_t,std::uint64_t> seek_impl(void* handle,std::common_type_t<std::ptrdiff_t,std::int64_t> offset,seekdir s)
{
	std::int64_t distance_to_move_high{};
	if(!win32::SetFilePointerEx(handle,offset,std::addressof(distance_to_move_high),static_cast<std::uint32_t>(s)))
		throw_win32_error();
	return distance_to_move_high;
}

}

template<std::integral ch_type>
inline std::common_type_t<std::size_t,std::uint64_t> seek(basic_win32_io_observer<ch_type> handle,std::common_type_t<std::ptrdiff_t,std::int64_t> offset=0,seekdir s=seekdir::cur)
{
	return win32::details::seek_impl(handle.handle,offset,s);
}

template<std::integral ch_type,std::contiguous_iterator Iter>
inline Iter read(basic_win32_io_observer<ch_type> handle,Iter begin,Iter end)
{
	return begin+win32::details::read_impl(handle.handle,std::to_address(begin),(end-begin)*sizeof(*begin))/sizeof(*begin);
}

template<std::integral ch_type,std::contiguous_iterator Iter>
inline Iter write(basic_win32_io_observer<ch_type> handle,Iter cbegin,Iter cend)
{
	return cbegin+win32::details::write_impl(handle.handle,std::to_address(cbegin),(cend-cbegin)*sizeof(*cbegin))/sizeof(*cbegin);
}

template<std::integral ch_type,std::contiguous_iterator Iter>
inline void async_read_callback(basic_win32_io_observer<char>,basic_win32_io_observer<ch_type> h,Iter cbegin,Iter cend,
	iocp_overlapped_observer callback,std::ptrdiff_t offset=0)
{
	std::size_t to_read((cend-cbegin)*sizeof(*cbegin));
	if constexpr(4<sizeof(std::size_t))
		if(static_cast<std::size_t>(UINT32_MAX)<to_read)
			to_read=static_cast<std::size_t>(UINT32_MAX);
	if constexpr(4<sizeof(std::size_t))
	{
		callback.native_handle()->Offset=static_cast<std::size_t>(offset)&std::numeric_limits<std::uint32_t>::max();
		callback.native_handle()->OffsetHigh=static_cast<std::size_t>(offset)>>static_cast<std::size_t>(32);
	}
	else
	{
		callback.native_handle()->Offset=static_cast<std::uint32_t>(offset);
		callback.native_handle()->OffsetHigh=0;
	}
	if(!win32::ReadFile(h.native_handle(),std::to_address(cbegin),static_cast<std::uint32_t>(to_read),nullptr,callback.native_handle()))[[likely]]
	{
		auto err(win32::GetLastError());
		if(err==997)[[likely]]
			return;
		throw_win32_error(err);
	}
}

template<std::integral char_type>
inline constexpr io_type_t<basic_win32_io_observer<char>> async_scheduler_type(basic_win32_io_observer<char_type>)
{
	return {};
}

template<std::integral char_type>
inline constexpr io_type_t<iocp_overlapped> async_overlapped_type(basic_win32_io_observer<char_type>)
{
	return {};
}

template<std::integral ch_type,std::contiguous_iterator Iter>
inline void async_write_callback(basic_win32_io_observer<char> over,basic_win32_io_observer<ch_type> h,Iter cbegin,Iter cend,
	iocp_overlapped_observer callback,std::ptrdiff_t offset=0)
{
	std::size_t to_write((cend-cbegin)*sizeof(*cbegin));
	if constexpr(4<sizeof(std::size_t))
		if(static_cast<std::size_t>(UINT32_MAX)<to_write)
			to_write=static_cast<std::size_t>(UINT32_MAX);
	if constexpr(4<sizeof(std::size_t))
	{
		callback.native_handle()->Offset=static_cast<std::size_t>(offset)&std::numeric_limits<std::uint32_t>::max();
		callback.native_handle()->OffsetHigh=static_cast<std::size_t>(offset)>>static_cast<std::size_t>(32);
	}
	else
	{
		callback.native_handle()->Offset=static_cast<std::uint32_t>(offset);
		callback.native_handle()->OffsetHigh=0;
	}
	if(!win32::WriteFile(h.native_handle(),std::to_address(cbegin),static_cast<std::uint32_t>(to_write),nullptr,callback.native_handle()))[[likely]]
	{
		auto err(win32::GetLastError());
		if(err==997)[[likely]]
			return;
		throw_win32_error(err);
	}
}

template<std::integral ch_type>
inline void cancel(basic_win32_io_observer<ch_type> h)
{
	if(!fast_io::win32::CancelIo(h.native_handle()))
		throw_win32_error();
}

template<std::integral ch_type,typename... Args>
requires requires(basic_win32_io_observer<ch_type> h,Args&& ...args)
{
	fast_io::win32::DeviceIoControl(h.native_handle(),std::forward<Args>(args)...);
}
inline void io_control(basic_win32_io_observer<ch_type> h,Args&& ...args)
{
	if(!fast_io::win32::DeviceIoControl(h.native_handle(),std::forward<Args>(args)...))
		throw_win32_error();
}

/*
template<std::integral ch_type>
inline auto memory_map_in_handle(basic_win32_io_observer<ch_type> handle)
{
	return handle.native_handle();
}
*/

template<std::integral ch_type>
class basic_win32_file:public basic_win32_io_handle<ch_type>
{
	void seek_end_local()
	{
		basic_win32_file<ch_type> local{this->native_handle()};
		seek(*this,0,seekdir::end);
		local.release();
	};
public:
	using char_type=ch_type;
	using native_handle_type = basic_win32_io_handle<ch_type>::native_handle_type;
	using basic_win32_io_handle<ch_type>::native_handle;
	using async_scheduler_type = basic_win32_io_observer<char>;
	explicit constexpr basic_win32_file()=default;
	template<typename native_hd>
	requires std::same_as<native_handle_type,std::remove_cvref_t<native_hd>>
	explicit constexpr basic_win32_file(native_hd handle) noexcept:basic_win32_io_handle<ch_type>(handle){}
	template<typename ...Args>
	requires requires(Args&& ...args)
	{
		{win32::CreateFileW(std::forward<Args>(args)...)}->std::same_as<native_handle_type>;
	}
	basic_win32_file(fast_io::native_interface_t,Args&& ...args):basic_win32_io_handle<char_type>(win32::CreateFileW(std::forward<Args>(args)...))
	{
		if(native_handle()==((void*) (std::intptr_t)-1))
			throw_win32_error();
	}
	template<open_mode om,perms pm>
	basic_win32_file(std::string_view filename,open_interface_t<om>,perms_interface_t<pm>):
				basic_win32_io_handle<char_type>(
				details::create_file_a_impl<(om&open_mode::inherit)!=open_mode::none>(filename,
				details::win32_file_openmode<om,pm>::mode.dwDesiredAccess,
				details::win32_file_openmode<om,pm>::mode.dwShareMode,
				details::win32_file_openmode<om,pm>::mode.dwCreationDisposition,
				details::win32_file_openmode<om,pm>::mode.dwFlagsAndAttributes)
				)
	{
		if constexpr ((om&open_mode::ate)!=open_mode::none)
			seek_end_local();
	}
	template<open_mode om>
	basic_win32_file(std::string_view filename,open_interface_t<om>):basic_win32_io_handle<char_type>(
				details::create_file_a_impl<(om&open_mode::inherit)!=open_mode::none>(filename,
				details::win32_file_openmode_single<om>::mode.dwDesiredAccess,
				details::win32_file_openmode_single<om>::mode.dwShareMode,
				details::win32_file_openmode_single<om>::mode.dwCreationDisposition,
				details::win32_file_openmode_single<om>::mode.dwFlagsAndAttributes))
	{
		if constexpr ((om&open_mode::ate)!=open_mode::none)
			seek_end_local();
	}
	template<open_mode om>
	basic_win32_file(std::string_view filename,open_interface_t<om>,perms p):basic_win32_io_handle<char_type>(
				details::create_file_a_impl<(om&open_mode::inherit)!=open_mode::none>(filename,
				details::win32_file_openmode_single<om>::mode.dwDesiredAccess,
				details::win32_file_openmode_single<om>::mode.dwShareMode,
				details::win32_file_openmode_single<om>::mode.dwCreationDisposition,
				details::dw_flag_attribute_with_perms(details::win32_file_openmode_single<om>::mode.dwFlagsAndAttributes,p)))
	{
		if constexpr ((om&open_mode::ate)!=open_mode::none)
			seek_end_local();
	}
	basic_win32_file(std::string_view filename,open_mode om,perms pm=static_cast<perms>(420)):basic_win32_io_handle<char_type>(nullptr)
	{
		auto const mode(details::calculate_win32_open_mode_with_perms(om,pm));
		if((om&open_mode::inherit)==open_mode::none)
		{
			native_handle()=details::create_file_a_impl
			(filename,
			mode.dwDesiredAccess,
			mode.dwShareMode,
			mode.dwCreationDisposition,
			mode.dwFlagsAndAttributes);
		}
		else
		{
			native_handle()=details::create_file_a_impl<true>
				(filename,
				mode.dwDesiredAccess,
				mode.dwShareMode,
				mode.dwCreationDisposition,
				mode.dwFlagsAndAttributes);
		}
		if ((om&open_mode::ate)!=open_mode::none)
			seek_end_local();
	}
	basic_win32_file(std::string_view file,std::string_view mode,perms pm=static_cast<perms>(420)):
		basic_win32_file(file,fast_io::from_c_mode(mode),pm){}
	basic_win32_file(io_async_t) requires(std::same_as<char_type,char>):
		basic_win32_io_handle<char_type>(details::create_io_completion_port(bit_cast<void*>(static_cast<std::uintptr_t>(-1)),nullptr,0,0)){}
	basic_win32_file(io_temp_t):basic_win32_io_handle<char_type>(details::create_win32_temp_file()){}
/*
	template<std::integral dir_char_type,open_mode om>
	basic_win32_file(basic_win32_io_observer<dir_char_type> directory,std::string_view filename,open_interface_t<om>,perms pm=static_cast<perms>(420))
	{
		constexpr auto& mode{details::nt::nt_file_openmode_single<om>::mode};
		wchar_t const* part_name{};
		win32::nt::rtl_relative_name_u relative_name;
		win32::nt::unicode_string nt_name;
		details::temp_unique_arr_ptr<wchar_t> buffer(filename.size());
		auto buffer_end{utf_code_convert(filename.data(),filename.data()+filename.size(),buffer.data())};
		win32::nt::io_status_block block{};
		win32::nt::object_attributes obj{.Length=sizeof(win32::nt::object_attributes),
			.RootDirectory=relative_name.containing_directory,
			.ObjectName=std::addressof(relative_name.relative_name),
			.Attributes=0x00000040	//Todo
		};

		auto const status{win32::nt::nt_create_file(
			std::addressof(this->native_handle()),
		mode.DesiredAccess,std::addressof(obj),std::addressof(block),nullptr,mode.FileAttributes,
		mode.ShareAccess,mode.CreateDisposition,mode.CreateOptions,nullptr,0)};
		if(status)
			throw_nt_error(status);
	}
*/
	template<typename... Args>
	basic_win32_file(io_async_t,basic_win32_io_observer<char> iob,Args&& ...args):basic_win32_file(std::forward<Args>(args)...)
	{
		basic_win32_file<ch_type> guard(this->native_handle());
		details::create_io_completion_port(this->native_handle(),iob.native_handle(),bit_cast<std::uintptr_t>(this->native_handle()),0);
		guard.release();
	}
	~basic_win32_file()
	{
		if(*this)[[likely]]
			fast_io::win32::CloseHandle(this->native_handle());
	}
	constexpr basic_win32_file(basic_win32_file const&)=default;
	constexpr basic_win32_file& operator=(basic_win32_file const&)=default;
	constexpr basic_win32_file(basic_win32_file&&) noexcept=default;
	constexpr basic_win32_file& operator=(basic_win32_file&&) noexcept=default;
};

template<std::integral ch_type>
inline constexpr auto zero_copy_in_handle(basic_win32_io_observer<ch_type> iob)
{
	return iob.handle;
}

template<std::integral ch_type>
inline void truncate(basic_win32_io_observer<ch_type> handle,std::size_t size)
{
	seek(handle,size,seekdir::beg);
	if(!win32::SetEndOfFile(handle.handle))
		throw_win32_error();
}

namespace win32::details
{

inline std::common_type_t<std::size_t,std::uint64_t> file_size_impl(void* handle)
{
/*
Microsoft's Document said 
https://docs.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-getfilesizeex
Windows Store apps:  GetFileSizeEx is not supported. Use GetFileInformationByHandleEx.
*/
	file_standard_info finfo{};
	if(!GetFileInformationByHandleEx(handle,file_info_by_handle_class::FileStandardInfo,std::addressof(finfo),sizeof(finfo)))
		throw_win32_error();
	return finfo.EndOfFile;
}

inline file_type file_type_impl(void* handle)
{
/*
https://docs.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-getfiletype
*/
	std::uint32_t ftvalue{GetFileType(handle)};
	switch(ftvalue)
	{
	case 0x0002:
		return file_type::character;
	case 0x0001:
		return file_type::regular;
	case 0x0003:
		return file_type::fifo;
	case 0x8000:
		return file_type::remote;
	case 0x0000:
	{
		auto errcd=GetLastError();
		if(errcd==0)
			return file_type::unknown;
		throw_win32_error(errcd);
		[[fallthrough]];
	}
	default:
		return file_type::unknown;
	};
}

}

template<std::integral ch_type>
inline std::common_type_t<std::size_t,std::uint64_t> size(basic_win32_io_observer<ch_type> handle)
{
	return win32::details::file_size_impl(handle.handle);
}

template<std::integral ch_type>
inline file_type type(basic_win32_io_observer<ch_type> handle)
{
	return win32::details::file_type_impl(handle.handle);
}

template<std::integral ch_type>
class basic_win32_pipe
{
public:
	using char_type = ch_type;
	using native_handle_type = std::array<basic_win32_file<ch_type>,2>;
	native_handle_type pipes;
	template<typename ...Args>
	basic_win32_pipe(fast_io::native_interface_t, Args&& ...args)
	{
		if(!win32::CreatePipe(
			std::addressof(pipes.front().native_handle()),
			std::addressof(pipes.back().native_handle()),
			std::forward<Args>(args)...))
			throw_win32_error();
	}
	basic_win32_pipe()
	{
		win32::security_attributes sec_attr
		{
			.nLength=sizeof(win32::security_attributes),
			.bInheritHandle = true
		};
		
		if(!win32::CreatePipe(
			std::addressof(pipes.front().native_handle()),
			std::addressof(pipes.back().native_handle()),
			std::addressof(sec_attr),0))
			throw_win32_error();
	}
/*
	template<std::size_t om>
	basic_win32_pipe(open::interface_t<om>):basic_win32_pipe()
	{
		auto constexpr omb(om&~open::binary.value);
		static_assert(omb==open::in.value||omb==open::out.value||omb==(open::in.value|open::out.value),u8"pipe open mode must be in or out");
		if constexpr (!(om&~open::in.value)&&(om&~open::out.value))
			pipes.front().close();
		if constexpr ((om&~open::in.value)&&!(om&~open::out.value))
			pipes.back().close();
	}*/
	auto& native_handle()
	{
		return pipes;
	}
	auto& in()
	{
		return pipes.front();
	}
	auto& out()
	{
		return pipes.back();
	}
};

template<std::integral ch_type,std::contiguous_iterator Iter>
inline Iter read(basic_win32_pipe<ch_type>& h,Iter begin,Iter end)
{
	return read(h.in(),begin,end);
}
template<std::integral ch_type,std::contiguous_iterator Iter>
inline Iter write(basic_win32_pipe<ch_type>& h,Iter begin,Iter end)
{
	return write(h.out(),begin,end);
}


template<std::integral ch_type>
inline std::array<void*,2> redirect_handle(basic_win32_pipe<ch_type>& hd)
{
	return {hd.in().native_handle(),hd.out().native_handle()};
}

template<std::integral ch_type>
inline constexpr void flush(basic_win32_pipe<ch_type>&){}
using win32_io_observer=basic_win32_io_observer<char>;
using win32_io_handle=basic_win32_io_handle<char>;
using win32_file=basic_win32_file<char>;
using win32_pipe=basic_win32_pipe<char>;

using u8win32_io_observer=basic_win32_io_observer<char8_t>;
using u8win32_io_handle=basic_win32_io_handle<char8_t>;
using u8win32_file=basic_win32_file<char8_t>;
using u8win32_pipe=basic_win32_pipe<char8_t>;

using wwin32_io_observer=basic_win32_io_observer<wchar_t>;
using wwin32_io_handle=basic_win32_io_handle<wchar_t>;
using wwin32_file=basic_win32_file<wchar_t>;
using wwin32_pipe=basic_win32_pipe<wchar_t>;

using io_async_observer=win32_io_observer;
using io_async_scheduler=win32_file;

inline constexpr std::uint32_t win32_stdin_number(-10);
inline constexpr std::uint32_t win32_stdout_number(-11);
inline constexpr std::uint32_t win32_stderr_number(-12);

template<std::integral char_type=char>
inline basic_win32_io_observer<char_type> win32_stdin()
{
	return {fast_io::win32::GetStdHandle(-10)};
}
template<std::integral char_type=char>
inline basic_win32_io_observer<char_type> win32_stdout()
{
	return basic_win32_io_observer<char_type>{fast_io::win32::GetStdHandle(-11)};
}
template<std::integral char_type=char>
inline basic_win32_io_observer<char_type> win32_stderr()
{
	return basic_win32_io_observer<char_type>{fast_io::win32::GetStdHandle(-12)};
}
template<std::integral char_type=char>
inline basic_win32_io_observer<char_type> native_stdin()
{
	return {fast_io::win32::GetStdHandle(-10)};
}
template<std::integral char_type=char>
inline basic_win32_io_observer<char_type> native_stdout()
{
	return basic_win32_io_observer<char_type>{fast_io::win32::GetStdHandle(-11)};
}
template<std::integral char_type=char>
inline basic_win32_io_observer<char_type> native_stderr()
{
	return basic_win32_io_observer<char_type>{fast_io::win32::GetStdHandle(-12)};
}

template<std::integral char_type>
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<basic_win32_io_observer<char_type>>)
{
	return print_reserve_size(io_reserve_type<void*>);
}

template<std::integral char_type,std::contiguous_iterator caiter,typename U>
inline constexpr caiter print_reserve_define(io_reserve_type_t<basic_win32_io_observer<char_type>>,caiter iter,U&& v)
{
	return print_reserve_define(io_reserve_type<void*>,iter,v.handle);
}
}
