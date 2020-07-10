#pragma once

namespace fast_io
{


template<bool async=false>
class basic_socket
{
	sock::details::socket_type handle=sock::details::invalid_socket;
	void close_impl()
	{
#ifdef __cpp_exceptions
	try
	{
#endif
		if(handle!=sock::details::invalid_socket)
			sock::details::closesocket(handle);
#ifdef __cpp_exceptions
	}
	catch(...)
	{}
#endif
	}
public:
	basic_socket()=default;
	basic_socket(sock::details::socket_type v):handle(v){}
	template<typename ...Args>
	basic_socket(native_interface_t,Args&& ...args):handle(sock::details::socket(std::forward<Args>(args)...)){}
	basic_socket(sock::family family,sock::type const &type,sock::protocal const &protocal = sock::protocal::none):
		handle(sock::details::socket(static_cast<sock::details::address_family>(family),static_cast<int>(type),static_cast<int>(protocal))){}
	auto& native_handle() {return handle;}
	basic_socket(basic_socket const& other)
#if defined(__WINNT__) || defined(_MSC_VER)
	{
		auto const current_process(win32::GetCurrentProcess());
		void* new_handle{};
		if(!win32::DuplicateHandle(current_process,bit_cast<void*>(other.handle),current_process,std::addressof(new_handle), 0, true, 2/*DUPLICATE_SAME_ACCESS*/))
#ifdef __cpp_exceptions
			throw win32_error();
#else
			fast_terminate();
#endif
		handle=bit_cast<sock::details::socket_type>(new_handle);
	}
#else
	:handle(
#if defined(__linux__)&&(defined(__x86_64__) || defined(__arm64__) || defined(__aarch64__) )
		system_call<
		#if defined(__x86_64__)
			32
		#elif defined(__arm64__) || defined(__aarch64__)
			23
		#endif
			,int>
#else
		dup
#endif
	(other.handle))
	{
		system_call_throw_error(handle);
	}
#endif
	
