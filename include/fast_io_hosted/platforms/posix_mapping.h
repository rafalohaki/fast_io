#pragma once
#include <sys/mman.h>
#include <sys/stat.h>

namespace fast_io
{


enum class posix_file_map_attribute
{
none=PROT_NONE,write=PROT_WRITE,execute=PROT_EXEC,read=PROT_READ
};

constexpr posix_file_map_attribute operator&(posix_file_map_attribute x, posix_file_map_attribute y) noexcept
{
using utype = typename std::underlying_type<posix_file_map_attribute>::type;
return static_cast<posix_file_map_attribute>(static_cast<utype>(x) & static_cast<utype>(y));
}

constexpr posix_file_map_attribute operator|(posix_file_map_attribute x, posix_file_map_attribute y) noexcept
{
using utype = typename std::underlying_type<posix_file_map_attribute>::type;
return static_cast<posix_file_map_attribute>(static_cast<utype>(x) | static_cast<utype>(y));
}

constexpr posix_file_map_attribute operator^(posix_file_map_attribute x, posix_file_map_attribute y) noexcept
{
using utype = typename std::underlying_type<posix_file_map_attribute>::type;
return static_cast<posix_file_map_attribute>(static_cast<utype>(x) ^ static_cast<utype>(y));
}

constexpr posix_file_map_attribute operator~(posix_file_map_attribute x) noexcept
{
using utype = typename std::underlying_type<posix_file_map_attribute>::type;
return static_cast<posix_file_map_attribute>(~static_cast<utype>(x));
}

inline posix_file_map_attribute& operator&=(posix_file_map_attribute& x, posix_file_map_attribute y) noexcept{return x=x&y;}

inline posix_file_map_attribute& operator|=(posix_file_map_attribute& x, posix_file_map_attribute y) noexcept{return x=x|y;}

inline posix_file_map_attribute& operator^=(posix_file_map_attribute& x, posix_file_map_attribute y) noexcept{return x=x^y;}

inline constexpr posix_file_map_attribute to_posix_file_map_attribute(file_map_attribute x)
{
	switch(x)
	{
	case file_map_attribute::execute_read:return posix_file_map_attribute::execute|posix_file_map_attribute::read;
	case file_map_attribute::execute_read_write:return posix_file_map_attribute::execute|posix_file_map_attribute::read;
	case file_map_attribute::execute_write_copy:return posix_file_map_attribute::execute|posix_file_map_attribute::write;
	case file_map_attribute::read_only:return posix_file_map_attribute::read;
	case file_map_attribute::read_write:return posix_file_map_attribute::read|posix_file_map_attribute::write;
	case file_map_attribute::write_copy:return posix_file_map_attribute::write;
	default:
		throw_posix_error(EINVAL);
	};
}

class posix_memory_map_file
{
public:
	using value_type = std::byte;
	using pointer = value_type*;
	using const_pointer = value_type const*;
	using const_iterator = const_pointer;
	using iterator = pointer;
	using reference = value_type&;
	using const_reference = value_type const&;
	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;
	using reverse_iterator = std::reverse_iterator<iterator>;
	pointer address_begin{},address_end{};
	constexpr posix_memory_map_file()=default;
	constexpr posix_memory_map_file(std::byte* addbg,std::byte* added):posix_memory_map_io_observer{addbg,added}
	{}
	posix_memory_map_file(posix_at_entry bf,file_map_attribute attr,std::size_t bytes,std::uintmax_t start_address=0)
		:posix_memory_map_io_observer{reinterpret_cast<std::byte*>(mmap64(nullptr,bytes,static_cast<int>(to_posix_file_map_attribute(attr)),MAP_SHARED,bf.native_handle(),start_address))}
	{
		if(this->address_begin==MAP_FAILED)[[unlikely]]
			throw_posix_error();
		this->address_end=this->address_begin+bytes;
	}
	posix_memory_map_file(posix_memory_map_file const&)=delete;
	posix_memory_map_file& operator=(posix_memory_map_file const&)=delete;
	posix_memory_map_file(posix_memory_map_file&& other) noexcept:posix_memory_map_io_observer{other.address_begin,other.address_end}
	{
		other.address_end=other.address_begin=reinterpret_cast<std::byte*>(MAP_FAILED);
	}
	posix_memory_map_file& operator=(posix_memory_map_file&& other) noexcept
	{
		if(__builtin_addressof(other)==this)
			return *this;
		if(this->address_begin!=reinterpret_cast<std::byte*>(MAP_FAILED))[[likely]]
			munmap(this->address_begin,this->bytes());
		this->address_begin=other.address_begin;
		this->address_end=other.address_end;
		other.address_begin=reinterpret_cast<std::byte*>(MAP_FAILED);
		other.address_end=reinterpret_cast<std::byte*>(MAP_FAILED);
		return *this;
	}
	constexpr pointer data() const noexcept
	{
		return address_begin;
	}
	constexpr bool empty() const noexcept
	{
		return address_begin==address_end;
	}
	constexpr std::size_t size() const noexcept
	{
		return static_cast<std::size_t>(address_end-address_begin);
	}
	constexpr const_iterator cbegin() const noexcept
	{
		return address_begin;
	}
	constexpr const_iterator begin() const noexcept
	{
		return address_begin;
	}
	constexpr iterator begin() noexcept
	{
		return address_begin;
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
		return const_reverse_iterator{address_begin};
	}
	constexpr reverse_iterator rend() noexcept
	{
		return reverse_iterator{address_begin};
	}
	constexpr const_reverse_iterator rend() const noexcept
	{
		return const_reverse_iterator{address_begin};
	}
	constexpr const_reference front() const noexcept
	{
		return *address_begin;
	}
	constexpr reference front() noexcept
	{
		return *address_begin;
	}
	constexpr const_reference back() const noexcept
	{
		return address_end[-1];
	}
	constexpr reference back() noexcept
	{
		return address_end[-1];
	}
	constexpr reference operator[](size_type size) noexcept
	{
		return address_begin[size];
	}
	constexpr const_reference operator[](size_type size) const noexcept
	{
		return address_begin[size];
	}
	void close() noexcept
	{
		if(this->address_begin!=MAP_FAILED)[[likely]]
		{
			munmap(this->address_begin,this->bytes());
			this->address_begin=reinterpret_cast<std::byte*>(MAP_FAILED);
		}
	}
	~posix_memory_map_file()
	{
		if(this->address_begin!=MAP_FAILED)[[likely]]
			munmap(this->address_begin,this->bytes());
	}
};

using native_memory_map_file = posix_memory_map_file;

}
