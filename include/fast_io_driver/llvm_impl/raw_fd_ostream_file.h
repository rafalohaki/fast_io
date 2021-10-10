#pragma once

namespace fast_io::llvm
{

namespace details
{

inline raw_fd_ostream* open_llvm_raw_fd_ostream_from_fd(int fd)
{
	return new ::llvm::raw_fd_ostream(fd,true);
}

}

template<std::integral ch_type>
requires (sizeof(char_type)==1)
class basic_raw_fd_ostream_file:public basic_general_raw_ostream_io_observer<ch_type>
{
public:
	using char_type = ch_type;
	using native_handle_type = typename basic_general_raw_ostream_io_observer<char_type>::native_handle_type;
	explicit constexpr basic_raw_fd_ostream_file() noexcept = default;
	template<typename T>
	requires std::same_as<T,::llvm::raw_fd_ostream>
	explicit constexpr basic_raw_fd_ostream_file(T* p) noexcept:basic_general_raw_ostream_io_observer<char_type>{p}{}

#if !defined(__AVR__)
	basic_raw_fd_ostream_file(basic_posix_io_handle<char_type>&& pioh,::fast_io::open_mode):basic_general_raw_ostream_io_observer<char_type>{::fast_io::llvm::details::open_llvm_raw_fd_ostream_from_fd(pioh.fd)}
	{
		pioh.fd=-1;
	}
#if (defined(_WIN32)&&!defined(__WINE__)) || defined(__CYGWIN__)
//windows specific. open posix file from win32 io handle
	template<win32_family family>
	basic_raw_fd_ostream_file(basic_win32_family_io_handle<family,char_type>&& win32_handle,open_mode mode):
		basic_raw_fd_ostream_file(basic_posix_file<char_type>(::fast_io::freestanding::move(win32_handle),mode),mode)
	{
	}
	template<nt_family family>
	basic_raw_fd_ostream_file(basic_nt_family_io_handle<family,char_type>&& nt_handle,open_mode mode):
		basic_raw_fd_ostream_file(basic_posix_file<char_type>(::fast_io::freestanding::move(nt_handle),mode),mode)
	{
	}
#endif
	basic_raw_fd_ostream_file(native_fs_dirent fsdirent,open_mode om,perms pm=static_cast<perms>(436)):
		basic_raw_fd_ostream_file(basic_posix_file<char_type>(fsdirent,om,pm),om)
	{}
	basic_raw_fd_ostream_file(cstring_view file,open_mode om,perms pm=static_cast<perms>(436)):
		basic_raw_fd_ostream_file(basic_posix_file<char_type>(file,om,pm),om)
	{}
	basic_raw_fd_ostream_file(native_at_entry nate,cstring_view file,open_mode om,perms pm=static_cast<perms>(436)):
		basic_raw_fd_ostream_file(basic_posix_file<char_type>(nate,file,om,pm),om)
	{}
	basic_raw_fd_ostream_file(wcstring_view file,open_mode om,perms pm=static_cast<perms>(436)):
		basic_raw_fd_ostream_file(basic_posix_file<char_type>(file,om,pm),om)
	{}
	basic_raw_fd_ostream_file(native_at_entry nate,wcstring_view file,open_mode om,perms pm=static_cast<perms>(436)):
		basic_raw_fd_ostream_file(basic_posix_file<char_type>(nate,file,om,pm),om)
	{}
	basic_raw_fd_ostream_file(u8cstring_view file,open_mode om,perms pm=static_cast<perms>(436)):
		basic_raw_fd_ostream_file(basic_posix_file<char_type>(file,om,pm),om)
	{}
	basic_raw_fd_ostream_file(native_at_entry nate,u8cstring_view file,open_mode om,perms pm=static_cast<perms>(436)):
		basic_raw_fd_ostream_file(basic_posix_file<char_type>(nate,file,om,pm),om)
	{}
	basic_raw_fd_ostream_file(u16cstring_view file,open_mode om,perms pm=static_cast<perms>(436)):
		basic_raw_fd_ostream_file(basic_posix_file<char_type>(file,om,pm),om)
	{}
	basic_raw_fd_ostream_file(native_at_entry nate,u16cstring_view file,open_mode om,perms pm=static_cast<perms>(436)):
		basic_raw_fd_ostream_file(basic_posix_file<char_type>(nate,file,om,pm),om)
	{}
	basic_raw_fd_ostream_file(u32cstring_view file,open_mode om,perms pm=static_cast<perms>(436)):
		basic_raw_fd_ostream_file(basic_posix_file<char_type>(file,om,pm),om)
	{}
	basic_raw_fd_ostream_file(native_at_entry nate,u32cstring_view file,open_mode om,perms pm=static_cast<perms>(436)):
		basic_raw_fd_ostream_file(basic_posix_file<char_type>(nate,file,om,pm),om)
	{}
#endif
};

}
