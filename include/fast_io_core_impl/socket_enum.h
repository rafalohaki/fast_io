//https://github.com/bminor/glibc/blob/5f72f9800b250410cad3abfeeb09469ef12b2438/sysdeps/unix/sysv/linux/bits/socket.h

namespace fast_io
{
#undef unix

enum class protocol_family: std::uint32_t
{
	unspec = 0,
	local = 1,
	unix = 1,
	file = 1,
	inet = 2,
	ax25 = 3,
	ipx = 4,
	appletalk = 5,
	netrom = 6,
	bridge = 7,
	atmpvc = 8,
	x25 = 9,
	inet6 = 10,
	rose = 11,
	decnet = 12,
	netbeui = 13,
	security = 14,
	key = 15,
	netlink = 16,
	route = 16,
	packet = 17,
	ash = 18,
	econet = 19,
	atmsvc = 20,
	rds = 21,
	sna = 22,
	irda = 23,
	pppox = 24,
	wanpipe = 25,
	llc = 26,
	ib = 27,
	mpls = 28,
	can = 29,
	tipc = 30,
	bluetooth = 31,
	iucv = 32,
	rxrpc = 33,
	isdn = 34,
	phonet = 35,
	ieee802154 = 36,
	caif = 37,
	alg = 38,
	nfc = 39,
	vsock = 40,
	kcm = 41,
	qipcrtr = 42,
	smc = 43,
	xdp = 44,
	max = 45
};

enum class message_flag: std::uint32_t
{
	oob = 0x01,
	peek = 0x02,
	dontroute = 0x04,
	tryhard = 0x04,
	ctrunc = 0x08,
	proxy = 0x10,
	trunc = 0x20,
	dontwait = 0x40,
	eor = 0x80,
	waitall = 0x100,
	fin = 0x200,
	syn = 0x400,
	confirm = 0x800,
	rst = 0x1000,
	errqueue = 0x2000,
	nosignal = 0x4000,
	more = 0x8000,
	waitforone = 0x10000,
	batch = 0x40000,
	zerocopy = 0x4000000,
	fastopen = 0x20000000,
	cmsg_cloexec = 0x40000000
};

constexpr message_flag operator&(message_flag x, message_flag y) noexcept
{
using utype = typename std::underlying_type<message_flag>::type;
return static_cast<message_flag>(static_cast<utype>(x) & static_cast<utype>(y));
}

constexpr message_flag operator|(message_flag x, message_flag y) noexcept
{
using utype = typename std::underlying_type<message_flag>::type;
return static_cast<message_flag>(static_cast<utype>(x) | static_cast<utype>(y));
}

constexpr message_flag operator^(message_flag x, message_flag y) noexcept
{
using utype = typename std::underlying_type<message_flag>::type;
return static_cast<message_flag>(static_cast<utype>(x) ^ static_cast<utype>(y));
}

constexpr message_flag operator~(message_flag x) noexcept
{
using utype = typename std::underlying_type<message_flag>::type;
return static_cast<message_flag>(~static_cast<utype>(x));
}

inline constexpr message_flag& operator&=(message_flag& x, message_flag y) noexcept{return x=x&y;}

inline constexpr message_flag& operator|=(message_flag& x, message_flag y) noexcept{return x=x|y;}

inline constexpr message_flag& operator^=(message_flag& x, message_flag y) noexcept{return x=x^y;}

constexpr message_flag operator&(message_flag x, message_flag y) noexcept
{
using utype = typename std::underlying_type<message_flag>::type;
return static_cast<message_flag>(static_cast<utype>(x) & static_cast<utype>(y));
}

constexpr message_flag operator|(message_flag x, message_flag y) noexcept
{
using utype = typename std::underlying_type<message_flag>::type;
return static_cast<message_flag>(static_cast<utype>(x) | static_cast<utype>(y));
}

constexpr message_flag operator~(message_flag x) noexcept
{
using utype = typename std::underlying_type<message_flag>::type;
return static_cast<message_flag>(~static_cast<utype>(x));
}

inline constexpr message_flag& operator&=(message_flag& x, message_flag y) noexcept{return x=x&y;}

inline constexpr message_flag& operator|=(message_flag& x, message_flag y) noexcept{return x=x|y;}

}
