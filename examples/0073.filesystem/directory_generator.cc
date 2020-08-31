#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"

int main()
{
	fast_io::directory dir(".");
	for(auto const& e : generator(dir))
		println(filename(e));
}