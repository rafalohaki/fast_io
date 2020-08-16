#include"../../include/fast_io.h"

int main()
{
	fast_io::native_file nfl("mem_map.txt",fast_io::open_mode::in|fast_io::open_mode::out|fast_io::open_mode::app|fast_io::open_mode::trunc);
	constexpr std::size_t preallocate_file_size{10000};
	truncate(nfl,preallocate_file_size);		//This means extend not truncate.
	fast_io::native_memory_map_file mpf(nfl,fast_io::file_map_attribute::read_write,preallocate_file_size,0);
	fast_io::omemory_map omp{mpf};
	println(omp,"Hello World\n");
	mpf.close();
	truncate(nfl,omp.written_bytes());	//truncate after we know how many bytes we've written
}
