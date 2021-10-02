﻿#include<fast_io.h>
#include<fast_io_device.h>
#include<fast_io_driver/cryptopp_driver.h>
#include<CryptoPP/MD5.h>

/*
This is just for demo purpose. You should avoid md5 in general because it is insecure.
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
	CryptoPP::MD5 sha;
	fast_io::cryptopp::iterated_hash_ref ref(sha);
	fast_io::ibuf_file ibf(argv[1]);
	auto transmitted{transmit(ref,ibf)};
	println(upper(do_final(ref))," *",chvw(argv[1]),"\nTransmitted:",transmitted," bytes\tElapsed Time:",fast_io::posix_clock_gettime(fast_io::posix_clock_id::realtime)-t0);
}