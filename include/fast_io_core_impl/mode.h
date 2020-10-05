#pragma once

namespace fast_io
{

/*
https://github.com/gcc-mirror/gcc/blob/41d6b10e96a1de98e90a7c0378437c3255814b16/libstdc%2B%2B-v3/include/bits/fs_fwd.h#L73

Being binary compatible with libstdc++'s file_type
*/
enum class file_type : signed char
{
none = 0, not_found = -1, regular = 1, directory = 2, symlink = 3,
block = 4, character = 5, fifo = 6, socket = 7, unknown = 8, remote = 9
};

template<std::integral char_type>
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<char_type,file_type>) noexcept
{
	return 20;
}

template<std::integral char_type,std::random_access_iterator caiter>
inline constexpr caiter print_reserve_define(io_reserve_type_t<char_type,file_type>,caiter it,file_type f) noexcept
{
	switch(f)
	{
	case file_type::none:
		return details::copy_string_literal(u8"none",it); 
	case file_type::not_found:
		return details::copy_string_literal(u8"not_found",it); 
	case file_type::regular:
		return details::copy_string_literal(u8"regular",it); 
	case file_type::directory:
		return details::copy_string_literal(u8"directory",it); 
	case file_type::symlink:
		return details::copy_string_literal(u8"symlink",it);
	case file_type::block:
		return details::copy_string_literal(u8"block",it);
	case file_type::character:
		return details::copy_string_literal(u8"character",it);
	case file_type::fifo:
		return details::copy_string_literal(u8"fifo",it);
	case file_type::socket:
		return details::copy_string_literal(u8"socket",it);
	case file_type::remote:
		return details::copy_string_literal(u8"remote",it);
	default:
		return details::copy_string_literal(u8"unknown",it);
	}
}

enum class shut{in,out,io};

enum class open_mode:std::uint32_t
{
none=0,
//	indicates that the open mode has not been evaluated yet
app=1<<0,
//	*["a"]seek to the end of stream before each write.
archive=1<<1,
//	[Windows]FILE_ATTRIBUTE_ARCHIVE 0x20
binary=1<<2,
//	["b"]open in binary mode. native_file is always binary.
//	The text mode is provided by the POSIX layer, while native_file is win32_file or nt_file, which ignores this toggle.
case_insensitive=1<<3,
//	[Windows]NT OBJ_CASE_INSENSITIVE 0x00000040L
compressed=1<<4,
//	[Windows]FILE_ATTRIBUTE_COMPRESSED 0x800
creat=1<<5,
//	*POSIX O_CREAT
direct=1<<6,
//	*POSIX O_DIRECT
directory=1<<7,
//	*POSIX O_DIRECTORY.
encrypted=1<<8,
//	[Windows]FILE_ATTRIBUTE_ENCRYPTED 0x4000
excl=1<<9,
//	*["x"]POSIX O_EXCL
follow=1<<10,
//	*Inverse of POSIX O_NOFOLLOW.
hidden=1<<11,
//	[Windows]FILE_ATTRIBUTE_HIDDEN 0x2
in=1<<12,
//	*["r"]open for reading.
inherit=1<<13,
//	*Inverse of POSIX O_CLOEXEC.
no_atime=1<<14,
//	[Linux]O_NOATIME
no_block=1<<15,
//	*POSIX O_NONBLOCK
no_ctty=1<<16,
//	[Linux]O_NOCTTY
no_recall=1<<17,
//	[Windows]FILE_FLAG_OPEN_NO_RECALL 0x00100000
normal=1<<18,
//	[Windows]FILE_ATTRIBUTE_NORMAL 0x80
offline=1<<19,
//	[Windows]FILE_ATTRIBUTE_OFFLINE 0x1000
out=1<<20,
//	*["w"]open for writing.
path=1<<21,
//	[Linux]O_PATH
random_access=1<<22,
//	[Windows]FILE_FLAG_RANDOM_ACCESS 0x10000000
search=1<<23,
//	POSIX O_SEARCH
session_aware=1<<24,
//	[Windows]FILE_FLAG_SESSION_AWARE 0x00800000
sync=1<<25,
//	*POSIX O_SYNC
system=1<<26,
//	[Windows]FILE_ATTRIBUTE_SYSTEM 0x4
temporary=1<<27,
//	*[Linux]O_TMPFILE. [Windows]FILE_ATTRIBUTE_TEMPORARY 0x100 + FILE_FLAG_DELETE_ON_CLOSE 0x04000000
trunc=1<<28,
//	*POSIX O_TRUNC
tty_init=1<<29,
//	POSIX O_TTY_INIT
};

constexpr open_mode operator&(open_mode x, open_mode y) noexcept
{
using utype = typename std::underlying_type<open_mode>::type;
return static_cast<open_mode>(static_cast<utype>(x) & static_cast<utype>(y));
}

