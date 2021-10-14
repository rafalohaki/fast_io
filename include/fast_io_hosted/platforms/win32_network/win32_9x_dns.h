#pragma once

namespace fast_io
{
namespace win32
{

struct hostent
{
char  *h_name;
char  **h_aliases;
short h_addrtype;
short h_length;
char  **h_addr_list;
};

#if defined(_MSC_VER) && !defined(__clang__)
__declspec(dllimport)
#elif __has_cpp_attribute(gnu::dllimport)
[[gnu::dllimport]]
#endif
extern hostent* __stdcall gethostbyname(char const*) noexcept
#if defined(__clang__) || defined(__GNUC__)
#if SIZE_MAX<=UINT32_MAX &&(defined(__x86__) || defined(_M_IX86) || defined(__i386__))
#if !defined(__clang__)
asm("gethostbyname@4")
#else
asm("_gethostbyname@4")
#endif
#else
asm("gethostbyname")
#endif
#endif
;
}

namespace details
{

inline constexpr ::fast_io::ip hostent_to_ip_impl(::fast_io::win32::hostent* ent,std::size_t pos,std::uint_least16_t port) noexcept
{
	::fast_io::ip ipaddr;
	if(ent->h_addrtype==2)
	{
		if(ent->h_length==sizeof(posix_in_addr))[[likely]]
		{
			ipaddr.port=port;
			ipaddr.isv4=true;
			my_memcpy(__builtin_addressof(ipaddr.address.v4),ent->h_addr_list[pos],sizeof(posix_in_addr));
		}
	}
	else if(ent->h_addrtype==23)
	{
		if(ent->h_length==sizeof(posix_in6_addr))[[likely]]
		{
			ipaddr.port=port;
			my_memcpy(__builtin_addressof(ipaddr.address.v6),ent->h_addr_list[pos],sizeof(posix_in6_addr));
		}
	}
	return ipaddr;
}

}

class win32_9x_dns_io_observer
{
public:
	using native_handle_type = ::fast_io::win32::hostent*;
	native_handle_type res{};
	inline constexpr native_handle_type native_handle() const noexcept
	{
		return res;
	}
	explicit constexpr operator bool() noexcept
	{
		return res;
	}
	inline native_handle_type release() noexcept
	{
		native_handle_type temp{res};
		res=nullptr;
		return temp;
	}
};

inline constexpr ::fast_io::ip to_ip(win32_9x_dns_io_observer d,std::uint_least16_t port)
{
	return ::fast_io::details::hostent_to_ip_impl(d.res,0u,port);
}

struct win32_9x_dns_iterator
{
	using native_handle_type = ::fast_io::win32::hostent*;
	native_handle_type res{};
	std::size_t pos{};
};

inline constexpr win32_9x_dns_iterator operator*(win32_9x_dns_iterator d) noexcept
{
	return {d.res};
}

inline constexpr win32_9x_dns_iterator& operator++(win32_9x_dns_iterator& d) noexcept
{
	++d.pos;
	return d;
}

inline constexpr win32_9x_dns_iterator operator++(win32_9x_dns_iterator d, int) noexcept
{
	auto temp{d};
	++d;
	return temp;
}

inline constexpr win32_9x_dns_iterator begin(win32_9x_dns_io_observer w9xniob) noexcept
{
	return {w9xniob.res,0u};
}

inline constexpr win32_9x_dns_iterator cbegin(win32_9x_dns_io_observer w9xniob) noexcept
{
	return {w9xniob.res,0u};
}

inline constexpr ::fast_io::freestanding::default_sentinel_t end(win32_9x_dns_io_observer) noexcept
{
	return {};
}

inline constexpr ::fast_io::freestanding::default_sentinel_t cend(win32_9x_dns_io_observer) noexcept
{
	return {};
}

inline constexpr bool operator==(win32_9x_dns_iterator a, ::fast_io::freestanding::default_sentinel_t) noexcept
{
	return a.res==nullptr||(a.res->h_addr_list[a.pos]==nullptr);
}
inline constexpr bool operator!=(win32_9x_dns_iterator a, ::fast_io::freestanding::default_sentinel_t b) noexcept
{
	return !(a==b);
}

inline constexpr ::fast_io::ip to_ip(win32_9x_dns_iterator d,std::uint_least16_t port)
{
	return ::fast_io::details::hostent_to_ip_impl(d.res,d.pos,port);
}

namespace details
{
inline ::fast_io::win32::hostent* win32_9x_gethostbyname_impl(char const* name)
{
	::fast_io::win32::hostent* ret{::fast_io::win32::gethostbyname(name)};
	if(ret==nullptr)
		throw_win32_error(static_cast<std::uint32_t>(::fast_io::win32::WSAGetLastError()));
	return ret;
}

template<typename T>
inline constexpr auto win32_9x_dns_open_alien_char_type_impl(T const* t,std::size_t n)
{
	posix_api_encoding_converter converter(t,n);
	return win32_9x_gethostbyname_impl(converter.native_c_str());
}

template<typename T>
requires ::fast_io::constructible_to_os_c_str<T>
inline constexpr auto win32_9x_dns_open_impl(T const& t)
{
	if constexpr(::std::is_array_v<T>)
	{
		using cstr_char_type = std::remove_extent_t<T>;
		static_assert(::std::integral<cstr_char_type>);
		auto p{t};
		if constexpr(sizeof(cstr_char_type)==sizeof(char))
		{
			using char_type_may_alias_ptr
#if __has_cpp_attribute(gnu::may_alias)
			[[gnu::may_alias]]
#endif
			= char const*;
			return win32_9x_gethostbyname_impl(reinterpret_cast<char_type_may_alias_ptr>(p));
		}
		else
		{
			return win32_9x_dns_open_alien_char_type_impl(p,::fast_io::details::cal_array_size(t));
		}
	}
	else
	{
		using cstr_char_type = std::remove_pointer_t<decltype(t.c_str())>;
		if constexpr(sizeof(cstr_char_type)==sizeof(char))
		{
			return win32_9x_gethostbyname_impl(reinterpret_cast<char const*>(t.c_str()));
		}
		else
		{
#if __STDC_HOSTED__==1 && (!defined(_GLIBCXX_HOSTED) || _GLIBCXX_HOSTED==1) && __cpp_lib_ranges >= 201911L
			if constexpr(::std::ranges::contiguous_range<std::remove_cvref_t<T>>)
			{
				return win32_9x_dns_open_alien_char_type_impl(::std::ranges::data(t),::std::ranges::size(t));
			}
			else
#endif
			{
				auto ptr{t.c_str()};
				return win32_9x_dns_open_alien_char_type_impl(ptr,::fast_io::cstr_len(ptr));
			}
		}
	}
}
}

class win32_9x_dns_file:public win32_9x_dns_io_observer
{
public:
	using native_handle_type = typename win32_9x_dns_io_observer::native_handle_type;
	constexpr win32_9x_dns_file() noexcept = default;
	template<typename native_hd>
	requires std::same_as<native_handle_type,std::remove_cvref_t<native_hd>>
	explicit constexpr win32_9x_dns_file(native_hd res1) noexcept: win32_9x_dns_io_observer{res1}{}
	explicit constexpr win32_9x_dns_file(decltype(nullptr)) noexcept = delete;
	template<typename T>
	requires ::fast_io::constructible_to_os_c_str<T>
	explicit win32_9x_dns_file(T const& t):win32_9x_dns_io_observer{::fast_io::details::win32_9x_dns_open_impl(t)}{}
	void close() noexcept
	{
		this->res=nullptr;
	}
};

#if (defined(_WIN32_WINDOWS) || _WIN32_WINNT <= 0x0500)
using native_dns_io_observer = win32_9x_dns_io_observer;
using native_dns_file = win32_9x_dns_file;
using native_dns_iterator = win32_9x_dns_iterator;
#endif

}
