#include<fast_io.h>

int main()
try
{
	fast_io::native_dll_file dll("a.dll",fast_io::dll_mode::posix_rtld_lazy);
	auto proc{dll_load_symbol(dll,"GetProc")};
}
#if defined(_WIN32)
catch(fast_io::win32_error e)
{
	perrln(e);
}
#else
catch(fast_io::posix_rtld_error e)
{
	perrln(e);
}
#endif