#include"../../../include/fast_io.h"
#include"../../timer.h"
#include<fstream>

int main()
{
	constexpr std::size_t N{
#ifdef FAST_IO_BENCHMARK_DATA_SIZE
FAST_IO_BENCHMARK_DATA_SIZE
#else
100000000
#endif
};
	fast_io::timer tm("fstream");
	constexpr std::string_view view("Hello World\n");
	std::ofstream fout("fstream.txt",std::ofstream::binary);
	auto& rdbuf(*fout.rdbuf());
	for(std::size_t i{};i!=N;++i)
		rdbuf.sputn(view.data(),view.size());
}