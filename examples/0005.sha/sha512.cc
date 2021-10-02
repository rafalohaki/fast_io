#include<fast_io.h>
#include<fast_io_device.h>
#include<fast_io_crypto.h>

/*
fast_io's own implementation. No need to install anything.
*/

int main(int argc,char** argv)
{
	using namespace fast_io::mnp;
	if(argc!=2)
	{
		perr("Usage: ",chvw(*argv)," <file>\n");
		return 1;
	}
	auto t0{fast_io::posix_clock_gettime(fast_io::posix_clock_id::realtime)};
	fast_io::sha512 md;
	fast_io::hash_processor processor(md);
	fast_io::ibuf_file ibf(argv[1]);
	auto transmitted{transmit(processor,ibf)};
	processor.do_final();
	println(md," *",chvw(argv[1]),"\nTransmitted:",transmitted," bytes\tElapsed Time:",fast_io::posix_clock_gettime(fast_io::posix_clock_id::realtime)-t0);
}