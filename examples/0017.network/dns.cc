#include<string_view>
#include<fast_io.h>
#include<fast_io_device.h>

int main()
{
	fast_io::net_service service;
	fast_io::u8iobuf_socket_file socket{fast_io::tcp_connect(fast_io::to_ip(fast_io::native_dns_file("www.jgjy.gov.cn"),80))};
	print(socket,u8"GET / HTTP/1.1\r\n"
	"Host:www.jgjy.gov.cn\r\n"
	"User-agent:whatever\r\n"
	"Accept-Type:*/*\r\n"
	"Connection:close\r\n\r\n");
	fast_io::u8http_header_buffer buffer;
	scan(socket,buffer);
	fast_io::u8native_file nf(u8"index.html",fast_io::open_mode::out);
	transmit(nf,socket);
}
