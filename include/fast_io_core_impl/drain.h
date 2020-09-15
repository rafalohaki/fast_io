#pragma once

namespace fast_io
{

template<std::integral char_type,buffer_output_stream source_type>
constexpr basic_io_scatter_t<char_type> print_scatter_define(print_scatter_type_t<char_type>,manip::drainage<source_type&> source)
{
	auto bg{obuffer_begin(source.reference)};
	auto curr{obuffer_curr(source.reference)};
	return {bg,curr-bg};
}

template<output_stream dest_type,buffer_output_stream source_type>
constexpr void print_define(dest_type& dest,manip::drainage<source_type&> source)
{
	write(dest,obuffer_begin(source.reference),obuffer_curr(source.reference));
}

template<output_stream dest_type,output_stream source_type>
inline constexpr void drain(dest_type&& dest,source_type&& source)
{
	print_freestanding(std::forward<dest_type>(dest),drainage(source));
}


}