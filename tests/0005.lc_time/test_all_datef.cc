#include<fast_io.h>
#include<fast_io_i18n.h>
#include<fast_io_device.h>

int main()
{
	fast_io::l10n loc("");
	auto ts{utc(posix_clock_gettime(fast_io::posix_clock_id::realtime))};
	char8_t format_buffer[3]{u8'%',u8'%',u8'%'};
	fast_io::basic_io_scatter_t<char8_t> test_format{format_buffer,2};
	for(char8_t i{u8'A'};i<=u8'Z';++i)
	{
		format_buffer[1]=i;
		println(fast_io::mnp::chvw(i)," ",::fast_io::details::lc_print_reserve_size_time_format_common_impl(loc.loc.u8all->time,ts,test_format));
	}
	for(char8_t i{u8'a'};i<=u8'z';++i)
	{
		format_buffer[1]=i;
		println(fast_io::mnp::chvw(i)," ",::fast_io::details::lc_print_reserve_size_time_format_common_impl(loc.loc.u8all->time,ts,test_format));
	}
	format_buffer[1]=u8'O';
	test_format.len=3;
	print("%Ox start strings:\n");
	for(char8_t i{u8'A'};i<=u8'Z';++i)
	{
		format_buffer[2]=i;
		println(fast_io::mnp::chvw(i)," ",::fast_io::details::lc_print_reserve_size_time_format_common_impl(loc.loc.u8all->time,ts,test_format));
	}
	for(char8_t i{u8'a'};i<=u8'z';++i)
	{
		format_buffer[2]=i;
		println(fast_io::mnp::chvw(i)," ",::fast_io::details::lc_print_reserve_size_time_format_common_impl(loc.loc.u8all->time,ts,test_format));
	}
}