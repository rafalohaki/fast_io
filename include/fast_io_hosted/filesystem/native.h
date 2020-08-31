#pragma once

#if _BSD_SOURCE || _SVID_SOURCE || (_POSIX_C_SOURCE >= 200809L || _XOPEN_SOURCE >= 700)
#include"posix_directory.h"
#endif


namespace fast_io
{

#if _BSD_SOURCE || _SVID_SOURCE || (_POSIX_C_SOURCE >= 200809L || _XOPEN_SOURCE >= 700)
using native_directory_io_observer = posix_directory_io_observer;
using native_directory_io_handle = posix_directory_io_handle;
using native_directory_file = posix_directory_file;
using directory = native_directory_file;
#endif

}