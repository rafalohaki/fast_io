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

template<output_stream output>
requires std::same_as<typename output::char_type,char>
inline void print_define(output& out,std::system_error const &e)
{
	auto const& code(e.code());
	print_freestanding(out,u8"std::system_error, value:",code.value(),u8"\tmessage:",code.message(),u8"\twhat:",chvw(e.what()));
}

}