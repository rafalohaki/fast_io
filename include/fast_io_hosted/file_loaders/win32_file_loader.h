#pragma once

namespace fast_io
{

namespace win32::details
{

struct win32_file_loader_return_value_t
{
	char* address_start;
	char* address_end;
};

inline std::size_t win32_load_file_get_file_size(void* handle)
{
	by_handle_file_information bhdi;
	if(!GetFileInformationByHandle(handle,std::addressof(bhdi)))
		throw_win32_error();
	if constexpr(sizeof(std::size_t)<sizeof(std::uint64_t))
	{
		if(bhdi.nFileSizeHigh)
			throw_win32_error(0x000000DF);
		if constexpr(sizeof(std::size_t)<sizeof(std::uint32_t))
		{
			if(bhdi.nFileSizeLow>static_cast<std::uint32_t>(SIZE_MAX))
				throw_win32_error(0x000000DF);
		}
		return static_cast<std::size_t>(bhdi.nFileSizeLow);
	}
	else
	{
		return static_cast<std::size_t>((static_cast<std::uint64_t>(bhdi.nFileSizeHigh)<<32)|bhdi.nFileSizeLow);
	}
}

inline win32_file_loader_return_value_t win32_load_address_impl(void* handle)
{
	std::size_t file_size{win32_load_file_get_file_size(handle)};
	if(file_size==0)
		return {nullptr,nullptr};
	void* hfilemappingobj{CreateFileMappingW(handle,nullptr,0x08,0,0,nullptr)};
	if(hfilemappingobj==nullptr)
		throw_win32_error();
	win32_file map_hd{hfilemappingobj};
	auto base_ptr{MapViewOfFile(hfilemappingobj,1,0,0,file_size)};
	if(base_ptr==nullptr)
		throw_win32_error();
	return {reinterpret_cast<char*>(base_ptr),reinterpret_cast<char*>(base_ptr)+file_size};
}

inline auto win32_load_file_impl(nt_fs_dirent fsdirent,perms pm)
{
	win32_file wf(fsdirent,fast_io::open_mode::in,pm);
	return win32_load_address_impl(wf.handle);
}

template<std::integral char_type>
inline auto win32_load_file_impl(basic_cstring_view<char_type> str,perms pm)
{
	win32_file wf(str,fast_io::open_mode::in,pm);
	return win32_load_address_impl(wf.handle);
}

template<std::integral char_type>
inline auto win32_load_file_impl(nt_at_entry ent,basic_cstring_view<char_type> str,perms pm)
{
	win32_file wf(ent,str,fast_io::open_mode::in,pm);
	return win32_load_address_impl(wf.handle);
}

inline void win32_unload_address(void const* address) noexcept
{
	if(address)
		UnmapViewOfFile(address);
}

}

class win32_file_loader
{
public:
	using value_type = char;
	using pointer = char*;
	using const_pointer = char const*;
	using const_iterator = const_pointer;
	using iterator = pointer;
	using reference = char&;
	using const_reference = char const&;
	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;
	using reverse_iterator = std::reverse_iterator<iterator>;

