#include"../../include/fast_io.h"

int main()
{
	fast_io::ibuf_white_hole rtgl;
	std::string str;
	fast_io::ostring_ref oref(str);
	transmit(oref,rtgl,1000);
	for(std::uint8_t const e : str)
		println(e);
}