#pragma once

namespace fast_io
{


namespace details
{
struct empty
{};
}

template<std::integral ch_type>
class basic_socket_io_observer
{
public:
	using char_type = ch_type;
	using native_handle_type = sock::details::socket_type;
	native_handle_type soc=sock::details::invalid_socket;
	inline constexpr operator bool() noexcept
	{
		return soc!=sock::details::invalid_socket;
	}
	inline constexpr native_handle_type release() noexcept
	{
		auto temp{soc};
		soc=sock::details::invalid_socket;
		return temp;
	}
	inline constexpr void reset() noexcept
	{
		soc=sock::details::invalid_socket;
	}
	inline constexpr void reset(native_handle_type newsoc) noexcept
	{
		soc=newsoc;
	}
	inline constexpr void swap(basic_socket_io_observer& other) noexcept
	{
		std::swap(soc,other.soc);
	}
	inline constexpr auto& native_handle() const noexcept
	{	
		return soc;
	}
	inline constexpr auto& native_handle() noexcept
	{	
		return soc;
	}
	inline explicit constexpr operator basic_posix_io_observer<char_type>() noexcept requires(std::same_as<native_handle_type,int>)
	{
		return basic_posix_io_observer<char_type>{soc};
	}
};

template<std::integral ch_type,std::contiguous_iterator Iter>
inline Iter read(basic_socket_io_observer<ch_type> soc,Iter begin,Iter end)
{
	return begin+((sock::details::recv(soc.soc,std::to_address(begin),static_cast<std::size_t>((end-begin)*sizeof(*begin)),0))/sizeof(*begin));
}
template<std::integral ch_type,std::contiguous_iterator Iter>
inline Iter write(basic_socket_io_observer<ch_type> soc,Iter begin,Iter end)
{
	return begin+(sock::details::send(soc.soc,std::to_address(begin),static_cast<std::size_t>((end-begin)*sizeof(*begin)),0)/sizeof(*begin));
}
#if !(defined(__WINNT__) || defined(_MSC_VER))
template<std::integral ch_type>
inline auto redirect_handle(basic_socket_io_observer<ch_type> soc)
{
	return soc.soc;
}
#endif

#if defined(__linux__)
template<std::integral ch_type>
inline constexpr auto zero_copy_in_handle(basic_socket_io_observer<ch_type> soc)
{
	return soc.soc;
}
template<std::integral ch_type>
inline constexpr auto zero_copy_out_handle(basic_socket_io_observer<ch_type> soc)
{
	return soc.soc;
}
#endif

namespace details
{

enum class send_recv_message_impl_enum
{
send_message=0,
recv_message=1
};

template<send_recv_message_impl_enum enm,std::integral ch_type,typename T>
requires std::same_as<message_hdr,std::remove_cvref_t<T>>
inline std::size_t send_recv_message_impl(basic_socket_io_observer<ch_type> soc,T& message,message_flag flag)
{

#ifdef _WIN32
	using socklen_t=int;
#endif
	if constexpr(sizeof(socklen_t)<sizeof(std::size_t))
	{
		if(static_cast<std::size_t>(std::numeric_limits<socklen_t>::max())<message.namelen)
			throw_posix_error(EIO);
	}
#ifdef _WIN32
	if constexpr(sizeof(std::uint32_t)<sizeof(std::size_t))
	{
		if(static_cast<std::size_t>(std::numeric_limits<std::uint32_t>::max())<message.controllen)
			throw_posix_error(EIO);
	}
	if(4096<message.iovlen)
		throw_posix_error(EIO);
#ifndef _MSC_VER
	struct __attribute__((__may_alias__)) socket_address_may_alias:SOCKADDR
	{};
#endif
	std::array<WSABUF,4096> wsabufs;
	for(std::size_t i{};i!=message.iovlen;++i)
	{
		if constexpr(sizeof(std::uint32_t)<sizeof(std::size_t))
		{
			if(static_cast<std::size_t>(std::numeric_limits<std::uint32_t>::max())<message.iov[i].len)
				throw_posix_error(EIO);
		}
		wsabufs[i].len=message.iov[i].len;
		wsabufs[i].buf=reinterpret_cast<char*>(const_cast<void*>(message.iov[i].base));
	}
	WSAMSG msg{.name=
#ifndef _MSC_VER
	reinterpret_cast<socket_address_may_alias*>(const_cast<void*>(message.name)),
#else
	reinterpret_cast<SOCKADDR*>(message.name),
#endif
	.namelen=static_cast<int>(message.namelen),
	.lpBuffers=wsabufs.data(),
	.dwBufferCount=static_cast<std::uint32_t>(message.iovlen),
	.Control={static_cast<std::uint32_t>(message.controllen),reinterpret_cast<char*>(const_cast<void*>(message.control))},
	.dwFlags=static_cast<std::uint32_t>(message.flags)
	};
	long unsigned number_of_bytes_sent_or_received{};
	if constexpr(enm==send_recv_message_impl_enum::send_message)
		sock::details::wsasendmsg(soc.soc,std::addressof(msg),static_cast<std::uint32_t>(flag),std::addressof(number_of_bytes_sent_or_received),nullptr,nullptr);
	else
		sock::details::wsarecvmsg(soc.soc,std::addressof(msg),static_cast<std::uint32_t>(flag),std::addressof(number_of_bytes_sent_or_received),nullptr,nullptr);
	return number_of_bytes_sent_or_received;
#else

/*
struct msghdr {
               void         *msg_name;        Optional address 
               socklen_t     msg_namelen;     Size of address 
               struct iovec *msg_iov;         Scatter/gather array 
               size_t        msg_iovlen;      # elements in msg_iov 
               void         *msg_control;     Ancillary data, see below 
               size_t        msg_controllen;  Ancillary data buffer len 
               int           msg_flags;       Flags (unused) 
           };
*/
	msghdr msg{.msg_name=const_cast<void*>(message.name),.msg_namelen=static_cast<socklen_t>(message.namelen),
		.msg_iov=reinterpret_cast<details::iovec_may_alias*>(const_cast<io_scatter_t*>(message.iov)),
		.msg_iovlen=message.iovlen,
		.msg_control=const_cast<void*>(message.control),
		.msg_controllen=message.controllen,
		.msg_flags=message.flags};
	if constexpr(enm==send_recv_message_impl_enum::send_message)
		return sock::details::sendmsg(soc.soc,std::addressof(msg),static_cast<int>(flag));
	else
		return sock::details::recvmsg(soc.soc,std::addressof(msg),static_cast<int>(flag));
#endif

}

}


template<std::integral ch_type>
inline std::size_t send_message(basic_socket_io_observer<ch_type> soc,message_hdr const& message,message_flag flag)
{
	return details::send_recv_message_impl<details::send_recv_message_impl_enum::send_message>(soc,message,flag);
}

template<std::integral ch_type>
inline std::size_t recv_message(basic_socket_io_observer<ch_type> soc,message_hdr& message,message_flag flag)
{
	return details::send_recv_message_impl<details::send_recv_message_impl_enum::recv_message>(soc,message,flag);
}

#ifndef _WIN32
template<std::integral ch_type>
inline std::size_t scatter_read(basic_socket_io_observer<ch_type> soc,std::span<io_scatter_t const> sp)
{
	message_hdr hdr{nullptr,0,sp.data(),sp.size(),nullptr,0,0};
	return recv_message(soc,hdr,{});
}

template<std::integral ch_type>
inline std::size_t scatter_write(basic_socket_io_observer<ch_type> soc,std::span<io_scatter_t const> sp)
{
	return send_message(soc,message_hdr{nullptr,0,sp.data(),sp.size(),nullptr,0,0},{});
}
#endif
template<std::integral ch_type>
class basic_socket_io_handle:public basic_socket_io_observer<ch_type>
{
public:
	using char_type = ch_type;
	using native_handle_type = sock::details::socket_type;
	constexpr basic_socket_io_handle()=default;
	constexpr basic_socket_io_handle(native_handle_type soc):basic_socket_io_observer<ch_type>{soc}{}
	void close()
	{
		if(*this)[[likely]]
			sock::details::closesocket(this->native_handle());
	}
#if defined(__WINNT__) || defined(_MSC_VER)
	basic_socket_io_handle(basic_socket_io_handle const&)=delete;
	basic_socket_io_handle& operator=(basic_socket_io_handle const&)=delete;
#else
	basic_socket_io_handle(basic_socket_io_handle const& other):basic_socket_io_observer<ch_type>{details::sys_dup(other.native_handle()),other.addr}
	{
	}
	basic_socket_io_handle& operator=(basic_socket_io_handle const& other)
	{
		this->native_handle()=details::sys_dup2(other.native_handle(),this->native_handle());
		return *this;
	}
#endif
	constexpr basic_socket_io_handle(basic_socket_io_handle&& other) noexcept:basic_socket_io_observer<ch_type>{other.release(),other.addr}{}
	basic_socket_io_handle& operator=(basic_socket_io_handle&& other) noexcept
	{
		if(other.native_handle()==this->native_handle())
			return *this;
		if(*this)[[likely]]
			sock::details::closesocket_ignore_error(this->native_handle());
		this->native_handle()=other.release();
		return *this;
	}
};

template<std::integral ch_type>
class basic_socket_file:public basic_socket_io_handle<ch_type>
{
public:
	using char_type = ch_type;
	using native_handle_type = sock::details::socket_type;
	constexpr basic_socket_file()=default;
	constexpr basic_socket_file(native_handle_type soc):basic_socket_io_handle<ch_type>{soc}{}
	constexpr basic_socket_file(basic_socket_file const&)=default;
	constexpr basic_socket_file& operator=(basic_socket_file const&)=default;
	constexpr basic_socket_file(basic_socket_file&&) noexcept=default;
	constexpr basic_socket_file& operator=(basic_socket_file&&) noexcept=default;
	~basic_socket_file()
	{
		if(*this)[[likely]]
			sock::details::closesocket_ignore_error(*this);
	}
	template<typename ...Args>
	basic_socket_file(native_interface_t,Args&& ...args):basic_socket_io_handle<ch_type>(sock::details::socket(std::forward<Args>(args)...)){}
	basic_socket_file(sock::family family,sock::type type,sock::protocal protocal):basic_socket_io_handle<ch_type>(sock::details::socket(static_cast<sock::details::address_family>(family),static_cast<int>(type),static_cast<int>(protocal))){}
};
template<std::integral char_type>
inline void connect(basic_socket_io_observer<char_type> siob,endpoint& info)
{
	sock::details::connect(siob.soc,info.storage.sock,static_cast<std::size_t>(info.storage_size));
}

template<std::integral char_type>
inline void bind(basic_socket_io_observer<char_type> siob,endpoint& info)
{
	sock::details::bind(siob.soc,info.storage.sock,static_cast<std::size_t>(info.storage_size));
}

template<std::integral char_type>
inline void listen(basic_socket_io_observer<char_type> siob)
{
	sock::details::listen(siob.soc,10);
}


using socket_io_observer=basic_socket_io_observer<char>;
using socket_io_handle=basic_socket_io_handle<char>;
using socket_file=basic_socket_file<char>;

template<std::integral ch_type>
struct packet
{
	using char_type = ch_type;
	basic_socket_io_observer<char_type> siob;
	endpoint& addr;
};

template<std::integral char_type> packet(basic_socket_io_observer<char_type>,endpoint&) -> packet<char_type>;

#ifdef _WIN32

template<std::integral char_type,std::contiguous_iterator Iter>
inline Iter write(packet<char_type> pack,Iter begin,Iter end)
{
	std::size_t size{(end-begin)*sizeof(*begin)};
	if(std::numeric_limits<int>::max()<size)
		size=std::numeric_limits<int>::max();
	return begin+sock::details::sendto(pack.siob.soc,reinterpret_cast<char const*>(std::to_address(begin)),static_cast<int>(size),0,std::addressof(pack.addr.storage.sock),static_cast<int>(pack.addr.storage_size))/sizeof(*begin);
}

template<std::integral char_type,std::contiguous_iterator Iter>
inline Iter read(packet<char_type> pack,Iter begin,Iter end)
{
	std::size_t size{(end-begin)*sizeof(*begin)};
	if(std::numeric_limits<int>::max()<size)
		size=std::numeric_limits<int>::max();
	int socksize{static_cast<int>(pack.addr.storage_size)};
	auto ret{begin+sock::details::recvfrom(pack.siob.soc,
		reinterpret_cast<char*>(std::to_address(begin)),
		static_cast<int>(size),
		0,
		std::addressof(pack.addr.storage.sock),
		std::addressof(socksize))/sizeof(*begin)};
	pack.addr.storage_size=socksize;
	return ret;
}

#else
template<std::integral char_type,std::contiguous_iterator Iter>
inline Iter write(packet<char_type> pack,Iter begin,Iter end)
{
	io_scatter_t sc{begin,(end-begin)*sizeof(*begin)};
	return begin+send_message(pack.siob,message_hdr{std::addressof(pack.addr.storage.sock),static_cast<std::size_t>(pack.addr.storage_size),std::addressof(sc),1,nullptr,0,0},{})/sizeof(*begin);
}

template<std::integral char_type,std::contiguous_iterator Iter>
inline Iter read(packet<char_type> pack,Iter begin,Iter end)
{
	io_scatter_t sc{begin,(end-begin)*sizeof(*begin)};
	message_hdr hdr{std::addressof(pack.addr.storage.sock),static_cast<std::size_t>(pack.addr.storage_size),std::addressof(sc),1,nullptr,0,0};
	return begin+recv_message(pack.siob,hdr,{})/sizeof(*begin);
}

template<std::integral char_type>
inline std::size_t scatter_write(packet<char_type> pack,std::span<io_scatter_t const> sp)
{
	return send_message(pack.siob,message_hdr{std::addressof(pack.addr.storage.sock),static_cast<std::size_t>(pack.addr.storage_size),sp.data(),sp.size(),nullptr,0,0},{});
}

template<std::integral char_type>
inline std::size_t scatter_read(packet<char_type> pack,std::span<io_scatter_t const> sp)
{
	message_hdr hdr{std::addressof(pack.addr.storage.sock),static_cast<std::size_t>(pack.addr.storage_size),sp.data(),sp.size(),nullptr,0,0};
	return recv_message(pack.siob,hdr,{});
}
#endif


template<std::integral char_type>
inline basic_socket_file<char_type> accept(basic_socket_io_observer<char_type> siob,endpoint& ep)
{
	return {sock::details::accept(siob.soc,ep.storage.sock,ep.storage_size)};
}

template<std::integral ch_type>
class acceptor:public basic_socket_file<ch_type>
{
public:
	endpoint ep;
	using char_type = ch_type;
	using native_handle_type = sock::details::socket_type;
	acceptor(basic_socket_io_observer<char_type> siob)
	{
		this->native_handle()=sock::details::accept(siob.soc,ep.storage.sock,ep.storage_size);
	}
};

template<std::integral char_type> acceptor(basic_socket_io_observer<char_type>) -> acceptor<char_type>;

template<std::integral ch_type>
class basic_tcp_client:public basic_socket_file<ch_type>
{
public:
	endpoint ep;
	using char_type = ch_type;
	using native_handle_type = sock::details::socket_type;
	template<typename T>
	requires requires(T t)
	{
		{family(t)}->std::convertible_to<sock::family>;
	}
	basic_tcp_client(T const& addr,std::uint16_t port):
		basic_socket_file<ch_type>(family(addr),sock::type::stream,sock::protocal::tcp),
		ep{to_socket_address_storage(addr,port)}
	{
		connect(*this,ep);
	}
	basic_tcp_client(std::uint16_t port):basic_tcp_client(fast_io::ipv4{},port){}
};

using tcp_client = basic_tcp_client<char>;

template<std::integral ch_type>
class basic_udp:public basic_socket_file<ch_type>
{
public:
	using char_type = ch_type;
	using native_handle_type = sock::details::socket_type;
	basic_udp():basic_socket_file<ch_type>(sock::family::ipv4,sock::type::datagrams,sock::protocal::udp){}
};

using udp = basic_udp<char>;

template<std::integral ch_type>
class basic_udp_server:public basic_socket_file<ch_type>
{
public:
	endpoint ep;
	using char_type = ch_type;
	using native_handle_type = sock::details::socket_type;
	template<typename T>
	requires requires(T t)
	{
		{family(t)}->std::convertible_to<sock::family>;
	}
	basic_udp_server(T const& addr,std::uint16_t port):
		basic_socket_file<ch_type>(family(addr),sock::type::datagrams,sock::protocal::udp),
		ep{to_socket_address_storage(addr,port)}
	{
		bind(*this,ep);
	}
	basic_udp_server(std::uint16_t port):basic_udp_server(ipv4{},port){}
};

using udp_server = basic_udp_server<char>;

template<std::integral ch_type>
class basic_tcp_server:public basic_socket_file<ch_type>
{
public:
	endpoint ep;
	using char_type = ch_type;
	using native_handle_type = sock::details::socket_type;
	template<typename T>
	requires requires(T t)
	{
		{family(t)}->std::convertible_to<sock::family>;
	}
	basic_tcp_server(T const& addr,std::uint16_t port):
		basic_socket_file<ch_type>(family(addr),sock::type::stream,sock::protocal::tcp),
		ep{to_socket_address_storage(addr,port)}
	{
		bind(*this,ep);
		listen(*this);
	}
	basic_tcp_server(std::uint16_t port):basic_tcp_server(ipv4{},port){}
};

using tcp_server = basic_tcp_server<char>;
}