#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"

int main()
{
	fast_io::inative_file inv("file_size.cc");
	println("file size of file_size.cc is ",size(inv)," bytes\n"
	"file type of file_size.cc is ",type(inv));
}