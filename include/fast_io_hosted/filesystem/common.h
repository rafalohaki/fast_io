#pragma once

namespace fast_io
{

inline std::string_view extension(directory_entry pioe) noexcept
{
	std::string_view fname{filename(pioe)};
	using namespace std::string_view_literals;
	if(fname=="."sv||fname==".."sv)
		return {};
	auto start{fname.rfind('.')};
	if(start==cstring_view::npos)
		return {};
	return fname.substr(start);
}

}