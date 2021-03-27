#pragma once
#include <sys/socket.h>

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
	constexpr auto& native_handle() const noexcept
	{
		return fd;
	}
	constexpr auto& native_handle() noexcept
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

template<std::integral ch_type>
class basic_posix_socket_io_handle: public basic_posix_socket_io_observer<ch_type>
{
public:
	using char_type = ch_type;
	using native_handle_type = int;
	template<typename native_hd>
	requires std::same_as<native_handle_type,std::remove_cvref_t<native_hd>>
	explicit constexpr basic_posix_socket_io_handle(native_hd fd) noexcept: basic_posix_socket_io_observer{fd}
	{}
	basic_posix_socket_io_handle(basic_posix_socket_io_handle const& dp):basic_posix_io_observer<char_type>{details::sys_dup(dp.fd)}
	{
	}
	basic_posix_socket_io_handle& operator=(basic_posix_socket_io_handle const& dp)
	{
		this->fd=details::sys_dup2(dp.fd,this->fd);
		return *this;
	}
	constexpr basic_posix_socket_io_handle(basic_posix_socket_io_handle&& b) noexcept : basic_posix_io_observer<char_type>{b.fd}
	{
		b.fd = -1;
	}
	basic_posix_socket_io_handle& operator=(basic_posix_socket_io_handle&& b) noexcept
	{
		if(b.fd!=this->fd)
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
		if(*this)[[likely]]
		{
			details::sys_close_throw_error(this->fd);
			this->fd=-1;
		}
	}
};

