#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"
#include"../../include/fast_io_driver/liburing.h"

int main()
{
	fast_io::io_uring ior(fast_io::io_async);
	fast_io::onative_file onf("io_uring.txt");
	std::array<fast_io::io_scatter_t,1> scatters{fast_io::io_scatter_t{const_cast<char*>("Hello World"),11}};
	async_scatter_write_callback(ior,onf,scatters,[]
	{
		::debug_print("succ!");
	});
}
