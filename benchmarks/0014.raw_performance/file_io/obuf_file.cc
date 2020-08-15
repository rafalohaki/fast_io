#include"../../../include/fast_io.h"
#include"../../../include/fast_io_device.h"
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
	{
	fast_io::timer tm("obuf_file");
	constexpr std::string_view view("Hello World\n");
	fast_io::obuf_file obf("obuf_file.txt");
	for(std::size_t i{};i!=N;++i)
		print(obf,view);
	}
}