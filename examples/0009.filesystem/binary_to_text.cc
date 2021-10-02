#include<fast_io_device.h>
#include<fast_io.h>

int main(int argc,char const** argv)
try
{
	if(argc<3)
	{
		perr("Usage: ",fast_io::mnp::chvw(*argv)," <source directory> <dest directory>\n");
		return 1;
	}
	using namespace std::string_view_literals;
	fast_io::dir_file df(argv[1]);
	fast_io::dir_file df_crlf(argv[2]);
	for(auto ent: current(at(df)))
		if(extension(ent)==u8".cc"sv)
		{
			fast_io::native_file pf(drt(ent),fast_io::open_mode::in|fast_io::open_mode::text);//sorry i am stupid
			fast_io::c_file_unlocked pf2(at(df_crlf),native_filename(ent),fast_io::open_mode::out|fast_io::open_mode::text);
			transmit(pf2,pf);
		}
}
#ifdef _WIN32
#ifndef _WIN32_WINDOWS
catch(fast_io::nt_error ec)
{
	perrln(ec);
}
#endif
catch(fast_io::win32_error ec)
{
	perrln(ec);
}
#endif
catch(fast_io::posix_error ec)
{
	perrln(ec);
}
catch(std::exception const& e)
{
	perrln(e);
}
catch(...)
{
	perr("unknown EH thrown\n");
}