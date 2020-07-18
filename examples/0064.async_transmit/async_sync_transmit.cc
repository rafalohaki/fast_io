#include"../../include/fast_io.h"
#include"../../include/fast_io_async.h"

inline fast_io::task io_task(fast_io::io_async_observer ioa)
{
	fast_io::native_file in_file("in.txt",fast_io::open_mode::in|fast_io::open_mode::binary);
	fast_io::native_file out_file(fast_io::io_async,ioa,"out.txt",fast_io::open_mode::out|fast_io::open_mode::binary|fast_io::open_mode::no_block);
	co_await async_sync_transmit(ioa,out_file,in_file);
}

int main()
{
	using namespace std::chrono_literals;
	fast_io::io_async_scheduler ioa(fast_io::io_async);
	io_task(ioa);
	for(;;fast_io::io_async_wait_timeout(ioa,1ms));
}