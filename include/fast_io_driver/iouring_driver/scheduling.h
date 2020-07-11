#pragma once

namespace fast_io::linux
{
inline void single_thread_io_async_scheduling(std::stop_token token,io_uring_observer ring)
{
	for(bool got_comp{};!token.stop_requested();)
	{
		io_uring_cqe *cqe{};
		int ret{};
		if(got_comp)
		{
			ret = io_uring_peek_cqe(ring.ring,std::addressof(cqe))
			if(ret == -EAGAIN)
			{
				cqe = {};
				ret = {};
			}
		}
		else
		{
			ret = io_uring_wait_cqe(ring.ring,std::addressof(cqe));
			got_comp = 1;
		}
		if(ret<0)
			throw_posix_error(-ret);
		if(!cqe)
			continue;
		std::unique_ptr<fast_io::details::io_uring_overlapped_base> data{fast_io::details::bit_cast<fast_io::details::io_uring_overlapped_base*>(io_uring_cqe_get_data(cqe))};
		data->invoke(cqe);
		io_uring_cqe_seen(ring.ring,std::addressof(cqe));
	}
}
}