constexpr open_mode operator|(open_mode x, open_mode y) noexcept
{
using utype = typename std::underlying_type<open_mode>::type;
return static_cast<open_mode>(static_cast<utype>(x) | static_cast<utype>(y));
}

constexpr open_mode operator^(open_mode x, open_mode y) noexcept
{
using utype = typename std::underlying_type<open_mode>::type;
return static_cast<open_mode>(static_cast<utype>(x) ^ static_cast<utype>(y));
}

constexpr open_mode operator~(open_mode x) noexcept
{
using utype = typename std::underlying_type<open_mode>::type;
return static_cast<open_mode>(~static_cast<utype>(x));
}

inline constexpr open_mode& operator&=(open_mode& x, open_mode y) noexcept{return x=x&y;}

inline constexpr open_mode& operator|=(open_mode& x, open_mode y) noexcept{return x=x|y;}

inline constexpr open_mode& operator^=(open_mode& x, open_mode y) noexcept{return x=x^y;}



inline constexpr open_mode c_supported(open_mode m) noexcept
{
using utype = typename std::underlying_type<open_mode>::type;
constexpr auto c_supported_values{static_cast<utype>(open_mode::binary)|
	static_cast<utype>(open_mode::excl)|
	static_cast<utype>(open_mode::out)|
	static_cast<utype>(open_mode::app)|
	static_cast<utype>(open_mode::in)|
	static_cast<utype>(open_mode::trunc)};
return static_cast<open_mode>(static_cast<utype>(m)&c_supported_values);
}

[[noreturn]] inline void throw_posix_error(int);

inline constexpr char const* to_c_mode(open_mode m) noexcept
{
	using utype = typename std::underlying_type<open_mode>::type;
	switch(static_cast<utype>(c_supported(m)))
	{
//Action if file already exists;	Action if file does not exist;	c-style mode;	Explanation
//Read from start;	Failure to open;	"r";	Open a file for reading
	case static_cast<utype>(open_mode::in):
		return "r";
//Destroy contents;	Create new;	"w";	Create a file for writing
	case static_cast<utype>(open_mode::out):
	case static_cast<utype>(open_mode::out)|static_cast<utype>(open_mode::trunc):
		return "w";
//Append to file;	Create new;	"a";	Append to a file
	case static_cast<utype>(open_mode::app):
	case static_cast<utype>(open_mode::out)|static_cast<utype>(open_mode::app):
		return "a";
//Read from start;	Error;	"r+";		Open a file for read/write
	case static_cast<utype>(open_mode::out)|static_cast<utype>(open_mode::in):
		return "r+";
//Destroy contents;	Create new;	"w+";	Create a file for read/write
	case static_cast<utype>(open_mode::out)|static_cast<utype>(open_mode::in)|static_cast<utype>(open_mode::trunc):
		return "w+";
//Write to end;	Create new;	"a+";	Open a file for read/write
	case static_cast<utype>(open_mode::out)|static_cast<utype>(open_mode::in)|static_cast<utype>(open_mode::app):
	case static_cast<utype>(open_mode::in)|static_cast<utype>(open_mode::app):
		return "a+";
//Destroy contents;	Error;	"wx";	Create a file for writing
	case static_cast<utype>(open_mode::out)|static_cast<utype>(open_mode::excl):
	case static_cast<utype>(open_mode::out)|static_cast<utype>(open_mode::trunc)|static_cast<utype>(open_mode::excl):
		return "wx";
//Append to file;	Error;	"ax";	Append to a file
	case static_cast<utype>(open_mode::app)|static_cast<utype>(open_mode::excl):
	case static_cast<utype>(open_mode::out)|static_cast<utype>(open_mode::app)|static_cast<utype>(open_mode::excl):
		return "ax";
//Destroy contents;	Error;	"w+x";	Create a file for read/write
	case static_cast<utype>(open_mode::out)|static_cast<utype>(open_mode::in)|static_cast<utype>(open_mode::trunc)|static_cast<utype>(open_mode::excl):
		return "w+x";
//Write to end;	Error;	"a+x";	Open a file for read/write
	case static_cast<utype>(open_mode::out)|static_cast<utype>(open_mode::in)|static_cast<utype>(open_mode::app)|static_cast<utype>(open_mode::excl):
	case static_cast<utype>(open_mode::in)|static_cast<utype>(open_mode::app)|static_cast<utype>(open_mode::excl):
		return "a+x";
	break;
	
//binary support

//Action if file already exists;	Action if file does not exist;	c-style mode;	Explanation
//Read from start;	Failure to open;	"rb";	Open a file for reading
	case static_cast<utype>(open_mode::in)|static_cast<utype>(open_mode::binary):
		return "rb";
//Destroy contents;	Create new;	"wb";	Create a file for writing
	case static_cast<utype>(open_mode::out)|static_cast<utype>(open_mode::binary):
	case static_cast<utype>(open_mode::out)|static_cast<utype>(open_mode::trunc)|static_cast<utype>(open_mode::binary):
		return "wb";
//Append to file;	Create new;	"ab";	Append to a file
	case static_cast<utype>(open_mode::app)|static_cast<utype>(open_mode::binary):
	case static_cast<utype>(open_mode::out)|static_cast<utype>(open_mode::app)|static_cast<utype>(open_mode::binary):
		return "ab";
//Read from start;	Error;	"r+b";		Open a file for read/write
	case static_cast<utype>(open_mode::out)|static_cast<utype>(open_mode::in)|static_cast<utype>(open_mode::binary):
		return "r+b";
//Destroy contents;	Create new;	"w+b";	Create a file for read/write
	case static_cast<utype>(open_mode::out)|static_cast<utype>(open_mode::in)|static_cast<utype>(open_mode::trunc)|static_cast<utype>(open_mode::binary):
		return "w+b";
//Write to end;	Create new;	"a+b";	Open a file for read/write
	case static_cast<utype>(open_mode::out)|static_cast<utype>(open_mode::in)|static_cast<utype>(open_mode::app)|static_cast<utype>(open_mode::binary):
	case static_cast<utype>(open_mode::in)|static_cast<utype>(open_mode::app)|static_cast<utype>(open_mode::binary):
		return "a+b";
//Destroy contents;	Error;	"wxb";	Create a file for writing
	case static_cast<utype>(open_mode::out)|static_cast<utype>(open_mode::excl)|static_cast<utype>(open_mode::binary):
	case static_cast<utype>(open_mode::out)|static_cast<utype>(open_mode::trunc)|static_cast<utype>(open_mode::excl)|static_cast<utype>(open_mode::binary):
		return "wxb";
//Append to file;	Error;	"axb";	Append to a file
	case static_cast<utype>(open_mode::app)|static_cast<utype>(open_mode::excl)|static_cast<utype>(open_mode::binary):
	case static_cast<utype>(open_mode::out)|static_cast<utype>(open_mode::app)|static_cast<utype>(open_mode::excl)|static_cast<utype>(open_mode::binary):
		return "axb";
//Destroy contents;	Error;	"w+xb";	Create a file for read/write
	case static_cast<utype>(open_mode::out)|static_cast<utype>(open_mode::in)|static_cast<utype>(open_mode::trunc)|static_cast<utype>(open_mode::excl)|static_cast<utype>(open_mode::binary):
		return "w+xb";
//Write to end;	Error;	"a+xb";	Open a file for read/write
	case static_cast<utype>(open_mode::out)|static_cast<utype>(open_mode::in)|static_cast<utype>(open_mode::app)|static_cast<utype>(open_mode::excl)|static_cast<utype>(open_mode::binary):
	case static_cast<utype>(open_mode::in)|static_cast<utype>(open_mode::app)|static_cast<utype>(open_mode::excl)|static_cast<utype>(open_mode::binary):
		return "a+xb";
	break;
	default:
		return "";
	}
}

