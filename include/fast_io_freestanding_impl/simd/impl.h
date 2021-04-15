#pragma once

#if defined(__SSE__) && defined(__x86_64__) && !defined(FAST_IO_DISABLE_CODECVT)
#include"x86_64.h"
#endif