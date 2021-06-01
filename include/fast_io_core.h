#pragma once

//fast_io_core.h is required to be usable in freestanding environment with EVEN dynamic memory allocation and exceptions are disabled.

#if !defined(__cplusplus)
#error "You are not using C++ compiler"
#endif

#if defined(__GNUC__) && __GNUC__>=11 && __cplusplus<202002L
#error "fast_io requires at least C++20 standard compiler."
#else
#include"fast_io_concept.h"

#include<version>
#include"fast_io_core_impl/empty.h"
#include<cstddef>
#include<concepts>
#include"fast_io_core_impl/freestanding/impl.h"
#include<type_traits>

#include<limits>
#include<cstdint>
//I know functions like memcpy would probably not be provided in freestanding environment. However, you can implement them on these platforms.
#if __cpp_lib_three_way_comparison >= 201907L
#include<compare>
#endif

//The C++20 standard does not require these headers to be freestanding.
//However, they are clearly necessary and usable even in freestanding environment.
//Leaving no room for a lower-level language: A C++ Subset
//http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p1105r0.html

#include<bit>			//for std::endian, std::rotl and std::bit_cast etc

//fast_io core
#include"fast_io_core_impl/utils.h"
#include"fast_io_core_impl/terminate.h"
#include"fast_io_core_impl/intrinsics.h"
#include"fast_io_core_impl/parse_code.h"

#include"fast_io_core_impl/ebcdic.h"
#include"fast_io_core_impl/char_category.h"


#include"fast_io_core_impl/overflow.h"


#if __cpp_lib_three_way_comparison >= 201907L
#include"fast_io_core_impl/compare.h"
#endif

#include"fast_io_core_impl/alias.h"

#include"fast_io_core_impl/secure_clear_guard.h"
#include"fast_io_core_impl/local_new_array_ptr.h"
#include"fast_io_core_impl/dynamic_io_buffer.h"
#include"fast_io_core_impl/manip/impl.h"
#include"fast_io_core_impl/mode.h"
#include"fast_io_core_impl/perms.h"
#include"fast_io_core_impl/seek.h"
#include"fast_io_core_impl/drain.h"

#include"fast_io_core_impl/igenerator.h"
#include"fast_io_core_impl/io_ref.h"
#include"fast_io_core_impl/print_freestanding.h"
#include"fast_io_core_impl/scan_transmit/scan_transmit.h"

#include"fast_io_core_impl/scan_freestanding.h"
// This should provide an option macro to disable any generation for table in freestanding environments.
#include"fast_io_core_impl/integers/integer.h"

#include"fast_io_core_impl/black_hole.h"
#include"fast_io_core_impl/istring_view.h"


#include"fast_io_core_impl/transmit.h"
#include"fast_io_core_impl/random_access_transmit.h"
#include"fast_io_core_impl/flush.h"


#ifdef __cpp_lib_source_location
#include<source_location>
#include"fast_io_core_impl/source_location.h"
#endif
#include"fast_io_core_impl/iso/isos.h"

#include"fast_io_core_impl/enums/impl.h"

#include"fast_io_core_impl/simd/impl.h"

#if !(defined(FAST_IO_DISABLE_CODECVT)&&(__STDC_HOSTED__==0 || (defined(_GLIBCXX_HOSTED) && _GLIBCXX_HOSTED==0)))
#include"fast_io_core_impl/codecvt/impl.h"
#endif
#include"fast_io_core_impl/io_deco_ref.h"

#include"fast_io_core_impl/unsafe_fprint.h"
#endif
#include"fast_io_core_impl/timestamp_counter.h"
