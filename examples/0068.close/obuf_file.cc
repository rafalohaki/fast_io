#include"../../include/fast_io_device.h"

int main()
{
	fast_io::obuf_file obf("obf_close.txt");
	
	obf.close();
}