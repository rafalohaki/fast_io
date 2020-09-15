#pragma once

namespace fast_io
{
/*We have no choice but to assume it is ansi based. Even On EBCDIC systems
since we cannot prevent people link from other libraries using ANSI that throw exception*/

template<output_stream output>
requires std::same_as<typename output::char_type,char>
inline void print_define(output& out,std::exception const &e)
{
	print_freestanding(out,chvw(e.what()));
}

}