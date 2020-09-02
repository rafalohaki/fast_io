#include"../../include/fast_io.h"
#include"../../include/fast_io_legacy.h"
#include"../../include/fast_io_device.h"

int main()
{
	fast_io::dir_file dnf(".");
	fast_io::filebuf_file fb(fast_io::at,dnf,"io_at_filebuf_file.txt",fast_io::open_mode::binary|fast_io::open_mode::out);
	std::ofstream out;
	*out.rdbuf()=std::move(*fb.native_handle());
	fast_io::filebuf_io_observer fiob{out.rdbuf()};
	out<<"Hello world from std::ofstream\n";
	print(fiob,"Hello World from fast_io\n");
	out<<"Hello world from std::ofstream again\n";
	print(fiob,"Hello World from fast_io again\n");
}