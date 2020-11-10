#pragma once

#include"io_observer.h"

#if defined(__WINNT__) || defined(_MSC_VER)
#include"win32.h"
#else
#include <sys/types.h>
#include <sys/wait.h>
#include"posix_exec.h"
#include"posix.h"
#endif

//#include"process.h"

namespace fast_io
{
#if defined(__WINNT__) || defined(_MSC_VER)
using process = win32_process;
using jprocess = win32_jprocess;
#else
using process = posix_process;
using jprocess = posix_jprocess;
#endif
}