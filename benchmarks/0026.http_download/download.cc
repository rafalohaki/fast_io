#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"
#include"../../include/fast_io_network.h"


int main()
try
{
	auto t0{std::chrono::high_resolution_clock::now()};
	fast_io::ibuf_tcp_client hd(fast_io::dns_once("soft.wzhszm.com"),80);
	print(hd,u8"GET /yy_24400.zip HTTP/1.1\r\n"
		"Host:soft.wzhszm.com\r\n"
		"User-agent:whatever\r\n"
		"Accept-Type:*/*\r\n"
		"Connection:close\r\n\r\n");
	skip_http_header(hd);
	fast_io::onative_file osf("yy_24400.zip");
	std::size_t bytes{transmit(osf,hd)};
	double elapsed{std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now()-t0).count()};
	if(elapsed==0)[[unlikely]]
		println("Transmitted:",bytes,u8" bytes\tElapsed Time:",elapsed);
	else
		println("Transmitted:",bytes,u8" bytes\tElapsed Time:",elapsed,"\tMB/sec:",static_cast<double>(bytes)/elapsed/1048576.0);
}
catch(std::exception const& e)
{
	perrln(e);
	return 1;
}