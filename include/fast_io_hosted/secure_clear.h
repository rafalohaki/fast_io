#pragma once

namespace fast_io
{
namespace win32
{
#if defined(_WIN32)
extern "C" void __stdcall RtlSecureZeroMemory(void*,std::size_t);
#endif
}
inline void secure_clear(void* data,std::size_t size)
{
#if defined(_WIN32)
	win32::RtlSecureZeroMemory(data, size);
#elif defined(__linux__) || defined(__unix__)
	::explicit_bzero(data, size);
#else
	for(std::byte volatile* vd(reinterpret_cast<std::byte volatile*>(data)),ed(vd+size);vd!=ed;++vd)
		*vd=0;
#endif
}
}