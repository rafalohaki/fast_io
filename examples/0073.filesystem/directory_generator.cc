#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"

int main()
{
	fast_io::dir_file dir(".");
	for(auto const& e : current(dir))
		println("inode:",inode(e),"\tfilename:",filename(e),"\ttype:",type(e));
}