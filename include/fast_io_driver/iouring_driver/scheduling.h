#pragma once

namespace fast_io::linux
{
inline void single_thread_io_async_scheduling(io_uring_observer ring)
{
	while(true)
	{
		io_uring_cqe *cqe{};
		int ret{io_uring_wait_cqe(ring.ring,std::addressof(cqe))};
		if(ret<0)
			throw_posix_error(-ret);
		if(!cqe)
			continue;
		void* data{io_uring_cqe_get_data(cqe)};
		std::int32_t res{cqe->res};
		io_uring_cqe_seen(ring.ring,cqe);
		if(res<0)
			continue;
		io_uring_overlapped over(bit_cast<typename io_uring_overlapped::native_handle_type>(data));
		over(static_cast<std::size_t>(res));
	}
}
}
