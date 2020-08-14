#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"
#include<vector>
#include"../timer.h"
int main()
{
	fast_io::timer t("pmr");
	fast_io::obuf_file obf("unsync_pool_result_monotonic_io_buffer_vector.txt");
	print(obf,"Hello World\n");
	std::size_t sum{};
	{
	fast_io::monotonic_io_buffer_resource obf_resource(obf);
	std::pmr::unsynchronized_pool_resource pool(std::addressof(obf_resource));
	for(std::size_t i{};i!=10000000;++i)
	{
		std::pmr::vector<std::size_t> vec({1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,7,8,9,10},std::addressof(pool));
		std::pmr::vector<std::size_t> vec2({1,2,3,4,5,6,7,8,9,10,1,2,3,4,5,6,7,8,9,10},std::addressof(pool));
		sum+=vec.size();
		sum+=vec2.size();
	}
	}
	println(obf,"sum is: ",sum);
}