#include"../../include/fast_io.h"
#include <Windows.h>
#undef interface
#undef min
#undef max
#undef unix
#include <winternl.h>

int main()
{
	::debug_println("FILE_GENERIC_WRITE=0x",fast_io::hex(FILE_GENERIC_WRITE),"\n"
		"FILE_GENERIC_READ=0x",fast_io::hex(FILE_GENERIC_READ),"\n",
		"FILE_DIRECTORY_FILE=0x",fast_io::hex(FILE_DIRECTORY_FILE));
}