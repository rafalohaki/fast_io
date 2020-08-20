#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"
#include<fstream>
#include"../../include/fast_io_legacy.h"
int main()
{
	std::fstream fin("fstream.cc");
	fast_io::filebuf_io_observer fiob{fin.rdbuf()};
	println("file size of file_size.cc is ",size(fiob)," bytes\n"
	"file type of file_size.cc is ",type(fiob));
}