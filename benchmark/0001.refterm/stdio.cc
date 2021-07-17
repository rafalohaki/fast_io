#include<fast_io.h>

int main()
{
	fast_io::u8obuf_file timer_obf(u"iostream.txt");
	auto t0{posix_clock_gettime(fast_io::posix_clock_id::monotonic)};
	{
	for(std::size_t i{};i!=1000000;++i)
		printf("%zu\n",i);
	}
	print(timer_obf,posix_clock_gettime(fast_io::posix_clock_id::monotonic)-t0,u8"s\n");
}
