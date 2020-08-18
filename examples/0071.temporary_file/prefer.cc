#include"../../include/fast_io_device.h"

int main()
{
	fast_io::obuf_file obf(fast_io::io_temp);//KISS. No TOCTTOU. BEST performance. BEST OS support. Do not add all kinds of format string bullshit
	print(obf,"Hello World\n");
	rewind(obf);		//rewind the stream
	fast_io::onative_file destination_file("destination.txt");
	transmit(destination_file,obf);
}
