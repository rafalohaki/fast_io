#include"../../../include/fast_io.h"
#include"../../timer.h"

int main(int argc,char** argv)
{
	if(argc<2)
	{
		perr("Usage: ",fast_io::chvw(*argv)," N\n");
		return 1;
	}
	auto const N{fast_io::to<std::size_t>(std::string_view(argv[1]))};
	fast_io::timer tm("c_file_unlocked");
	constexpr std::string_view view("Hello World\n");
	fast_io::c_file_unlocked cfl("c_file_unlocked.txt","wb");
	for(std::size_t i{};i!=N;++i)
		print(cfl,view);
}