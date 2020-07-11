#pragma once

namespace fast_io
{


template<std::integral char_type,typename Func>
inline void async_scatter_write_callback(io_uring_observer ring, basic_posix_io_observer<char_type> piob,std::span<io_scatter_t const> span,Func&& callback)
{
	io_uring_overlapped over(std::in_place,std::forward<Func>(callback));
	auto sqe{io_uring_get_sqe(ring.ring)};
	if(sqe==nullptr)
		throw_posix_error();
	io_uring_prep_writev(sqe,piob.fd,reinterpret_cast<details::iovec_may_alias const*>(span.data()),span.size(),0);
	io_uring_sqe_set_data(sqe,over.native_handle());
	if(io_uring_submit(ring.ring)<0)
		throw_posix_error();
	over.release();
}

}
