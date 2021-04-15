#pragma once

//fast_io_crypto.h defines what you could use for cryptography
//It is likely usable in a freestanding environment
#if !defined(__cplusplus)
#error "You are not using C++ compiler"
#endif

#if __cplusplus<202002L && !defined(_MSC_VER)
#error "fast_io requires at least C++20 standard compiler."
#else
#if __STDC_HOSTED__==1 && (!defined(_GLIBCXX_HOSTED) || _GLIBCXX_HOSTED==1)

/*
TODO: Make crypto work in freestanding environment
*/
#include"fast_io_core.h"
//#include"fast_io_crypto/symmetric_crypto.h"
#include"fast_io_crypto/hash/hash.h"
#include"fast_io_crypto/tls/tls.h"

#endif

#endif