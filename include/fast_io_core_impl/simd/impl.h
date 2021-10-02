#pragma once

#if defined(__SSE__) && defined(__x86_64__) && (defined(__GNUC__) || defined(__clang__))
#include"x86_64.h"
#endif