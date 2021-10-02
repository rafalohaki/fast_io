#include<fast_io.h>
#include<fast_io_device.h>
#include<random>

int main()
{
	fast_io::obuf_file obf("graph.txt");
	std::size_t m = 600000;
	std::size_t n = 50000;
	println(obf,m," ",n);
	std::mt19937_64 rg{ std::random_device{}() };
	std::uniform_int_distribution<std::size_t> node_dist{ 0, n - 1 };
	std::uniform_int_distribution<std::size_t> weight_dist{ 0, 1000000 };
	for (std::size_t i = 0; i < m; ++i)
		println(obf,node_dist(rg)," ",node_dist(rg)," ",weight_dist(rg));
}