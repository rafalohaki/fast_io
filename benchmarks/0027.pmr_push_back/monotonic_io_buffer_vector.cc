
#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"
#include<vector>
#include"../timer.h"

int main()
{
	fast_io::timer t("pmr");
	fast_io::ibuf_file ibf("random_numbers.txt");
	fast_io::obuf_file obf("result_monotonic_io_buffer_vector.txt");
	fast_io::monotonic_io_buffer_resource obf_resource(obf);
	std::pmr::vector<std::size_t> vec(std::addressof(obf_resource));
	for(std::size_t value{};scan<true>(ibf,value);vec.emplace_back(value));
	std::size_t val{};
	scan(ibf,val);
	fast_io::monotonic_io_buffer_resource ibf_resource(ibf);
	std::pmr::vector<std::size_t> vec2(std::move(vec),std::addressof(ibf_resource));
	for(auto const & e : vec2)
		println(obf,e);
}