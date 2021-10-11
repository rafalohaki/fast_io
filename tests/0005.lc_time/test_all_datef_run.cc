#include<fast_io.h>
#include<fast_io_i18n.h>
#include<fast_io_device.h>
#include<memory>

inline void test(fast_io::u8lc_all const* u8all,char8_t i,fast_io::iso8601_timestamp const& ts,fast_io::basic_io_scatter_t<char8_t>& test_format)
{
	std::size_t format_total_size{::fast_io::details::lc_print_reserve_size_time_format_common_impl(u8all->time,ts,test_format)};
	std::unique_ptr<char8_t> uptr{new char8_t[format_total_size]};

	auto res{::fast_io::details::lc_print_reserve_define_time_fmt_common_impl(u8all->time,uptr.get(),ts,test_format)};
	if(static_cast<std::size_t>(res-uptr.get())>format_total_size)
		print("bugged: ");
	println(fast_io::mnp::chvw(i)," reserved:",format_total_size," used:",res-uptr.get()," result:",fast_io::basic_io_scatter_t<char>{reinterpret_cast<char const*>(uptr.get()),static_cast<std::size_t>(res-uptr.get())});
}

int main()
{
	fast_io::l10n loc("");
	auto ts{utc(posix_clock_gettime(fast_io::posix_clock_id::realtime))};
	char8_t format_buffer[3]{u8'%',u8'%',u8'%'};
	fast_io::basic_io_scatter_t<char8_t> test_format{format_buffer,2};
	for(char8_t i{u8'A'};i<=u8'Z';++i)
	{
		format_buffer[1]=i;
		test(loc.loc.u8all,i,ts,test_format);
	}
	for(char8_t i{u8'a'};i<=u8'z';++i)
	{
		format_buffer[1]=i;
		test(loc.loc.u8all,i,ts,test_format);
	}
	format_buffer[1]=u8'O';
	test_format.len=3;
	print("%Ox start strings:\n");
	for(char8_t i{u8'A'};i<=u8'Z';++i)
	{
		format_buffer[2]=i;
		test(loc.loc.u8all,i,ts,test_format);
	}
	for(char8_t i{u8'a'};i<=u8'z';++i)
	{
		format_buffer[2]=i;
		test(loc.loc.u8all,i,ts,test_format);
	}
}