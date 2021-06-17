#pragma once

namespace fast_io
{

template<std::integral ch_type>
class basic_posix_socket_io_observer
{
public:
	using char_type = ch_type;
	using native_handle_type = int;
	int fd{-1};
	explicit constexpr operator bool() noexcept
	{
		return fd!=-1;
	}
	constexpr int const& native_handle() const noexcept
	{
		return fd;
	}
	constexpr int& native_handle() noexcept
	{
		return fd;
	}
	constexpr int release() noexcept
	{
		auto t{fd};
		fd=-1;
		return t;
	}
	explicit constexpr operator basic_posix_io_observer<char_type>() noexcept
	{
		return basic_posix_io_observer<char_type>{fd};
	}
};

template<std::integral char_type>
inline constexpr basic_posix_socket_io_observer<char_type> io_value_handle(basic_posix_socket_io_observer<char_type> value) noexcept
{
	return value;
}

#if defined(__linux__)
template<std::integral ch_type>
inline constexpr int zero_copy_in_handle(basic_posix_socket_io_observer<ch_type> soc) noexcept
{
	return soc.fd;
}
template<std::integral ch_type>
inline constexpr int zero_copy_out_handle(basic_posix_socket_io_observer<ch_type> soc) noexcept
{
	return soc.fd;
}
#endif

namespace details
{

inline std::size_t posix_socket_write_impl(int fd, void const* data,std::size_t to_write)
{
#if defined(__linux__) && defined(__NR_send)
	std::ptrdiff_t written{system_call<__NR_send,std::ptrdiff_t>(fd,data,to_write)};
	system_call_throw_error(written);
	return static_cast<std::size_t>(written);
#else
	std::ptrdiff_t written{send(fd,data,to_write,0)};
	if(written<0)
		throw_posix_error();
	return static_cast<std::size_t>(written);
#endif
}

inline std::size_t posix_socket_read_impl(int fd, void* data,std::size_t to_write)
{
#if defined(__linux__) && defined(__NR_recv)
	std::ptrdiff_t written{system_call<__NR_recv,std::ptrdiff_t>(fd,data,to_write)};
	system_call_throw_error(written);
	return static_cast<std::size_t>(written);
#else
	std::ptrdiff_t written{recv(fd,data,to_write,0)};
	if(written<0)
		throw_posix_error();
	return static_cast<std::size_t>(written);
#endif
}

inline void posix_connect_posix_socket_real_impl(int fd,void const* addr,socklen_t addrlen)
{
#if defined(__linux__) && defined(__NR_connect)
	system_call_throw_error(system_call<__NR_connect,int>(fd,addr,addrlen));
#else
	using sockaddr_alias_const_ptr
#if __has_cpp_attribute(gnu::may_alias)
	[[gnu::may_alias]]
#endif
	= struct sockaddr const*;
	if(::connect(fd,reinterpret_cast<sockaddr_alias_const_ptr>(addr),addrlen)==-1)
		throw_posix_error();
#endif
}

inline void posix_connect_posix_socket_impl(int fd,void const* addr,std::size_t addrlen)
{
	if constexpr(sizeof(socklen_t)<sizeof(std::size_t)||(sizeof(socklen_t)==sizeof(std::size_t)&&std::signed_integral<socklen_t>))
	{
		if(static_cast<std::size_t>(std::numeric_limits<socklen_t>::max())<addrlen)
			throw_posix_error(EINVAL);
		posix_connect_posix_socket_real_impl(fd,addr,static_cast<socklen_t>(static_cast<std::make_unsigned_t<socklen_t>>(addrlen)));
	}
	else
	{
		posix_connect_posix_socket_real_impl(fd,addr,static_cast<socklen_t>(static_cast<std::make_unsigned_t<socklen_t>>(addrlen)));
	}
}

}

template<std::integral ch_type,::fast_io::freestanding::contiguous_iterator Iter>
[[nodiscard]]
inline Iter read(basic_posix_socket_io_observer<ch_type> h,Iter begin,Iter end)
{
	return begin+details::posix_socket_read_impl(h.fd,::fast_io::freestanding::to_address(begin),(end-begin)*sizeof(*begin))/sizeof(*begin);
}
template<std::integral ch_type,::fast_io::freestanding::contiguous_iterator Iter>
inline Iter write(basic_posix_socket_io_observer<ch_type> h,Iter cbegin,Iter cend)
{
	return cbegin+details::posix_socket_write_impl(h.fd,::fast_io::freestanding::to_address(cbegin),(cend-cbegin)*sizeof(*cbegin))/sizeof(*cbegin);
}

template<std::integral ch_type>
[[nodiscard]]
inline io_scatter_status_t scatter_read(basic_posix_socket_io_observer<ch_type> h,io_scatters_t sp)
{
	return details::posix_scatter_read_impl(h.fd,sp);
}

template<std::integral ch_type>
inline io_scatter_status_t scatter_write(basic_posix_socket_io_observer<ch_type> h,io_scatters_t sp)
{
	return details::posix_scatter_write_impl(h.fd,sp);
}

template<std::integral ch_type>
inline constexpr void posix_connect(basic_posix_socket_io_observer<ch_type> h,void const* addr,std::size_t addrlen)
{
	details::posix_connect_posix_socket_impl(h.fd,addr,addrlen);
}

template<std::integral ch_type>
class basic_posix_socket_io_handle: public basic_posix_socket_io_observer<ch_type>
{
public:
	using char_type = ch_type;
	using native_handle_type = int;
	constexpr basic_posix_socket_io_handle() noexcept = default;
	template<typename native_hd>
	requires std::same_as<native_handle_type,std::remove_cvref_t<native_hd>>
	explicit constexpr basic_posix_socket_io_handle(native_hd fd) noexcept: basic_posix_socket_io_observer<char_type>{fd}
	{}
	basic_posix_socket_io_handle(basic_posix_socket_io_handle const& dp):basic_posix_socket_io_observer<char_type>{details::sys_dup(dp.fd)}
	{
	}
	basic_posix_socket_io_handle& operator=(basic_posix_socket_io_handle const& dp)
	{
		this->fd=details::sys_dup2(dp.fd,this->fd);
		return *this;
	}
	constexpr basic_posix_socket_io_handle(basic_posix_socket_io_handle&& b) noexcept : basic_posix_socket_io_observer<char_type>{b.fd}
	{
		b.fd = -1;
	}
	basic_posix_socket_io_handle& operator=(basic_posix_socket_io_handle&& b) noexcept
	{
		if(__builtin_addressof(b)!=__builtin_addressof(b))
		{
			if(this->fd!=-1)[[likely]]
				details::sys_close(this->fd);
			this->fd=b.fd;
			b.fd = -1;
		}
		return *this;
	}
	inline constexpr void reset(native_handle_type newfd=-1) noexcept
	{
		if(this->fd!=-1)[[likely]]
			details::sys_close(this->fd);
		this->fd=newfd;
	}
	void close()
	{
		if(this->fd!=-1)[[likely]]
		{
			details::sys_close_throw_error(this->fd);
			this->fd=-1;
		}
	}
};

inline constexpr int to_posix_sock_family(sock_family dom) noexcept
{
switch(dom)
{
#ifdef AF_ALG
case sock_family::alg:
	return AF_ALG;
#endif
#ifdef AF_APPLETALK
case sock_family::appletalk:
	return AF_APPLETALK;
#endif
#ifdef AF_ARP
case sock_family::arp:
	return AF_ARP;
#endif
#ifdef AF_ASH
case sock_family::ash:
	return AF_ASH;
#endif
#ifdef AF_ATM
case sock_family::atm:
	return AF_ATM;
#endif
#ifdef AF_ATM
case sock_family::atm:
	return AF_ATM;
#endif
#ifdef AF_ATMPVC
case sock_family::atmpvc:
	return AF_ATMPVC;
#endif
#ifdef AF_ATMSVC
case sock_family::atmsvc:
	return AF_ATMSVC;
#endif
#ifdef AF_AX25
case sock_family::ax25:
	return AF_AX25;
#endif
#ifdef AF_BAN
case sock_family::ban:
	return AF_BAN;
#endif
#ifdef AF_BLUETOOTH
case sock_family::bluetooth:
	return AF_BLUETOOTH;
#endif
#ifdef AF_BRIDGE
case sock_family::bridge:
	return AF_BRIDGE;
#endif
#ifdef AF_CAIF
case sock_family::caif:
	return AF_CAIF;
#endif
#ifdef AF_CAN
case sock_family::can:
	return AF_CAN;
#endif
#ifdef AF_CCITT
case sock_family::ccitt:
	return AF_CCITT;
#endif
#ifdef AF_CHAOS
case sock_family::chaos:
	return AF_CHAOS;
#endif
#ifdef AF_CLUSTER
case sock_family::cluster:
	return AF_CLUSTER;
#endif
#ifdef AF_CNT
case sock_family::cnt:
	return AF_CNT;
#endif
#ifdef AF_COIP
case sock_family::coip:
	return AF_COIP;
#endif
#ifdef AF_DATAKIT
case sock_family::datakit:
	return AF_DATAKIT;
#endif
#ifdef AF_DECnet
case sock_family::decnet:
	return AF_DECnet;
#endif
#ifdef AF_DII
case sock_family::dii:
	return AF_DII;
#endif
#ifdef AF_ECMA
case sock_family::ecma:
	return AF_ECMA;
#endif
#ifdef AF_ECONET
case sock_family::econet:
	return AF_ECONET;
#endif
#ifdef AF_FIREFOX
case sock_family::firefox:
	return AF_FIREFOX;
#endif
#ifdef AF_HYLINK
case sock_family::hylink:
	return AF_HYLINK;
#endif
#ifdef AF_HYPERV
case sock_family::hyperv:
	return AF_HYPERV;
#endif
#ifdef AF_IB
case sock_family::ib:
	return AF_IB;
#endif
#ifdef AF_IEEE12844
case sock_family::ieee12844:
	return AF_IEEE12844;
#endif
#ifdef AF_IEEE80211
case sock_family::ieee80211:
	return AF_IEEE80211;
#endif
#ifdef AF_IEEE802154
case sock_family::ieee802154:
	return AF_IEEE802154;
#endif
#ifdef AF_IMPLINK
case sock_family::implink:
	return AF_IMPLINK;
#endif
#ifdef AF_INET
case sock_family::inet:
	return AF_INET;
#endif
#ifdef AF_INET6
case sock_family::inet6:
	return AF_INET6;
#endif
#ifdef AF_INET6_SDP
case sock_family::inet6_sdp:
	return AF_INET6_sdp;
#endif
#ifdef AF_INET_SDP
case sock_family::inet_sdp:
	return AF_INET_SDP;
#endif
#ifdef AF_IPX
case sock_family::ipx:
	return AF_IPX;
#endif
#ifdef AF_IRDA
case sock_family::irda:
	return AF_IRDA;
#endif
#ifdef AF_ISDN
case sock_family::isdn:
	return AF_ISDN;
#endif
#ifdef AF_ISO
case sock_family::iso:
	return AF_ISO;
#endif
#ifdef AF_IUCV
case sock_family::iucv:
	return AF_IUCV;
#endif
#ifdef AF_KCM
case sock_family::kcm:
	return AF_KCM;
#endif
#ifdef AF_KEY
case sock_family::key:
	return AF_KEY;
#endif
#ifdef AF_LAT
case sock_family::lat:
	return AF_LAT;
#endif
#ifdef AF_LINK
case sock_family::link:
	return AF_LINK;
#endif
#ifdef AF_LLC
case sock_family::llc:
	return AF_LLC;
#endif
#ifdef AF_LOCAL
case sock_family::local:
	return AF_LOCAL;
#endif
#ifdef AF_MPLS
case sock_family::mpls:
	return AF_MPLS;
#endif
#ifdef AF_NATM
case sock_family::natm:
	return AF_NATM;
#endif
#ifdef AF_NETBEUI
case sock_family::netbeui:
	return AF_NETBEUI;
#endif
#ifdef AF_NETBIOS
case sock_family::netbios:
	return AF_NETBIOS;
#endif
#ifdef AF_NETGRAPH
case sock_family::netgraph:
	return AF_NETGRAPH;
#endif
#ifdef AF_NETROM
case sock_family::netrom:
	return AF_NETROM;
#endif
#ifdef AF_NETDES
case sock_family::netdes:
	return AF_NETDES;
#endif
#ifdef AF_NFC
case sock_family::nfc:
	return AF_NFC;
#endif
#ifdef AF_PACKET
case sock_family::packet:
	return AF_PACKET;
#endif
#ifdef AF_PHONET
case sock_family::phonet:
	return AF_PHONET;
#endif
#ifdef AF_PPPOX
case sock_family::pppox:
	return AF_PPPOX;
#endif
#ifdef AF_pseudo_HDRCMPLT
case sock_family::pseudo_hdrcmplt:
	return AF_pseudo_HDRCMPLT;
#endif
#ifdef AF_pseudo_KEY
case sock_family::pseudo_key:
	return AF_pseudo_KEY;
#endif
#ifdef AF_pseudo_PIP
case sock_family::pseudo_pip:
	return AF_pseudo_PIP;
#endif
#ifdef AF_pseudo_RTIP
case sock_family::pseudo_rtip:
	return AF_pseudo_RTIP;
#endif
#ifdef AF_pseudo_XTP
case sock_family::pseudo_xtp:
	return AF_pseudo_XTP;
#endif
#ifdef AF_pseudo_PUP
case sock_family::pseudo_pup:
	return AF_pseudo_PUP;
#endif
#ifdef AF_QIPCRTR
case sock_family::qipcrtr:
	return AF_QIPCRTR;
#endif
#ifdef AF_RDS
case sock_family::rds:
	return AF_RDS;
#endif
#ifdef AF_ROSE
case sock_family::rose:
	return AF_ROSE;
#endif
#ifdef AF_ROUTE
case sock_family::route:
	return AF_ROUTE;
#endif
#ifdef AF_RXRPC
case sock_family::rxrpc:
	return AF_RXRPC;
#endif
#ifdef AF_SCLUSTER
case sock_family::scluster:
	return AF_SCLUSTER;
#endif
#ifdef AF_SECURITY
case sock_family::security:
	return AF_SECURITY;
#endif
#ifdef AF_SIP
case sock_family::sip:
	return AF_SIP;
#endif
#ifdef AF_SLOW
case sock_family::slow:
	return AF_SLOW;
#endif
#ifdef AF_SMC
case sock_family::smc:
	return AF_SMC;
#endif
#ifdef AF_SNA
case sock_family::sna:
	return AF_SNA;
#endif
#ifdef AF_TCNPROCESS
case sock_family::tcnprocess:
	return AF_TCNPROCESS;
#endif
#ifdef AF_TCNMESSAGE
case sock_family::tcnmessage:
	return AF_TCNMESSAGE;
#endif
#ifdef AF_TIPC
case sock_family::tipc:
	return AF_TIPC;
#endif
#ifdef AF_VOICEVIEW
case sock_family::voiceview:
	return AF_VOICEVIEW;
#endif
#ifdef AF_VSOCK
case sock_family::vsock:
	return AF_VSOCK;
#endif
#ifdef AF_WANPIPE
case sock_family::wanpipe:
	return AF_WANPIPE;
#endif
#ifdef AF_X25
case sock_family::x25:
	return AF_X25;
#endif
#ifdef AF_XDP
case sock_family::xdp:
	return AF_XDP;
#endif
default:
#ifdef AF_UNSPEC
	return AF_UNSPEC;
#else
	return -1;
#endif
}
}

inline constexpr int to_native_sock_family(sock_family dom) noexcept
{
	return to_posix_sock_family(dom);
}

inline constexpr int to_posix_sock_protocal(sock_protocal dom) noexcept
{
switch(dom)
{
#ifdef IPPROTO_AH
case sock_protocal::ah:
	return IPPROTO_AH;
#endif
#ifdef IPPROTO_BEETPH
case sock_protocal::beetph:
	return IPPROTO_BEETPH;
#endif
#ifdef IPPROTO_CARP
case sock_protocal::carp:
	return IPPROTO_CARP;
#endif
#ifdef IPPROTO_CBT
case sock_protocal::cbt:
	return IPPROTO_CBT;
#endif
#ifdef IPPROTO_COMP
case sock_protocal::comp:
	return IPPROTO_COMP;
#endif
#ifdef IPPROTO_DCCP
case sock_protocal::dccp:
	return IPPROTO_DCCP;
#endif
#ifdef IPPROTO_DSTOPTS
case sock_protocal::dstopts:
	return IPPROTO_DSTOPTS;
#endif
#ifdef IPPROTO_EGP
case sock_protocal::egp:
	return IPPROTO_EGP;
#endif
#ifdef IPPROTO_EIGRP
case sock_protocal::eigrp:
	return IPPROTO_EIGRP;
#endif
#ifdef IPPROTO_ENCAP
case sock_protocal::encap:
	return IPPROTO_ENCAP;
#endif
#ifdef IPPROTO_ESP
case sock_protocal::esp:
	return IPPROTO_ESP;
#endif
#ifdef IPPROTO_ETHERNET
case sock_protocal::ethernet:
	return IPPROTO_ETHERNET;
#endif
#ifdef IPPROTO_FRAGMENT
case sock_protocal::fragment:
	return IPPROTO_FRAGMENT;
#endif
#ifdef IPPROTO_GGP
case sock_protocal::ggp:
	return IPPROTO_GGP;
#endif
#ifdef IPPROTO_GRE
case sock_protocal::gre:
	return IPPROTO_GRE;
#endif
#ifdef IPPROTO_HOPOPTS
case sock_protocal::hopopts:
	return IPPROTO_HOPOPTS;
#endif
#ifdef IPPROTO_ICLFXBM
case sock_protocal::iclfxbm:
	return IPPROTO_ICLFXBM;
#endif
#ifdef IPPROTO_ICMP
case sock_protocal::icmp:
	return IPPROTO_ICMP;
#endif
#ifdef IPPROTO_ICMPV6
case sock_protocal::icmpv6:
	return IPPROTO_ICMPV6;
#endif
#ifdef IPPROTO_IDP
case sock_protocal::idp:
	return IPPROTO_IDP;
#endif
#ifdef IPPROTO_IGMP
case sock_protocal::igmp:
	return IPPROTO_IGMP;
#endif
#ifdef IPPROTO_IGP
case sock_protocal::igp:
	return IPPROTO_IGP;
#endif
#ifdef IPPROTO_IP
case sock_protocal::ip:
	return IPPROTO_IP;
#endif
#ifdef IPPROTO_IPV4
case sock_protocal::ipv4:
	return IPPROTO_IPV4;
#endif
#ifdef IPPROTO_IPV6
case sock_protocal::ipv6:
	return IPPROTO_IPV6;
#endif
#ifdef IPPROTO_L2TP
case sock_protocal::l2tp:
	return IPPROTO_L2TP;
#endif
#ifdef IPPROTO_MOBILE
case sock_protocal::mobile:
	return IPPROTO_MOBILE;
#endif
#ifdef IPPROTO_MOBILITY
case sock_protocal::mobility:
	return IPPROTO_MOBILITY;
#endif
#ifdef IPPROTO_MOBILITY_OLD
case sock_protocal::mobility_old:
	return IPPROTO_MOBILITY_OLD;
#endif
#ifdef IPPROTO_MPLS
case sock_protocal::mpls:
	return IPPROTO_MPLS;
#endif
#ifdef IPPROTO_MPTCP
case sock_protocal::mptcp:
	return IPPROTO_MPTCP;
#endif
#ifdef IPPROTO_MTP
case sock_protocal::mtp:
	return IPPROTO_MTP;
#endif
#ifdef IPPROTO_ND
case sock_protocal::nd:
	return IPPROTO_ND;
#endif
#ifdef IPPROTO_NONE
case sock_protocal::nonext:
	return IPPROTO_NONE;
#endif
#ifdef IPPROTO_OSPF
case sock_protocal::ospf:
	return IPPROTO_OSPF;
#endif
#ifdef IPPROTO_PGM
case sock_protocal::pgm:
	return IPPROTO_PGM;
#endif
#ifdef IPPROTO_PIGP
case sock_protocal::pigp:
	return IPPROTO_PIGP;
#endif
#ifdef IPPROTO_PIM
case sock_protocal::pim:
	return IPPROTO_PIM;
#endif
#ifdef IPPROTO_PUP
case sock_protocal::pup:
	return IPPROTO_PUP;
#endif
#ifdef IPPROTO_RAW
case sock_protocal::raw:
	return IPPROTO_RAW;
#endif
#ifdef IPPROTO_RDP
case sock_protocal::rdp:
	return IPPROTO_RDP;
#endif
#ifdef IPPROTO_ROUTING
case sock_protocal::routing:
	return IPPROTO_ROUTING;
#endif
#ifdef IPPROTO_RSVP
case sock_protocal::rsvp:
	return IPPROTO_RSVP;
#endif
#ifdef IPPROTO_SCTP
case sock_protocal::sctp:
	return IPPROTO_SCTP;
#endif
#ifdef IPPROTO_ST
case sock_protocal::st:
	return IPPROTO_ST;
#endif
#ifdef IPPROTO_TCP
case sock_protocal::tcp:
	return IPPROTO_TCP;
#endif
#ifdef IPPROTO_TP
case sock_protocal::tp:
	return IPPROTO_TP;
#endif
#ifdef IPPROTO_UDP
case sock_protocal::udp:
	return IPPROTO_UDP;
#endif
#ifdef IPPROTO_UDPLITE
case sock_protocal::udplite:
	return IPPROTO_UDPLITE;
#endif
#ifdef IPPROTO_VRRP
case sock_protocal::vrrp:
	return IPPROTO_VRRP;
#endif
default:
	return -1;
}
}

inline constexpr int to_native_sock_protocal(sock_protocal prot) noexcept
{
	return to_posix_sock_protocal(prot);
}

inline constexpr int to_posix_sock_type(sock_type soc) noexcept
{
switch(soc)
{
#ifdef SOCK_STREAM
case sock_type::stream:
	return SOCK_STREAM;
#endif
#ifdef SOCK_DGRAM
case sock_type::dgram:
	return SOCK_DGRAM;
#endif
#ifdef SOCK_SEQPACKET
case sock_type::seqpacket:
	return SOCK_SEQPACKET;
#endif
#ifdef SOCK_RAW
case sock_type::raw:
	return SOCK_RAW;
#endif
#ifdef SOCK_RDM
case sock_type::rdm:
	return SOCK_RDM;
#endif
#ifdef SOCK_PACKET
case sock_type::packet:
	return SOCK_PACKET;
#endif
default:
	return -1;
};
}

inline constexpr int to_native_sock_type(sock_type soc) noexcept
{
	return to_posix_sock_type(soc);
}

inline constexpr int to_posix_sock_open_mode(open_mode om) noexcept
{
	int ret{
#ifdef SOCK_CLOEXEC
	SOCK_CLOEXEC
#endif
	};
	if((om&open_mode::inherit)==open_mode::inherit)
		ret=0;
	if((om&open_mode::no_block)==open_mode::no_block)
#ifdef SOCK_NONBLOCK
		ret|=SOCK_NONBLOCK;
#else
		ret=-1;
#endif
	return ret;
}

inline constexpr int to_native_sock_open_mode(open_mode om) noexcept
{
	return to_posix_sock_open_mode(om);
}

namespace details
{

inline int open_socket_impl(sock_family d,sock_type t,open_mode m,sock_protocal p)
{
#if defined(__linux__) && defined(__NR_socket)
	int fd{system_call<__NR_socket,int>(to_posix_sock_family(d),to_posix_sock_type(t)|to_posix_sock_open_mode(m),to_posix_sock_protocal(p))};
	system_call_throw_error(fd);
	return fd;
#else
	int fd{socket(to_posix_sock_family(d),to_posix_sock_type(t)|to_posix_sock_open_mode(m),to_posix_sock_protocal(p))};
	if(fd==-1)
		throw_posix_error();
	return fd;
#endif
}

}

struct
#if __has_cpp_attribute(gnu::trivial_abi)
[[gnu::trivial_abi]]
#endif
posix_socket_factory
{
	using native_handle_type = int;
	int fd{-1};
	constexpr posix_socket_factory(int v) noexcept:fd(v){};
	posix_socket_factory(posix_socket_factory const&)=delete;
	posix_socket_factory& operator=(posix_socket_factory const&)=delete;
	~posix_socket_factory()
	{
		if(fd!=-1)[[likely]]
			details::sys_close(fd);
	}
};

template<std::integral ch_type>
class basic_posix_socket_file: public basic_posix_socket_io_handle<ch_type>
{
public:
	using char_type = ch_type;
	using native_handle_type = int;
	constexpr basic_posix_socket_file() noexcept = default;
	template<typename native_hd>
	requires std::same_as<native_handle_type,std::remove_cvref_t<native_hd>>
	explicit constexpr basic_posix_socket_file(native_hd fd) noexcept: basic_posix_socket_io_handle<char_type>{fd}{}

