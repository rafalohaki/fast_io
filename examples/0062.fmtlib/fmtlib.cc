#include"../../include/fast_io_driver/fmt_driver.h"
#include"../../include/fast_io_device.h"
#include<fmt/compile.h>

int main()
{
	fmt_fprint("hello {} {}\n",1,"from fast_io::c_stdout()");
	fmt_fprint(fast_io::out(),"hello {} {}\n","from","fast_io::out()");
	fast_io::obuf_file obf("obf.txt");
	fmt_fprint(obf,"hello {1} {0} \n",4,"from fast_io::obuf_file");
}