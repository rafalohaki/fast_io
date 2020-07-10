#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"
#include"../../include/fast_io_network.h"

int main()
{
	auto t0{std::chrono::high_resolution_clock::now()};
	fast_io::ibuf_file ibf("ova.mp4");
	fast_io::tcp_client client(fast_io::ipv4{127,0,0,1},10000);
	auto bytes{transmit(client,ibf)};
	double elapsed{std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now()-t0).count()};
	if(elapsed==0)[[unlikely]]
		println("Transmitted:",bytes,u8" bytes\tElapsed Time:",elapsed);
	else
		println("Transmitted:",bytes,u8" bytes\tElapsed Time:",elapsed,"\tMB/sec:",static_cast<double>(bytes)/elapsed/1048576.0);
}