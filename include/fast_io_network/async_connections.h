#pragma once

namespace fast_io
{
template<std::integral char_type>
class async_connect_coroutine
{
public:
	io_async_observer ioa{};
	basic_socket_io_observer<char_type> socket_iob{};
	endpoint& ep;
	typename io_async_overlapped_t<basic_socket_io_observer<char_type>>::type overlapped;
	constexpr bool await_ready() const { return false; }
	constexpr void await_resume() const { }
	void await_suspend(std::coroutine_handle<> handle)
	{
		overlapped=typename io_async_overlapped_t<basic_socket_io_observer<char_type>>::type(std::in_place,[handle,this](std::size_t calb)
		{
			handle.resume();
		});
		async_connect_callback(ioa,socket_iob,ep,overlapped);
	}
};

template<std::integral char_type>
inline constexpr async_connect_coroutine<char_type> async_connect(io_async_observer ioa,basic_socket_io_observer<char_type> siob,endpoint& ep)
{
	return {ioa,siob,ep};
}

template<std::integral char_type>
inline constexpr async_connect_coroutine<char_type> async_connect(io_async_observer ioa,basic_async_tcp_client<char_type>& siob)
{
	return {ioa,siob.native_handle(),siob.ep};
}

template<std::integral char_type>
class async_accept_coroutine
{
public:
	io_async_observer ioa{};
	basic_socket_io_observer<char_type> socket_iob{};
	basic_async_acceptor<char_type> async_acpt{};
	typename io_async_overlapped_t<basic_socket_io_observer<char_type>>::type overlapped;
	constexpr bool await_ready() const { return false; }
	constexpr basic_async_acceptor<char_type> await_resume() const { return std::move(async_acpt);}
	void await_suspend(std::coroutine_handle<> handle)
	{
		overlapped=typename io_async_overlapped_t<basic_socket_io_observer<char_type>>::type(std::in_place,[handle,this](std::size_t calb)
		{
			this->async_acpt.native_handle()=calb;
			handle.resume();
		});
		async_accept_callback(ioa,socket_iob,async_acpt.ep,overlapped);
	}
};

template<std::integral char_type>
inline constexpr async_accept_coroutine<char_type> async_accept(io_async_observer ioa,basic_socket_io_observer<char_type> siob)
{
	return {ioa,siob};
}


}
