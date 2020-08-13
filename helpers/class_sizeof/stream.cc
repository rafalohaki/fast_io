#include"../../include/fast_io.h"
#include<istream>
#include<ostream>
#include<streambuf>
#include<sstream>
#include<fstream>
#include<cstdio>
#include"../../include/fast_io_device.h"
#include"../../include/fast_io_legacy.h"

int main()
{
	println(
	"C FILE*  :\tsizeof(std::FILE*)=",sizeof(std::FILE*),"\n"
	"locale   :\tsizeof(std::locale)=",sizeof(std::locale),"\n",
	"stream   :\tsizeof(std::istream)=",sizeof(std::istream),"\tsizeof(std::ostream)=",sizeof(std::ostream),"\tsizeof(std::iostream)=",sizeof(std::iostream),"\tsizeof(std::streambuf)=",sizeof(std::streambuf),"\n"
	"fstream  :\tsizeof(std::ifstream)=",sizeof(std::ifstream),"\tsizeof(std::ofstream)=",sizeof(std::ofstream),"\tsizeof(std::fstream)=",sizeof(std::fstream),"\tsizeof(std::filebuf)=",sizeof(std::filebuf),"\n"
	"sstream  :\tsizeof(std::istringstream)=",sizeof(std::istringstream),"\tsizeof(std::ostringstream)=",sizeof(std::ostringstream),"\tsizeof(std::stringstream)=",sizeof(std::stringstream),"\tsizeof(std::stringbuf)=",sizeof(std::stringbuf),"\n"
	"fast_io  :\tsizeof(fast_io::ibuf_file)=",sizeof(fast_io::ibuf_file),"\tsizeof(fast_io::obuf_file)=",sizeof(fast_io::obuf_file),"\tsizeof(fast_io::iobuf_file)=",sizeof(fast_io::iobuf_file),"\tsizeof(fast_io::native_file)=",sizeof(fast_io::native_file),"\n"
	"fast_io_legacy:\tsizeof(fast_io::c_file)=",sizeof(fast_io::c_file),"\tsizeof(fast_io::filebuf_file)=",sizeof(fast_io::filebuf_file));
}
/*
This is a test under GCC/ MSVCRT/ libstdc++
C FILE*  :      sizeof(std::FILE*)=8
locale   :      sizeof(std::locale)=8
stream   :      sizeof(std::istream)=280        sizeof(std::ostream)=272        sizeof(std::iostream)=288       sizeof(std::streambuf)=64
fstream  :      sizeof(std::ifstream)=472       sizeof(std::ofstream)=464       sizeof(std::fstream)=480        sizeof(std::filebuf)=192
sstream  :      sizeof(std::istringstream)=384  sizeof(std::ostringstream)=376  sizeof(std::stringstream)=392   sizeof(std::stringbuf)=104
fast_io  :      sizeof(fast_io::ibuf_file)=32   sizeof(fast_io::obuf_file)=32   sizeof(fast_io::iobuf_file)=56  sizeof(fast_io::native_file)=8
fast_io_legacy: sizeof(fast_io::c_file)=8       sizeof(fast_io::filebuf_file)=8

iostream is a total waste of memory. 272 bytes for a std::ostream??? Are you kidding me?
*/