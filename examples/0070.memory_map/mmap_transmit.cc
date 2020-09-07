#include"../../include/fast_io.h"

int main()
{
	fast_io::native_file nfl("mem_map_transmit.txt",fast_io::open_mode::in|fast_io::open_mode::out|fast_io::open_mode::app|fast_io::open_mode::trunc);
	constexpr std::size_t preallocate_file_size{10};
	truncate(nfl,preallocate_file_size);		//This means extend not truncate.
	fast_io::native_memory_map_file mpf(nfl,fast_io::file_map_attribute::read_write,preallocate_file_size,0);
	fast_io::omemory_map omp{mpf};		//omemory_map does not check any bounds
	println("transmitted:",transmit(omp,fast_io::c_stdin()));		//This is safe since transmit function directly dumps bytes to mmap region. It at most transmits 10 bytes
	mpf.close();
	truncate(nfl,omp.written_bytes());	//truncate after we know how many bytes we've written
}
