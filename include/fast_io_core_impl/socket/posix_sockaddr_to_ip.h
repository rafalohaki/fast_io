#pragma once

namespace fast_io
{

inline constexpr ::fast_io::ip to_ip_with_ai_addr(int ai_family,posix_sockaddr const* ai_addr,std::uint_least16_t port) noexcept
{
	::fast_io::ip ret;
	switch(ai_family)
	{
	case 2:
	{
		::fast_io::details::my_memcpy(__builtin_addressof(ret.address.v4),__builtin_addressof(reinterpret_cast<posix_sockaddr_in const*>(ai_addr)->sin_addr),sizeof(posix_in_addr));
		ret.port = port;
		ret.isv4=true;
		break;
	}
	case 23:
	{
		::fast_io::details::my_memcpy(__builtin_addressof(ret.address.v6),__builtin_addressof(reinterpret_cast<posix_sockaddr_in6 const*>(ai_addr)->sin6_addr),sizeof(posix_in6_addr));
		ret.port = port;
		break;
	}
	}
	return ret;
}

}
