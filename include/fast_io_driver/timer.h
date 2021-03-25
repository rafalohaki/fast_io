#pragma once

namespace fast_io
{
struct timer
{
	std::u8string_view s;
	unix_timestamp t0;
	explicit timer(std::u8string_view strvw):
		s(strvw),t0(posix_clock_gettime(posix_clock_id::monotonic)){}
	timer(timer const &)=delete;
	timer& operator=(timer const &)=delete;
	~timer()
	{
		perr(fast_io::u8err(),s, u8":", posix_clock_gettime(posix_clock_id::monotonic)-t0,u8"s\n");
	}
};
}
