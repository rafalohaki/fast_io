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
	fast_io::http_status status;
	scan(hd,status);
	println(u8"http version:",status.version,u8"\tcode is ",status.code);
	std::size_t content_length{};
	for(auto line:scan_http_header(hd))
	{
		println(u8"key:",line.key,u8"\tvalue:",line.value);
		if(line.key=="Content-Length")
			content_length=fast_io::to<std::size_t>(line.value);
	}
	fast_io::obuf_file obf("ind.html");
	transmit(obf,hd,content_length);

	fast_io::ibuf_file ib("request_header.txt");
	fast_io::http_request_status request_status;
	scan(ib, request_status);

	println(request_status);
}
catch(std::exception const& e)
{
	perrln(e);
	return 1;
}