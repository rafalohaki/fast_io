#pragma once

namespace fast_io
{

struct ipv4
{
	posix_in_addr address{};
	std::uint16_t port{};
};

struct ipv6
{
	posix_in6_addr address{};
	std::uint16_t port{};
};

struct ip
{
	union
	{
		ipv4 v4;
		ipv6 v6;
	}address{.v6={}};
	bool isv4{};
	constexpr ip() noexcept = default;
	explicit constexpr ip(ipv4 add):address{.v4=add},isv4{true}{}
	explicit constexpr ip(ipv6 add):address{.v6=add}{}
};

}
