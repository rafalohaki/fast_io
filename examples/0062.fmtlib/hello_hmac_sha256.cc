#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"
#include"../../include/fast_io_crypto.h"
#include"../../include/fast_io_driver/fmt_driver.h"

int main()
{
	fast_io::hmac_sha256 sha("Hello");
	fast_io::hash_processor processor(sha);
	fmt_fprint(processor,"Hello {1} {0}","sha256","hmac");
	processor.do_final();
	println(sha);
}

/*
g++ -o hello_hmac_sha256 hello_hmac_sha256.cc -Ofast -std=c++20 -s -lfmt -maes -msse4.2 -msha -march=native
*/