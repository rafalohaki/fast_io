#include<fast_io.h>
#include<fast_io_device.h>
#include<fast_io_driver/cryptopp_driver.h>
#include<CryptoPP/sha.h>

/*
Need to install CryptoPP first
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
	CryptoPP::SHA256 sha;
	fast_io::cryptopp::iterated_hash_ref ref(sha);
	fast_io::ibuf_file ibf(::fast_io::mnp::os_c_str(argv[1]));
	auto transmitted{transmit(ref,ibf)};
	println(upper(do_final(ref))," *",chvw(argv[1]),"\nTransmitted:",transmitted," bytes\tElapsed Time:",fast_io::posix_clock_gettime(fast_io::posix_clock_id::realtime)-t0);
}