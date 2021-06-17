#include<fast_io.h>
#include<fast_io_network.h>

int main()
{
	fast_io::net_service service;
	
	for(fast_io::native_socket_file socket(fast_io::tcp_listen(200));;)
		try
		{
			fast_io::native_socket_file file(tcp_accept(socket));
			println(fast_io::err(),utc(fast_io::posix_clock_gettime(fast_io::posix_clock_id::realtime)));
			print(file,"Hello World\n");
		}
		catch(...)
		{
		}
}
