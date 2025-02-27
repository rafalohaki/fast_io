﻿#include<fast_io.h>
#include<fast_io_device.h>
#include<random>

int main()
{
	fast_io::u8ibuf_file ibuf(u"freq.txt");
	fast_io::details::naive_vector<char8_t> vec;
	{
		char8_t c;
		for(std::size_t f;scan<true>(ibuf,fast_io::mnp::ch_get(c),f);)
			for(std::size_t i{};i!=f;++i)
				vec.emplace_back(fast_io::char_category::to_c_lower(c));
	}
	if(vec.empty())
	{
		perr("No frequency\n");
		return 1;
	}
	fast_io::ibuf_white_hole_engine eng;
	std::uniform_int_distribution<std::size_t> ud(0,vec.size()-1);
	std::uniform_int_distribution<std::size_t> rlen(6,12);
	fast_io::u8obuf_file obf(u"name.txt");
	for(std::size_t i(0);i!=1000000;++i)
	{
		for(std::size_t j(0),s(rlen(eng));j!=s;++j)
			print(obf,fast_io::mnp::chvw(vec[ud(eng)]));
		println(obf);
	}		
}
