#pragma once

namespace fast_io
{

enum class utime_flags
{
none,now,omit
};

struct unix_timestamp_option
{
	utime_flags flags{utime_flags::omit};
	unix_timestamp timestamp{};
	constexpr unix_timestamp_option() noexcept=default;
	constexpr unix_timestamp_option(unix_timestamp ts) noexcept:flags(utime_flags::none),timestamp(ts){}
	constexpr unix_timestamp_option(utime_flags fg) noexcept:flags(fg){}
};

}