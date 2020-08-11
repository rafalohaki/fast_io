#pragma once

namespace fast_io
{

template<std::integral char_type>
inline void async_connect_callback(io_uring_observer iu,basic_socket_io_observer<char_type> sio,endpoint& ep,io_uring_overlapped_observer callback)
{
	auto sqe{io_uring_get_sqe(ring.ring)};
	for(;sqe==nullptr;sqe=io_uring_get_sqe(ring.ring))
		submit(ring);
	io_uring_prep_connect(sqe,sio.soc,std::addressof(ep.storage.sock),ep.storage_size);
	io_uring_sqe_set_data(sqe,callback.native_handle());
}

template<std::integral char_type>
inline void async_accept_callback(io_uring_observer iu,basic_socket_io_observer<char_type> sio,endpoint& ep,io_uring_overlapped_observer callback)
{
	auto sqe{io_uring_get_sqe(ring.ring)};
	for(;sqe==nullptr;sqe=io_uring_get_sqe(ring.ring))
		submit(ring);
	io_uring_prep_accept(sqe,sio.soc,std::addressof(ep.storage.sock),std::addressof(ep.storage_size),0);//,std::addressof(ep.storage.sock),ep.storage_size);
	io_uring_sqe_set_data(sqe,callback.native_handle());
}


template<std::integral char_type,std::contiguous_iterator Iter>
inline void async_write_callback(io_uring_observer iu,basic_socket_io_observer<char_type> sio,Iter begin,Iter end,io_uring_overlapped_observer callback,std::ptrdiff_t offset=0)
{
	auto sqe{io_uring_get_sqe(ring.ring)};
	for(;sqe==nullptr;sqe=io_uring_get_sqe(ring.ring))
		submit(ring);
	io_uring_prep_send(sqe,sio.soc,std::to_address(begin),(end-begin)*sizeof(*begin),0);
	io_uring_sqe_set_data(sqe,callback.native_handle());
}

template<std::integral char_type,std::contiguous_iterator Iter>
inline void async_read_callback(io_uring_observer iu,basic_socket_io_observer<char_type> sio,Iter begin,Iter end,io_uring_overlapped_observer callback,std::ptrdiff_t offset=0)
{
	auto sqe{io_uring_get_sqe(ring.ring)};
	for(;sqe==nullptr;sqe=io_uring_get_sqe(ring.ring))
		submit(ring);
	io_uring_prep_recv(sqe,sio.soc,std::to_address(begin),(end-begin)*sizeof(*begin),0);
	io_uring_sqe_set_data(sqe,callback.native_handle());
}



}