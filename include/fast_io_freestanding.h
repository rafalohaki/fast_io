#pragma once

//fast_io_freestanding.h is usable when the underlining system implements dynamic memory allocations and exceptions

#include"fast_io_core.h"
//#include<stdexcept>
//#include<bitset>
//#include<system_error>
//#include<cmath>
#include"fast_io_freestanding_impl/exception.h"
#include"fast_io_freestanding_impl/posix_error.h"
//compile floating point is slow since it requires algorithms like ryu
#include"fast_io_freestanding_impl/floating_point/floating.h"
#include"fast_io_freestanding_impl/io_io_file.h"
//#include"fast_io_freestanding_impl/natural.h"
#include"fast_io_freestanding_impl/io_buffer/impl.h"
#include"fast_io_freestanding_impl/naive_vector.h"
#include"fast_io_freestanding_impl/auto_indent.h"
#include"fast_io_freestanding_impl/ovector.h"
#include"fast_io_freestanding_impl/serialize.h"

namespace fast_io
{

template<input_stream input>
using basic_ibuf = basic_io_buffer<input,buffer_mode::in>;
template<output_stream output>
using basic_obuf = basic_io_buffer<output,buffer_mode::out>;
template<io_stream strm>
using basic_iobuf = basic_io_buffer<strm,buffer_mode::in|buffer_mode::out|buffer_mode::tie>;

template<std::integral internal_char_type,
	input_stream input,
	encoding_scheme internal_scheme=encoding_scheme::execution_charset,
	encoding_scheme external_scheme=encoding_scheme::execution_charset>
using basic_ibuf_code_cvt = basic_io_buffer<input,
	buffer_mode::in,
	basic_decorators<internal_char_type,
	basic_code_converter<typename input::char_type,external_scheme,internal_scheme>,
	empty_decorator>>;

template<std::integral internal_char_type,
	output_stream output,
	encoding_scheme internal_scheme=encoding_scheme::execution_charset,
	encoding_scheme external_scheme=encoding_scheme::execution_charset>
using basic_obuf_code_cvt = basic_io_buffer<output,
	buffer_mode::out,
	basic_decorators<internal_char_type,
	empty_decorator,
	basic_code_converter<internal_char_type,internal_scheme,external_scheme>>>;

template<std::integral internal_char_type,
	io_stream input_output,
	encoding_scheme internal_scheme=encoding_scheme::execution_charset,
	encoding_scheme external_scheme=encoding_scheme::execution_charset>
using basic_iobuf_code_cvt = basic_io_buffer<input_output,
	buffer_mode::in|buffer_mode::out|buffer_mode::tie,
	basic_decorators<internal_char_type,
	basic_code_converter<typename input_output::char_type,external_scheme,internal_scheme>,
	basic_code_converter<internal_char_type,internal_scheme,external_scheme>>>;

template<std::integral char_type>
using basic_iobuf_io_io_observer = basic_iobuf<basic_io_io_observer<char_type>>;
template<std::integral char_type>
using basic_iobuf_io_io_handle = basic_iobuf<basic_io_io_handle<char_type>>;
template<std::integral char_type>
using basic_iobuf_io_file = basic_iobuf<basic_io_file<char_type>>;
#ifndef _MSC_VER
template<std::integral char_type>
using basic_io_files = basic_ovector<basic_io_file<char_type>>;
using io_files = basic_io_files<char>;
#endif

using iobuf_io_io_observer = basic_iobuf_io_io_observer<char>;
using iobuf_io_io_handle = basic_iobuf_io_io_handle<char>;
using iobuf_io_file = basic_iobuf_io_file<char>;

using u8iobuf_io_io_observer = basic_iobuf_io_io_observer<char8_t>;
using u8iobuf_io_io_handle = basic_iobuf_io_io_handle<char8_t>;
using u8iobuf_io_file = basic_iobuf_io_file<char8_t>;

using wiobuf_io_io_observer = basic_iobuf_io_io_observer<wchar_t>;
using wiobuf_io_io_handle = basic_iobuf_io_io_handle<wchar_t>;
using wiobuf_io_file = basic_iobuf_io_file<wchar_t>;
}
