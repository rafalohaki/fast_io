#include"../../../include/fast_io.h"
#include"../../../include/fast_io_device.h"
#include"../../../include/fast_io_driver/openssl_driver.h"


int main()
{
	fast_io::openssl::bio_file bio("read.txt",fast_io::open_interface<fast_io::open_mode::binary|fast_io::open_mode::in>);
	for(std::size_t s;scan<true>(bio,s);println(s));
}
