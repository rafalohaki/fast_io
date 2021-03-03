cmake_minimum_required(VERSION 3.15)
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	# using Visual Studio C++
add_compile_options(/MD /source-charset:UTF-8)
message(STATUS "MSVC does not support the GB18030 execution charset correctly. It is still buildable, but locale files will not display correctly on Chinese Computers. I prefer using GCC to build this.")
else()
add_compile_options(-finput-charset=UTF-8)
add_link_options(-s)
endif()

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
message(STATUS "Clang does not support -fexec-charset=GB18030, which means it will not work correctly on Computers in China. Consider using GCC to build this. We will not build dynamic libraries that store locale data for GB18030.")
else()
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

set(CMAKE_CXX_STANDARD 20)
set(BUILD_STATIC_LIBS OFF)

project(testfilter)


file(GLOB_RECURSE example ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)
message(STATUS ${example})
foreach(file IN LISTS example)
    # Get file name without directory
    get_filename_component(name ${file} NAME_WE)
    add_executable(${name} ${file})
endforeach()
