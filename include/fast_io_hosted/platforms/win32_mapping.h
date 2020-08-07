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

}