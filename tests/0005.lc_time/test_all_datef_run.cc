#include <string>
#include<fast_io.h>
#include<fast_io_i18n.h>
#include<fast_io_device.h>

int main()
{
	fast_io::l10n loc("ja_JP", "utf-8");
	std::u8string s(1000, u8'\0');
	fast_io::u8win32_file f{ "u8out.txt", fast_io::open_mode::out };
	auto ts{ utc(posix_clock_gettime(fast_io::posix_clock_id::realtime)) };
	char8_t format_buffer[3]{ u8'%',u8'%',u8'%' };
	fast_io::basic_io_scatter_t<char8_t> test_format{ format_buffer,2 };
	auto test = [&] (char8_t i) {
		std::size_t reserved{ ::fast_io::details::lc_print_reserve_size_time_format_common_impl(loc.loc.u8all->time, ts, test_format) };
		auto itr = ::fast_io::details::lc_print_reserve_define_time_fmt_common_impl(loc.loc.u8all->time, s.data(), ts, test_format);
		*itr = u8'\0';
		std::size_t actual(itr - s.data());
		print(f, fast_io::mnp::chvw(i), u8" reserved:", reserved, u8"\tactually use:", actual);
		if (reserved < actual)
			print(f, u8"\twrong!");
		print(f,u8"\tresult:");
		write(f,s.data(),itr);
		println(f);
	};
	for (char8_t i{ u8'A' }; i <= u8'Z'; ++i)
	{
		format_buffer[1] = i;
		test(i);
	}
	for (char8_t i{ u8'a' }; i <= u8'z'; ++i)
	{
		format_buffer[1] = i;
		test(i);
	}
	format_buffer[1] = u8'O';
	test_format.len = 3;
	print(f, u8"%Ox start strings:\n");
	for (char8_t i{ u8'A' }; i <= u8'Z'; ++i)
	{
		format_buffer[2] = i;
		test(i);
	}
	for (char8_t i{ u8'a' }; i <= u8'z'; ++i)
	{
		format_buffer[2] = i;
		test(i);
	}
	format_buffer[1] = u8'E';
	print(f, u8"%Ex start strings:\n");
	for (char8_t i{ u8'A' }; i <= u8'Z'; ++i)
	{
		format_buffer[2] = i;
		test(i);
	}
	for (char8_t i{ u8'a' }; i <= u8'z'; ++i)
	{
		format_buffer[2] = i;
		test(i);
	}
}