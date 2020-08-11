#pragma once

namespace fast_io
{

template<std::integral char_type>
inline constexpr io_type_t<io_uring_observer> async_scheduler_type(basic_posix_io_observer<char_type>)
{
	return {};
}

template<std::integral char_type>
inline constexpr io_type_t<io_uring_overlapped> async_overlapped_type(basic_posix_io_observer<char_type>)
{
	return {};
}

template<std::integral char_type>
inline void async_scatter_write_callback(io_uring_observer ring,basic_posix_io_observer<char_type> piob,
		std::span<io_scatter_t const> span,io_uring_overlapped_observer callback,std::ptrdiff_t offset=0)
{
	auto sqe{io_uring_get_sqe(ring.ring)};
	for(;sqe==nullptr;sqe=io_uring_get_sqe(ring.ring))
		submit(ring);
	io_uring_prep_writev(sqe,piob.fd,reinterpret_cast<details::iovec_may_alias const*>(span.data()),span.size(),offset);
	io_uring_sqe_set_data(sqe,callback.native_handle());
}

template<std::integral char_type,std::contiguous_iterator Iter>
inline void async_write_callback(io_uring_observer ring, basic_posix_io_observer<char_type> piob,
	Iter begin,Iter end,io_uring_overlapped_observer callback,std::ptrdiff_t offset=0)
{
	auto sqe{io_uring_get_sqe(ring.ring)};
	for(;sqe==nullptr;sqe=io_uring_get_sqe(ring.ring))
		submit(ring);
	fast_io::io_scatter_t const sct{const_cast<void*>(static_cast<void const*>(std::to_address(begin))),(end-begin)*sizeof(*begin)};
	io_uring_prep_writev(sqe,piob.fd,reinterpret_cast<details::iovec_may_alias const*>(std::addressof(sct)),1,offset);
	io_uring_sqe_set_data(sqe,callback.native_handle());

}


template<std::integral char_type>
inline void async_scatter_read_callback(io_uring_observer ring,basic_posix_io_observer<char_type> piob,
		std::span<io_scatter_t const> span,io_uring_overlapped_observer callback,std::ptrdiff_t offset=0)
{
	auto sqe{io_uring_get_sqe(ring.ring)};
	for(;sqe==nullptr;sqe=io_uring_get_sqe(ring.ring))
		submit(ring);
	io_uring_prep_readv(sqe,piob.fd,reinterpret_cast<details::iovec_may_alias const*>(span.data()),span.size(),offset);
	io_uring_sqe_set_data(sqe,callback.native_handle());
}

template<std::integral char_type,std::contiguous_iterator Iter>
inline void async_read_callback(io_uring_observer ring, basic_posix_io_observer<char_type> piob,
	Iter begin,Iter end,io_uring_overlapped_observer callback,std::ptrdiff_t offset=0)
{
	auto sqe{io_uring_get_sqe(ring.ring)};
	for(;sqe==nullptr;sqe=io_uring_get_sqe(ring.ring))
		submit(ring);
	fast_io::io_scatter_t const sct{static_cast<void*>(std::to_address(begin)),(end-begin)*sizeof(*begin)};
	io_uring_prep_readv(sqe,piob.fd,reinterpret_cast<details::iovec_may_alias const*>(std::addressof(sct)),1,offset);
	io_uring_sqe_set_data(sqe,callback.native_handle());
}

}

