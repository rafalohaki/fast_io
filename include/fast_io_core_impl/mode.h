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
block = 4, character = 5, fifo = 6, socket = 7, unknown = 8
};


enum class open_mode:std::uint32_t
{
none = 0,
app = 1 << 0,
archive = 1 << 1,
ate = 1 << 2,
binary = 1 << 3,
compressed = 1 << 4,
creat = 1 << 5,
direct = 1 << 6,
directory = 1 << 7,
encrypted = 1 << 8,
excl = 1 << 9,
follow = 1 << 10,
hidden = 1 << 11,
in = 1 << 12,
inherit = 1 << 13,
no_atime = 1 << 14,
no_block = 1 << 15,
no_ctty = 1 << 16,
no_recall = 1 << 17,
normal = 1 << 18,
offline = 1 << 19,
out = 1 << 20,
posix_semantics = 1 << 21,
random_access = 1 << 22,
session_aware = 1 << 23,
sync = 1 << 24,
system = 1 << 25,
temporary = 1 << 26,
trunc = 1 << 27
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

template<open_mode pm>
struct open_interface_t
{
inline constexpr static open_mode value{pm};
};

template<open_mode pm>
inline constexpr open_interface_t<pm> open_interface{};

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

inline auto constexpr to_c_mode(open_mode const& m)
{
	using namespace std::string_view_literals;
	using utype = typename std::underlying_type<open_mode>::type;
	switch(static_cast<utype>(c_supported(m)))
	{
//Action if file already exists;	Action if file does not exist;	c-style mode;	Explanation
//Read from start;	Failure to open;	"r";	Open a file for reading
	case static_cast<utype>(open_mode::in):
		return "r"sv;
//Destroy contents;	Create new;	"w";	Create a file for writing
	case static_cast<utype>(open_mode::out):
	case static_cast<utype>(open_mode::out)|static_cast<utype>(open_mode::trunc):
		return "w"sv;
//Append to file;	Create new;	"a";	Append to a file
	case static_cast<utype>(open_mode::app):
	case static_cast<utype>(open_mode::out)|static_cast<utype>(open_mode::app):
		return "a"sv;
//Read from start;	Error;	"r+";		Open a file for read/write
	case static_cast<utype>(open_mode::out)|static_cast<utype>(open_mode::in):
		return "r+"sv;
//Destroy contents;	Create new;	"w+";	Create a file for read/write
	case static_cast<utype>(open_mode::out)|static_cast<utype>(open_mode::in)|static_cast<utype>(open_mode::trunc):
		return "w+"sv;
//Write to end;	Create new;	"a+";	Open a file for read/write
	case static_cast<utype>(open_mode::out)|static_cast<utype>(open_mode::in)|static_cast<utype>(open_mode::app):
	case static_cast<utype>(open_mode::in)|static_cast<utype>(open_mode::app):
		return "a+"sv;
//Destroy contents;	Error;	"wx";	Create a file for writing
	case static_cast<utype>(open_mode::out)|static_cast<utype>(open_mode::excl):
	case static_cast<utype>(open_mode::out)|static_cast<utype>(open_mode::trunc)|static_cast<utype>(open_mode::excl):
		return "wx"sv;
//Append to file;	Error;	"ax";	Append to a file
	case static_cast<utype>(open_mode::app)|static_cast<utype>(open_mode::excl):
	case static_cast<utype>(open_mode::out)|static_cast<utype>(open_mode::app)|static_cast<utype>(open_mode::excl):
		return "ax"sv;
//Destroy contents;	Error;	"w+x";	Create a file for read/write
	case static_cast<utype>(open_mode::out)|static_cast<utype>(open_mode::in)|static_cast<utype>(open_mode::trunc)|static_cast<utype>(open_mode::excl):
		return "w+x"sv;
//Write to end;	Error;	"a+x";	Open a file for read/write
	case static_cast<utype>(open_mode::out)|static_cast<utype>(open_mode::in)|static_cast<utype>(open_mode::app)|static_cast<utype>(open_mode::excl):
	case static_cast<utype>(open_mode::in)|static_cast<utype>(open_mode::app)|static_cast<utype>(open_mode::excl):
		return "a+x"sv;
	break;
	
//binary support

//Action if file already exists;	Action if file does not exist;	c-style mode;	Explanation
//Read from start;	Failure to open;	"rb";	Open a file for reading
	case static_cast<utype>(open_mode::in)|static_cast<utype>(open_mode::binary):
		return "rb"sv;
//Destroy contents;	Create new;	"wb";	Create a file for writing
	case static_cast<utype>(open_mode::out)|static_cast<utype>(open_mode::binary):
	case static_cast<utype>(open_mode::out)|static_cast<utype>(open_mode::trunc)|static_cast<utype>(open_mode::binary):
		return "wb"sv;
//Append to file;	Create new;	"ab";	Append to a file
	case static_cast<utype>(open_mode::app)|static_cast<utype>(open_mode::binary):
	case static_cast<utype>(open_mode::out)|static_cast<utype>(open_mode::app)|static_cast<utype>(open_mode::binary):
		return "ab"sv;
//Read from start;	Error;	"r+b";		Open a file for read/write
	case static_cast<utype>(open_mode::out)|static_cast<utype>(open_mode::in)|static_cast<utype>(open_mode::binary):
		return "r+b"sv;
//Destroy contents;	Create new;	"w+b";	Create a file for read/write
	case static_cast<utype>(open_mode::out)|static_cast<utype>(open_mode::in)|static_cast<utype>(open_mode::trunc)|static_cast<utype>(open_mode::binary):
		return "w+b"sv;
//Write to end;	Create new;	"a+b";	Open a file for read/write
	case static_cast<utype>(open_mode::out)|static_cast<utype>(open_mode::in)|static_cast<utype>(open_mode::app)|static_cast<utype>(open_mode::binary):
	case static_cast<utype>(open_mode::in)|static_cast<utype>(open_mode::app)|static_cast<utype>(open_mode::binary):
		return "a+b"sv;
//Destroy contents;	Error;	"wxb";	Create a file for writing
	case static_cast<utype>(open_mode::out)|static_cast<utype>(open_mode::excl)|static_cast<utype>(open_mode::binary):
	case static_cast<utype>(open_mode::out)|static_cast<utype>(open_mode::trunc)|static_cast<utype>(open_mode::excl)|static_cast<utype>(open_mode::binary):
		return "wxb"sv;
//Append to file;	Error;	"axb";	Append to a file
	case static_cast<utype>(open_mode::app)|static_cast<utype>(open_mode::excl)|static_cast<utype>(open_mode::binary):
	case static_cast<utype>(open_mode::out)|static_cast<utype>(open_mode::app)|static_cast<utype>(open_mode::excl)|static_cast<utype>(open_mode::binary):
		return "axb"sv;
//Destroy contents;	Error;	"w+xb";	Create a file for read/write
	case static_cast<utype>(open_mode::out)|static_cast<utype>(open_mode::in)|static_cast<utype>(open_mode::trunc)|static_cast<utype>(open_mode::excl)|static_cast<utype>(open_mode::binary):
		return "w+xb"sv;
//Write to end;	Error;	"a+xb";	Open a file for read/write
	case static_cast<utype>(open_mode::out)|static_cast<utype>(open_mode::in)|static_cast<utype>(open_mode::app)|static_cast<utype>(open_mode::excl)|static_cast<utype>(open_mode::binary):
	case static_cast<utype>(open_mode::in)|static_cast<utype>(open_mode::app)|static_cast<utype>(open_mode::excl)|static_cast<utype>(open_mode::binary):
		return "a+xb"sv;
	break;
	default:
#ifdef __cpp_exceptions
		throw fast_io_text_error("unknown open mode");
#else
		fast_terminate();
#endif
	}
}

template<std::integral ch_type>
inline auto constexpr from_c_mode(std::basic_string_view<ch_type> csm)
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
			default:
#ifdef __cpp_exceptions
				throw fast_io_text_error("unknown C-style open mode");
#else
				fast_terminate();
#endif
		}
	return v;
}
struct native_interface_t
{
explicit constexpr native_interface_t() = default;
};
inline constexpr native_interface_t native_interface;

struct io_cookie_t
{
explicit constexpr io_cookie_t()=default;
};
inline constexpr io_cookie_t io_cookie{};

struct io_async_t
{
explicit constexpr io_async_t()=default;
};
inline constexpr io_async_t io_async{};

struct io_temp_t
{
explicit constexpr io_temp_t()=default;
};
inline constexpr io_temp_t io_temp{};

}