	basic_socket& operator=(basic_socket const& other)
	{
#if defined(__WINNT__) || defined(_MSC_VER)
		auto const current_process(win32::GetCurrentProcess());
		void* new_handle{};
		if(!win32::DuplicateHandle(current_process,bit_cast<void*>(other.handle),current_process,std::addressof(new_handle), 0, true, 2/*DUPLICATE_SAME_ACCESS*/))
#ifdef __cpp_exceptions
			throw win32_error();
#else
			fast_terminate();
#endif
		if(handle)
			fast_io::win32::CloseHandle(handle);
		handle=bit_cast<sock::details::socket_type>(new_handle);
#else
		auto newfd(
#if defined(__linux__)&&(defined(__x86_64__) || defined(__arm64__) || defined(__aarch64__) )
		system_call<
		#if defined(__x86_64__)
			33
		#elif defined(__arm64__) || defined(__aarch64__)
			24
		#endif
		,int>
#else
		dup2
#endif
(other.handle,handle));
		system_call_throw_error(newfd);
		handle=newfd;
#endif
		return *this;
	}
	basic_socket(basic_socket && soc) noexcept:handle(soc.handle)
	{
		soc.handle = sock::details::invalid_socket;
	}
	basic_socket& operator=(basic_socket && soc) noexcept
	{
		if(soc.handle!=handle)
		{
			close_impl();
			handle = soc.handle;
			soc.handle = sock::details::invalid_socket;
		}
		return *this;
	}
	~basic_socket()
	{
		close_impl();
	}
};
template<bool async>
inline constexpr void flush(basic_socket<async>&)
{
}
#if defined(__linux__) || defined(__BSD_VISIBLE)
inline auto zero_copy_in_handle(basic_socket<false>& soc)
{
	return soc.native_handle();
}

inline auto zero_copy_out_handle(basic_socket<false>& soc)
{
	return soc.native_handle();
}
#endif

#if defined(__WINNT__) || defined(_MSC_VER)

//Bug to be fixed. disable first
/*
TransmitFile is garbage
inline auto zero_copy_out_handle(basic_socket<false>& soc)
{
	return soc.native_handle();
}
*/

/*
namespace sock::details
{

inline std::size_t win32_socket_scatter_write_impl(sock::details::socket_type soc,std::span<io_scatter_t const> scat,WSABUF* buf)
{
	for(std::size_t i{};i!=scat.size();++i)
	{
		buf[i].len=scat[i].len;
		buf[i].buf=reinterpret_cast<char*>(scat[i].base);
	}
	long unsigned dwbufcount;
	wsasend(soc,buf,scat.size(),std::addressof(dwbufcount),0,nullptr,nullptr);
	return dwbufcount;
}

}

inline std::size_t scatter_write(basic_socket<false>& soc,std::span<io_scatter_t const> scat)
{
	if(scat.size()<=1024)
	{
		std::array<WSABUF,1024> buf;
		return sock::details::win32_socket_scatter_write_impl(soc.native_handle(),scat,buf.data());
	}
	else
	{
		std::unique_ptr<WSABUF[]> uptr(new WSABUF[scat.size()]);
		return sock::details::win32_socket_scatter_write_impl(soc.native_handle(),scat,uptr.get());
	}
}
*/
#endif

struct address_info
{
	socket_address_storage storage={};
	socklen_t storage_size=sizeof(socket_address_storage);
};

template<fast_io::output_stream output>
inline void print_define(output& out,address_info const& a)
{
	if(a.storage_size==4)
	{
		std::array<std::uint8_t, 4> storage;
		memcpy(storage.data(),std::addressof(a.storage),4);
		print(out,storage.front(),u8":",storage[1],u8":",storage[2],u8":",storage.back());
	}
	else if(a.storage_size==16)
	{
		std::array<std::uint16_t, 8> storage;
		memcpy(storage.data(),std::addressof(a.storage),16);
		print(out,u8"[",hex(storage.front()),u8":",hex(storage[1]),u8":",
			hex(storage[2]),u8":",hex(storage[3]),u8":",
			hex(storage[4]),u8":",hex(storage[5]),u8":",
			hex(storage[6]),u8":",hex(storage[7]),u8"]");
	}
	else [[unlikely]]
		print(out,u8"unknown");
}

template<bool async=false>
class basic_connected_socket:public basic_socket<async>
{
public:
	template<typename ...Args>
	requires std::constructible_from<basic_socket<async>,Args...>
	basic_connected_socket(Args&& ...args):basic_socket<async>(std::forward<Args>(args)...){}
	using basic_socket<async>::native_handle;
};

template<bool async,std::contiguous_iterator Iter>
inline Iter read(basic_connected_socket<async>& soc,Iter begin,Iter end)
{
	return begin+((sock::details::recv(soc.native_handle(),std::to_address(begin),static_cast<int>((end-begin)*sizeof(*begin)),0))/sizeof(*begin));
}
template<bool async,std::contiguous_iterator Iter>
inline Iter write(basic_connected_socket<async>& soc,Iter begin,Iter end)
{
	return begin+(sock::details::send(soc.native_handle(),std::to_address(begin),static_cast<int>((end-begin)*sizeof(*begin)),0)/sizeof(*begin));
}
#if !(defined(__WINNT__) || defined(_MSC_VER))
template<bool async>
inline auto redirect_handle(basic_connected_socket<async>& soc)
{
	return soc.native_handle();
}
#endif
template<bool async=false>
class basic_connected_server
{
	basic_connected_socket<async> soc;
public:
	constexpr basic_connected_server()=default;
	template<typename addrType,std::integral U,typename ...Args>
	requires (!std::integral<addrType>)
	basic_connected_server(addrType const& add,U u,Args&& ...args):soc(family(add),std::forward<Args>(args)...)
	{
		auto stg(to_socket_address_storage(add,u));
		sock::details::bind(soc.native_handle(),stg,native_socket_address_size(add));
		sock::details::listen(soc.native_handle(),10);
	}
	constexpr auto& native_handle()
	{
		return soc;
	}
};
template<bool async=false>
class basic_tcp_server:public basic_connected_server<async>
{
public:
	template<typename addrType,std::integral U>
	requires (!std::integral<addrType>)
	basic_tcp_server(addrType const& add,U u):basic_connected_server<async>(family(add),u,fast_io::sock::type::stream){}
	template<std::integral U>
	basic_tcp_server(U u):basic_connected_server<async>(fast_io::ipv4{},u,fast_io::sock::type::stream){}
};


#if defined(__WINNT__) || defined(_MSC_VER)
#else
inline void unblock(basic_socket<true>& sv)
{
	if(::fcntl(sv.native_handle(), F_SETFL, O_NONBLOCK)==-1)
#ifdef __cpp_exceptions
		throw posix_error();
#else
		fast_terminate();
#endif
}

inline void unblock(basic_connected_server<true>& sv)
{
	unblock(sv.native_handle());
}
#endif

template<std::integral ch_type,bool async=false>
class basic_acceptor:public basic_connected_socket<async>
{
	address_info cinfo;
public:
	using native_handle_type = sock::details::socket_type;
	using char_type = ch_type;
	using basic_connected_socket<async>::native_handle;
	constexpr basic_acceptor()=default;
	basic_acceptor(basic_connected_server<async>& listener_socket)
	{
		native_handle()=sock::details::accept(listener_socket.native_handle().native_handle(),cinfo.storage,cinfo.storage_size);
		if constexpr(async)
			unblock(*this);
	}
/*
	basic_acceptor(async_server& listener_socket)
	{
#if defined(__WINNT__) || defined(_MSC_VER)
		native_handle()=sock::details::accept(listener_socket.native_handle().native_handle(),cinfo.storage,cinfo.storage_size);
#else
		native_handle()=sock::details::accept(listener_socket.native_handle().native_handle(),cinfo.storage,cinfo.storage_size);
		unblock(*this);
#endif
	}*/
	constexpr auto& info()
	{
		return cinfo;
	}
	constexpr auto& info() const
	{
		return cinfo;
	}
};


template<std::integral ch_type,bool async=false>
class basic_connected_client:public basic_connected_socket<async>
{
	address_info cinfo;
public:
	using char_type = ch_type;
	using basic_connected_socket<async>::native_handle;
	constexpr basic_connected_client()=default;
	template<typename T,std::integral U,typename ...Args>
	basic_connected_client(T const& add,U u,Args&& ...args):basic_connected_socket<async>(family(add),std::forward<Args>(args)...),cinfo{to_socket_address_storage(add,u),sizeof(socket_address_storage)}
	{
		sock::details::connect(native_handle(),cinfo.storage,native_socket_address_size(add));
	}
	constexpr auto& info()
	{
		return cinfo;
	}
	constexpr auto& info() const
	{
		return cinfo;
	}
};

template<std::integral ch_type,bool async=false>
class basic_tcp_client:public basic_connected_client<ch_type,async>
{
public:
	using char_type = ch_type;
	constexpr basic_tcp_client()=default;
	template<typename T,std::integral U>
	basic_tcp_client(T const& add,U u):basic_connected_client<ch_type,async>(add,u,fast_io::sock::type::stream){}
};
/*
class no_connection_socket:public socket
{
public:
	using char_type = ch_type;
	template<typename T,std::integral U,typename ...Args>
	no_connection_socket(fast_io::sock::family fam):socket(fam,){}
};
*/

}