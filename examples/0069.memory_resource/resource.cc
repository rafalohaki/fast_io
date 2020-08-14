
#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"
#include<vector>

int main()
{
	fast_io::obuf_file obf("w.txt");
	fast_io::obuf_file obf2(obf);
	print(obf,"Hello World\n");

//We can just recyle the unused space of output buffer as an allocator when it is idle.

/*
This proves I/O buffer is another piece of memory just as stack and heap
*/

	fast_io::monotonic_io_buffer_resource io_resource(obf);
	std::pmr::vector<std::size_t> vec(std::addressof(io_resource));
	for(std::size_t i{};i!=500000;++i)
		vec.emplace_back(i);
	for(auto const& e : vec)
		println(obf2,e);
}