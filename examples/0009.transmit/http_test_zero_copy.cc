#include"../../include/fast_io_device.h"
#include"../../include/fast_io_network.h"
#include"../../include/fast_io.h"

int main()
#ifdef __cpp_exceptions
try
#endif
{
	fast_io::ibuf_tcp_client hd(fast_io::dns_once("xxxx"),80);
	fast_io::inative_file request_file("http_request.txt");
	transmit(hd,request_file);
	skip_http_header(hd);
	fast_io::onative_file osf("index.html");
	transmit(osf,hd);
}
#ifdef __cpp_exceptions
catch(std::exception const& e)
{
	perrln(e);
	return 1;
}
#endif