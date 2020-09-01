#pragma once
#include <dirent.h>

namespace fast_io
{

class posix_directory_io_observer
{
public:
	using native_handle_type = DIR*;
	native_handle_type dirp{};
	constexpr auto& native_handle() noexcept
	{
		return dirp;
	}
	constexpr auto& native_handle() const noexcept
	{
		return dirp;
	}
	explicit constexpr operator bool() const noexcept
	{
		return dirp;
	}
	operator basic_posix_io_observer<char>() const noexcept
	{
		if(dirfd==nullptr)
		{
			errno=EBADF;
			return {-1};
		}
		return {::dirfd(dirp)};
	}
	constexpr native_handle_type release() noexcept
	{
		auto temp{dirp};
		dirp=nullptr;
		return temp;
	}
};

namespace details
{

inline DIR* sys_dup_dir(DIR* dirp)
{
	if(dirfd==nullptr)
		throw_posix_error(EBADF);
	auto fd{::dirfd(dirp)};
	if(fd==-1)
		throw_posix_error();
	auto newfd{details::sys_dup(fd)};
	auto newdir{::fdopendir(newfd)};
	if(newdir==nullptr)
	{
		details::sys_close(newfd);
		throw_posix_error();
	}
	return newdir;
}

}

class posix_directory_io_handle:public posix_directory_io_observer
{
public:
	using native_handle_type = DIR*;
	constexpr posix_directory_io_handle()=default;
	template<typename native_hd>
	requires std::same_as<native_handle_type,std::remove_cvref_t<native_hd>>
	constexpr posix_directory_io_handle(native_hd dirp):posix_directory_io_observer{dirp}{}
	posix_directory_io_handle(posix_directory_io_handle const& other):posix_directory_io_observer{details::sys_dup_dir(other.dirp)}{}
	posix_directory_io_handle& operator=(posix_directory_io_handle const& other)
	{
		auto newdir{details::sys_dup_dir(other.dirp)};
		if(this->native_handle())[[likely]]
			::closedir(this->native_handle());
		this->native_handle()=newdir;
		return *this;
	}
	constexpr posix_directory_io_handle(posix_directory_io_handle&& other) noexcept:posix_directory_io_observer{other.release()}{}

