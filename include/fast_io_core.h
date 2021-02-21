#pragma once

//fast_io_core.h is required to be usable in freestanding environment with EVEN dynamic memory allocation and exceptions are disabled.
#include<cstddef>
#include<version>
#include"fast_io_concept.h"
#include<memory>
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
#include<utility>		//for std::move, std::forward, std::addressof
#include<array>
#include<string_view>
#include<ranges>		//for ranges concepts
#include<algorithm>		//std::fill std::fill_n std::copy std::copy_n
#include<cstring>		//for memcpy
#include<bit>			//for std::endian, std::rotl and std::bit_cast etc


#include"fast_io_core_impl/intrinsics.h"

#include"fast_io_core_impl/ebcdic.h"
#include"fast_io_core_impl/char_category.h"
//fast_io core
#include"fast_io_core_impl/utils.h"

#include"fast_io_core_impl/overflow.h"
#include"fast_io_core_impl/terminate.h"
#if __cpp_lib_three_way_comparison >= 201907L
#include"fast_io_core_impl/compare.h"
#endif
#include"fast_io_core_impl/memptr.h"
#include"fast_io_core_impl/local_new_array_ptr.h"
#include"fast_io_core_impl/internal_temporary_buffer.h"
#include"fast_io_core_impl/manip/impl.h"
#include"fast_io_core_impl/mode.h"
#include"fast_io_core_impl/perms.h"
#include"fast_io_core_impl/seek.h"
#include"fast_io_core_impl/drain.h"

#include"fast_io_core_impl/ospan.h"

#include"fast_io_core_impl/igenerator.h"
#include"fast_io_core_impl/unit_character_input_buffer.h"
#include"fast_io_core_impl/io_ref.h"
#include"fast_io_core_impl/print_scan.h"
#include"fast_io_core_impl/print_freestanding.h"
//#include"fast_io_core_impl/print_reserver.h"
#include"fast_io_core_impl/scan_transmit/scan_transmit.h"



#include"fast_io_core_impl/scan_freestanding.h"

// This should provide an option macro to disable any generation for table in freestanding environments.
#include"fast_io_core_impl/integers/integer.h"

#include"fast_io_core_impl/black_hole.h"
#include"fast_io_core_impl/istring_view.h"
// Provide template prototype for custom string types
//#include"fast_io_core_impl/skip.h"

// Precondition should be removed in the future. Programming bugs
//#include"fast_io_core_impl/read_write_ranges.h"

#include"fast_io_core_impl/secure_clear_guard.h"
#include"fast_io_core_impl/transmit.h"
#include"fast_io_core_impl/random_access_transmit.h"
#include"fast_io_core_impl/transmit_once.h"
#include"fast_io_core_impl/unsigned_extension.h"
#include"fast_io_core_impl/flush.h"


#if __cpp_lib_source_location >= 201907L
#include<source_location>
#include"fast_io_core_impl/source_location.h"
#endif

#include"fast_io_core_impl/posix_error_scatter.h"
#include"fast_io_core_impl/iso/isos.h"

#include"fast_io_core_impl/codecvt/impl.h"
#include"fast_io_core_impl/io_deco_ref.h"