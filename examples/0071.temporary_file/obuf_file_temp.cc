#include"../../include/fast_io_device.h"

int main()
{
	fast_io::obuf_file obf("temp.txt",fast_io::open_mode::temporary|fast_io::open_mode::excl);

//PLEASE ALWAYS FOLLOW temporary with EXCL or it will create TOCTTOU vuln.
}