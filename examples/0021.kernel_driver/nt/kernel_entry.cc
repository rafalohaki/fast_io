#include<fast_io.h>
#include<fast_io_driver/nt_kernel.h>


extern "C" std::uint32_t DriverEntry(::fast_io::win32::nt::driver_object* driverobject,::fast_io::win32::nt::utf16_string* registrypath) noexcept
{
	print(fast_io::u16nt_dbg(),u"Hello World from fast_io:\ndriverobject:",driverobject,u"\nregistrypath:",*registrypath,u"(",registrypath,u")\n");
	return 0;
}
