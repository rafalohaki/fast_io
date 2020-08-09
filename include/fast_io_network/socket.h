#pragma once

namespace fast_io
{


namespace details
{
struct empty
{};
}

template<std::integral ch_type,bool contain_address_info=true>
class basic_socket_io_observer
{
public:
	using char_type = ch_type;
	using native_handle_type = sock::details::socket_type;
	native_handle_type soc=sock::details::invalid_socket;
	[[no_unique_address]] std::conditional_t<contain_address_info,address_info,details::empty> addr;
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
		std::swap(addr,other.addr);
	}
	inline constexpr auto& native_handle() const noexcept
	{	
		return soc;
	}
	inline constexpr auto& native_handle() noexcept
	{	
		return soc;
	}
	inline constexpr auto& address() const noexcept requires(contain_address_info)
	{
		return addr;
	}
	inline constexpr auto& address() noexcept requires(contain_address_info)
	{
		return addr;
	}
	inline static constexpr bool with_address_info() noexcept
	{
		return contain_address_info; 
	}
	inline explicit constexpr operator basic_posix_io_observer<char_type>() noexcept requires(std::same_as<native_handle_type,int>)
	{
		return basic_posix_io_observer<char_type>{soc};
	}
};

template<std::integral ch_type,bool contain_address_info,std::contiguous_iterator Iter>
inline Iter read(basic_socket_io_observer<ch_type,contain_address_info> soc,Iter begin,Iter end)
{
	return begin+((sock::details::recv(soc.soc,std::to_address(begin),static_cast<int>((end-begin)*sizeof(*begin)),0))/sizeof(*begin));
}
template<std::integral ch_type,bool contain_address_info,std::contiguous_iterator Iter>
inline Iter write(basic_socket_io_observer<ch_type,contain_address_info> soc,Iter begin,Iter end)
{
	return begin+(sock::details::send(soc.soc,std::to_address(begin),static_cast<int>((end-begin)*sizeof(*begin)),0)/sizeof(*begin));
}
#if !(defined(__WINNT__) || defined(_MSC_VER))
template<std::integral ch_type,bool contain_address_info>
inline auto redirect_handle(basic_socket_io_observer<ch_type,contain_address_info> soc)
{
	return soc.soc;
}
#endif

#if defined(__linux__)
template<std::integral ch_type,bool contain_address_info>
inline constexpr auto zero_copy_in_handle(basic_socket_io_observer<ch_type,contain_address_info> soc)
{
	return soc.soc;
}
template<std::integral ch_type,bool contain_address_info>
inline constexpr auto zero_copy_out_handle(basic_socket_io_observer<ch_type,contain_address_info> soc)
{
	return soc.soc;
}
#endif

template<std::integral ch_type,bool contain_address_info=true>
class basic_socket_io_handle:public basic_socket_io_observer<ch_type,contain_address_info>
{
public:
	using char_type = ch_type;
	using native_handle_type = sock::details::socket_type;
	constexpr basic_socket_io_handle()=default;
	constexpr basic_socket_io_handle(native_handle_type soc):basic_socket_io_observer<ch_type,contain_address_info>{soc}{}
	void close()
	{
		if(*this)[[likely]]
			sock::details::closesocket(this->native_handle());
	}
#if defined(__WINNT__) || defined(_MSC_VER)
	basic_socket_io_handle(basic_socket_io_handle const&)=delete;
	basic_socket_io_handle& operator=(basic_socket_io_handle const&)=delete;
#else
	basic_socket_io_handle(basic_socket_io_handle const& other):basic_socket_io_observer<ch_type,contain_address_info>{details::sys_dup(other.native_handle())}
	{
	}
	basic_socket_io_handle& operator=(basic_socket_io_handle const& other)
	{
		this->native_handle()=sys_dup2(other.native_handle(),this->native_handle());
		return *this;
	}
#endif
	constexpr basic_socket_io_handle(basic_socket_io_handle&& other) noexcept:basic_socket_io_observer<ch_type,contain_address_info>{other.release()}{}
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

template<std::integral ch_type,bool contain_address_info=true>
class basic_socket_file:public basic_socket_io_handle<ch_type,contain_address_info>
{
public:
	using char_type = ch_type;
	using native_handle_type = sock::details::socket_type;
	constexpr basic_socket_file()=default;
	constexpr basic_socket_file(native_handle_type soc):basic_socket_io_handle<ch_type,contain_address_info>{soc}{}
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
	basic_socket_file(native_interface_t,Args&& ...args):basic_socket_io_handle<ch_type,contain_address_info>(sock::details::socket(std::forward<Args>(args)...)){}
	basic_socket_file(sock::family family,sock::type type,sock::protocal protocal):basic_socket_io_handle<ch_type,contain_address_info>(sock::details::socket(static_cast<sock::details::address_family>(family),static_cast<int>(type),static_cast<int>(protocal))){}
};

template<std::integral char_type>
inline void connect(basic_socket_io_observer<char_type,true> siob)
{
	sock::details::connect(siob.soc,siob.addr.storage.sock,static_cast<std::size_t>(siob.addr.storage_size));
}

template<std::integral char_type>
inline void listen(basic_socket_io_observer<char_type,true>& siob)
{
	sock::details::bind(siob.soc,siob.addr.storage.sock,static_cast<std::size_t>(siob.addr.storage_size));
	sock::details::listen(siob.soc,10);
}

template<std::integral char_type,bool contain_address_info>
inline basic_socket_file<char_type,true> accept(basic_socket_io_observer<char_type,contain_address_info>& siob)
{
	basic_socket_file<char_type,true> bsf;
	bsf.native_handle()=sock::details::accept(siob.soc,bsf.addr.storage.sock,bsf.addr.storage_size);
	return bsf;
}

using socket_io_observer=basic_socket_io_observer<char>;
using socket_io_handle=basic_socket_io_handle<char>;
using socket_file=basic_socket_file<char>;

template<std::integral ch_type>
class basic_tcp:public basic_socket_file<ch_type>
{
public:
	using char_type = ch_type;
	using native_handle_type = sock::details::socket_type;
	template<typename T>
	requires requires(T t)
	{
		family(t);
	}
	basic_tcp(T const& addr,std::uint16_t port):basic_socket_file<ch_type>(family(addr),sock::type::stream,sock::protocal::none)
	{
		this->address()={to_socket_address_storage(addr,port),sizeof(sockaddr_storage)};
	}
	basic_tcp(std::uint16_t port):basic_tcp(fast_io::ipv4{},port){}
};

using tcp = basic_tcp<char>;

}