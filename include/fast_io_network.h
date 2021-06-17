#pragma once


#if !defined(__cplusplus)
#error "You are not using C++ compiler"
#endif

#if defined(__GNUC__) && __GNUC__>=11 && __cplusplus<202002L
#error "fast_io requires at least C++20 standard compiler."
#else

#include"fast_io_hosted.h"

#include"fast_io_net/sock_domain.h"
#include"fast_io_net/sock_protocal.h"
#include"fast_io_net/sock_type.h"

#if defined(_WIN32) || defined(__CYGWIN__)
#include "fast_io_net/win32/win32.h"
#ifndef __CYGWIN__
#include "fast_io_net/socket_file_common.h"
#endif
#endif

#if !defined(_WIN32) && __has_include(<sys/socket.h>) && !defined(__wasi__)
#include <sys/socket.h>
#include "fast_io_net/posix/posix.h"
#include "fast_io_net/socket_file_common.h"
#endif

#endif
