#include"../../../include/fast_io.h"
#include"../../../include/fast_io_device.h"
#include<filesystem>

inline void test(std::vector<std::string_view> const& views,std::size_t n,fast_io::native_io_observer log)
{
	println(log,"Testing n= ",n);
	auto data_size{fast_io::concat(n)};
	std::filesystem::create_directory("benchmarks");
	for(auto const e : views)
	{
		println(log,"Testing ",e);
		auto txt_file{fast_io::concat("benchmarks\\",e,".txt")};
		for(std::size_t i{};i!=10;++i)
		{
			fast_io::jprocess(e,{data_size},{.in=fast_io::in(),.out=log,.err=log},"benchmarks");
			std::filesystem::remove(txt_file);
		}
		println(log);
	}
	std::filesystem::remove_all("benchmarks");
	print(log,"\n\n\n");
}

int main()
{
	fast_io::onative_file log("log.txt");
	std::vector<std::string_view> views{"c_file","c_file_unlocked","filebuf_file","fstream","fwrite","obuf_file","obuf_file_mutex"};
	for(auto const e : views)
		fast_io::jprocess("D:\\msys64\\mingw64\\bin\\g++",{"-o",e,fast_io::concat(e,".cc"),"-Ofast","-std=c++20","-s","-march=native","-flto"},{.in=fast_io::in(),.out=log,.err=log});
	for(std::size_t n{1};n<1000000ULL;n*=10)
	{
		test(views,n,log);
	}
	for(std::size_t n{1000000ULL};n<=100000000ULL;n+=5000000ULL)
		test(views,n,log);
}