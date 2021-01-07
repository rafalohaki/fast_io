#pragma once

namespace fast_io
{

template<buffer_output_stream output>
requires (std::same_as<typename output::char_type,char>)
inline constexpr void print_define(output out,std::source_location const& location)
{
	print_freestanding(out,manipulators::chvw(location.file_name()));
	put(out,':');
	print_freestanding(out,location.line());
	put(out,':');
	print_freestanding(out,location.column());
	put(out,':');
	print_freestanding(out,manipulators::chvw(location.function_name()));
}

}