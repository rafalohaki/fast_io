#pragma once

namespace fast_io
{

struct
#if __has_cpp_attribute(gnu::may_alias)
[[gnu::may_alias]]
#endif
posix_addrinfo
{
int              ai_flags{};
int              ai_family{};
int              ai_socktype{};
int              ai_protocol{};
posix_socklen_t  ai_addrlen{};
#if defined(__BIONIC__) || defined(__BSD_VISIBLE) || defined(__CYGWIN__) || defined(__DARWIN_C_LEVEL) || defined(_PICOLIBC__)
char            *ai_canonname{};
posix_sockaddr  *ai_addr{};
#else
posix_sockaddr  *ai_addr{};
char            *ai_canonname{};
#endif
posix_addrinfo  *ai_next{};
};

namespace details
{
extern int libc_getaddrinfo(char const *node, char const* service,
                posix_addrinfo const *hints,
                posix_addrinfo **res) noexcept __asm__("getaddrinfo");

extern void libc_freeaddrinfo(posix_addrinfo *res) noexcept __asm__("freeaddrinfo");
}

class posix_dns_io_observer
{
public:
	using char_type = char;
	using native_handle_type = posix_addrinfo*;
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

struct posix_dns_iterator
{
	using native_handle_type = posix_addrinfo*;
	native_handle_type res{};
};

inline constexpr bool operator==(posix_dns_iterator a, posix_dns_iterator b) noexcept
{
	return a.res == b.res;
}
inline constexpr bool operator!=(posix_dns_iterator a, posix_dns_iterator b) noexcept
{
	return !(a==b);
}

inline constexpr posix_dns_io_observer operator*(posix_dns_iterator d) noexcept
{
	return {d.res};
}

inline constexpr posix_dns_iterator& operator++(posix_dns_iterator& d) noexcept
{
	d.res = d.res->ai_next;
	return d;
}

inline constexpr posix_dns_iterator operator++(posix_dns_iterator& d, int) noexcept
{
	posix_dns_iterator temp{d};
	++d;
	return temp;
}

inline constexpr ::fast_io::ip to_ip(posix_dns_io_observer d,std::uint16_t port)
{
	return to_ip_with_ai_addr(d.res->ai_family,d.res->ai_addr,port);
}

namespace details
{

inline posix_addrinfo* my_getaddrinfo_impl(char const* node,char const* service,posix_addrinfo const* hints)
{
	posix_addrinfo* res{};
	int ec{libc_getaddrinfo(node,service,hints,__builtin_addressof(res))};
	if(ec)
		throw_posix_error(ec);
	return res;
}

template<typename T>
inline constexpr auto posix_dns_open_alien_char_type_impl(T const* t,std::size_t n)
{
	posix_api_encoding_converter converter(t,n);
	constexpr posix_addrinfo info{.ai_family=AF_UNSPEC};
	return my_getaddrinfo_impl(converter.native_c_str(),nullptr,__builtin_addressof(info));
}

template<typename T>
requires ::fast_io::constructible_to_os_c_str<T>
inline constexpr auto posix_dns_open_impl(T const& t)
{
	if constexpr(::std::is_array_v<T>)
	{
		using cstr_char_type = std::remove_extent_t<T>;
		static_assert(::std::integral<cstr_char_type>);
		auto p{t};
		if constexpr(sizeof(cstr_char_type)==sizeof(char))
		{
			constexpr posix_addrinfo info{.ai_family=AF_UNSPEC};
			using char_type_may_alias_ptr
#if __has_cpp_attribute(gnu::may_alias)
			[[gnu::may_alias]]
#endif
			= char const*;
			return my_getaddrinfo_impl(reinterpret_cast<char_type_may_alias_ptr>(p),nullptr,__builtin_addressof(info));
		}
		else
		{
			return posix_dns_open_alien_char_type_impl(p,::fast_io::details::cal_array_size(t));
		}
	}
	else
	{
		using cstr_char_type = std::remove_pointer_t<decltype(t.c_str())>;
		if constexpr(sizeof(cstr_char_type)==sizeof(char))
		{
			constexpr posix_addrinfo info{.ai_family=AF_UNSPEC};
			return my_getaddrinfo_impl(reinterpret_cast<char const*>(t.c_str()),nullptr,__builtin_addressof(info));
		}
		else
		{
#if __STDC_HOSTED__==1 && (!defined(_GLIBCXX_HOSTED) || _GLIBCXX_HOSTED==1) && __cpp_lib_ranges >= 201911L
			if constexpr(::std::ranges::contiguous_range<std::remove_cvref_t<T>>)
			{
				return posix_dns_open_alien_char_type_impl(::std::ranges::data(t),::std::ranges::size(t));
			}
			else
#endif
			{
				auto ptr{t.c_str()};
				return posix_dns_open_alien_char_type_impl(ptr,::fast_io::cstr_len(ptr));
			}
		}
	}
}

}

class
#if __has_cpp_attribute(gnu::trivial_abi)
[[gnu::trivial_abi]]
#endif
posix_dns_file:public posix_dns_io_observer
{
public:
	using char_type = typename posix_dns_io_observer::char_type;
	using native_handle_type = typename posix_dns_io_observer::native_handle_type;
	constexpr posix_dns_file() noexcept = default;
	template<typename native_hd>
	requires std::same_as<native_handle_type,std::remove_cvref_t<native_hd>>
	explicit constexpr posix_dns_file(native_hd res1) noexcept: posix_dns_io_observer(res1){}
	explicit constexpr posix_dns_file(decltype(nullptr)) noexcept = delete;
	posix_dns_file(char_type const* node,char_type const* service,posix_addrinfo const* hints):posix_dns_io_observer{details::my_getaddrinfo_impl(node,service,hints)}{}
	template<typename T>
	requires ::fast_io::constructible_to_os_c_str<T>
	explicit posix_dns_file(T const& t):posix_dns_io_observer{::fast_io::details::posix_dns_open_impl(t)}{}
	posix_dns_file(posix_dns_file const&)=delete;
	posix_dns_file& operator=(posix_dns_file const&)=delete;
	constexpr posix_dns_file(posix_dns_file&& other) noexcept: posix_dns_io_observer{other.res}
	{
		other.res=nullptr;
	}
	posix_dns_file& operator=(posix_dns_file&& other) noexcept
	{
		if(__builtin_addressof(other)==this)
			return *this;
		if(this->res)[[likely]]
			details::libc_freeaddrinfo(this->res);
		this->res=other.res;
		other.res=nullptr;
		return *this;
	}
	void close() noexcept
	{
		if(this->res)[[likely]]
		{
			details::libc_freeaddrinfo(this->res);
			this->res=nullptr;
		}
	}
	~posix_dns_file()
	{
		if(this->res)[[likely]]
			details::libc_freeaddrinfo(this->res);
	}
};

inline constexpr posix_dns_iterator begin(posix_dns_io_observer pdniob) noexcept
{
	return {pdniob.res};
}

inline constexpr posix_dns_iterator cbegin(posix_dns_io_observer pdniob) noexcept
{
	return {pdniob.res};
}

inline constexpr posix_dns_iterator end(posix_dns_io_observer) noexcept
{
	return {};
}

inline constexpr posix_dns_iterator cend(posix_dns_io_observer) noexcept
{
	return {};
}

inline posix_dns_file posix_addrinfo_serivce(char const* node,char const* service)
{
	constexpr posix_addrinfo info{.ai_family=AF_UNSPEC};
	return posix_dns_file(node,service,__builtin_addressof(info));
}

inline posix_dns_file native_addrinfo_serivce(char const* node,char const* service)
{
	return posix_addrinfo_serivce(node,service);
}

inline posix_file_factory addrinfo_service_connect(posix_dns_io_observer dniob,open_mode mode={})
{
	auto& info{*dniob.res};
	posix_file pf(details::sys_socket(info.ai_family,info.ai_socktype|to_native_sock_open_mode(mode),info.ai_protocol));
	posix_connect(pf,info.ai_addr,info.ai_addrlen);
	return posix_file_factory(pf.release());
}

using native_addrinfo = posix_addrinfo;
using native_dns_io_observer = posix_dns_io_observer;
using native_dns_file = posix_dns_file;
using native_dns_iterator = posix_dns_iterator;

}
