inline void test()
{
	for(std::size_t i{};i!=1000000;++i)
		std::cout<<i<<'\n';
}

int main()
{
	fast_io::u8obuf_file timer_obf(u"iostream.txt");
	auto t0{posix_clock_gettime(fast_io::posix_clock_id::monotonic)};
	test();
	print(timer_obf,posix_clock_gettime(fast_io::posix_clock_id::monotonic)-t0,u8"s\n");
}
