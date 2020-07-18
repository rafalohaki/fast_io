#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"
#include"../../include/fast_io_async.h"

inline fast_io::task io_task(fast_io::async_observer iocp)
{
	fast_io::native_file file(fast_io::io_async,iocp,"native.txt",fast_io::open_mode::out|fast_io::open_mode::binary|fast_io::open_mode::no_block);
	co_await fast_io::async_println(iocp,file,"Hello World ",5," ",6," ",8," ",9," ",4.2);
}

int main()
{
	using namespace std::chrono_literals;
	fast_io::async_scheduler iocp(fast_io::io_async);
	io_task(iocp);
	for(;;fast_io::io_async_wait_timeout(iocp,1ms));
}