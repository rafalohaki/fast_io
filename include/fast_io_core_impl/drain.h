#pragma once

namespace fast_io
{

template<buffer_output_stream source_type>
constexpr io_scatter_t print_scatter_define(manip::drainage<source_type&> source)
{
	auto bg{obuffer_begin(source.reference)};
	auto curr{obuffer_curr(source.reference)};
	return io_scatter(bg,(curr-bg)*sizeof(bg));
}

template<output_stream dest_type,buffer_output_stream source_type>
constexpr void print_define(dest_type& dest,manip::drainage<source_type&> source)
{
	write(dest,obuffer_begin(source.reference),obuffer_curr(source.reference));
}

template<output_stream dest_type,buffer_output_stream source_type>
inline constexpr void drain(dest_type&& dest,source_type&& source)
{
	print(std::forward<dest_type>(dest),drainage(source));
}


}