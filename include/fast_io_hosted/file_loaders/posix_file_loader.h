#pragma once

namespace fast_io
{

namespace details
{

inline std::size_t posix_loader_get_file_size(int fd)
{
#ifdef _WIN32
	struct __stat64 st;
#elif defined(__linux__)
	struct stat64 st;
#else
	struct stat st;
#endif
	if(
#ifdef _WIN32
_fstat64
#elif defined(__linux__)
fstat64
#else
fstat
#endif
	(fd,std::addressof(st))<0)
		throw_posix_error();
	using st_size_unsigned_type = std::make_unsigned_t<decltype(st.st_size)>;
	if constexpr(sizeof(st_size_unsigned_type)>sizeof(std::size_t))
	{
		if(static_cast<st_size_unsigned_type>(st.st_size)>static_cast<st_size_unsigned_type>(SIZE_MAX))
			throw_posix_error(EINVAL);
	}
	return static_cast<std::size_t>(st.st_size);
}

struct load_file_allocation_guard
{
	void* address{};
	explicit load_file_allocation_guard(std::size_t file_size):address(malloc(file_size))
	{
		if(address==nullptr)
			throw_posix_error(EINVAL);
	}
	load_file_allocation_guard(load_file_allocation_guard const&)=delete;
	load_file_allocation_guard& operator=(load_file_allocation_guard const&)=delete;
	~load_file_allocation_guard()
	{
		free(address);
	}
};

template<bool allocation>
inline char* posix_load_address(int fd,std::size_t file_size)
{
	if constexpr(allocation)
	{
		if(file_size==0)
			return nullptr;
		load_file_allocation_guard guard{file_size};
		posix_io_observer piob{fd};
		auto addr{reinterpret_cast<char*>(guard.address)};
		auto addr_ed{addr+file_size};
		for(auto i{addr};i!=addr_ed;)
		{
			auto after{read(piob,i,addr_ed)};
			if(after==i)
				throw_posix_error();
			i=after;
		}
		guard.address=nullptr;
		return addr;
	}
	else
	{
#if defined(_WIN32) || (defined(__NEWLIB__)&&!defined(__CYGWIN__)) || defined(__MSDOS__)
	static_assert(allocation);
#else
	if(file_size==0)
		return (char*)-1;
	void* address{mmap(nullptr,file_size,PROT_READ|PROT_WRITE,MAP_PRIVATE
#if defined(MAP_POPULATE)
|MAP_POPULATE
#endif
,fd,0)};
	if(address==MAP_FAILED)
		throw_posix_error();
	return reinterpret_cast<char*>(address);
#endif
	}
}

template<bool allocation>
inline void posix_unload_address(void* address,[[maybe_unused]] std::size_t file_size) noexcept
{
	if constexpr(allocation)
	{
		free(address);
	}
	else
	{
#if defined(_WIN32) || (defined(__NEWLIB__)&&!defined(__CYGWIN__)) || defined(__MSDOS__)
		static_assert(allocation);
#else
		if(address!=(void*)-1)[[likely]]
			munmap(address,file_size);
#endif
	}
}

struct posix_file_loader_return_value_t
{
	char* address_start;
	char* address_end;
};

template<bool allocation>
inline posix_file_loader_return_value_t posix_load_address_impl(int fd)
{
	std::size_t size{posix_loader_get_file_size(fd)};
	auto add{posix_load_address<allocation>(fd,size)};
	return {add,add+size};
}

template<bool allocation>
inline auto posix_load_file_impl(native_fs_dirent fsdirent,perms pm)
{
	posix_file pf(fsdirent,fast_io::open_mode::in,pm);
	return posix_load_address_impl<allocation>(pf.fd);
}

template<bool allocation,std::integral char_type>
inline auto posix_load_file_impl(basic_cstring_view<char_type> str,perms pm)
{
	posix_file pf(str,fast_io::open_mode::in,pm);
	return posix_load_address_impl<allocation>(pf.fd);
}

template<bool allocation,std::integral char_type>
inline auto posix_load_file_impl(posix_at_entry ent,basic_cstring_view<char_type> str,perms pm)
{
	posix_file pf(ent,str,fast_io::open_mode::in,pm);
	return posix_load_address_impl<allocation>(pf.fd);
}


template<bool allocation=false>
class posix_file_loader_impl
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
	inline constexpr posix_file_loader_impl() noexcept=default;
	inline explicit posix_file_loader_impl(posix_io_observer piob)
	{
		auto ret{posix_load_address_impl<allocation>(piob.fd)};
		address_start=ret.address_start;
		address_end=ret.address_end;
	}
	inline explicit posix_file_loader_impl(native_fs_dirent fsdirent,perms pm=static_cast<perms>(436))
	{
		auto ret{posix_load_file_impl<allocation>(fsdirent,pm)};
		address_start=ret.address_start;
		address_end=ret.address_end;
	}
	inline explicit posix_file_loader_impl(cstring_view filename,perms pm=static_cast<perms>(436))
	{
		auto ret{posix_load_file_impl<allocation>(filename,pm)};
		address_start=ret.address_start;
		address_end=ret.address_end;
	}
	inline explicit posix_file_loader_impl(wcstring_view filename,perms pm=static_cast<perms>(436))
	{
		auto ret{posix_load_file_impl<allocation>(filename,pm)};
		address_start=ret.address_start;
		address_end=ret.address_end;
	}
	inline explicit posix_file_loader_impl(u8cstring_view filename,perms pm=static_cast<perms>(436))
	{
		auto ret{posix_load_file_impl<allocation>(filename,pm)};
		address_start=ret.address_start;
		address_end=ret.address_end;
	}
	inline explicit posix_file_loader_impl(u16cstring_view filename,perms pm=static_cast<perms>(436))
	{
		auto ret{posix_load_file_impl<allocation>(filename,pm)};
		address_start=ret.address_start;
		address_end=ret.address_end;
	}
	inline explicit posix_file_loader_impl(u32cstring_view filename,perms pm=static_cast<perms>(436))
	{
		auto ret{posix_load_file_impl<allocation>(filename,pm)};
		address_start=ret.address_start;
		address_end=ret.address_end;
	}
	inline explicit posix_file_loader_impl(posix_at_entry ent,cstring_view filename,perms pm=static_cast<perms>(436))
	{
		auto ret{posix_load_file_impl<allocation>(ent,filename,pm)};
		address_start=ret.address_start;
		address_end=ret.address_end;
	}
	inline explicit posix_file_loader_impl(posix_at_entry ent,wcstring_view filename,perms pm=static_cast<perms>(436))
	{
		auto ret{posix_load_file_impl<allocation>(ent,filename,pm)};
		address_start=ret.address_start;
		address_end=ret.address_end;
	}
	inline explicit posix_file_loader_impl(posix_at_entry ent,u8cstring_view filename,perms pm=static_cast<perms>(436))
	{
		auto ret{posix_load_file_impl<allocation>(ent,filename,pm)};
		address_start=ret.address_start;
		address_end=ret.address_end;
	}
	inline explicit posix_file_loader_impl(posix_at_entry ent,u16cstring_view filename,perms pm=static_cast<perms>(436))
	{
		auto ret{posix_load_file_impl<allocation>(ent,filename,pm)};
		address_start=ret.address_start;
		address_end=ret.address_end;
	}
	inline explicit posix_file_loader_impl(posix_at_entry ent,u32cstring_view filename,perms pm=static_cast<perms>(436))
	{
		auto ret{posix_load_file_impl<allocation>(ent,filename,pm)};
		address_start=ret.address_start;
		address_end=ret.address_end;
	}
	posix_file_loader_impl(posix_file_loader_impl const&)=delete;
	posix_file_loader_impl& operator=(posix_file_loader_impl const&)=delete;
	constexpr posix_file_loader_impl(posix_file_loader_impl&& other) noexcept:address_start(other.address_start),address_end(other.address_end)
	{
		if constexpr(allocation)
			other.address_end=other.address_start=nullptr;
		else
			other.address_end=other.address_start=(void*)-1;
	}
	posix_file_loader_impl& operator=(posix_file_loader_impl && other) noexcept
	{
		if(std::addressof(other)==this)
			return *this;
		posix_unload_address<allocation>(address_start,static_cast<std::size_t>(address_end-address_start));
		address_start=other.address_start;
		address_end=other.address_end;
		if constexpr(allocation)
			other.address_end=other.address_start=nullptr;
		else
			other.address_end=other.address_start=(void*)-1;
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
	constexpr std::size_t max_size() const noexcept
	{
		return SIZE_MAX;
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
	~posix_file_loader_impl()
	{
		posix_unload_address<allocation>(address_start,address_end-address_start);
	}
};

template<bool h>
inline constexpr basic_io_scatter_t<char> print_alias_define(io_alias_t,posix_file_loader_impl<h> const& load) noexcept
{
	return {load.data(),load.size()};
}

}

#if !defined(_WIN32) && (!defined(__NEWLIB__)||defined(__CYGWIN__)) && !defined(__MSDOS__)
using posix_file_loader = details::posix_file_loader_impl<false>;
#endif

using allocation_file_loader = details::posix_file_loader_impl<true>;

}