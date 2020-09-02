#include<exception>
#include<cstdint>
#include"../../include/fast_io.h"

int main()
try
{
	fast_io::nt_file folder("folder",fast_io::open_mode::in|fast_io::open_mode::directory);
	fast_io::nt_file fl(fast_io::at,folder,"qqwqq.txt",fast_io::open_mode::out);
}
catch(std::exception const& e)
{
	perrln(e);
	return 1;
}