#pragma once
/*
https://en.cppreference.com/w/cpp/freestanding
There are two kinds of implementations defined by the C++ standard:
hosted and freestanding implementations.
For hosted implementations the set of standard library headers required by the C++ standard is much larger than for freestanding ones.
*/
//fast_io_hosted defines what we could use in a hosted environment.
#include"fast_io_freestanding.h"
#include"fast_io_hosted/mmap.h"
#include"fast_io_hosted/platforms/native.h"
#include"fast_io_hosted/omap.h"
#if !defined(__NEWLIB__)||defined(_GLIBCXX_HAS_GTHREADS)
#include"fast_io_hosted/iomutex.h"
#include"fast_io_hosted/parallel.h"
#endif
#include"fast_io_hosted/chrono.h"
#include"fast_io_hosted/process/native.h"
#include"fast_io_hosted/white_hole/white_hole.h"
#include"fast_io_hosted/secure_clear.h"
