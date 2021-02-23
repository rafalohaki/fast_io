#pragma once

namespace fast_io
{

inline basic_io_scatter_t<char> print_scatter_define(print_scatter_type_t<char>,std::exception const &e)
{
	auto const c_str{e.what()};
	return {c_str,strlen(c_str)};
}
}