﻿#include<fast_io.h>
#include<fast_io_device.h>

int main(int argc,char** argv)
#ifdef __cpp_exceptions
try
#endif
{
	using namespace fast_io::mnp;
	if(argc!=3)
	{
		perr("Usage: ",chvw(*argv)," <input UTF8 file> <output UTF-32 file>\n");
		return 1;
	}
	auto t0{fast_io::posix_clock_gettime(fast_io::posix_clock_id::realtime)};
	fast_io::u16iutf8_file file(argv[1]);//This will do codecvt from UTF-8 to UTF-16
	fast_io::obuf_file obf(argv[2],fast_io::open_mode::excl);
	auto transmitted{transmit(obf,file)};
	println(chvw(argv[1])," => ",chvw(argv[2]),"\nTransmitted:",transmitted," utf-16 characters\tElapsed Time:",fast_io::posix_clock_gettime(fast_io::posix_clock_id::realtime)-t0);
}
#ifdef __cpp_exceptions
catch(fast_io::posix_error e)
{
	perrln(e);
}
#ifdef _WIN32
catch(fast_io::win32_error e)
{
	perrln(e);
}
#endif
catch(std::exception const& e)
{
	perrln(e);
}
catch(...)
{
	perr("unknown EH\n");
}
#endif