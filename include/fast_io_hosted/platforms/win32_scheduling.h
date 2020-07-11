#pragma once

namespace fast_io
{
namespace win32
{
inline void single_thread_io_async_scheduling(win32_io_observer iocp)
{
	for(;;)
	{
		std::uint32_t transferred{};
		std::uintptr_t completionkey{};
		overlapped *over{};
		if(!GetQueuedCompletionStatus(iocp.native_handle(),std::addressof(transferred),std::addressof(completionkey),
			std::addressof(over),std::numeric_limits<std::uint32_t>::max()))
		{
#ifdef __cpp_exceptions
			throw win32_error();
#else
			fast_terminate();
#endif
		}

		win32_overlapped_base& base(*reinterpret_cast<win32_overlapped_base*>(over));
		base.function_ptr(over,completionkey,transferred);
/*		switch(base.operations)
		{
		case win32_overlapped_operations::read:
		break;
		case win32_overlapped_operations::write:
			base.function_ptr(over,completionkey,transferred);
		break;
		case win32_overlapped_operations::transmit:
		break;
		case win32_overlapped_operations::cancel:
		break;
		}*/
	}
}
}

}
