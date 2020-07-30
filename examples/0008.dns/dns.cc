#include"../../include/fast_io.h"
#include"../../include/fast_io_network.h"

int main(int argc,char** argv)
try
{
	if(argc!=2)
	{
		perrln(u8"Usage: ",std::string_view(*argv),u8" <domain>");
		return 1;
	}
	for(auto const& address:fast_io::dns(argv[1]))
		println(address);
}
catch(std::exception const & e)
{
	perrln(e);
	return 1;
}
