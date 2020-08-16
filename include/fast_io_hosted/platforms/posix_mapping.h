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
#ifdef __cpp_exceptions
		throw fast_io_text_error("unknown file_mapping_attribute");
#else
		fast_terminate();
#endif
	};
}

class posix_memory_map_io_observer
{
public:
	using native_handle_type = void*;
	std::byte *address_begin{},*address_end{};
	constexpr std::size_t bytes() const noexcept
	{
		return address_end-address_begin;
	}
};

class posix_memory_map_file:public posix_memory_map_io_observer
{
public:
	using native_handle_type = void*;
	constexpr posix_memory_map_file()=default;
	constexpr posix_memory_map_file(std::byte* addbg,std::byte* added):posix_memory_map_io_observer{addbg,added}
	{}
	template<std::integral char_type>
	posix_memory_map_file(basic_posix_io_observer<char_type> bf,file_map_attribute attr,std::size_t bytes,std::size_t start_address=0)
		:posix_memory_map_io_observer{reinterpret_cast<std::byte*>(mmap(nullptr,bytes,static_cast<int>(to_posix_file_map_attribute(attr)),MAP_SHARED,bf.native_handle(),start_address))}
	{
		if(this->address_begin==MAP_FAILED)[[unlikely]]
			throw_posix_error();
	}
	posix_memory_map_file(posix_memory_map_file const&)=delete;
	posix_memory_map_file& operator=(posix_memory_map_file const&)=delete;
	posix_memory_map_file(posix_memory_map_file&& other) noexcept:posix_memory_map_io_observer{other.address_begin,other.address_end}
	{
		other.address_end=other.address_begin=reinterpret_cast<std::byte*>(MAP_FAILED);
	}
	posix_memory_map_file& operator=(posix_memory_map_file&& other) noexcept
	{
		if(std::addressof(other)==this)
			return *this;
		if(this->address_begin!=reinterpret_cast<std::byte*>(MAP_FAILED))[[likely]]
			munmap(this->address_begin,this->bytes());
		this->address_begin=other.address_begin;
		this->address_end=other.address_end;
		other.address_begin=reinterpret_cast<std::byte*>(MAP_FAILED);
		other.address_end=reinterpret_cast<std::byte*>(MAP_FAILED);
		return *this;
	}
	void close() noexcept
	{
		if(this->address_begin!=MAP_FAILED)[[likely]]
		{
			munmap(this->address_begin,this->bytes());
			this->address_begin=nullptr;
		}
	}
	~posix_memory_map_file()
	{
		if(this->address_begin!=MAP_FAILED)[[likely]]
			munmap(this->address_begin,this->bytes());
	}
};

using native_memory_map_io_observer = posix_memory_map_io_observer;
using native_memory_map_file = posix_memory_map_file;

}
