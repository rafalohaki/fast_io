#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"
#include"../../include/fast_io_network.h"
#include"../../include/fast_io_crypto.h"

int main()
try
{
	auto t0{std::chrono::high_resolution_clock::now()};
	fast_io::ibuf_tcp_client hd(fast_io::dns_once("xxxx"),80);
	print(hd,u8"GET / HTTP/1.1\r\n"
		"Host:xxxx\r\n"
		"User-agent:whatever\r\n"
		"Accept-Type:*/*\r\n"
		"Connection:close\r\n\r\n");
	skip_http_header(hd);
//Download the webpage and calculate its sha256.
	fast_io::io_files files;
	fast_io::sha256 sha;
	fast_io::hash_processor processor(sha);
	files.emplace_back(fast_io::io_cookie,processor);
	files.emplace_back(fast_io::io_cookie,std::in_place_type<fast_io::onative_file>,"index.html");
	std::size_t transmitted{transmit(files,hd)};
	processor.do_final();
	println(sha," *index.html\nTransmitted:",transmitted,u8" bytes\tElapsed Time:",std::chrono::high_resolution_clock::now()-t0);
}
catch(std::exception const& e)
{
	perrln(e);
	return 1;
}