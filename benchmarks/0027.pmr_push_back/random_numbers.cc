#include"../../include/fast_io_device.h"
#include<random>

int main()
{
	fast_io::obuf_file obf("random_numbers.txt");
	std::mt19937_64 eng;
	constexpr std::size_t n{1000000};
	for(std::size_t i{};i!=n;++i)
		println(obf,eng());
}