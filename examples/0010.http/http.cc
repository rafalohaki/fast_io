#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"
#include"../../include/fast_io_network.h"


int main()
try
{
	fast_io::ibuf_tcp_client hd(fast_io::dns_once("xxxx"),80);
	print(hd,u8"GET / HTTP/1.1\r\n"
		"Host:xxxx\r\n"
		"User-agent:whatever\r\n"
		"Accept-Type:*/*\r\n"
		"Connection:close\r\n\r\n");
	skip_http_header(hd);
	fast_io::onative_file osf("index.html");
	transmit(osf,hd);
}
catch(std::exception const& e)
{
	perrln(e);
	return 1;
}