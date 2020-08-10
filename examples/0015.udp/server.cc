#include"../../include/fast_io.h"
#include"../../include/fast_io_network.h"


int main()
try
{
	for(fast_io::tcp_server hd(2000);;)
	{
		fast_io::acceptor accept(hd);
		print(accept,u8"hello world from server\n","okay\n");
	}
}
catch(std::exception const & e)
{
	perrln(e);
	return 1;
}