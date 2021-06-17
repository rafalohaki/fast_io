#include<fast_io.h>
#include<fast_io_network.h>

int main()
try
{
	fast_io::net_service service;
	fast_io::iobuf_socket_file socket(tcp_connect(fast_io::ipv4{{127,0,0,1},200}));
	transmit(fast_io::out(),socket);
}
catch(fast_io::win32_error e)
{
	perrln(e);
	return 1;
}