	posix_directory_io_handle& operator=(posix_directory_io_handle&& other) noexcept
	{
		if(this==std::addressof(other))
			return *this;
		if(this->native_handle())[[likely]]
			::closedir(this->native_handle());
		this->native_handle()=other.release();
		return *this;
	}
	inline void reset(native_handle_type dirp=nullptr) noexcept
	{
		if(this->native_handle())[[likely]]
			::closedir(this->native_handle());
		this->native_handle()=dirp;
	}
	void close()
	{
		if(*this)[[likely]]
		{
			if(::closedir(this->native_handle())==-1)
				throw_posix_error();
			this->native_handle()=nullptr;
		}
	}
};

class posix_directory_file:public posix_directory_io_handle
{
public:
	using native_handle_type = DIR*;
	constexpr posix_directory_file()=default;
	template<typename native_hd>
	requires std::same_as<native_handle_type,std::remove_cvref_t<native_hd>>
	constexpr posix_directory_file(native_hd dirp):posix_directory_io_handle(dirp){}
	posix_directory_file(posix_io_handle&& pioh):posix_directory_io_handle(fdopendir(pioh.native_handle()))
	{
		if(this->native_handle()==nullptr)
			throw_posix_error();
		pioh.release();
	}
	posix_directory_file(std::string_view filename):posix_directory_file(posix_file(filename,open_interface<open_mode::in|open_mode::no_block|open_mode::directory|open_mode::binary|open_mode::large_file>))
	{
/*
https://code.woboq.org/userspace/glibc/sysdeps/posix/opendir.c.html
glibc:
enum {
  opendir_oflags = O_RDONLY|O_NDELAY|O_DIRECTORY|O_LARGEFILE|O_CLOEXEC
};
*/
	}
	posix_directory_file(io_at_t,native_io_observer niob,std::string_view filename):posix_directory_file(posix_file(io_at,niob,filename,open_interface<open_mode::in|open_mode::no_block|open_mode::directory|open_mode::binary|open_mode::large_file>))
	{
	}
	posix_directory_file(posix_directory_file const&) = default;
	posix_directory_file& operator=(posix_directory_file const&) = default;
	constexpr posix_directory_file(posix_directory_file&&) noexcept = default;
	posix_directory_file& operator=(posix_directory_file&&) noexcept = default;
	~posix_directory_file()
	{
		if(this->native_handle())[[likely]]
			::closedir(this->native_handle());
	}
};

inline void rewind(posix_directory_io_observer pdiob) noexcept
{
	::rewinddir(pdiob.dirp);	
}

inline void seek(posix_directory_io_observer pdiob,std::common_type_t<std::int64_t,std::ptrdiff_t> offset)
{
	if constexpr(sizeof(long)<sizeof(offset))
	{
		if(static_cast<std::common_type_t<std::int64_t,std::ptrdiff_t>>(std::numeric_limits<long>::max())<offset)
			throw_posix_error(EFBIG);
	}
	::seekdir(pdiob.dirp,static_cast<long>(offset));	
}

inline std::common_type_t<std::uint64_t,std::size_t> tell(posix_directory_io_observer pdiob)
{
	auto ret{::telldir(pdiob.dirp)};
	if(ret==-1)
		throw_posix_error();
	return ret;
}

struct posix_directory_entry
{
	struct dirent* entry{};
};

inline constexpr std::string_view filename(posix_directory_entry pioe) noexcept
{
	return pioe.entry->d_name;
}

inline constexpr std::common_type_t<std::size_t,std::uint64_t> inode(posix_directory_entry pioe) noexcept
{
	return pioe.entry->d_ino;
}

inline constexpr file_type type(posix_directory_entry pioe) noexcept
{
	switch(pioe.entry->d_type)
	{
	case DT_BLK:
		return file_type::block;
	case DT_CHR:
		return file_type::character;
	case DT_DIR:
		return file_type::directory;
	case DT_FIFO:
		return file_type::fifo;
	case DT_LNK:
		return file_type::symlink;
	case DT_REG:
		return file_type::regular;
	case DT_SOCK:
		return file_type::socket;
	case DT_UNKNOWN:
		return file_type::unknown;
	default:
		return file_type::not_found;
	};
}

struct posix_directory_iterator
{
	DIR* dirp{};
	struct dirent* entry{};
};

struct posix_directory_generator
{
	DIR* dirp{};
};

inline posix_directory_entry operator*(posix_directory_iterator pdit) noexcept
{
	return {pdit.entry};
}

inline posix_directory_iterator& operator++(posix_directory_iterator& pdit)
{
	errno=0;
	pdit.entry=readdir(pdit.dirp);
	if(pdit.entry==nullptr&&errno)
		throw_posix_error();
	return pdit;
}

inline constexpr posix_directory_iterator cbegin(posix_directory_generator pdg)
{
	posix_directory_iterator pdit{pdg.dirp};
	++pdit;
	return pdit;
}

inline constexpr std::default_sentinel_t cend(posix_directory_generator) noexcept
{
	return {};
}

inline constexpr bool operator==(std::default_sentinel_t, posix_directory_iterator const& b) noexcept
{
	return b.entry == nullptr;
}
inline constexpr bool operator==(posix_directory_iterator const& b, std::default_sentinel_t) noexcept
{
	return b.entry == nullptr;
}
inline constexpr bool operator!=(std::default_sentinel_t, posix_directory_iterator const& b) noexcept
{
	return b.entry;
}
inline constexpr bool operator!=(posix_directory_iterator const& b, std::default_sentinel_t) noexcept
{
	return b.entry;
}
inline constexpr posix_directory_generator generator(posix_directory_io_observer piob) noexcept
{
	return {piob};
}

}