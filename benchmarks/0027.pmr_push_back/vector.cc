
#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"
#include<vector>
#include"../timer.h"

int main()
{
	fast_io::timer t("vector");
	fast_io::ibuf_file ibf("random_numbers.txt");
	fast_io::obuf_file obf("vector.txt");
	std::vector<std::size_t> vec;
	for(std::size_t value{};scan<true>(ibf,value);vec.emplace_back(value));
	for(auto const & e : vec)
		println(obf,e);
}