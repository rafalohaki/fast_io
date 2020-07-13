#include"../../include/fast_io.h"

int main()
{
	fast_io::white_hole rtgl;
	std::string str;
	fast_io::ostring_ref oref(str);
	transmit(oref,rtgl,1000);
	for(auto const e : str)
		println(e);
}