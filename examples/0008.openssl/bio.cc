#include<fast_io.h>
#include<fast_io_driver/openssl_driver.h>

int main()
{
	fast_io::bio_file bf("bio.txt",fast_io::open_mode::out);
	print(bf,"Hello World\n");
}