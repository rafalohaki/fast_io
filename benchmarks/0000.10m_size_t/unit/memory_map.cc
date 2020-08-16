#include"../../timer.h"
#include"../../../include/fast_io_device.h"


int main()
{
	constexpr std::size_t N(10000000);
	constexpr std::size_t preallocate_file_size{78888890};
	{
	fast_io::timer t("output");
	fast_io::native_file nf("memory_map.txt",fast_io::open_mode::in|fast_io::open_mode::out|fast_io::open_mode::app|fast_io::open_mode::trunc);
	truncate(nf,preallocate_file_size);
	fast_io::native_memory_map_file mmf(nf,fast_io::file_map_attribute::read_write,preallocate_file_size);
	fast_io::omemory_map bomp(mmf);
	for(std::size_t i{};i!=N;++i)
		println(bomp,i);
	}
	std::vector<std::size_t> vec(N);
	{
	fast_io::timer t("input");
	fast_io::native_file nf("memory_map.txt",fast_io::open_mode::in);
	fast_io::native_memory_map_file mmf(nf,fast_io::file_map_attribute::read_only,preallocate_file_size);
	fast_io::imemory_map bomp(mmf);
	for(std::size_t i{};i!=N;++i)
		scan(bomp,vec[i]);
	}
}