	basic_posix_socket_file(sock_family d,sock_type t,open_mode m,sock_protocal p)
		:basic_posix_socket_io_handle<ch_type>{details::open_socket_impl(d,t,m,p)}
	{}

	basic_posix_socket_file(io_dup_t,basic_posix_socket_io_observer<ch_type> piob):basic_posix_socket_io_handle<char_type>{details::sys_dup(piob.fd)}
	{}
	basic_posix_socket_file(basic_posix_socket_file const&)=default;
	basic_posix_socket_file& operator=(basic_posix_socket_file const&)=default;
	constexpr basic_posix_socket_file(basic_posix_socket_file &&) noexcept=default;
	basic_posix_socket_file& operator=(basic_posix_socket_file &&) noexcept=default;
	void open(sock_family d,sock_type t,open_mode m,sock_protocal p)
	{
		if(this->fd!=-1)
			return;
		this->fd = details::open_socket_impl(d,t,m,p);
	}
	void reopen(sock_family d,sock_type t,open_mode m,sock_protocal p)
	{
		auto newtemp{details::open_socket_impl(d,t,m,p)};
		if(this->fd==-1)
			details::sys_close(this->fd);
		this->fd=newtemp;
	}
	explicit constexpr basic_posix_socket_file(posix_socket_factory&& other) noexcept : basic_posix_socket_io_handle<char_type>{other.fd}
	{
		other.fd=-1;
	}
	~basic_posix_socket_file()
	{
		if(this->fd!=-1)[[likely]]
			details::sys_close(this->fd);
	}
};

namespace details
{

inline int posix_tcp_connect_v4_impl(ipv4 v4,open_mode m)
{
	basic_posix_socket_file<char> soc(sock_family::inet,sock_type::stream,m,sock_protocal::ip);
	constexpr auto inet{to_posix_sock_family(sock_family::inet)};
	posix_sockaddr_in in{.sin_family=inet,.sin_port=big_endian(static_cast<std::uint16_t>(v4.port)),.sin_addr=v4.address};
	posix_connect(soc,__builtin_addressof(in),sizeof(in));
	return soc.release();
}

inline int posix_tcp_connect_v6_impl(ipv6 v6,open_mode m)
{
	basic_posix_socket_file<char> soc(sock_family::inet6,sock_type::stream,m,sock_protocal::ip);
	constexpr auto inet6{to_posix_sock_family(sock_family::inet6)};
	posix_sockaddr_in6 in6{.sin6_family=inet6,.sin6_port=big_endian(static_cast<std::uint16_t>(v6.port)),.sin6_addr=v6.address};
	posix_connect(soc,__builtin_addressof(in6),sizeof(in6));
	return soc.release();
}

inline int posix_tcp_connect_ip_impl(ip v,open_mode m)
{
	basic_posix_socket_file<char> soc(v.isv4?sock_family::inet:sock_family::inet6,sock_type::stream,m,sock_protocal::ip);
	if(v.isv4)
	{
		constexpr auto inet{to_posix_sock_family(sock_family::inet)};
		posix_sockaddr_in in{.sin_family=inet,.sin_port=big_endian(v.address.v4.port),.sin_addr=v.address.v4.address};
		posix_connect(soc,__builtin_addressof(in),sizeof(in));
	}
	else
	{
		constexpr auto inet6{to_posix_sock_family(sock_family::inet6)};
		posix_sockaddr_in6 in6{.sin6_family=inet6,.sin6_port=big_endian(v.address.v6.port),.sin6_addr=v.address.v6.address};
		posix_connect(soc,__builtin_addressof(in6),sizeof(in6));
	}
	return soc.release();
}

}

inline posix_socket_factory posix_tcp_connect(ipv4 v4,open_mode m=open_mode{})
{
	return {details::posix_tcp_connect_v4_impl(v4,m)};
}

inline posix_socket_factory posix_tcp_connect(ipv6 v6,open_mode m=open_mode{})
{
	return {details::posix_tcp_connect_v6_impl(v6,m)};
}

inline posix_socket_factory posix_tcp_connect(ip v,open_mode m=open_mode{})
{
	return {details::posix_tcp_connect_ip_impl(v,m)};
}

using posix_socket_io_observer=basic_posix_socket_io_observer<char>;
using wposix_socket_io_observer=basic_posix_socket_io_observer<wchar_t>;
using u8posix_socket_io_observer=basic_posix_socket_io_observer<char8_t>;
using u16posix_socket_io_observer=basic_posix_socket_io_observer<char16_t>;
using u32posix_socket_io_observer=basic_posix_socket_io_observer<char32_t>;

using posix_socket_file=basic_posix_socket_file<char>;
using wposix_socket_file=basic_posix_socket_file<wchar_t>;
using u8posix_socket_file=basic_posix_socket_file<char8_t>;
using u16posix_socket_file=basic_posix_socket_file<char16_t>;
using u32posix_socket_file=basic_posix_socket_file<char32_t>;

using posix_socket_io_handle=basic_posix_socket_io_handle<char>;
using wposix_socket_io_handle=basic_posix_socket_io_handle<wchar_t>;
using u8posix_socket_io_handle=basic_posix_socket_io_handle<char8_t>;
using u16posix_socket_io_handle=basic_posix_socket_io_handle<char16_t>;
using u32posix_socket_io_handle=basic_posix_socket_io_handle<char32_t>;


template<std::integral ch_type>
using basic_native_socket_io_observer = basic_posix_socket_io_observer<ch_type>;
template<std::integral ch_type>
using basic_native_socket_io_handle = basic_posix_socket_io_handle<ch_type>;
template<std::integral ch_type>
using basic_native_socket_file = basic_posix_socket_file<ch_type>;
using native_socket_factory = posix_socket_factory;

inline posix_socket_factory tcp_connect(ipv4 v4,open_mode m=open_mode{})
{
	return {details::posix_tcp_connect_v4_impl(v4,m)};
}
inline posix_socket_factory tcp_connect(ipv6 v6,open_mode m=open_mode{})
{
	return {details::posix_tcp_connect_v6_impl(v6,m)};
}
inline posix_socket_factory tcp_connect(ip v,open_mode m=open_mode{})
{
	return {details::posix_tcp_connect_ip_impl(v,m)};
}

}
