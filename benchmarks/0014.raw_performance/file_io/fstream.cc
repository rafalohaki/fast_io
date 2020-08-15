#include"../../../include/fast_io.h"
#include"../../timer.h"
#include<fstream>

int main(int argc,char** argv)
{
	if(argc<2)
	{
		perr("Usage: ",fast_io::chvw(*argv)," N\n");
		return 1;
	}
	auto const N{fast_io::to<std::size_t>(std::string_view(argv[1]))};
	fast_io::timer tm("fstream");
	constexpr std::string_view view("Hello World\n");
	std::ofstream fout("fstream.txt",std::ofstream::binary);
	auto& rdbuf(*fout.rdbuf());
	for(std::size_t i{};i!=N;++i)
		rdbuf.sputn(view.data(),view.size());
}