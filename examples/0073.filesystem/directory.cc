#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"

int main()
{
	fast_io::directory dir(".");
	fast_io::obuf_file obf(fast_io::at,dir,"hello_directory.txt");
	print(obf,"Hello World\n");
}