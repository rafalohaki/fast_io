#include"../../include/fast_io.h"
#include"../../include/fast_io_legacy.h"

int main()
{
	fast_io::filebuf_file fb("filebuf_file.txt",fast_io::open_mode::binary|fast_io::open_mode::out);
	std::ostream out(fb.native_handle());
	out<<"Hello world from std::ostream\n";
	print(fb,"Hello World from fast_io\n");
	out<<"Hello world from std::ostream again\n";
	print(fb,"Hello World from fast_io again\n");
}