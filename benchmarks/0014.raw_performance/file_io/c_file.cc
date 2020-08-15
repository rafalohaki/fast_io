#include"../../../include/fast_io.h"
#include"../../timer.h"

int main()
{
	constexpr std::size_t N{
#ifdef FAST_IO_BENCHMARK_DATA_SIZE
FAST_IO_BENCHMARK_DATA_SIZE
#else
100000000
#endif
};
	fast_io::timer tm("c_file");
	constexpr std::string_view view("Hello World\n");
	fast_io::c_file cfl("c_file.txt","wb");
	for(std::size_t i{};i!=N;++i)
		print(cfl,view);
}