#include"../../timer.h"

int main()
{
	constexpr std::size_t N(10000000);
	{
	fast_io::timer t("output");
	fast_io::native_file nf("memory_map.txt",fast_io::open_mode::in|fast_io::open_mode::out|fast_io::open_mode::app|fast_io::open_mode::trunc);
	constexpr std::size_t preallocate_file_size{78888890};
	truncate(nf,preallocate_file_size);
	fast_io::native_memory_map_file mmf(nf,fast_io::file_map_attribute::read_write,preallocate_file_size,0);
	fast_io::omemory_map bomp(mmf);
	for(std::size_t i{};i!=N;++i)
		println(bomp,i);
	}
}