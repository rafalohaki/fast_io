#pragma once
#include"win32_library.h"


namespace fast_io::sock::details
{

inline fast_io::details::win32_library const ws2_32_dll(L"ws2_32.dll");

inline fast_io::details::win32_library const mswsock_dll(L"mswsock.dll");

using address_family = std::int16_t;


using socket_type = std::uint64_t;
inline constexpr socket_type invalid_socket(~0);

template<typename prototype>
inline auto get_proc_address(char const* proc)
{
	auto address(::GetProcAddress(ws2_32_dll.get(),proc));
	if(address==nullptr)
		throw_win32_error();
	return bit_cast<prototype>(address);
}

inline auto get_last_error()
{
	auto func{get_proc_address<int(*)()>("WSAGetLastError")};
	return func();
}

template<typename prototype>
inline auto get_proc_address_mswsock(char const* proc)
{
	auto address(::GetProcAddress(mswsock_dll.get(),proc));
	if(address==nullptr)
		throw_win32_error();
	return bit_cast<prototype>(address);
}

template<typename prototype,typename ...Args>
inline auto call_win32_ws2_32(char const *name,Args&& ...args)
{
	auto ret(get_proc_address<prototype>(name)(std::forward<Args>(args)...));
	if(ret==-1)
		throw_win32_error(get_last_error());
	return ret;
}

template<typename prototype,typename ...Args>
inline auto call_win32_ws2_32_invalid_socket(char const *name,Args&& ...args)
{
	auto ret(get_proc_address<prototype>(name)(std::forward<Args>(args)...));
	if(ret==invalid_socket)
		throw_win32_error(get_last_error());
	return ret;
}

template<typename prototype,typename ...Args>
inline auto call_win32_ws2_32_minus_one(char const *name,Args&& ...args)
{
	auto ret(get_proc_address<prototype>(name)(std::forward<Args>(args)...));
	if(ret==-1)
		throw_win32_error(get_last_error());
	return ret;
}

template<typename prototype,typename ...Args>
inline auto call_win32_ws2_32_nullptr(char const *name,Args&& ...args)
{
	auto ret(get_proc_address<prototype>(name)(std::forward<Args>(args)...));
	if(ret==nullptr)
		throw_win32_error(get_last_error());
	return ret;
}

template<typename ...Args>
inline auto socket(Args&& ...args)
{
	return call_win32_ws2_32_invalid_socket<decltype(::socket)*>("socket",std::forward<Args>(args)...);
}

template<typename T>
inline auto accept(SOCKET sck,T& sock_address,int& storage_size)
{
	return call_win32_ws2_32_invalid_socket<decltype(::accept)*>("accept",sck,reinterpret_cast<sockaddr*>(std::addressof(sock_address)),std::addressof(storage_size));
}
/*
template<typename T>
inline auto accept_ex(SOCKET sck,T& sock_address,int& storage_size)
{
	WSAOVERLAPPED overlapped{};
	return call_win32_ws2_32_invalid_socket<decltype(::AcceptEx)*>("AcceptEx",sck,reinterpret_cast<sockaddr*>(std::addressof(sock_address)),std::addressof(storage_size),std::addressof(overlapped));
}
*/
template<typename T,std::unsigned_integral sock_type_size>
inline auto connect(SOCKET sck,T& sock_address,sock_type_size size)
{
	return call_win32_ws2_32_minus_one<decltype(::connect)*>("connect",sck,reinterpret_cast<sockaddr*>(std::addressof(sock_address)),static_cast<int>(size));
}

template<typename mem_address,typename ...Args>
inline auto send(SOCKET sock,mem_address const* add,Args&& ...args)
{
	return call_win32_ws2_32<decltype(::send)*>("send",sock,static_cast<char const*>(static_cast<void const*>(add)),std::forward<Args>(args)...);
}
template<typename mem_address,typename ...Args>
inline auto recv(SOCKET sock,mem_address* add,Args&& ...args)
{
	return call_win32_ws2_32<decltype(::recv)*>("recv",sock,static_cast<char*>(static_cast<void*>(add)),std::forward<Args>(args)...);
}



template<typename ...Args>
inline auto wsasend(Args&& ...args)
{
	return call_win32_ws2_32<decltype(::WSASend)*>("WSASend",std::forward<Args>(args)...);
}

template<typename ...Args>
inline auto wsasendmsg(Args&& ...args)
{
	return call_win32_ws2_32<decltype(::WSASendMsg)*>("WSASendMsg",std::forward<Args>(args)...);
}
/*
inline auto my_sendmsg(SOCKET sck,message_hdr const* ptr,std::uint32_t flags)
{
	
}
*/
template<typename ...Args>
inline void closesocket_ignore_error(Args&& ...args) noexcept
{
	auto func{::GetProcAddress(ws2_32_dll.get(),"closesocket")};
	if(func==nullptr)[[unlikely]]
		fast_terminate();
	bit_cast<int __stdcall(*)(socket_type)>(func)(std::forward<Args>(args)...);
}

template<typename ...Args>
inline auto closesocket(Args&& ...args)
{
	return call_win32_ws2_32<decltype(::closesocket)*>("closesocket",std::forward<Args>(args)...);
}

template<typename T,std::unsigned_integral sock_type_size>
inline auto bind(SOCKET sck,T& sock_address,sock_type_size size)
{
	return call_win32_ws2_32_minus_one<decltype(::bind)*>("bind",sck,reinterpret_cast<sockaddr*>(std::addressof(sock_address)),static_cast<int>(size));
}

template<typename ...Args>
inline auto listen(Args&& ...args)
{
	return call_win32_ws2_32<decltype(::listen)*>("listen",std::forward<Args>(args)...);
}


template<typename ...Args>
inline void getaddrinfo(Args&& ...args)
{
	auto ec(get_proc_address<decltype(::getaddrinfo)*>("getaddrinfo")(std::forward<Args>(args)...));
	if(ec)
		throw_win32_error();
}

template<typename ...Args>
inline void freeaddrinfo(Args&& ...args)
{
	get_proc_address<decltype(::freeaddrinfo)*>("freeaddrinfo")(std::forward<Args>(args)...);
}

class win32_startup
{
public:
	win32_startup()
	{
		auto WSAStartup(reinterpret_cast<decltype(::WSAStartup)*>(reinterpret_cast<void(*)()>(::GetProcAddress(ws2_32_dll.get(),"WSAStartup"))));
		WSADATA data;
		if(auto error_code=WSAStartup(2<<8|2,std::addressof(data)))
			throw_win32_error(error_code);
	}
	win32_startup(win32_startup const&) = delete;
	win32_startup& operator=(win32_startup const&) = delete;
	~win32_startup()
	{
		auto WSACleanup(reinterpret_cast<decltype(::WSACleanup)*>(reinterpret_cast<void(*)()>(::GetProcAddress(ws2_32_dll.get(),"WSACleanup"))));
		WSACleanup();
	}
};

inline win32_startup const startup;

}

