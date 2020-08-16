#include"../../../include/fast_io.h"
#include"../../../include/fast_io_device.h"
#include"../../timer.h"

int main(int argc,char** argv)
{
	if(argc<2)
	{
		perr("Usage: ",fast_io::chvw(*argv)," N\n");
		return 1;
	}
	auto const N{fast_io::to<std::size_t>(std::string_view(argv[1]))};
	fast_io::timer tm("omemory_map");
	constexpr std::string_view view("Hello World\n");
	fast_io::native_file nfl("omemory_map.txt",fast_io::open_mode::in|fast_io::open_mode::out|fast_io::open_mode::app|fast_io::open_mode::trunc|fast_io::open_mode::creat);
	std::size_t const file_size{view.size()*N};
	truncate(nfl,file_size);		//truncate will increase file size
	fast_io::native_memory_map_file mmf(nfl,fast_io::file_map_attribute::read_write,file_size,0);
	fast_io::omemory_map bomp(mmf);
	for(std::size_t i{};i!=N;++i)
		print(bomp,view);
/*
	mmf.close();
	truncate(nfl,bomp.written_bytes());we have precomputed the maximum size. there is no need to retruncate again
*/
}
