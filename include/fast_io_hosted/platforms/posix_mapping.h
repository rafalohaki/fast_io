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

class posix_file_map
{
	std::span<std::byte> rg;
	void close_impl() noexcept
	{
		if(rg.data())
			munmap(rg.data(), rg.size());
	}
public:
	template<std::integral ch_type>
	posix_file_map(basic_posix_io_observer<ch_type> bf,file_map_attribute attr,std::size_t bytes,std::size_t start_address=0)
	{
        struct stat file_stat;
        auto fstat_ret(fstat(bf.native_handle(), std::addressof(file_stat)));
        if (fstat_ret == -1)
                throw_posix_error();
        std::size_t file_size_in_bytes(file_stat.st_size);
        if (bytes > file_size_in_bytes)
        {
            // allocate more space for this file
            auto fallocate_ret(fallocate(bf.native_handle(), 0, file_size_in_bytes, bytes - file_size_in_bytes));
            if (fallocate_ret == -1)
                throw_posix_error();
        }
        auto ret(static_cast<std::byte*>(mmap(nullptr, bytes, static_cast<int>(to_posix_file_map_attribute(attr)), MAP_SHARED, bf.native_handle(), start_address)));
        if (ret == MAP_FAILED)
                throw_posix_error();
        rg = {ret, bytes};
	}
	//auto native_handle() const {return wfm.native_handle();}
	auto& region()
	{
		return rg;
	}
	posix_file_map(posix_file_map const&)=delete;
	posix_file_map& operator=(posix_file_map const&)=delete;
	posix_file_map(posix_file_map&& pm) noexcept:rg(pm.rg)
	{
		pm.rg={};
	}
	posix_file_map& operator=(posix_file_map&& pm) noexcept
	{
		if(std::addressof(pm)!=this)
		{
			close_impl();
			rg=pm.rg;
			pm.rg={};
		}
		return *this;
	}
	void close() noexcept
	{
		close_impl();
		rg={};
	}
	~posix_file_map()
	{
		close_impl();
	}
};

}
