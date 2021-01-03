#pragma once

namespace fast_io
{
//I just realize C++20 date time is purely nonsense and trash

struct iso8601_timestamp
{
	intiso_t year{};
	std::uint8_t month{};
	std::uint8_t day{};
	std::uint8_t hours{};
	std::uint8_t minutes{};
	std::uint8_t seconds{};
	uintiso_t subseconds{};
};

}