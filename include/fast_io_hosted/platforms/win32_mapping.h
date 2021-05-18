#pragma once

namespace fast_io
{


enum class win32_file_map_attribute
{
copy=0x00000001,write=0x00000002,read=0x00000004,all_access=0x000f001f,execute=0x00000020
};

constexpr win32_file_map_attribute operator&(win32_file_map_attribute x, win32_file_map_attribute y) noexcept
{
using utype = typename std::underlying_type<win32_file_map_attribute>::type;
return static_cast<win32_file_map_attribute>(static_cast<utype>(x) & static_cast<utype>(y));
}

constexpr win32_file_map_attribute operator|(win32_file_map_attribute x, win32_file_map_attribute y) noexcept
{
using utype = typename std::underlying_type<win32_file_map_attribute>::type;
return static_cast<win32_file_map_attribute>(static_cast<utype>(x) | static_cast<utype>(y));
}

constexpr win32_file_map_attribute operator^(win32_file_map_attribute x, win32_file_map_attribute y) noexcept
{
using utype = typename std::underlying_type<win32_file_map_attribute>::type;
return static_cast<win32_file_map_attribute>(static_cast<utype>(x) ^ static_cast<utype>(y));
}

constexpr win32_file_map_attribute operator~(win32_file_map_attribute x) noexcept
{
using utype = typename std::underlying_type<win32_file_map_attribute>::type;
return static_cast<win32_file_map_attribute>(~static_cast<utype>(x));
}

inline win32_file_map_attribute& operator&=(win32_file_map_attribute& x, win32_file_map_attribute y) noexcept{return x=x&y;}

inline win32_file_map_attribute& operator|=(win32_file_map_attribute& x, win32_file_map_attribute y) noexcept{return x=x|y;}

inline win32_file_map_attribute& operator^=(win32_file_map_attribute& x, win32_file_map_attribute y) noexcept{return x=x^y;}

inline constexpr win32_file_map_attribute to_win32_file_map_attribute(file_map_attribute x)
{
	switch(x)
	{
	case file_map_attribute::execute_read:return win32_file_map_attribute::execute|win32_file_map_attribute::read;
	case file_map_attribute::execute_read_write:return win32_file_map_attribute::execute|win32_file_map_attribute::read;
	case file_map_attribute::execute_write_copy:return win32_file_map_attribute::execute|win32_file_map_attribute::write|win32_file_map_attribute::copy;
	case file_map_attribute::read_only:return win32_file_map_attribute::read;
	case file_map_attribute::read_write:return win32_file_map_attribute::read|win32_file_map_attribute::write;
	case file_map_attribute::write_copy:return win32_file_map_attribute::write|win32_file_map_attribute::copy;
	default:
		throw_win32_error(0x000000A0);
	};
}

class win32_memory_map_io_observer
{
public:
	using native_handle_type = void*;
	void* handle{};
	std::byte *address_begin{},*address_end{};
	constexpr auto& native_handle() const noexcept
	{
		return handle;
	}
	constexpr auto& native_handle() noexcept
	{
		return handle;
	}
	constexpr std::size_t bytes() const noexcept
	{
		return address_end-address_begin;
	}
};

namespace win32::details
{

inline void* create_file_mapping_impl(void* handle,file_map_attribute attr)
{
	void* addr{win32::CreateFileMappingW(handle,nullptr,static_cast<std::uint32_t>(attr),0,0,nullptr)};
	if(addr==nullptr)
		throw_win32_error();
	return addr;
}

}


class win32_memory_map_file:public win32_memory_map_io_observer
{
public:
	using native_handle_type = void*;
	constexpr win32_memory_map_file()=default;
	win32_memory_map_file(void* handle,std::byte* address_begin,std::byte* address_end):win32_memory_map_io_observer{handle,address_begin,address_end}{}
	template<std::integral char_type>
	win32_memory_map_file(basic_win32_io_observer<char_type> bf,file_map_attribute attr,std::size_t bytes,std::uintmax_t start_address=0):
		win32_memory_map_io_observer{win32::details::create_file_mapping_impl(bf.native_handle(),attr)}
	{
		void *base_ptr{win32::MapViewOfFile(this->native_handle(),static_cast<std::uint32_t>(to_win32_file_map_attribute(attr)),start_address>>32,static_cast<std::uint32_t>(start_address),bytes)};
		if(base_ptr==nullptr)
		{
			if(this->native_handle())[[likely]]
				win32::CloseHandle(this->native_handle());
			throw_win32_error();
		}
		this->address_begin=reinterpret_cast<std::byte*>(base_ptr);
		this->address_end=this->address_begin+bytes;
	}
	win32_memory_map_file(win32_memory_map_file const&)=delete;
	win32_memory_map_file& operator=(win32_memory_map_file const&)=delete;
	constexpr win32_memory_map_file(win32_memory_map_file&& other) noexcept:win32_memory_map_io_observer{other.handle,other.address_begin,other.address_end}
	{
		other.handle=nullptr;
		other.address_end=other.address_begin=nullptr;
	}
	win32_memory_map_file& operator=(win32_memory_map_file&& other) noexcept
	{
		if(__builtin_addressof(other)==this)
			return *this;
		if(this->address_begin)[[likely]]
			win32::UnmapViewOfFile(this->address_begin);
		if(this->handle)[[likely]]
			win32::CloseHandle(this->handle);
		this->handle=other.handle;
		this->address_begin=other.address_begin;
		this->address_end=other.address_end;
		other.handle=nullptr;
		other.address_end=other.address_begin=nullptr;
		return *this;
	}
	void close() noexcept
	{
		if(this->address_begin)[[likely]]
		{
			win32::UnmapViewOfFile(this->address_begin);
			this->address_begin=nullptr;
		}
		if(this->handle)[[likely]]
		{
			win32::CloseHandle(this->handle);
			this->handle=nullptr;
		}
	}
	~win32_memory_map_file()
	{
		if(this->address_begin)[[likely]]
			win32::UnmapViewOfFile(this->address_begin);
		if(this->handle)[[likely]]
			win32::CloseHandle(this->handle);
	}
};

#ifndef __CYGWIN__
using native_memory_map_io_observer = win32_memory_map_io_observer;
using native_memory_map_file = win32_memory_map_file;
#endif

}
