#pragma once

namespace fast_io
{
/*
out of range is contract violation. which is undefined behavior.
*/
struct iso6709_dddmmss
{
	std::int16_t degrees{};
	std::uint8_t minutes{};
	std::uint8_t seconds{};
};

template<typename T>
struct basic_iso6709_coordinates
{
	using coordinate_type = T;
	T latitude{};
	T longtitude{};
};

using iso6709_dddmmss_coordinates = basic_iso6709_coordinates<iso6709_dddmmss>;

}