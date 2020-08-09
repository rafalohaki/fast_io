#include"../../include/fast_io.h"
#include"../../include/fast_io_network.h"


int main()
try
{
//	debug_println(__LINE__);
	fast_io::server hd(2000,fast_io::sock::type::datagrams,fast_io::sock::protocal::udp);
	for(;;)
	try
	{
		fast_io::ibuf_socket_acceptor accept(hd);
		println(fast_io::out(),u8"a client connect to this server");
		print_flush(accept,u8"hello world\n");
	}
	catch(std::exception const & e)
	{
		perrln(e);
	}
}
catch(std::exception const & e)
{
	perrln(e);
	return 1;
}