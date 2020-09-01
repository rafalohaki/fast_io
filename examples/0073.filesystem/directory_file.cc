#include<vector>
#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"

int main()
{
	fast_io::directory dir(".");
	for(auto const& e: recursive(dir))
	{
		if(type(e)==fast_io::file_type::regular&&extension(e)==".cc")
		{
			println("inode:",inode(e),"\tfilename:",filename(e),"\ttype:",type(e));
		}
	}
}