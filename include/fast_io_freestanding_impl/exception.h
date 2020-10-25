#pragma once

namespace fast_io
{

inline basic_io_scatter_t<char> print_alias_define(io_alias_t,std::exception const &e)
{
	return {e.what(),strlen(e.what())};
}
}