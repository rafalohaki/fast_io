#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"
#include"../../include/fast_io_async.h"
#include"../../include/fast_io_network.h"


inline fast_io::task echo(fast_io::io_async_observer ioa,auto acceptor)
{
	std::array<char,4096> buffer;
	co_await async_write(ioa,acceptor,buffer.begin(),co_await async_read(ioa,acceptor,buffer.begin(),buffer.end()));
}

inline fast_io::task io_task(fast_io::io_async_observer ioa)
{
	for(fast_io::tcp_server server(2000);;)
		echo(ioa,co_await async_accept(ioa,server));
}

int main()
{
	using namespace std::chrono_literals;
	fast_io::io_async_scheduler ioa(fast_io::io_async);
	io_task(ioa);
	for(;;fast_io::io_async_wait_timeout(ioa,1s));
}
