#pragma once

namespace fast_io
{


[[noreturn]] inline void fast_terminate() noexcept
{
//https://llvm.org/doxygen/Compiler_8h_source.html

#if defined(_MSC_VER)
	__debugbreak();
#elif __has_builtin(__builtin_trap)
	__builtin_trap();
#else
	std::abort();
#endif

}

}