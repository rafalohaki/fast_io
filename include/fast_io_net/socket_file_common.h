#pragma once

namespace fast_io
{
using native_socket_file=basic_native_socket_file<char>;
using wnative_socket_file=basic_native_socket_file<wchar_t>;
using u8native_socket_file=basic_native_socket_file<char8_t>;
using u16native_socket_file=basic_native_socket_file<char16_t>;
using u32native_socket_file=basic_native_socket_file<char32_t>;
}
