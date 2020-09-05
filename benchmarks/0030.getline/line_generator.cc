#include"../timer.h"
#include"../../include/fast_io_core_impl/scan_generator.h"
#include"../../include/fast_io_device.h"

int main()
{
	std::size_t count{};
	{
	fast_io::timer tm(__FILE__);

	fast_io::ibuf_file ibf("obuf_file.txt");
	for(auto const& e : line_generator(ibf))
		count+=e.size();
	}
	println("count is: ",count);
}