namespace fast_io::details
{
//zero copy IO for win32
template<bool rac=false, zero_copy_output_stream output,zero_copy_input_stream input>
inline std::uintmax_t zero_copy_transmit_once(output& outp,input& inp,std::uintmax_t bytes,std::int64_t offset)
{
	if constexpr(rac)
	{
		fast_io::win32::overlapped ov{};
		memcpy(std::addressof(ov),std::addressof(offset),sizeof(std::int64_t));
	if(!((fast_io::sock::details::get_proc_address_mswsock<decltype(fast_io::win32::TransmitFile)*>
		("TransmitFile"))(zero_copy_out_handle(outp),zero_copy_in_handle(inp),bytes,0,std::addressof(ov),nullptr,32/*TF_USE_DEFAULT_WORKER*/)))
		throw_win32_error();
	}
	else
	{
	if(!((fast_io::sock::details::get_proc_address_mswsock<decltype(fast_io::win32::TransmitFile)*>
		("TransmitFile"))(zero_copy_out_handle(outp),zero_copy_in_handle(inp),bytes,0,nullptr,nullptr,0/*TF_USE_DEFAULT_WORKER*/)))
		throw_win32_error();
	}
	return bytes;
}


template<bool rac=false,zero_copy_output_stream output,zero_copy_input_stream input>
inline std::uintmax_t zero_copy_transmit(output& outp,input& inp,std::int64_t offset,std::uintmax_t bytes)
{
	constexpr std::uintmax_t maximum_transmit_bytes(2147483646);
	std::uintmax_t transmitted{};
	for(;bytes;)
	{
		std::uintmax_t should_transfer(maximum_transmit_bytes);
		if(bytes<should_transfer)
			should_transfer=bytes;
		std::uintmax_t transferred_this_round(details::zero_copy_transmit_once<rac>(outp,inp,should_transfer,offset));
		transmitted+=transferred_this_round;
		if(transferred_this_round!=should_transfer)
			return transmitted;
		bytes-=transferred_this_round;
	}
	return transmitted;
	
}
template<bool rac=false,zero_copy_output_stream output,zero_copy_input_stream input>
inline std::uintmax_t zero_copy_transmit(output& outp,input& inp,std::int64_t offset)
{
	constexpr std::uintmax_t maximum_transmit_bytes(2147483646);
	for(std::uintmax_t transmitted{};;)
	{
		std::uintmax_t transferred_this_round(details::zero_copy_transmit_once<rac>(outp,inp,maximum_transmit_bytes,offset));
		transmitted+=transferred_this_round;
		if(transferred_this_round!=maximum_transmit_bytes)
			return transmitted;
	}
}
}
