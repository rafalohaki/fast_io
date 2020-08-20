#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"
#include<iostream>
#include"../../include/fast_io_legacy.h"
int main()
{
	fast_io::streambuf_io_observer siob{std::cout.rdbuf()};
	println("file type of std::cout is ",type(siob));
}