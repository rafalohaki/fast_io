#pragma once

//fast_io_freestanding.h is usable when the underlining system implements dynamic memory allocations and exceptions

#include"fast_io_core.h"
#include<stdexcept>
#include<string>
#include<bitset>
//#include<system_error>
//#include<cmath>
#include"fast_io_freestanding_impl/string/impl.h"
#include"fast_io_freestanding_impl/concat.h"
#include"fast_io_freestanding_impl/manip.h"
#include"fast_io_freestanding_impl/exception.h"
#include"fast_io_freestanding_impl/reserve_read_write.h"
#include"fast_io_freestanding_impl/code_cvt/utf.h"
#include"fast_io_freestanding_impl/posix_error.h"
//compile floating point is slow since it requires algorithms like ryu
#include"fast_io_freestanding_impl/floating_point/floating.h"
#include"fast_io_freestanding_impl/iobuf.h"
#include"fast_io_freestanding_impl/io_io_file.h"
#include"fast_io_freestanding_impl/send_receive.h"
#include"fast_io_freestanding_impl/natural.h"

#include"fast_io_freestanding_impl/transformers/transformers.h"
#include"fast_io_freestanding_impl/indirect_ibuffer.h"
#include"fast_io_freestanding_impl/indirect_obuffer.h"
#include"fast_io_freestanding_impl/ovector.h"
//#include"fast_io_freestanding_impl/ucs.h"

namespace fast_io
{

template<std::integral char_type>
using basic_iobuf_io_io_observer = basic_iobuf<basic_io_io_observer<char_type>>;
template<std::integral char_type>
using basic_iobuf_io_io_handle = basic_iobuf<basic_io_io_handle<char_type>>;
template<std::integral char_type>
using basic_iobuf_io_file = basic_iobuf<basic_io_file<char_type>>;

template<std::integral char_type>
using basic_io_files = basic_ovector<basic_io_file<char_type>>;

using io_files = basic_io_files<char>;

using iobuf_io_io_observer = basic_iobuf_io_io_observer<char>;
using iobuf_io_io_handle = basic_iobuf_io_io_handle<char>;
using iobuf_io_file = basic_iobuf_io_file<char>;

using u8iobuf_io_io_observer = basic_iobuf_io_io_observer<char8_t>;
using u8iobuf_io_io_handle = basic_iobuf_io_io_handle<char8_t>;
using u8iobuf_io_file = basic_iobuf_io_file<char8_t>;


#ifndef __MSDOS__
using wiobuf_io_io_observer = basic_iobuf_io_io_observer<wchar_t>;
using wiobuf_io_io_handle = basic_iobuf_io_io_handle<wchar_t>;
using wiobuf_io_file = basic_iobuf_io_file<wchar_t>;
#endif
}
