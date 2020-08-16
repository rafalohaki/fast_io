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
#ifdef __cpp_exceptions
		throw fast_io_text_error("unknown file_mapping_attribute");
#else
		fast_terminate();
#endif
	};
}

class win32_file_mapping
{
	void* handle;
	void close_impl() noexcept
	{
		if(handle)
			win32::CloseHandle(handle);
	}
public:
	using native_handle_type = void*;
	template<std::integral ch_type>
	win32_file_mapping(basic_win32_io_observer<ch_type> bf,file_map_attribute attr,std::size_t size):
	handle(win32::CreateFileMappingW(
	bf.native_handle(),nullptr,static_cast<std::uint32_t>(attr),
#if (_WIN64 || __x86_64__ || __ppc64__)
			size>>32
#else
			0
#endif
	,static_cast<std::uint32_t>(size),nullptr))
	{
		if(handle==nullptr)
			throw_win32_error();
	}
	win32_file_mapping(win32_file_mapping const&)=delete;
	win32_file_mapping& operator=(win32_file_mapping const&)=delete;
	win32_file_mapping(win32_file_mapping&& bmv) noexcept:handle(bmv.handle)
	{
		bmv.handle=nullptr;
	}
	win32_file_mapping& operator=(win32_file_mapping&& bmv) noexcept
	{
		if(handle!=bmv.handle)
		{
			close_impl();
			handle=bmv.handle;
			bmv.handle=nullptr;
		}
		return *this;
	}
	auto native_handle() const
	{
		return handle;
	}
	void close() noexcept
	{
		close_impl();
		handle=nullptr;
	}
	~win32_file_mapping()
	{
		close_impl();
	}
};

class win32_map_view_of_file
{
	std::span<std::byte> rg;
public:
	win32_map_view_of_file(win32_file_mapping& wm,win32_file_map_attribute attr,std::size_t bytes,std::size_t start_address=0):
			rg({reinterpret_cast<std::byte*>(win32::MapViewOfFile(wm.native_handle(),static_cast<std::uint32_t>(attr),
#if (_WIN64 || __x86_64__ || __ppc64__)
			start_address>>32
#else
			0
#endif
			,static_cast<std::uint32_t>(start_address),bytes)),bytes})
	{
		if(rg.data()==nullptr)
			throw_win32_error();
	}
	win32_map_view_of_file(win32_map_view_of_file const&)=delete;
	win32_map_view_of_file& operator=(win32_map_view_of_file const&)=delete;
	win32_map_view_of_file(win32_map_view_of_file&& wm) noexcept:rg(wm.rg)
	{
		wm.rg={};
	}
	void close() noexcept
	{
		if(rg.data())
			win32::UnmapViewOfFile(rg.data());
		rg={};
	}
	win32_map_view_of_file& operator=(win32_map_view_of_file&& wm) noexcept
	{
		if(std::addressof(wm)!=this)
		{
			if(rg.data())
				win32::UnmapViewOfFile(rg.data());
			rg=wm.rg;
			wm.rg={};
		}
		return *this;
	}
	auto& region()
	{
		return rg;
	}
	~win32_map_view_of_file()
	{
		if(rg.data())
			win32::UnmapViewOfFile(rg.data());
	}
};


class win32_file_map
{
	win32_file_mapping wfm;
	win32_map_view_of_file view;
public:
	template<std::integral ch_type>
	win32_file_map(basic_win32_io_observer<ch_type> bf,file_map_attribute attr,std::size_t bytes,std::size_t start_address=0):
		wfm(bf,attr,bytes),view(wfm,to_win32_file_map_attribute(attr),bytes,start_address)
	{
	}
	win32_file_map(win32_file_map const&)=delete;
	win32_file_map& operator=(win32_file_map const&)=delete;
	win32_file_map(win32_file_map&& wm) noexcept:wfm(std::move(wm.wfm)),view(std::move(wm.view))
	{
	}
	win32_file_map& operator=(win32_file_map&& wm) noexcept
	{
		if(std::addressof(wm)!=this)
		{
			wfm=std::move(wm.wfm);
			view=std::move(wm.view);
		}
		return *this;
	}
	auto native_handle() const {return wfm.native_handle();}
	auto& region()
	{
		return view.region();
	}
	void close()
	{
		wfm.close();
		view.close();
	}
};




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
	win32_memory_map_file(basic_win32_io_observer<char_type> bf,file_map_attribute attr,std::size_t bytes,std::size_t start_address=0):
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
		if(std::addressof(other)==this)
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

using native_memory_map_io_observer = win32_memory_map_io_observer;
using native_memory_map_file = win32_memory_map_file;

template<std::integral ch_type>
class basic_omemory_map
{
public:
	using char_type = ch_type;
	char_type *begin_ptr{},*curr_ptr{},*end_ptr{};
	constexpr basic_omemory_map() = default;
	basic_omemory_map(native_memory_map_io_observer iob,std::size_t offset=0):begin_ptr(reinterpret_cast<char_type*>(iob.address_begin+offset)),curr_ptr(this->begin_ptr),end_ptr(this->begin_ptr+iob.bytes()/sizeof(char_type)){}

	constexpr std::size_t written_bytes() const noexcept
	{
		return static_cast<std::size_t>(curr_ptr-begin_ptr)*sizeof(char_type);
	}
};

namespace details
{

template<std::integral char_type>
inline constexpr void write(basic_omemory_map<char_type>& bomp,char_type const* begin,char_type const* end) noexcept
{
	std::size_t const to_write(end-begin);
	non_overlapped_copy_n(begin,to_write,bomp.curr_ptr);
	bomp.curr_ptr+=to_write;
}
}

template<std::integral char_type,std::contiguous_iterator Iter>
constexpr void write(basic_omemory_map<char_type>& bomp,Iter begin,Iter end) noexcept
{
	details::write(bomp,std::to_address(begin),std::to_address(end));
}

template<std::integral char_type>
constexpr char_type* obuffer_begin(basic_omemory_map<char_type>& bomp) noexcept
{
	return bomp.begin_ptr;
}
template<std::integral char_type>
constexpr char_type* obuffer_curr(basic_omemory_map<char_type>& bomp) noexcept
{
	return bomp.curr_ptr;
}
template<std::integral char_type>
constexpr char_type* obuffer_end(basic_omemory_map<char_type>& bomp) noexcept
{
	return bomp.end_ptr;
}

template<std::integral char_type>
constexpr void overflow(basic_omemory_map<char_type>& bomp,char_type) noexcept
{
//ub for overflow
}

template<std::integral char_type>
constexpr void obuffer_set_curr(basic_omemory_map<char_type>& bomp,char_type* ptr) noexcept
{
	bomp.curr_ptr=ptr;
}

using omemory_map = basic_omemory_map<char>;


}