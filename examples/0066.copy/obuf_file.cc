#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"

int main()
{
	fast_io::obuf_file obf("w.txt");
	print(obf,"Hello World from obf\n");
	fast_io::obuf_file obf2(obf);
	print(obf2,"Hello World from obf2\n");
}