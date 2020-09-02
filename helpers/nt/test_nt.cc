#include<exception>
#include<cstdint>
#include"../../include/fast_io.h"

int main()
try
{
	fast_io::win32_file folder("folder",fast_io::open_mode::in|fast_io::open_mode::directory);
	fast_io::nt_file fl(fast_io::at,static_cast<fast_io::nt_io_observer>(folder),"qqwq.txt",fast_io::open_mode::out);
//	fast_io::nt_file fl("./folder/qqw.txt",fast_io::open_mode::out);
}
catch(std::exception const& e)
{
	perrln(e);
	return 1;
}