#pragma once
/*
https://en.cppreference.com/w/cpp/freestanding
There are two kinds of implementations defined by the C++ standard:
hosted and freestanding implementations.
For hosted implementations the set of standard library headers required by the C++ standard is much larger than for freestanding ones.
*/
//fast_io_hosted defines what we could use in a hosted environment.
#include"fast_io_freestanding.h"


#if __cpp_lib_filesystem >= 201703L
#include<filesystem>
#endif
#include"fast_io_hosted/cstring_view.h"

#include"fast_io_hosted/mmap.h"
#include"fast_io_hosted/chrono.h"
#include"fast_io_hosted/posix_status.h"
#include"fast_io_hosted/platforms/native.h"


#if !defined(__NEWLIB__)||defined(_GLIBCXX_HAS_GTHREADS)
#include"fast_io_hosted/iomutex.h"
#endif
#include"fast_io_hosted/wrapper.h"
#include"fast_io_hosted/filesystem/native.h"
#include"fast_io_hosted/process/native.h"
#include"fast_io_hosted/white_hole/white_hole.h"
#include<memory_resource>
#include"fast_io_hosted/pmr_resource.h"
