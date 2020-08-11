#pragma once

//fast_io_network.h deals with sockets
#include<thread>
#include<condition_variable>
#include"fast_io_hosted.h"
#if !defined(__WINNT__) && !defined(_MSC_VER)
#include <netinet/in.h>
#endif
#if __cpp_impl_coroutine >= 201902
#include<coroutine>
#include"fast_io_hosted/async_coro.h"
#include"fast_io_freestanding_impl/generator.h"
#endif
#include"fast_io_network/network.h"

namespace fast_io
{

using ibuf_tcp_client = basic_ibuf<tcp_client>;
template<std::integral char_type>
using basic_ibuf_acceptor = basic_ibuf<acceptor<char>>;
using ibuf_acceptor = basic_ibuf_acceptor<char>;

}