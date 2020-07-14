#include"../../include/fast_io_driver/fmt_driver.h"
#include"../../include/fast_io_driver/openssl_driver.h"

int main()
{
	fast_io::openssl::bio_file bf("openssl_bio.txt","wb");
	fmt_fprint(bf,"hello {1} {0} \n",4,"from fast_io::openssl::bio_file");
}
