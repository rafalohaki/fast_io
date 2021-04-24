#pragma once

namespace fast_io
{

namespace win32
{

struct rtl_critical_section
{
	void* debug_info;
	std::int32_t lock_count;
	std::int32_t recursion_count;
	void* owning_thread;
	void* lock_semaphore;
	std::uintptr_t spin_count;
};

}

struct win32_critical_section
{
	using native_handle_type = win32::rtl_critical_section;
	native_handle_type critical_section;
	win32_critical_section() noexcept
	{
		::fast_io::win32::InitializeCriticalSection(__builtin_addressof(critical_section));
	}
	win32_critical_section(win32_critical_section const&)=delete;
	win32_critical_section& operator=(win32_critical_section const&)=delete;
	void lock() noexcept
	{
		::fast_io::win32::EnterCriticalSection(__builtin_addressof(critical_section));
	}
	bool try_lock() noexcept
	{
		return ::fast_io::win32::TryEnterCriticalSection(__builtin_addressof(critical_section));
	}
	void unlock() noexcept
	{
		::fast_io::win32::LeaveCriticalSection(__builtin_addressof(critical_section));
	}
	~win32_critical_section()
	{
		::fast_io::win32::DeleteCriticalSection(__builtin_addressof(critical_section));
	}
};

}