#pragma once

namespace fast_io
{
using native_socket_io_observer=basic_native_socket_io_observer<char>;
using wnative_socket_io_observer=basic_native_socket_io_observer<wchar_t>;
using u8native_socket_io_observer=basic_native_socket_io_observer<char8_t>;
using u16native_socket_io_observer=basic_native_socket_io_observer<char16_t>;
using u32native_socket_io_observer=basic_native_socket_io_observer<char32_t>;

using native_socket_io_handle=basic_native_socket_io_handle<char>;
using wnative_socket_io_handle=basic_native_socket_io_handle<wchar_t>;
using u8native_socket_io_handle=basic_native_socket_io_handle<char8_t>;
using u16native_socket_io_handle=basic_native_socket_io_handle<char16_t>;
using u32native_socket_io_handle=basic_native_socket_io_handle<char32_t>;

using native_socket_file=basic_native_socket_file<char>;
using wnative_socket_file=basic_native_socket_file<wchar_t>;
using u8native_socket_file=basic_native_socket_file<char8_t>;
using u16native_socket_file=basic_native_socket_file<char16_t>;
using u32native_socket_file=basic_native_socket_file<char32_t>;

using native_socket_file=basic_native_socket_file<char>;
using wnative_socket_file=basic_native_socket_file<wchar_t>;
using u8native_socket_file=basic_native_socket_file<char8_t>;
using u16native_socket_file=basic_native_socket_file<char16_t>;
using u32native_socket_file=basic_native_socket_file<char32_t>;

template<std::integral char_type>
using basic_iobuf_socket_file = basic_iobuf<basic_native_socket_file<char_type>>;

using iobuf_socket_file=basic_iobuf_socket_file<char>;
using wiobuf_socket_file=basic_iobuf_socket_file<wchar_t>;
using u8iobuf_socket_file=basic_iobuf_socket_file<char8_t>;
using u16iobuf_socket_file=basic_iobuf_socket_file<char16_t>;
using u32iobuf_socket_file=basic_iobuf_socket_file<char32_t>;

}
