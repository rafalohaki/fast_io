cmake_minimum_required(VERSION 3.15)
project(fastio_examples)
find_package(Threceive)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  # using Visual Studio C++
add_definitions(/experimental:concepts)
#if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 10)
message(FATAL_ERROR "fast_io no longer supports GCC < 10 version because of incomplete C++ standard library support for concepts lite. Please build the master version of GCC from its official or mirror repo. Or use Visual Studio Preview")
endif()
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -s")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s")
add_definitions(-ffast-math -Wall -Wextra)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -s")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s")
add_definitions(-Xclang -fconcepts-ts -ffast-math)
endif()

set(CMAKE_CXX_STANDARD 20)

add_executable(0000.compile "./0000.compile/fast_io.cc")
add_executable(0001.hw "./0001.helloworld/hw.cc")
add_executable(0001.hw_u8 "./0001.helloworld/hw_u8.cc")
if (WIN32)
add_executable(0001.hello_world_win32_box "./0001.helloworld/hello_world_win32_box.cc")
endif (WIN32)

add_executable(0002.dec "./0002.a+b/dec.cc")
add_executable(0002.string "./0002.a+b/string.cc")
add_executable(0002.hex "./0002.a+b/hex.cc")
add_executable(0002.hexupper "./0002.a+b/hexupper.cc")
add_executable(0002.base36 "./0002.a+b/base36.cc")
add_executable(0002.base36upper "./0002.a+b/base36upper.cc")
add_executable(0002.base36upper_sgn "./0002.a+b/base36upper_sgn.cc")
add_executable(0002.bin "./0002.a+b/bin.cc")
add_executable(0002.dectext "./0002.a+b/dectext.cc")
add_executable(0002.floating "./0002.a+b/floating.cc")

add_executable(0003.all "./0003.floating_point/all.cc")
add_executable(0003.all_comma "./0003.floating_point/all_comma.cc")
add_executable(0003.comma "./0003.floating_point/comma.cc")
add_executable(0003.comma_pi "./0003.floating_point/comma_pi.cc")
add_executable(0003.fixed "./0003.floating_point/fixed.cc")
add_executable(0003.scientific "./0003.floating_point/scientific.cc")
add_executable(0003.sanshengage "./0003.floating_point/sanshengage.cc")

add_executable(0004.concat "./0004.string/concat.cc")
add_executable(0004.hello_kate "./0004.string/hello_kate.cc")
add_executable(0004.hello_kate_concat "./0004.string/hello_kate_concat.cc")
add_executable(0004.hello_kate_concat_more "./0004.string/hello_kate_concat_more.cc")
add_executable(0004.hello_kate_ostring_ref "./0004.string/hello_kate_ostring_ref.cc")

add_executable(0005.file_hw "./0005.file_io/file_hw.cc")

add_executable(0006.1ms "./0006.datetime/1ms.cc")
add_executable(0006.current_time "./0006.datetime/current_time.cc")

add_executable(0007.c_file_from_posix_io_handle "./0007.legacy/c_file_from_posix_io_handle.cc")
add_executable(0007.c_file_unlocked_from_posix_out "./0007.legacy/c_file_unlocked_from_posix_out.cc")
add_executable(0007.stream_io_observer "./0007.legacy/streambuf_io_observer.cc")
add_executable(0007.stream_fd "./0007.legacy/stream_fd.cc")

add_executable(0009.transmit "./0009.transmit/transmit.cc")
add_executable(0009.http_test_zero_copy "./0009.transmit/http_test_zero_copy.cc")
add_executable(0009.transmit_bytes "./0009.transmit/transmit_bytes.cc")
add_executable(0009.transmit_to_c_stdout "./0009.transmit/transmit_to_c_stdout.cc")

add_executable(0010.http "./0010.http/http.cc")

add_executable(0011.black_hole "./0011.black_hole/black_hole.cc")

add_executable(0013.client "./0013.tcp/client.cc")
add_executable(0013.server "./0013.tcp/server.cc")

add_executable(0016.process "./0016.process/process.cc")
if (WIN32)
add_executable(0019.win32 "./0019.win32/win32.cc")
endif (WIN32)

add_executable(0039.filebuf_file "./0039.filebuf_file/filebuf_file.cc")
add_executable(0039.posix_file "./0039.filebuf_file/posix_file.cc")
if (WIN32)
add_executable(0039.win32_file "./0039.filebuf_file/win32_file.cc")
endif (WIN32)
add_executable(0044.sha256 "./0044.sha/sha256.cc")
add_executable(0044.hmac_sha256 "./0044.sha/hmac_sha256.cc")

add_executable(0022.utf8_file "./0022.utf/utf8_file.cc")


add_executable(0073.fast_io_lines "./0073.filesystem/fast_io_lines.cc")
