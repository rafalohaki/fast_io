#pragma once

namespace fast_io
{

template<output_stream output>
requires (std::same_as<typename output::char_type,char>)
inline constexpr void print_define(output out,std::source_location const& location)
{
	if constexpr(buffer_output_stream<output>)
	{
		print_freestanding(out,chvw(location.file_name()));
		put(out,':');
		print_freestanding(out,location.line());
		put(out,':');
		print_freestanding(out,location.column());
		put(out,':');
		print_freestanding(out,chvw(location.function_name()));
	}
	else
	{
		print_freestanding(out,chvw(location.file_name()),
			":",location.line(),
			":",location.column(),
			":",chvw(location.function_name()));
	}
}

}