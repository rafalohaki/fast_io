#pragma once

namespace fast_io
{

class gai_error:public fast_io_error
{
public:
	int ec{};
	explicit gai_error(int errn):ec(errn){}
	constexpr auto code() const noexcept
	{
		return ec;
	}
#if __cpp_constexpr >= 201907L
	//constexpr
#endif
	void report(error_reporter& report) const override
	{
		print_freestanding(report,chvw(gai_strerror(ec)));
	}
};

inline void throw_gai_error(int err)
{
#ifdef __cpp_exceptions
	throw gai_error(err);
#else
	fast_terminate();
#endif
}

}

namespace fast_io::sock::details
{

template<typename Func,typename ...Args>
inline auto call_posix(Func&& func,Args&& ...args)
{
	auto ret(func(std::forward<Args>(args)...));
	if(ret==-1)
		throw_posix_error();
	return ret;
}

template<typename ...Args>
inline auto socket(Args&& ...args)
{
	return call_posix(::socket,std::forward<Args>(args)...);
}

template<typename T>
inline auto accept(int sck,T& sock_address,socklen_t& storage_size)
{
	return call_posix(::accept,sck,reinterpret_cast<sockaddr*>(std::addressof(sock_address)),std::addressof(storage_size));
}

template<typename T,std::unsigned_integral sock_type_size>
inline auto connect(int sck,T& sock_address,sock_type_size size)
{
	return call_posix(::connect,sck,reinterpret_cast<sockaddr*>(std::addressof(sock_address)),size);
}

template<typename ...Args>
inline auto send(Args&& ...args)
{
	return call_posix(::send,std::forward<Args>(args)...);
}
template<typename ...Args>
inline auto recv(Args&& ...args)
{
	return call_posix(::recv,std::forward<Args>(args)...);
}


template<typename ...Args>
inline auto sendmsg(Args&& ...args)
{
	return call_posix(::sendmsg,std::forward<Args>(args)...);
}
template<typename ...Args>
inline auto recvmsg(Args&& ...args)
{
	return call_posix(::recvmsg,std::forward<Args>(args)...);
}

template<typename ...Args>
inline auto closesocket(Args&& ...args)
{
	return fast_io::details::sys_close_throw_error(std::forward<Args>(args)...);
}

template<typename ...Args>
inline void closesocket_ignore_error(Args&& ...args)
{
	fast_io::details::sys_close(std::forward<Args>(args)...);
}

template<typename T,std::unsigned_integral sock_type_size>
inline auto bind(int sck,T& sock_address,sock_type_size size)
{
	return call_posix(::bind,sck,reinterpret_cast<sockaddr*>(std::addressof(sock_address)),size);
}

template<typename ...Args>
inline auto bind(Args&& ...args)
{
	return call_posix(::bind,std::forward<Args>(args)...);
}

template<typename ...Args>
inline auto listen(Args&& ...args)
{
	return call_posix(::listen,std::forward<Args>(args)...);
}

template<typename ...Args>
inline void getaddrinfo(Args&& ...args)
{
	int ec(::getaddrinfo(std::forward<Args>(args)...));
	if(ec)
		fast_io::throw_gai_error(ec);
}

template<typename ...Args>
inline void freeaddrinfo(Args&& ...args)
{
	::freeaddrinfo(std::forward<Args>(args)...);
}

template<typename ...Args>
inline auto shutdown(Args&& ...args)
{
	return call_posix(::shutdown,std::forward<Args>(args)...);
}

using address_family = sa_family_t;
using socket_type = int;
inline constexpr auto invalid_socket(-1);

}