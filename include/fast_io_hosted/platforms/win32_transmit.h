#pragma once

namespace fast_io::details
{


class unique_win32_handle
{
	void* handle{};
public:
	constexpr unique_win32_handle()=default;
	unique_win32_handle(void* val):handle(val){}
	unique_win32_handle(unique_win32_handle const&)=delete;
	unique_win32_handle& operator=(unique_win32_handle const&)=delete;
	auto get()
	{
		return handle;
	}
	constexpr operator bool() const
	{
		return handle;
	}
	constexpr unique_win32_handle(unique_win32_handle&& hd) noexcept:handle(hd.handle)
	{
		hd.handle=nullptr;
	}
	constexpr unique_win32_handle& operator=(unique_win32_handle&& hd) noexcept
	{
		if(handle!=hd.handle)[[likely]]
		{
			win32::CloseHandle(handle);
			handle=hd.handle;
			hd.handle=nullptr;
		}
		return *this;
	}
	~unique_win32_handle()
	{
		win32::CloseHandle(handle);
	}
};

class unique_win32_map_view
{
	void* handle{};
public:
	constexpr unique_win32_map_view()=default;
	unique_win32_map_view(void* val):handle(val){}
	unique_win32_map_view(unique_win32_map_view const&)=delete;
	unique_win32_map_view& operator=(unique_win32_map_view const&)=delete;
	auto get()
	{
		return handle;
	}
	constexpr operator bool() const
	{
		return handle;
	}
	constexpr unique_win32_map_view(unique_win32_map_view&& hd) noexcept:handle(hd.handle)
	{
		hd.handle=nullptr;
	}
	constexpr unique_win32_map_view& operator=(unique_win32_map_view&& hd) noexcept
	{
		if(handle!=hd.handle)[[likely]]
		{
			win32::UnmapViewOfFile(handle);
			handle=hd.handle;
			hd.handle=nullptr;
		}
		return *this;
	}
	~unique_win32_map_view()
	{
		win32::UnmapViewOfFile(handle);
	}
};


inline constexpr std::uint64_t parse_transmit_args(std::uint64_t filesize)
{
	return filesize;
}

inline constexpr std::uint64_t parse_transmit_args(std::uint64_t,std::uint64_t bytes)
{
	return bytes;
}


template<output_stream output,memory_map_input_stream input,typename... Args>
inline std::pair<bool,std::common_type_t<std::size_t,std::uint64_t>> memory_map_transmit_impl(output& outp,input& inp,Args&& ...args)
{
	void* inp_handle{};
	if constexpr(std::same_as<decltype(memory_map_in_handle(inp)),int>)
	{
		auto fd{memory_map_in_handle(inp)};
		int old_mode{_setmode(fd,_O_BINARY)};
		if(old_mode==-1)
			return {};
		if(old_mode!=_O_BINARY)
		{
			_setmode(fd,old_mode);
			return {};
		}
		auto os_handle{_get_os_handle(fd)};
		if(os_handle==-1)
			return {};
		inp_handle=bit_cast<void*>(os_handle);
	}
	else
		inp_handle=memory_map_in_handle(inp);
	std::int64_t file_position{};
	if(!fast_io::win32::SetFilePointerEx(inp_handle,0,std::addressof(file_position),1))
		return {};
	std::uint64_t filesize{};
	if(!fast_io::win32::GetFileSizeEx(inp_handle,std::addressof(filesize)))
		return {};
	std::uint64_t difference(filesize-file_position);
	if(difference<65536)
		return {};
	auto map_handle_temp{fast_io::win32::CreateFileMappingW(inp_handle,
		nullptr,
		0x02,	//PAGE_WRITECOPY to get a copy on write page
		filesize>>32,
		static_cast<std::uint32_t>(filesize),
		nullptr)};
	if(!map_handle_temp)
		return {};
	unique_win32_handle map_handle{map_handle_temp};
	std::size_t to_mapped_size{parse_transmit_args(filesize,std::forward<Args>(args)...)};
	auto map_view_handle_temp{fast_io::win32::MapViewOfFile(map_handle_temp,0x00000004,file_position>>32,
			static_cast<std::uint32_t>(file_position),to_mapped_size)};
	if(!map_view_handle_temp)
		return {};
	unique_win32_map_view map_view_handle(map_view_handle_temp);
/*	win32::win32_memory_range_entry entry{map_view_handle_temp,to_mapped_size};
	if(!fast_io::win32::PrefetchVirtualMemory(win32::GetCurrentProcess(),1,std::addressof(entry),0))
		return {};*/
	write(outp,reinterpret_cast<std::byte const*>(map_view_handle_temp),//std::byte can alias. Not UB
		reinterpret_cast<std::byte const*>(map_view_handle_temp)+to_mapped_size);
	return {true,to_mapped_size};
}
}