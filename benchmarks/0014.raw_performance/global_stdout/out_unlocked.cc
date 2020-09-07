#include"../../../include/fast_io.h"
#include"../../timer.h"

int main()
{
	constexpr std::size_t N{100000000};
	fast_io::timer tm("print");
	std::string view("Hello World\n");
	fast_io::c_io_observer ciob{stdout};
	std::lock_guard lg{ciob};
	fast_io::c_io_observer_unlocked ciolc{stdout};
	for(std::size_t i{};i!=N;++i)
		print(ciolc,view);
}