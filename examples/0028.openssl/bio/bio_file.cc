#include"../../../include/fast_io.h"
#include"../../../include/fast_io_device.h"
#include"../../../include/fast_io_driver/openssl_driver.h"


int main()
{
	fast_io::openssl::bio_file bio("wf.txt",fast_io::open_mode::binary|fast_io::open_mode::out);
	print(bio,"Hello World for fast_io before\n");
	BIO_printf(bio.native_handle(),"Hello World from BIO_printf\n");
	print(bio,"Hello World for fast_io after\n");
}
