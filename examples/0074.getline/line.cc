#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"

int main()
{
	fast_io::ibuf_file ibf("lines.txt");
	for(std::string str;scan<true>(ibf,fast_io::line(str));println(str));
}
