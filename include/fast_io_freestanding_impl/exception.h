#pragma once

namespace fast_io
{
template<typename T>
requires (std::same_as<T,std::exception>)
inline basic_io_scatter_t<char> print_scatter_define(print_scatter_type_t<char>,T const &e) noexcept
{
	auto const c_str{e.what()};
	return {c_str,strlen(c_str)};
}
}