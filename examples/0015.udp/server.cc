#include"../../include/fast_io.h"
#include"../../include/fast_io_network.h"


int main()
try
{
	fast_io::udp_server server(2001);
	fast_io::ip_endpoint client(fast_io::ipv4{127,0,0,1},2000);
	print(fast_io::packet(server,client),"Hello World\n");
	std::array<char,100> buffer;
	write(fast_io::out(),buffer.begin(),read(fast_io::packet(server,client),buffer.begin(),buffer.end()));
}
catch(std::exception const & e)
{
	perrln(e);
	return 1;
}