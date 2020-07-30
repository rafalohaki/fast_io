#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"
#include"../../include/fast_io_async.h"
#include"../../include/fast_io_legacy.h"
#include<fstream>
//probably only works for Linux
inline fast_io::task io_task(fast_io::io_async_observer ioa)
{
	std::ofstream fout("ofstream.txt",std::ofstream::binary);
	fast_io::filebuf_io_observer fiob{fout.rdbuf()};
	co_await fast_io::async_println(ioa,fiob,"Hello World ",5," ",6," ",8," ",9," ",4.2);
}

int main()
{
	using namespace std::chrono_literals;
	fast_io::io_async_scheduler ioa(fast_io::io_async);
	io_task(ioa);
	for(;;fast_io::io_async_wait_timeout(ioa,1ms));
}