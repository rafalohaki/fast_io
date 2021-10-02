#pragma once

//fast_io_crypto.h defines what you could use for cryptography
//It is likely usable in a freestanding environment
#if !defined(__cplusplus)
#error "You are not using C++ compiler"
#endif

#if defined(__GNUC__) && __GNUC__>=11 && __cplusplus<202002L
#error "fast_io requires at least C++20 standard compiler."
#else

/*
TODO: Make crypto work in freestanding environment
*/
#include"fast_io_core.h"
//#include"fast_io_crypto/symmetric_crypto.h"
#include"fast_io_crypto/hash/intrin_include.h"
#include"fast_io_crypto/hash/hash.h"
#include"fast_io_crypto/tls/tls.h"

#endif