#pragma once
#include<cstddef>
#include<cstdint>
#include<cstring>
#include<bit>
#include<array>

namespace fast_io
{

namespace details::allocation
{
#if defined(_WIN32)
extern "C" void* __stdcall GetProcessHeap() noexcept;
extern "C" void* __stdcall HeapAlloc(void*,std::uint32_t,std::size_t) noexcept;
extern "C" int __stdcall HeapFree(void*,std::size_t,void*) noexcept;
#endif

inline std::byte* my_allocate(std::size_t allocate_bytes) noexcept
{
#if defined(_WIN32)
	return reinterpret_cast<std::byte*>(HeapAlloc(GetProcessHeap(),0,allocate_bytes));
#else
	return reinterpret_cast<std::byte*>(::malloc(allocate_bytes));
#endif
}

struct page_mapped
{
	std::byte* page_mapped_end{};
	std::byte* page_mapped_capacity{};
	std::size_t allocated_pages{};
};

struct bucket
{
	std::byte* flist{};
	page_mapped pm;
};

inline std::byte* non_happy_buc_allocate(page_mapped& pm,std::size_t bytes) noexcept
{
	if(pm.allocated_pages==0)
	{
		pm.allocated_pages=4096;
		if(4096<bytes)
			pm.allocated_pages=bytes;
	}
	pm.page_mapped_capacity=(pm.page_mapped_end=my_allocate(pm.allocated_pages))+pm.allocated_pages;
	pm.allocated_pages<<=1;
	auto temp{pm.page_mapped_end};
	pm.page_mapped_end+=bytes;
	return temp;
}

inline std::byte* buc_allocate(bucket& buc,std::size_t bytes) noexcept
{
	if(buc.flist==nullptr)[[unlikely]]
	{
		if(buc.pm.page_mapped_end==buc.pm.page_mapped_capacity)[[unlikely]]
			return non_happy_buc_allocate(buc.pm,bytes);
		auto temp{buc.pm.page_mapped_end};
		buc.pm.page_mapped_end+=bytes;
		return temp;
	}
	std::byte* ptr{buc.flist};
	memcpy(std::addressof(buc.flist),buc.flist,sizeof(std::uintptr_t));
	return ptr;
}

inline void buc_deallocate(bucket& buc,std::byte* ptr) noexcept
{
	memcpy(ptr,std::addressof(buc.flist),sizeof(std::uintptr_t));
	buc.flist=ptr;
}

inline thread_local constinit std::array<bucket,sizeof(std::byte*)*8-5> buckets;

inline std::byte* real_allocate(std::size_t sz) noexcept
{
	using namespace fast_io::details::allocation;
	return buc_allocate(fast_io::details::allocation::buckets[std::bit_width(sz>>5)],std::bit_ceil(sz));
}
}
}

inline void* operator new(std::size_t sz) noexcept
{
	return fast_io::details::allocation::real_allocate(sz);
}

inline void operator delete(void* ptr,std::size_t sz) noexcept
{
	if(ptr==nullptr)[[unlikely]]
		return;
	fast_io::details::allocation::buc_deallocate(fast_io::details::allocation::buckets[std::bit_width(sz>>5)],reinterpret_cast<std::byte*>(ptr));
}

inline void* operator new[](std::size_t sz) noexcept
{
	auto ptr{fast_io::details::allocation::real_allocate(sizeof(std::size_t)+sz)};
	std::memcpy(ptr,std::addressof(sz),sizeof(sz));
	return ptr+sizeof(std::size_t);
}

inline void operator delete[](void* ptr) noexcept
{
	if(ptr==nullptr)[[unlikely]]
		return;
	std::size_t bytes;
	auto real_ptr{reinterpret_cast<std::byte*>(ptr)-sizeof(std::size_t)};
	std::memcpy(std::addressof(bytes),real_ptr,sizeof(std::size_t));
	operator delete(real_ptr,bytes);
}
