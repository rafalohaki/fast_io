#pragma once


namespace fast_io
{

using posix_socklen_t=::socklen_t;
using native_socklen_t=posix_socklen_t;

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
case sock_protocal::none:
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

inline void posix_connect_posix_socket_impl(int fd,void const* addr,posix_socklen_t addrlen)
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

inline void posix_bind_posix_socket_impl(int fd,void const* addr,posix_socklen_t addrlen)
{
#if defined(__linux__) && defined(__NR_bind)
	system_call_throw_error(system_call<__NR_bind,int>(fd,addr,addrlen));
#else
	using sockaddr_alias_const_ptr
#if __has_cpp_attribute(gnu::may_alias)
	[[gnu::may_alias]]
#endif
	= struct sockaddr const*;
	if(::bind(fd,reinterpret_cast<sockaddr_alias_const_ptr>(addr),addrlen)==-1)
		throw_posix_error();
#endif
}

inline void posix_listen_posix_socket_impl(int fd,int backlog)
{
#if defined(__linux__) && defined(__NR_listen)
	system_call_throw_error(system_call<__NR_listen,int>(fd,backlog));
#else
	using sockaddr_alias_const_ptr
#if __has_cpp_attribute(gnu::may_alias)
	[[gnu::may_alias]]
#endif
	= struct sockaddr const*;
	if(::listen(fd,backlog)==-1)
		throw_posix_error();
#endif
}

inline int posix_accept_posix_socket_impl(int fd,void* addr,posix_socklen_t* addrlen)
{
#if defined(__linux__) && defined(__NR_accept)
	int socfd{system_call<__NR_accept,int>(fd,addr,addrlen)};
	system_call_throw_error(socfd);
	return socfd;
#else
	using sockaddr_alias_const_ptr
#if __has_cpp_attribute(gnu::may_alias)
	[[gnu::may_alias]]
#endif
	= struct sockaddr const*;
	int socfd{::accept(fd,addr,addrlen)};
	if(socfd==-1)
		throw_posix_error();
	return socfd;
#endif
}

}

}
