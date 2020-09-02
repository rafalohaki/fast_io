#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"

int main()
{
	fast_io::dir_file dir(".");
	for(auto const& e: recursive(dir))
	{
		if(type(e)==fast_io::file_type::regular&&extension(e)==".cc")
		{
			print("inode:",inode(e),"\tfilename:",filename(e),"\ttype:",type(e),"\n\n");
			fast_io::inative_file infl(fast_io::at,e,filename(e));
			transmit(fast_io::c_stdout(),infl);
			print("\n\n\n\n");
		}
	}
}