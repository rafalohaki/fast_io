#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"

int main()
{
	fast_io::ibuf_file ibf("w.txt");
	fast_io::ibuf_file ibf2(ibf);
}