#include"../../include/fast_io.h"
#include"../../include/fast_io_legacy.h"

int main()
{
	fast_io::win32_file nfl("win32_file.txt",fast_io::open_mode::binary|fast_io::open_mode::out);
	fast_io::filebuf_file fb(std::move(nfl),fast_io::open_mode::binary|fast_io::open_mode::out);
	std::ostream out(fb.native_handle());
	out<<"Hello world from std::ofstream\n";
	print(fb,"Hello World from fast_io\n");
	out<<"Hello world from std::ofstream again\n";
	print(fb,"Hello World from fast_io again\n");
}