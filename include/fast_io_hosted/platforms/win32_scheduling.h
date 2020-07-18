#pragma once

namespace fast_io
{

inline void io_async_wait(win32_io_observer iocp)
{
	std::uint32_t transferred{};
	std::uintptr_t completionkey{};
	win32::overlapped *over{};
	if(!GetQueuedCompletionStatus(iocp.native_handle(),std::addressof(transferred),std::addressof(completionkey),
		std::addressof(over),std::numeric_limits<std::uint32_t>::max()))
	{
#ifdef __cpp_exceptions
		throw win32_error();
#else
		fast_terminate();
#endif
	}
	iocp_overlapped iocp_over{static_cast<iocp_overlapped_base*>(over)};
	iocp_over(static_cast<std::size_t>(transferred));
}

namespace details
{
inline bool iocp_io_async_wait_timeout_detail(win32_io_observer iocp,std::uint32_t millseconds)
{
	std::uint32_t transferred{};
	std::uintptr_t completionkey{};
	win32::overlapped *over{};
	if(!GetQueuedCompletionStatus(iocp.native_handle(),std::addressof(transferred),std::addressof(completionkey),
		std::addressof(over),std::numeric_limits<std::uint32_t>::max()))
	{
#ifdef __cpp_exceptions
		throw win32_error();
#else
		fast_terminate();
#endif
		return false;
	}
	iocp_overlapped iocp_over{static_cast<iocp_overlapped_base*>(over)};
	iocp_over(static_cast<std::size_t>(transferred));
	return true;
}
}

template<typename Rep,typename Period>
inline bool io_async_wait_timeout(win32_io_observer iocp,std::chrono::duration<Rep,Period> duration)
{
	return details::iocp_io_async_wait_timeout_detail(iocp,std::chrono::duration_cast<std::chrono::milliseconds>(duration).count());
}

}
