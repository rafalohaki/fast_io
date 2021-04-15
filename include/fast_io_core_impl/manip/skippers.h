#pragma once

namespace fast_io::manipulators
{

struct space_skipper
{
	using manip_tag = manip_tag_t;
	explicit constexpr space_skipper() noexcept=default;
};

template<::fast_io::freestanding::input_or_output_iterator Iter>
inline constexpr Iter skip_define(Iter begin,Iter end,space_skipper) noexcept
{
	return scan_skip_space(begin, end);
}

}