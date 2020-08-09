#include"../../include/fast_io_device.h"
#include"../../include/fast_io_network.h"
#include"../../include/fast_io.h"

int main()
try
{
	fast_io::ibuf_tcp_client hd(fast_io::dns_once("xxx"),80);
	fast_io::inative_file request_file("http_request.txt");
	transmit(hd,request_file);
	skip_http_header(hd);
	fast_io::onative_file osf("index.html");
	transmit(osf,hd);
}
catch(std::exception const& e)
{
	perrln(e);
	return 1;
}