#pragma once

namespace fast_io::details::heapalloc
{
#if defined(_WIN32)
extern "C" void* __stdcall GetProcessHeap() noexcept;
extern "C" void* __stdcall HeapAlloc(void*,std::uint32_t,std::size_t) noexcept;
extern "C" int __stdcall HeapFree(void*,std::uint32_t,void*) noexcept;
#endif
}

inline void* operator new(std::size_t sz) noexcept
{
	auto ptr{fast_io::details::heapalloc::HeapAlloc(fast_io::details::heapalloc::GetProcessHeap(),0,sz)};
	if(ptr==nullptr)
		fast_io::fast_terminate();
	return ptr;
}

inline void operator delete(void* ptr) noexcept
{
	if(ptr==nullptr)[[unlikely]]
		return;
	fast_io::details::heapalloc::HeapFree(fast_io::details::heapalloc::GetProcessHeap(),0,ptr);
}

inline void* operator new[](std::size_t sz) noexcept
{
	return fast_io::details::heapalloc::HeapAlloc(fast_io::details::heapalloc::GetProcessHeap(),0,sz);
}

inline void operator delete[](void* ptr) noexcept
{
	if(ptr==nullptr)[[unlikely]]
		return;
	fast_io::details::heapalloc::HeapFree(fast_io::details::heapalloc::GetProcessHeap(),0,ptr);
}
