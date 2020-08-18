#pragma once

#include"../../fast_io_crypto/hash/hash_processor.h"
#include"../../fast_io_crypto/hash/sha.h"

#if defined(__WINNT__) || defined(_MSC_VER)
#include"win32_api.h"
#include"win32_error.h"
#include"nt_api.h"
#include"nt_error.h"
#include"nt.h"
#include"win32_iocp_overlapped.h"
#include"win32.h"
//#include"com_error.h"
#endif
#if defined(__linux__)
#ifdef __x86_64__
#include"linux/amd64.h"
#endif
#endif
#include"linux/system_call.h"
#include"posix.h"

namespace fast_io
{

#if defined(__WINNT__) || defined(_MSC_VER)
inline constexpr auto native_stdin_number(win32_stdin_number);
inline constexpr auto native_stdout_number(win32_stdout_number);
inline constexpr auto native_stderr_number(win32_stderr_number);
using native_io_observer = win32_io_observer;
using native_io_handle = win32_io_handle;
using native_file = win32_file;
using native_pipe = win32_pipe;
using u8native_io_observer = u8win32_io_observer;
using u8native_io_handle = u8win32_io_handle;
using u8native_file = u8win32_file;
using u8native_pipe = u8win32_pipe;
using wnative_io_observer = wwin32_io_observer;
using wnative_io_handle = wwin32_io_handle;
using wnative_file = wwin32_file;
using wnative_pipe = wwin32_pipe;
template<std::integral ch_type>
using basic_native_io_observer = basic_win32_io_observer<ch_type>;
template<std::integral ch_type>
using basic_native_io_handle = basic_win32_io_handle<ch_type>;
template<std::integral ch_type>
using basic_native_file = basic_win32_file<ch_type>;
template<std::integral ch_type>
using basic_native_pipe = basic_win32_pipe<ch_type>;


#else
inline constexpr auto native_stdin_number(posix_stdin_number);
inline constexpr auto native_stdout_number(posix_stdout_number);
inline constexpr auto native_stderr_number(posix_stderr_number);

using native_io_observer = posix_io_observer;
using native_io_handle = posix_io_handle;
using native_file = posix_file;
using native_pipe = posix_pipe;
using u8native_io_observer = u8posix_io_observer;
using u8native_io_handle = u8posix_io_handle;
using u8native_file = u8posix_file;
using u8native_pipe = u8posix_pipe;
using wnative_io_observer = wposix_io_observer;
using wnative_io_handle = wposix_io_handle;
using wnative_file = wposix_file;
using wnative_pipe = wposix_pipe;
template<std::integral ch_type>
using basic_native_io_observer = basic_posix_io_observer<ch_type>;
template<std::integral ch_type>
using basic_native_io_handle = basic_posix_io_handle<ch_type>;
template<std::integral ch_type>
using basic_native_file = basic_posix_file<ch_type>;
template<std::integral ch_type>
using basic_native_pipe = basic_posix_pipe<ch_type>;

#endif

}