#include<fast_io.h>
#include<fast_io_driver/nt_kernel.h>


extern std::uint32_t DbgPrintEx(std::uint32_t comment,std::uint32_t level,char const* fmt,...) noexcept __asm__("DbgPrintEx");


extern "C" std::uint32_t DriverEntry(::fast_io::win32::nt::driver_object* driverobject,::fast_io::win32::nt::utf16_string* registrypath) noexcept
{
	DbgPrintEx(101,3,"DbgPrintEx works: 2nd one\n" );
	println(fast_io::u8nt_dbg(),u8"Hello World from fast_io: driverobjectajpfoaj[sfajsfpjw[pejfwqe[jqeg[opqwjgwpqe[gjwepotiopwit[qpwitp[qwetiwqpe[oriwpoe[riwope[riweo[priwqep[riwqp[]]]]]]]]]]]]]:",driverobject,u8" registrypath:",registrypath);
	return 0;
}
