#include<fast_io.h>
#include<fast_io_device.h>
#include<fast_io_driver/timer.h>
#include<vector>

int main()
{
	constexpr std::size_t N(10000000);
	{
	fast_io::timer t(u8"output");
	fast_io::obuf_file obf(u8"iobuf_file_bin.txt");
	for(std::size_t i{};i!=N;++i)
	{
		write(obf,__builtin_addressof(i),__builtin_addressof(i)+1);
	}
	}
	std::vector<std::size_t> vec(N);
	{
	fast_io::timer t(u8"input");
	fast_io::ibuf_file ibf(u8"iobuf_file_bin.txt");
	for(std::size_t i{};i!=N;++i)
	{
		auto it{read(ibf,vec.data()+i,vec.data()+(i+1))};
		if(it!=vec.data()+(i+1))
		{
			fast_io::fast_terminate();
		}
	}
//		scan(ibf,vec[i]);
	}
}
