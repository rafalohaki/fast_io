#include"../../../include/fast_io.h"
#include"../../../include/fast_io_device.h"
#include"../../../include/fast_io_driver/openssl_driver.h"


int main()
{
	fast_io::openssl::bio_file bio("wf.txt",fast_io::open_mode::binary|fast_io::open_mode::out);
	println(fast_io::out(),"fd:",static_cast<fast_io::posix_io_observer>(bio));
}
