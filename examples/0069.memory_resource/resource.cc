
#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"
#include<vector>
#include<random>

int main()
{
	fast_io::obuf_file obf("w.txt");
	fast_io::obuf_file obf2("q.txt");
	print(obf,"Hello World before idle\n");

//We can just recyle the unused space of output buffer as an allocator when it is idle.

/*
This proves I/O buffer is another piece of memory just as stack and heap
*/
	{
	fast_io::monotonic_io_buffer_resource io_resource(obf);
	std::pmr::unsynchronized_pool_resource up_resource(std::addressof(io_resource));
	std::pmr::vector<std::size_t> vec(std::addressof(up_resource));
	std::random_device device;
	for(std::size_t i{};i!=500;++i)
		vec.emplace_back(device());
	sort(vec.begin(),vec.end());
	for(auto const& e : vec)
		println(obf2,e);
	}

	print(obf,"Hello World after idle\n");
}