	pointer address_start{};
	pointer address_end{};
	inline constexpr win32_file_loader() noexcept=default;
	inline explicit win32_file_loader(win32_io_observer wiob)
	{
		auto ret{win32::details::win32_load_address_impl(wiob.handle)};
		address_start=ret.address_start;
		address_end=ret.address_end;
	}
	inline explicit win32_file_loader(nt_fs_dirent fsdirent,perms pm=static_cast<perms>(436))
	{
		auto ret{win32::details::win32_load_file_impl(fsdirent,pm)};
		address_start=ret.address_start;
		address_end=ret.address_end;
	}
	inline explicit win32_file_loader(cstring_view filename,perms pm=static_cast<perms>(436))
	{
		auto ret{win32::details::win32_load_file_impl(filename,pm)};
		address_start=ret.address_start;
		address_end=ret.address_end;
	}
	inline explicit win32_file_loader(wcstring_view filename,perms pm=static_cast<perms>(436))
	{
		auto ret{win32::details::win32_load_file_impl(filename,pm)};
		address_start=ret.address_start;
		address_end=ret.address_end;
	}
	inline explicit win32_file_loader(u8cstring_view filename,perms pm=static_cast<perms>(436))
	{
		auto ret{win32::details::win32_load_file_impl(filename,pm)};
		address_start=ret.address_start;
		address_end=ret.address_end;
	}
	inline explicit win32_file_loader(u16cstring_view filename,perms pm=static_cast<perms>(436))
	{
		auto ret{win32::details::win32_load_file_impl(filename,pm)};
		address_start=ret.address_start;
		address_end=ret.address_end;
	}
	inline explicit win32_file_loader(u32cstring_view filename,perms pm=static_cast<perms>(436))
	{
		auto ret{win32::details::win32_load_file_impl(filename,pm)};
		address_start=ret.address_start;
		address_end=ret.address_end;
	}
	inline explicit win32_file_loader(nt_at_entry ent,cstring_view filename,perms pm=static_cast<perms>(436))
	{
		auto ret{win32::details::win32_load_file_impl(ent,filename,pm)};
		address_start=ret.address_start;
		address_end=ret.address_end;
	}
	inline explicit win32_file_loader(nt_at_entry ent,wcstring_view filename,perms pm=static_cast<perms>(436))
	{
		auto ret{win32::details::win32_load_file_impl(ent,filename,pm)};
		address_start=ret.address_start;
		address_end=ret.address_end;
	}
	inline explicit win32_file_loader(nt_at_entry ent,u8cstring_view filename,perms pm=static_cast<perms>(436))
	{
		auto ret{win32::details::win32_load_file_impl(ent,filename,pm)};
		address_start=ret.address_start;
		address_end=ret.address_end;
	}
	inline explicit win32_file_loader(nt_at_entry ent,u16cstring_view filename,perms pm=static_cast<perms>(436))
	{
		auto ret{win32::details::win32_load_file_impl(ent,filename,pm)};
		address_start=ret.address_start;
		address_end=ret.address_end;
	}
	inline explicit win32_file_loader(nt_at_entry ent,u32cstring_view filename,perms pm=static_cast<perms>(436))
	{
		auto ret{win32::details::win32_load_file_impl(ent,filename,pm)};
		address_start=ret.address_start;
		address_end=ret.address_end;
	}
	win32_file_loader(win32_file_loader const&)=delete;
	win32_file_loader& operator=(win32_file_loader const&)=delete;
	constexpr win32_file_loader(win32_file_loader&& other) noexcept:address_start(other.address_start),address_end(other.address_end)
	{
		other.address_end=other.address_start=nullptr;
	}
	win32_file_loader& operator=(win32_file_loader && other) noexcept
	{
		if(std::addressof(other)==this)
			return *this;
		win32::details::win32_unload_address(address_start);
		address_start=other.address_start;
		address_end=other.address_end;
		other.address_end=other.address_start=nullptr;
		return *this;
	}
	constexpr char const* data() const noexcept
	{
		return address_start;
	}
	constexpr bool empty() const noexcept
	{
		return address_start==address_end;
	}
	constexpr std::size_t size() const noexcept
	{
		return static_cast<std::size_t>(address_end-address_start);
	}
	constexpr std::size_t max_size() const noexcept
	{
		return SIZE_MAX;
	}
	constexpr const_iterator cbegin() const noexcept
	{
		return address_start;
	}
	constexpr const_iterator begin() const noexcept
	{
		return address_start;
	}
	constexpr iterator begin() noexcept
	{
		return address_start;
	}
	constexpr const_iterator cend() const noexcept
	{
		return address_end;
	}
	constexpr const_iterator end() const noexcept
	{
		return address_end;
	}
	constexpr iterator end() noexcept
	{
		return address_end;
	}
	constexpr const_reverse_iterator crbegin() const noexcept
	{
		return const_reverse_iterator{address_end};
	}
	constexpr reverse_iterator rbegin() noexcept
	{
		return reverse_iterator{address_end};
	}
	constexpr const_reverse_iterator rbegin() const noexcept
	{
		return const_reverse_iterator{address_end};
	}
	constexpr const_reverse_iterator crend() const noexcept
	{
		return const_reverse_iterator{address_start};
	}
	constexpr reverse_iterator rend() noexcept
	{
		return reverse_iterator{address_start};
	}
	constexpr const_reverse_iterator rend() const noexcept
	{
		return const_reverse_iterator{address_start};
	}
	constexpr const_reference front() const noexcept
	{
		return *address_start;
	}
	constexpr reference front() noexcept
	{
		return *address_start;
	}
	constexpr const_reference back() const noexcept
	{
		return address_end[-1];
	}
	constexpr reference back() noexcept
	{
		return address_end[-1];
	}
	inline constexpr reference operator[](size_type size) noexcept
	{
		return address_start[size];
	}
	inline constexpr const_reference operator[](size_type size) const noexcept
	{
		return address_start[size];
	}
	explicit constexpr operator ::fast_io::freestanding::string_view() noexcept
	{
		return ::fast_io::freestanding::string_view(address_start,address_end);
	}
	~win32_file_loader()
	{
		win32::details::win32_unload_address(address_start);
	}
};


inline constexpr basic_io_scatter_t<char> print_alias_define(io_alias_t,win32_file_loader const& load) noexcept
{
	return {load.data(),load.size()};
}

}