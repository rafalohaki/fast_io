cmake_minimum_required(VERSION 3.15)

project(fast_io)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	# using Visual Studio C++
add_definitions(/experimental:concepts)
#if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -s")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s")
add_definitions(-ffast-math -Wall -Wextra)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -s")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s")
add_definitions(-Xclang -fconcepts-ts -ffast-math)
endif()

set(CMAKE_CXX_STANDARD 20)


INSTALL (DIRECTORY ${CMAKE_SOURCE_DIR}/include DESTINATION include)
INSTALL (DIRECTORY ${CMAKE_SOURCE_DIR}/man DESTINATION man/man3)