#pragma once

namespace fast_io
{

struct posix_addrinfo
{
int              ai_flags{};
int              ai_family{};
int              ai_socktype{};
int              ai_protocol{};
posix_socklen_t  ai_addrlen{};
posix_sockaddr  *ai_addr{};
char            *ai_canonname{};
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
	using native_handle_type = posix_addrinfo*;
	posix_addrinfo* res{};
	inline constexpr native_handle_type& native_handle() noexcept
	{
		return res;
	}
	inline constexpr native_handle_type const& native_handle() const noexcept
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
	posix_addrinfo* res{};
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
	return posix_dns_io_observer{d.res};
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

}

class posix_dns_file:public posix_dns_io_observer
{
public:
	constexpr posix_dns_file() noexcept = default;
	template<typename native_hd>
	requires std::same_as<native_handle_type,std::remove_cvref_t<native_hd>>
	explicit constexpr posix_dns_file(native_hd res1) noexcept: posix_dns_io_observer(res1){}
	posix_dns_file(char const* node,char const* service,posix_addrinfo const* hints):posix_dns_io_observer{details::my_getaddrinfo_impl(node,service,hints)}{}
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


inline posix_dns_file posix_dns_once(char const* node,char const* service)	//place holder
{
	constexpr posix_addrinfo info{.ai_family=AF_UNSPEC};
	return posix_dns_file(node,service,__builtin_addressof(info));
}

inline posix_dns_file dns_once(char const* node,char const* service)	//place holder
{
	return posix_dns_once(node,service);
}

inline posix_file_factory connect(posix_dns_io_observer dniob,open_mode mode={})
{
	auto& info{*dniob.res};
	posix_file pf(details::sys_socket(info.ai_family,info.ai_socktype|to_native_sock_open_mode(mode),info.ai_protocol));
	posix_connect(pf,info.ai_addr,info.ai_addrlen);
	return posix_file_factory(pf.release());
}


}
