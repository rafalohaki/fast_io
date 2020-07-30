#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"
#include"../../include/fast_io_network.h"

int main()
{
	fast_io::tcp_server hd(10000);
	fast_io::acceptor accept(hd);
	fast_io::onative_file obf("result.mp4");
	transmit(obf,accept);
}