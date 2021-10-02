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


}