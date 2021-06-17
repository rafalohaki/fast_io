#pragma once

namespace fast_io
{

template<win32_family family,std::integral ch_type>
class basic_win32_family_socket_io_observer
{
public:
	using char_type = ch_type;
	using native_handle_type = std::uintptr_t;
	native_handle_type hsocket{};
	constexpr native_handle_type release() noexcept
	{
		native_handle_type temp{hsocket};
		hsocket={};
		return temp;
	}
	constexpr native_handle_type const& native_handle() const noexcept
	{
		return hsocket;
	}
	constexpr native_handle_type& native_handle() noexcept
	{
		return hsocket;
	}
};

template<win32_family family,std::integral char_type>
inline constexpr basic_win32_family_socket_io_observer<family,char_type> io_value_handle(basic_win32_family_socket_io_observer<family,char_type> value) noexcept
{
	return value;
}

namespace win32::details
{

inline std::uintptr_t win32_duphsocket(std::uintptr_t s)
{
	return reinterpret_cast<std::uintptr_t>(win32_dup_impl(reinterpret_cast<void*>(s)));
}
inline std::uintptr_t win32_dup2hsocket(std::uintptr_t handle,std::uintptr_t newhandle)
{
	auto temp{win32_duphsocket(handle)};
	if(newhandle)[[likely]]
		closesocket(newhandle);
	return temp;
}

}

template<win32_family family,std::integral ch_type>
class basic_win32_family_socket_io_handle : public basic_win32_family_socket_io_observer<family,ch_type>
{
public:
	using char_type = ch_type;
	using native_handle_type = std::uintptr_t;
	constexpr basic_win32_family_socket_io_handle() noexcept=default;
	template<typename native_hd>
	requires std::same_as<native_handle_type,std::remove_cvref_t<native_hd>>
	explicit constexpr basic_win32_family_socket_io_handle(native_hd hsocket) noexcept: basic_win32_family_socket_io_observer<family,ch_type>{hsocket}
	{}
	basic_win32_family_socket_io_handle(basic_win32_family_socket_io_handle const& dp):basic_win32_family_socket_io_observer<family,char_type>{win32::details::win32_duphsocket(dp.hsocket)}
	{
	}
	basic_win32_family_socket_io_handle& operator=(basic_win32_family_socket_io_handle const& dp)
	{
		this->hsocket=win32::details::win32_dup2hsocket(dp.hsocket,this->hsocket);
		return *this;
	}
	constexpr basic_win32_family_socket_io_handle(basic_win32_family_socket_io_handle&& b) noexcept : basic_win32_family_socket_io_observer<family,char_type>{b.hsocket}
	{
		b.hsocket = 0;
	}
	basic_win32_family_socket_io_handle& operator=(basic_win32_family_socket_io_handle&& b) noexcept
	{
		if(b.hsocket!=this->hsocket)
		{
			if(this->hsocket)[[likely]]
				win32::closesocket(this->hsocket);
			this->hsocket=b.hsocket;
			b.hsocket = 0;
		}
		return *this;
	}
	constexpr void reset(native_handle_type newhsocket=0) noexcept
	{
		if(this->hsocket!=0)[[likely]]
			win32::closesocket(this->hsocket);
		this->hsocket=newhsocket;
	}
	void close()
	{
		if(this->hsocket)[[likely]]
		{
			if(win32::closesocket(this->hsocket))
				throw_win32_error(win32::WSAGetLastError());
			this->hsocket=0;
		}
	}
};

namespace win32::details
{
inline constexpr int win32_sock_domain(sock_domain dom) noexcept
{
	switch(dom)
	{
	case sock_domain::local:
		return 1;
	case sock_domain::inet:
		return 2;
	case sock_domain::implink:
		return 3;
	case sock_domain::pup:
		return 4;
	case sock_domain::chaos:
		return 5;
	case sock_domain::ipx:
		return 6;
	case sock_domain::iso:
		return 7;
	case sock_domain::ecma:
		return 8;
	case sock_domain::datakit:
		return 9;
	case sock_domain::ccitt:
		return 10;
	case sock_domain::sna:
		return 11;
	case sock_domain::decnet:
		return 12;
	case sock_domain::dli:
		return 13;
	case sock_domain::lat:
		return 14;
	case sock_domain::hylink:
		return 15;
	case sock_domain::appletalk:
		return 16;
	case sock_domain::netbios:
		return 17;
	case sock_domain::voiceview:
		return 18;
	case sock_domain::firefox:
		return 19;
	case sock_domain::ban:
		return 21;
	case sock_domain::atm:
		return 22;
	case sock_domain::inet6:
		return 23;
	case sock_domain::cluster:
		return 24;
	case sock_domain::ieee12844:
		return 25;
	case sock_domain::irda:
		return 26;
	case sock_domain::netdes:
		return 28;
	case sock_domain::tcnprocess:
		return 29;
	case sock_domain::tcnmessage:
		return 30;
	case sock_domain::iclfxbm:
		return 31;
	case sock_domain::bluetooth:
		return 32;
	case sock_domain::can:
		return 65537;
	default:
		return 0;
	}
}

inline constexpr int win32_sock_protocal(sock_protocal prot) noexcept
{
	switch(prot)
	{
	case sock_protocal::ip:
		return 0;
	case sock_protocal::hopopts:
		return 0;
	case sock_protocal::icmp:
		return 1;
	case sock_protocal::igmp:
		return 2;
	case sock_protocal::ggp:
		return 3;
	case sock_protocal::ipv4:
		return 4;
	case sock_protocal::st:
		return 5;
	case sock_protocal::tcp:
		return 6;
	case sock_protocal::cbt:
		return 7;
	case sock_protocal::egp:
		return 8;
	case sock_protocal::igp:
		return 9;
	case sock_protocal::pup:
		return 12;
	case sock_protocal::udp:
		return 17;
	case sock_protocal::idp:
		return 22;
	case sock_protocal::rdp:
		return 27;
	case sock_protocal::ipv6:
		return 41;
	case sock_protocal::routing:
		return 43;
	case sock_protocal::fragment:
		return 44;
	case sock_protocal::icmpv6:
		return 58;
	case sock_protocal::none:
		return 59;
	case sock_protocal::dstopts:
		return 60;
	case sock_protocal::nd:
		return 77;
	case sock_protocal::iclfxbm:
		return 78;
	case sock_protocal::pim:
		return 103;
	case sock_protocal::pgm:
		return 113;
	case sock_protocal::l2tp:
		return 115;
	case sock_protocal::sctp:
		return 132;
	case sock_protocal::raw:
		return 255;
	default:
		return -1;
	}
}

inline constexpr int win32_sock_type(sock_type dom) noexcept
{
	switch(dom)
	{
		case sock_type::stream:
			return 1;
		case sock_type::dgram:
			return 2;
		case sock_type::seqpacket:
			return 5;
		case sock_type::raw:
			return 3;
		case sock_type::rdm:
			return 4;
		default:
			return -1;
	};
}

inline constexpr std::uint32_t win32_sock_flags(open_mode m) noexcept
{
	std::uint32_t flags{};
	if((m&open_mode::no_block)==open_mode::no_block)
		flags|=0x01;
	if((m&open_mode::inherit)!=open_mode::inherit)
		flags|=0x80;
	return flags;
}

template<win32_family family>
inline std::uintptr_t open_win32_socket_impl(sock_domain d,sock_type t,open_mode m,sock_protocal p)
{
	int af{win32_sock_domain(d)};
	int tp{win32_sock_type(t)};
	int prt{win32_sock_protocal(p)};
	std::uint32_t dwflags{win32_sock_flags(m)};
	if constexpr(family==win32_family::wide_nt)
	{
		std::uintptr_t ret{WSASocketW(af,tp,prt,nullptr,0,dwflags)};
		if(ret==UINTPTR_MAX)
			throw_win32_error(WSAGetLastError());
		return ret;
	}
	else
	{
		std::uintptr_t ret{WSASocketA(af,tp,prt,nullptr,0,dwflags)};
		if(ret==UINTPTR_MAX)
			throw_win32_error(WSAGetLastError());
		return ret;
	}
}

}

template<win32_family family,std::integral ch_type>
class
#if __has_cpp_attribute(gnu::trivial_abi)
[[gnu::trivial_abi]]
#endif
basic_win32_family_socket_file : public basic_win32_family_socket_io_handle<family,ch_type>
{
public:
	using char_type = ch_type;
	using native_handle_type = std::uintptr_t;
	constexpr basic_win32_family_socket_file() noexcept=default;
	template<typename native_hd>
	requires std::same_as<native_handle_type,std::remove_cvref_t<native_hd>>
	explicit constexpr basic_win32_family_socket_file(native_hd hsocket) noexcept: basic_win32_family_socket_io_handle<family,ch_type>{hsocket}
	{}
	basic_win32_family_socket_file(io_dup_t,basic_win32_family_socket_io_observer<family,ch_type> wsiob):basic_win32_family_socket_io_handle<family,ch_type>{win32::details::win32_duphsocket(wsiob.hsocket)}
	{}
	basic_win32_family_socket_file(sock_domain d,sock_type t,open_mode m,sock_protocal p)
		:basic_win32_family_socket_file<family,ch_type>{win32::details::open_win32_socket_impl<family>(d,t,m,p)}
	{}
	basic_win32_family_socket_file(basic_win32_family_socket_file const&)=default;
	basic_win32_family_socket_file& operator=(basic_win32_family_socket_file const&)=default;
	constexpr basic_win32_family_socket_file(basic_win32_family_socket_file&&) noexcept=default;
	basic_win32_family_socket_file& operator=(basic_win32_family_socket_file&&) noexcept=default;
	~basic_win32_family_socket_file()
	{
		if(this->hsocket)[[likely]]
			win32::closesocket(this->hsocket);
	}
};

template<std::integral ch_type>
using basic_win32_socket_file_9xa = basic_win32_family_socket_file<win32_family::ansi_9x,ch_type>;

using win32_socket_file_9xa=basic_win32_socket_file_9xa<char>;
using wwin32_socket_file_9xa=basic_win32_socket_file_9xa<wchar_t>;
using u8win32_socket_file_9xa=basic_win32_socket_file_9xa<char8_t>;
using u16win32_socket_file_9xa=basic_win32_socket_file_9xa<char16_t>;
using u32win32_socket_file_9xa=basic_win32_socket_file_9xa<char32_t>;

template<std::integral ch_type>
using basic_win32_socket_file_ntw = basic_win32_family_socket_file<win32_family::wide_nt,ch_type>;

using win32_socket_file_ntw=basic_win32_socket_file_ntw<char>;
using wwin32_socket_file_ntw=basic_win32_socket_file_ntw<wchar_t>;
using u8win32_socket_file_ntw=basic_win32_socket_file_ntw<char8_t>;
using u16win32_socket_file_ntw=basic_win32_socket_file_ntw<char16_t>;
using u32win32_socket_file_ntw=basic_win32_socket_file_ntw<char32_t>;

template<std::integral ch_type>
using basic_win32_socket_file = basic_win32_family_socket_file<win32_family::native,ch_type>;

using win32_socket_file=basic_win32_socket_file<char>;
using wwin32_socket_file=basic_win32_socket_file<wchar_t>;
using u8win32_socket_file=basic_win32_socket_file<char8_t>;
using u16win32_socket_file=basic_win32_socket_file<char16_t>;
using u32win32_socket_file=basic_win32_socket_file<char32_t>;

#ifndef __CYGWIN__

template<std::integral ch_type>
using basic_native_socket_file = basic_win32_socket_file<ch_type>;

#endif

}
