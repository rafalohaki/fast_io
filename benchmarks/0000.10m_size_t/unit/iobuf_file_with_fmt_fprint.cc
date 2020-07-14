#include"../../timer.h"
#include"../../../include/fast_io.h"
#include"../../../include/fast_io_device.h"
#include"../../../include/fast_io_driver/fmt_driver.h"

int main()
{
	constexpr std::size_t N(10000000);
	{
	fast_io::timer t("output");
	fast_io::obuf_file obf("obuf_file_fmt.txt");
	for(std::size_t i{};i!=N;++i)
		fmt_fprint(obf,"{}\n",i);
	}
}