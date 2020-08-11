#include"../../include/fast_io.h"
#include"../../include/fast_io_network.h"

int main()
try
{
	fast_io::ip_endpoint info{fast_io::ipv4{127,0,0,1},2001};
	fast_io::udp hd;
	print(fast_io::packet(hd,info),"Hello World\n");
}
catch(std::exception const & e)
{
	perrln(e);
	return 1;
}