#pragma once

namespace fast_io
{

template<stream stm>
class acceptor:public stm
{
public:
	using char_type = typename stm::char_type;
	using native_handle_type = typename stm::native_handle_type;
	template<acceptance_stream lisn>
	constexpr acceptor(lisn& lsn):stm(accept(lsn)){}
};

template<acceptance_stream lisn> acceptor(lisn& lsn) -> acceptor<decltype(accept(lsn))>;


template<connection_stream stm>
class basic_client:public stm
{
public:
	using char_type = typename stm::char_type;
	using native_handle_type = typename stm::native_handle_type;
	template<typename... Args>
	requires std::constructible_from<stm,Args...>
	constexpr basic_client(Args&& ...args):stm(std::forward<Args>(args)...)
	{
		connect(*this);
	}
};

template<listener_stream stm>
class basic_server:public stm
{
public:
	using char_type = typename stm::char_type;
	using native_handle_type = typename stm::native_handle_type;
	template<typename... Args>
	requires std::constructible_from<stm,Args...>
	constexpr basic_server(Args&& ...args):stm(std::forward<Args>(args)...)
	{
		listen(*this);
	}
};

}