#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"
#include"../../include/fast_io_async.h"
#include"../../include/fast_io_network.h"

inline fast_io::task io_task(fast_io::io_async_observer ioa)
{
	fast_io::async_tcp_client client(fast_io::ipv4{127,0,0,1},2000);
	co_await async_connect(ioa,client);
	co_await async_println(ioa,client,"Hello World from async client\n",5," d",8);
	std::array<char,512> buffer;
	std::size_t sz{co_await async_read(ioa,client,{buffer.data(),buffer.size()})};
	write(fast_io::out(),buffer.begin(),buffer.begin()+sz);
}

int main()
{
	using namespace std::chrono_literals;
	fast_io::io_async_scheduler ioa(fast_io::io_async);
	io_task(ioa);
	for(;;fast_io::io_async_wait_timeout(ioa,1ms));
}
