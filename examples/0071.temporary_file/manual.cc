#include"../../include/fast_io_device.h"

int main()
{
	fast_io::obuf_file obf(
#ifdef _WIN32
"temp.txt"
#elif defined(__linux__)
"/tmp"
#else
"/tmp/obuf_file_temp.txt"
#endif
,fast_io::open_mode::temporary|fast_io::open_mode::excl);

//PLEASE ALWAYS FOLLOW temporary with EXCL or it will create TOCTTOU vuln if you do it manually.
}
