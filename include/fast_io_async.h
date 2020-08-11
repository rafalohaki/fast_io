#pragma once

#if defined(__linux__) || defined(_WIN32)
#include"fast_io_hosted.h"

#if defined(__linux__)
#include"fast_io_driver/liburing.h"
#else
#include"fast_io_hosted/platforms/win32_scheduling.h"
#endif

#include<coroutine>
#include"fast_io_hosted/async_coro.h"
#include"fast_io_freestanding_impl/generator.h"

#if defined(__linux__)
#include"fast_io_network.h"
#include"fast_io_driver/iouring_driver/socket.h"
#include"fast_io_network/async_connections.h"
#endif
//#include"fast_io_hosted/async_transmit.h"

namespace fast_io
{


#if defined(__WINNT__) || defined(_MSC_VER)
using io_async_observer=win32_io_observer;
using io_async_scheduler=win32_file;
#elif defined(__linux__)
using io_async_observer=io_uring_observer;
using io_async_scheduler=io_uring;
#endif

}

#endif