inline constexpr open_mode c_mode(std::string_view csm) noexcept
{
	open_mode v{};
	bool extended{};
	for(auto const& e : csm)
		if(e==0x2b)
			extended=true;
	for(auto const& e : csm)
		switch(e)
		{
			case u8'a':
				v|=open_mode::app;
				if(extended)
					v|=open_mode::in|open_mode::out;
			break;
			case u8'b':
				v|=open_mode::binary;
			break;
			case 0x72:
				v|=open_mode::in;
				if(extended)
					v|=open_mode::out;
			break;
			case 0x77:
				v|=open_mode::out;
				if(extended)
					v|=open_mode::in|open_mode::trunc;
			break;
			case 0x78:
				v|=open_mode::excl;
			break;
			case 0x2b:
			break;
		}
	return v;
}
struct native_interface_t
{
explicit constexpr native_interface_t() noexcept=default;
};
inline constexpr native_interface_t native_interface;

struct io_cookie_t
{
explicit constexpr io_cookie_t() noexcept=default;
};
inline constexpr io_cookie_t io_cookie{};

struct io_async_t
{
explicit constexpr io_async_t() noexcept=default;
};
inline constexpr io_async_t io_async{};

struct io_temp_t
{
explicit constexpr io_temp_t() noexcept=default;
};
inline constexpr io_temp_t io_temp{};

struct io_dup_t
{
explicit constexpr io_dup_t() noexcept=default;
};
inline constexpr io_dup_t io_dup{};

struct posix_at_entry
{
	int fd{-1};
	explicit constexpr posix_at_entry() noexcept=default;
	explicit constexpr posix_at_entry(int mfd) noexcept:fd(mfd){}
};

}