namespace details
{

inline constexpr int native_sock_domain(sock_domain dom)
{
switch(dom)
{
#ifdef AF_UNSPEC
case sock_domain::unspec:
	return AF_UNSPEC;
#endif
#ifdef AF_ALG
case sock_domain::alg:
	return AF_ALG;
#endif
#ifdef AF_APPLETALK
case sock_domain::apple_talk:
	return AF_APPLETALK;
#endif
#ifdef AF_ARP
case sock_domain::arp:
	return AF_ARP;
#endif
#ifdef AF_ASH
case sock_domain::ash:
	return AF_ASH;
#endif
#ifdef AF_ATM
case sock_domain::atm:
	return AF_ATM;
#endif
#ifdef AF_ATM
case sock_domain::atm:
	return AF_ATM;
#endif
#ifdef AF_ATMPVC
case sock_domain::atmpvc:
	return AF_ATMPVC;
#endif
#ifdef AF_ATMSVC
case sock_domain::atmsvc:
	return AF_ATMSVC;
#endif
#ifdef AF_AX25
case sock_domain::ax25:
	return AF_AX25;
#endif
#ifdef AF_BAN
case sock_domain::ban:
	return AF_BAN;
#endif
#ifdef AF_BLUETOOTH
case sock_domain::bluetooth:
	return AF_BLUETOOTH;
#endif
#ifdef AF_BRIDGE
case sock_domain::bridge:
	return AF_BRIDGE;
#endif
#ifdef AF_BRIDGE
case sock_domain::bridge:
	return AF_BRIDGE;
#endif
#ifdef AF_BRIDGE
case sock_domain::bridge:
	return AF_BRIDGE;
#endif
#ifdef AF_CAIF
case sock_domain::caif:
	return AF_CAIF;
#endif
#ifdef AF_CAN
case sock_domain::can:
	return AF_CAN;
#endif
#ifdef AF_CCITT
case sock_domain::ccitt:
	return AF_CCITT;
#endif
#ifdef AF_CHAOS
case sock_domain::chaos:
	return AF_CHAOS;
#endif
#ifdef AF_CLUSTER
case sock_domain::cluster:
	return AF_CLUSTER;
#endif
#ifdef AF_CNT
case sock_domain::cnt:
	return AF_CNT;
#endif
#ifdef AF_COIP
case sock_domain::coip:
	return AF_COIP;
#endif
#ifdef AF_DATAKIT
case sock_domain::datakit:
	return AF_DATAKIT;
#endif
#ifdef AF_DECnet
case sock_domain::decnet:
	return AF_DECnet;
#endif
#ifdef AF_NETBEUI
case sock_domain::netbeui:
	return AF_NETBEUI;
#endif
#ifdef AF_DII
case sock_domain::dii:
	return AF_DII;
#endif
#ifdef AF_ECMA
case sock_domain::ecma:
	return AF_ECMA;
#endif
#ifdef AF_ECONET
case sock_domain::econet:
	return AF_ECONET;
#endif
#ifdef AF_FIREFOX
case sock_domain::firefox:
	return AF_FIREFOX;
#endif
#ifdef AF_HYLINK
case sock_domain::hylink:
	return AF_HYLINK;
#endif
#ifdef AF_HYPERV
case sock_domain::hyperv:
	return AF_HYPERV;
#endif
#ifdef AF_IB
case sock_domain::ib:
	return AF_IB;
#endif
#ifdef AF_IEEE12844
case sock_domain::ieee12844:
	return AF_IEEE12844;
#endif
#ifdef AF_IEEE80211
case sock_domain::ieee80211:
	return AF_IEEE80211;
#endif
#ifdef AF_IEEE802154
case sock_domain::ieee802154:
	return AF_IEEE802154;
#endif
#ifdef AF_IMPLINK
case sock_domain::implink:
	return AF_IMPLINK;
#endif
#ifdef AF_INET
case sock_domain::inet:
	return AF_INET;
#endif
#ifdef AF_INET6
case sock_domain::inet6:
	return AF_INET6;
#endif
#ifdef AF_INET6_SDP
case sock_domain::inet6_sdp:
	return AF_INET6_sdp;
#endif
#ifdef AF_INET_SDP
case sock_domain::inet_sdp:
	return AF_INET_SDP;
#endif
#ifdef AF_IPX
case sock_domain::ipx:
	return AF_IPX;
#endif
#ifdef AF_IRDA
case sock_domain::irda:
	return AF_IRDA;
#endif
#ifdef AF_ISDN
case sock_domain::isdn:
	return AF_ISDN;
#endif
#ifdef AF_ISO
case sock_domain::iso:
	return AF_ISO;
#endif
#ifdef AF_IUCV
case sock_domain::icuv:
	return AF_IUCV;
#endif
#ifdef AF_KCM
case sock_domain::kcm:
	return AF_KCM;
#endif
#ifdef AF_KEY
case sock_domain::key:
	return AF_KEY;
#endif
#ifdef AF_LAT
case sock_domain::lat:
	return AF_LAT;
#endif
#ifdef AF_LINK
case sock_domain::link:
	return AF_LINK;
#endif
#ifdef AF_LLC
case sock_domain::llc:
	return AF_LLC;
#endif
#ifdef AF_LOCAL
case sock_domain::local:
	return AF_LOCAL;
#endif
#ifdef AF_MPLS
case sock_domain::mpls:
	return AF_MPLS;
#endif
#ifdef AF_NATM
case sock_domain::natm:
	return AF_NATM;
#endif
#ifdef AF_NETBEUI
case sock_domain::netbeui:
	return AF_NETBEUI;
#endif
#ifdef AF_NETBIOS
case sock_domain::netbios:
	return AF_NETBIOS;
#endif
#ifdef AF_NETGRAPH
case sock_domain::netgraph:
	return AF_NETGRAPH;
#endif
#ifdef AF_NETROM
case sock_domain::netrom:
	return AF_NETROM;
#endif
#ifdef AF_NETWORK_DESIGNERS
case sock_domain::network_designers:
	return AF_NETWORK_DESIGNERS;
#endif
#ifdef AF_NFC
case sock_domain::nfc:
	return AF_NFC;
#endif
#ifdef AF_PACKET
case sock_domain::packet:
	return AF_PACKET;
#endif
#ifdef AF_PHONET
case sock_domain::phonet:
	return AF_PHONET;
#endif
#ifdef AF_PPPOX
case sock_domain::pppox:
	return AF_PPPOX;
#endif
#ifdef AF_pseudo_HDRCMPLT
case sock_domain::pseudo_hdrcmplt:
	return AF_pseudo_HDRCMPLT;
#endif
#ifdef AF_pseudo_KEY
case sock_domain::pseudo_key:
	return AF_pseudo_KEY;
#endif
#ifdef AF_pseudo_PIP
case sock_domain::pseudo_pip:
	return AF_pseudo_PIP;
#endif
#ifdef AF_pseudo_RTIP
case sock_domain::pseudo_rtip:
	return AF_pseudo_RTIP;
#endif
#ifdef AF_pseudo_XTP
case sock_domain::pseudo_xtp:
	return AF_pseudo_XTP;
#endif
#ifdef AF_pseudo_PUP
case sock_domain::pseudo_pup:
	return AF_pseudo_PUP;
#endif
#ifdef AF_QIPCRTR
case sock_domain::qipcrtr:
	return AF_QIPCRTR;
#endif
#ifdef AF_RDS
case sock_domain::rds:
	return AF_RDS;
#endif
#ifdef AF_ROSE
case sock_domain::rose:
	return AF_ROSE;
#endif
#ifdef AF_ROUTE
case sock_domain::route:
	return AF_ROUTE;
#endif
#ifdef AF_RXRPC
case sock_domain::rxrpc:
	return AF_RXRPC;
#endif
#ifdef AF_SCLUSTER
case sock_domain::scluster:
	return AF_SCLUSTER;
#endif
#ifdef AF_SECURITY
case sock_domain::security:
	return AF_SECURITY;
#endif
#ifdef AF_SIP
case sock_domain::sip:
	return AF_SIP;
#endif
#ifdef AF_SLOW
case sock_domain::slow:
	return AF_SLOW;
#endif
#ifdef AF_SMC
case sock_domain::smc:
	return AF_SMC;
#endif
#ifdef AF_SNA
case sock_domain::sna:
	return AF_SNA;
#endif
#ifdef AF_TIPC
case sock_domain::tipc:
	return AF_TIPC;
#endif
#ifdef AF_VOICEVIEW
case sock_domain::voiceview:
	return AF_VOICEVIEW;
#endif
#ifdef AF_VSOCK
case sock_domain::vsock:
	return AF_VSOCK;
#endif
#ifdef AF_VSOCK
case sock_domain::vsock:
	return AF_VSOCK;
#endif
#ifdef AF_WANPIPE
case sock_domain::wanpipe:
	return AF_WANPIPE;
#endif
#ifdef AF_X25
case sock_domain::x25:
	return AF_X25;
#endif
#ifdef AF_XDP
case sock_domain::xdp:
	return AF_XDP;
#endif
default:
	throw_posix_error(EINVAL);
}
}

inline constexpr int native_sock_protocal(sock_protocal dom)
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
	throw_posix_error(EINVAL);
}
}

