#pragma once

namespace fast_io
{


[[noreturn]] inline void fast_terminate() noexcept
{
#ifndef FAST_IO_NOT_TERMINATE
//https://llvm.org/doxygen/Compiler_8h_source.html

#if defined(_MSC_VER)
	__debugbreak();
#else
#if __has_builtin(__builtin_trap)
	__builtin_trap();
#else
	//create a null pointer hardware exception to terminate the program
	*(reinterpret_cast<char volatile*>(0))=0;
#endif
#endif
#endif
}

}