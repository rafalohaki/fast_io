#pragma once

namespace fast_io
{

namespace details
{
template<typename... Args>
requires (sizeof...(Args)==4)
inline auto create_io_completion_port(Args&&... args)
{
	auto ptr{fast_io::win32::CreateIoCompletionPort(std::forward<Args>(args)...)};
	if(ptr==nullptr)[[unlikely]]
#ifdef __cpp_exceptions
		throw win32_error();
#else
		fast_terminate();
#endif
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
#ifdef __cpp_exceptions
			throw win32_error();
#else
			fast_terminate();
#endif
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
#ifdef __cpp_exceptions
			throw win32_error();
#else
			fast_terminate();
#endif
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
		mode.dwDesiredAccess|=0x80000000;//GENERIC_READ

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
	if((value&open_mode::excl)!=open_mode::none)
	{
		mode.dwCreationDisposition=1;//	CREATE_NEW
		if((value&open_mode::trunc)!=open_mode::none)
#ifdef __cpp_exceptions
			throw posix_error(EINVAL);
#else
			fast_terminate();
#endif
	}
	else if ((value&open_mode::trunc)!=open_mode::none)
	{
		if((value&open_mode::creat)!=open_mode::none)
			mode.dwCreationDisposition=2;// CREATE_ALWAYS
		else if((value&open_mode::in)!=open_mode::none)
			mode.dwCreationDisposition=5;//TRUNCATE_EXISTING
		else
		{
#ifdef __cpp_exceptions
			throw posix_error(EINVAL);
#else
			fast_terminate();
#endif
		}
	}
	else if((value&open_mode::in)==open_mode::none)
	{
		if((value&open_mode::app)!=open_mode::none)
			mode.dwCreationDisposition=4;//OPEN_ALWAYS
		else
			mode.dwCreationDisposition=2;//CREATE_ALWAYS
	}
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
	native_handle_type handle=nullptr;
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
	explicit constexpr operator basic_nt_io_observer<char_type>() const noexcept
	{
		return basic_nt_io_observer<char_type>{handle};
	}
};

template<std::integral ch_type>
class basic_win32_io_handle:public basic_win32_io_observer<ch_type>
{
public:
	using native_handle_type = void*;
	using char_type = ch_type;
	using async_scheduler_type = basic_win32_io_observer<char>;
protected:
	void close_impl() noexcept
	{
		if(this->native_handle())
			fast_io::win32::CloseHandle(this->native_handle());
	}
public:
	explicit constexpr basic_win32_io_handle() noexcept =default;
	explicit constexpr basic_win32_io_handle(native_handle_type handle) noexcept:
		basic_win32_io_observer<ch_type>{handle}{}
	explicit basic_win32_io_handle(std::uint32_t dw):
		basic_win32_io_observer<ch_type>{fast_io::win32::GetStdHandle(dw)}{}
	basic_win32_io_handle(basic_win32_io_handle const& other)
	{
		auto const current_process(win32::GetCurrentProcess());
		if(!win32::DuplicateHandle(current_process,other.native_handle(),current_process,std::addressof(this->native_handle()), 0, true, 2/*DUPLICATE_SAME_ACCESS*/))
#ifdef __cpp_exceptions
			throw win32_error();
#else
			fast_terminate();
#endif
	}
	basic_win32_io_handle& operator=(basic_win32_io_handle const& other)
	{
		auto const current_process(win32::GetCurrentProcess());
		void* new_handle{};
		if(!win32::DuplicateHandle(current_process,other.native_handle(),current_process,std::addressof(new_handle), 0, true, 2/*DUPLICATE_SAME_ACCESS*/))
#ifdef __cpp_exceptions
			throw win32_error();
#else
			fast_terminate();
#endif
		close_impl();
		this->native_handle()=new_handle;
		return *this;
	}
	constexpr basic_win32_io_handle(basic_win32_io_handle&& b) noexcept:
		basic_win32_io_observer<ch_type>(b.native_handle())
	{
		b.native_handle()=nullptr;
	}
	basic_win32_io_handle& operator=(basic_win32_io_handle&& b) noexcept
	{
		if(std::addressof(b)!=this)
		{
			close_impl();
			this->native_handle() = b.native_handle();
			b.native_handle()=nullptr;
		}
		return *this;
	}
	constexpr void detach() noexcept
	{
		this->native_handle()=nullptr;
	}
};

template<std::integral ch_type>
inline constexpr auto redirect_handle(basic_win32_io_observer<ch_type> hd)
{
	return hd.native_handle();
}

template<std::integral ch_type,typename T,std::integral U>
inline std::common_type_t<std::int64_t, std::size_t> seek(basic_win32_io_observer<ch_type> handle,seek_type_t<T>,U i=0,seekdir s=seekdir::cur)
{
	std::int64_t distance_to_move_high{};
	std::int64_t seekposition{seek_precondition<std::int64_t,T,ch_type>(i)};
	if(!win32::SetFilePointerEx(handle.native_handle(),seekposition,std::addressof(distance_to_move_high),static_cast<std::uint32_t>(s)))
#ifdef __cpp_exceptions
		throw win32_error();
#else
		fast_terminate();
#endif
	return distance_to_move_high;
}

template<std::integral ch_type,std::integral U>
inline auto seek(basic_win32_io_observer<ch_type> handle,U i=0,seekdir s=seekdir::cur)
{
	return seek(handle,seek_type<ch_type>,i,s);
}

template<std::integral ch_type,std::contiguous_iterator Iter>
inline Iter read(basic_win32_io_observer<ch_type> handle,Iter begin,Iter end)
{
	std::uint32_t numberOfBytesRead{};
	std::size_t to_read((end-begin)*sizeof(*begin));
	if constexpr(4<sizeof(std::size_t))
		if(static_cast<std::size_t>(UINT32_MAX)<to_read)
			to_read=static_cast<std::size_t>(UINT32_MAX);
	if(!win32::ReadFile(handle.native_handle(),std::to_address(begin),static_cast<std::uint32_t>(to_read),std::addressof(numberOfBytesRead),nullptr))
	{
		auto err(win32::GetLastError());
		if(err==109)
			return begin;
#ifdef __cpp_exceptions
		throw win32_error(err);
#else
		fast_terminate();
#endif
	}
	return begin+(numberOfBytesRead/sizeof(*begin));
}

template<std::integral ch_type,std::contiguous_iterator Iter>
inline Iter write(basic_win32_io_observer<ch_type> handle,Iter cbegin,Iter cend)
{
	std::size_t to_write((cend-cbegin)*sizeof(*cbegin));
	if constexpr(4<sizeof(std::size_t))
		if(static_cast<std::size_t>(UINT32_MAX)<to_write)
			to_write=static_cast<std::size_t>(UINT32_MAX);
	std::uint32_t numberOfBytesWritten;
	if(!win32::WriteFile(handle.native_handle(),std::to_address(cbegin),static_cast<std::uint32_t>(to_write),std::addressof(numberOfBytesWritten),nullptr))
#ifdef __cpp_exceptions
		throw win32_error();
#else
		fast_terminate();
#endif
	return cbegin+numberOfBytesWritten/sizeof(*cbegin);
}

template<std::integral ch_type,std::contiguous_iterator Iter>
inline void async_read_callback(basic_win32_io_observer<char>,basic_win32_io_observer<ch_type> h,Iter cbegin,Iter cend,
	iocp_overlapped_observer callback,std::ptrdiff_t offset=0)
{
	std::size_t to_read((cend-cbegin)*sizeof(*cbegin));
	if constexpr(4<sizeof(std::size_t))
		if(static_cast<std::size_t>(UINT32_MAX)<to_read)
			to_read=static_cast<std::size_t>(UINT32_MAX);
	callback.native_handle()->Offset=static_cast<std::size_t>(offset)&std::numeric_limits<std::uint32_t>::max();
	callback.native_handle()->OffsetHigh=static_cast<std::size_t>(offset)>>32;
	if(!win32::ReadFile(h.native_handle(),std::to_address(cbegin),static_cast<std::uint32_t>(to_read),nullptr,callback.native_handle()))[[likely]]
	{
		auto err(win32::GetLastError());
		if(err==997)[[likely]]
			return;
#ifdef __cpp_exceptions
		throw win32_error(err);
#else
		fast_terminate();
#endif
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
	callback.native_handle()->Offset=static_cast<std::size_t>(offset)&std::numeric_limits<std::uint32_t>::max();
	callback.native_handle()->OffsetHigh=static_cast<std::size_t>(offset)>>32;
	if(!win32::WriteFile(h.native_handle(),std::to_address(cbegin),static_cast<std::uint32_t>(to_write),nullptr,callback.native_handle()))[[likely]]
	{
		auto err(win32::GetLastError());
		if(err==997)[[likely]]
			return;
#ifdef __cpp_exceptions
		throw win32_error(err);
#else
		fast_terminate();
#endif
	}
}

template<std::integral ch_type>
inline void cancel(basic_win32_io_observer<ch_type> h)
{
	if(!fast_io::win32::CancelIo(h.native_handle()))
		details::throw_win32_error();
}

template<std::integral ch_type,typename... Args>
requires requires(basic_win32_io_observer<ch_type> h,Args&& ...args)
{
	fast_io::win32::DeviceIoControl(h.native_handle(),std::forward<Args>(args)...);
}
inline void io_control(basic_win32_io_observer<ch_type> h,Args&& ...args)
{
	if(!fast_io::win32::DeviceIoControl(h.native_handle(),std::forward<Args>(args)...))
		details::throw_win32_error();
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
		local.detach();
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
#ifdef __cpp_exceptions
			throw win32_error();
#else
			fast_terminate();
#endif
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
	template<typename... Args>
	basic_win32_file(io_async_t,basic_win32_io_observer<char> iob,Args&& ...args):basic_win32_file(std::forward<Args>(args)...)
	{
		basic_win32_file<ch_type> guard(this->native_handle());
		details::create_io_completion_port(this->native_handle(),iob.native_handle(),bit_cast<std::uintptr_t>(this->native_handle()),0);
		guard.release();
	}
	~basic_win32_file()
	{
		this->close_impl();
	}
	void close()
	{
		if(this->native_handle())[[likely]]
		{
			if(win32::CloseHandle(this->native_handle()))
#ifdef __cpp_exceptions
				throw win32_error();
#else
				fast_terminate();
#endif
			this->native_handle()=nullptr;
		}
	}
	constexpr native_handle_type release() noexcept
	{
		auto temp{this->native_handle()};
		this->detach();
		return temp;
	}
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
	if(!win32::SetEndOfFile(handle.native_handle()))
#ifdef __cpp_exceptions
		throw win32_error();
#else
		fast_terminate();
#endif
}

template<std::integral ch_type>
class basic_win32_pipe
{
public:
	using char_type = ch_type;
	using native_handle_type = std::array<basic_win32_file<ch_type>,2>;
private:
	native_handle_type pipes;
public:
	template<typename ...Args>
/*	requires requires(Args&& ...args)
	{
		{win32::CreatePipe(static_cast<void**>(static_cast<void*>(pipes.data())),static_cast<void**>(static_cast<void*>(pipes.data()+1)),std::forward<Args>(args)...)}->std::same_as<int>;
	}*/
	basic_win32_pipe(fast_io::native_interface_t, Args&& ...args)
	{
		if(!win32::CreatePipe(
			std::addressof(pipes.front().native_handle()),
			std::addressof(pipes.back().native_handle()),
			std::forward<Args>(args)...))
#ifdef __cpp_exceptions
			throw win32_error();
#else
			fast_terminate();
#endif
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
#ifdef __cpp_exceptions
			throw win32_error();
#else
			fast_terminate();
#endif
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
	void swap(basic_win32_pipe& o) noexcept
	{
		using std::swap;
		swap(pipes,o.pipes);
	}
};

template<std::integral ch_type,std::contiguous_iterator Iter>
inline Iter read(basic_win32_pipe<ch_type>& h,Iter begin,Iter end)
{
	return read(h.in(),begin,end);
}
template<std::integral ch_type,std::contiguous_iterator Iter>
inline auto write(basic_win32_pipe<ch_type>& h,Iter begin,Iter end)
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