inline constexpr int native_sock_type(sock_type dom)
{
switch(sock_type)
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
	throw_posix_error(EINVAL);
};
}

inline constexpr int native_sock_open_mode(open_mode om)
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
		throw_posix_error(EINVAL);
#endif
	return ret;
}

inline int open_socket_impl(sock_domain d,sock_type t,open_mode m,sock_protocal p)
{
	int fd{socket(native_sock_domain(d),native_sock_type(t)|native_sock_open_mode(m),native_sock_protocal(p))};
	if(fd=-1)
		throw_posix_error();
	return fd;
}

}

template<std::integral ch_type>
class basic_posix_socket_file: public basic_posix_socket_io_handle<ch_type>
{
public:
	using char_type = ch_type;
	using native_handle_type = int;
	template<typename native_hd>
	requires std::same_as<native_handle_type,std::remove_cvref_t<native_hd>>
	explicit constexpr basic_posix_socket_file(native_hd fd) noexcept: basic_posix_socket_io_handle<char_type>{fd}

	requires std::same_as<native_handle_type,std::remove_cvref_t<native_hd>>
	constexpr basic_posix_socket_file(native_hd fd) noexcept: basic_posix_socket_io_handle<ch_type>{fd}{}

	constexpr basic_posix_socket_file(sock_domain d,sock_type t,open_mode m,sock_protocal p)
		:basic_posix_socket_io_handle<ch_type>{details::open_socket_impl(d,t,m,p)}
	{}

	basic_posix_socket_file(io_dup_t,basic_posix_socket_io_observer<ch_type> piob):basic_posix_socket_io_handle<ch_type>{details::sys_dup(piob.fd)}
	{}
	constexpr basic_posix_socket_file(basic_posix_socket_file const&)=default;
	constexpr basic_posix_socket_file& operator=(basic_posix_socket_file const&)=default;
	constexpr basic_posix_socket_file(basic_posix_socket_file &&) noexcept=default;
	constexpr basic_posix_socket_file& operator=(basic_posix_socket_file &&) noexcept=default;
	~basic_posix_socket_file()
	{
		if(this->fd!=-1)[[likely]]
			details::sys_close(this->fd);
	}
};

}