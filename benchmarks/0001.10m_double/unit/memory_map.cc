#include"../../timer.h"
#include"../../../include/fast_io.h"
#include"../../../include/fast_io_device.h"
//#include"../../include/fast_io_crypto.h"
#include"../../../include/fast_io_legacy.h"
#include<random>

int main()
{
	constexpr std::size_t N(10000000);
	std::vector<double> vec;
	vec.reserve(N);
	std::mt19937_64 eng;
	std::uniform_real_distribution dis(-10000.0,10000.0);
	for(std::size_t i(0);i!=N;++i)
		vec.emplace_back(dis(eng));
	std::size_t written_bytes{};
	{
	fast_io::timer t("output");
	fast_io::native_file nf("memory_map.txt",fast_io::open_mode::in|fast_io::open_mode::out|fast_io::open_mode::app|fast_io::open_mode::trunc);
	truncate(nf,300000000);
	fast_io::native_memory_map_file mmf(nf,fast_io::file_map_attribute::read_write,300000000);
	fast_io::omemory_map bomp(mmf);
	for(std::size_t i{};i!=N;++i)
		println(bomp,vec[i]);
	written_bytes=bomp.written_bytes();
	mmf.close();
	truncate(nf,written_bytes);
	}
	std::vector<double> buffer(N);
	{
	fast_io::timer t("input");
	fast_io::native_file nf("memory_map.txt",fast_io::open_mode::in);
	fast_io::native_memory_map_file mmf(nf,fast_io::file_map_attribute::read_only,written_bytes);
	fast_io::imemory_map bomp(mmf);
	for(std::size_t i{};i!=N;++i)
		scan(bomp,buffer[i]);
	}
}
