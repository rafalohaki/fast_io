#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"
#include"../../include/fast_io_driver/liburing.h"
#include<coroutine>
#include"../../include/fast_io_hosted/async_coro.h"

inline fast_io::task io_task(fast_io::io_uring_observer ior)
{
	fast_io::onative_file nv("test.txt");
	std::array<fast_io::io_scatter_t,1> scatter{{const_cast<char*>("Hello World\n"),12}};
	co_await fast_io::async_scatter_write(ior,nv,scatter);
	
}

int main()
{
	fast_io::io_uring ior(fast_io::io_async);
	std::vector<std::jthread> pools;
	pools.reserve(std::thread::hardware_concurrency());
	pools.emplace_back([&](
	{
		fast_io::linux::single_thread_io_async_scheduling(ior);
	});
	io_task(ior);
}
