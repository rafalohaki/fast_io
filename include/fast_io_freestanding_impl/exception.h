#pragma once

namespace fast_io
{

inline basic_io_scatter_t<char> print_alias_define(io_alias_t,std::exception const &e)
{
	auto const c_str{e.what()};
	return {c_str,strlen(c_str)};
}
}