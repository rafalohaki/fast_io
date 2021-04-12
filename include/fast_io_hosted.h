#pragma once
/*
https://en.cppreference.com/w/cpp/freestanding
There are two kinds of implementations defined by the C++ standard:
hosted and freestanding implementations.
For hosted implementations the set of standard library headers required by the C++ standard is much larger than for freestanding ones.
*/
//fast_io_hosted defines what we could use in a hosted environment.
#include"fast_io_freestanding.h"
#include<string>
#include"fast_io_hosted/string/impl.h"
#include"fast_io_hosted/concat.h"
#ifdef __MSDOS__
#undef __STRICT_ANSI__
#endif

#include"fast_io_hosted/cstring_view.h"

#include"fast_io_hosted/mmap.h"
#include"fast_io_hosted/chrono.h"
#include"fast_io_hosted/posix_status.h"
#include"fast_io_hosted/platforms/native.h"
#include"fast_io_hosted/file_loaders/impl.h"

#if __cpp_lib_jthread >= 201703L
#include"fast_io_hosted/iomutex.h"
#endif
#include"fast_io_hosted/wrapper.h"
#include"fast_io_hosted/filesystem/native.h"

#ifdef FAST_IO_PROCESS_REVAMP
#include"fast_io_hosted/process_revamp/native.h"
#endif

#include"fast_io_hosted/white_hole/white_hole.h"
//#include<memory_resource>
//#include"fast_io_hosted/pmr_resource.h"
#include<complex>
#include"fast_io_hosted/complex.h"

#include"fast_io_hosted/time.h"
#if defined(_WIN32) || defined(__CYGWIN__)
#include"fast_io_hosted/box.h"
#endif
