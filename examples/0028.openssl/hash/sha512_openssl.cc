#include"../../../include/fast_io.h"
#include"../../../include/fast_io_device.h"
#include"../../../include/fast_io_driver/openssl_driver.h"

int main(int argc,char** argv)
try
{
	if(argc!=2)
	{
		perr("Usage: ",fast_io::chvw(*argv)," <file>\n");
		return 1;
	}
	auto t0{std::chrono::high_resolution_clock::now()};
	fast_io::openssl::sha512 sha;
	fast_io::ibuf_file ibf(argv[1]);
	auto transmitted{transmit(sha,ibf)};
	println(sha.do_final()," *",fast_io::chvw(argv[1]),"\nTransmitted:",transmitted,u8" bytes\tElapsed Time:",std::chrono::high_resolution_clock::now()-t0);
}
catch(std::exception const& e)
{
	perrln(e);
	return 2;
}