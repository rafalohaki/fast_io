#pragma once

namespace fast_io
{

template<std::integral char_type>
inline constexpr io_type_t<io_uring_observer> async_scheduler_type(basic_socket_io_observer<char_type>)
{
	return {};
}

template<std::integral char_type>
inline constexpr io_type_t<io_uring_overlapped> async_overlapped_type(basic_socket_io_observer<char_type>)
{
	return {};
}


template<std::integral char_type>
inline void async_connect_callback(io_uring_observer ring,basic_socket_io_observer<char_type> sio,endpoint& ep,io_uring_overlapped_observer callback)
{
	auto sqe{io_uring_get_sqe(ring.ring)};
	for(;sqe==nullptr;sqe=io_uring_get_sqe(ring.ring))
		submit(ring);
	io_uring_prep_connect(sqe,sio.soc,std::addressof(ep.storage.sock),ep.storage_size);
	io_uring_sqe_set_data(sqe,callback.native_handle());
}

template<std::integral char_type>
inline void async_accept_callback(io_uring_observer ring,basic_socket_io_observer<char_type> sio,endpoint& ep,io_uring_overlapped_observer callback)
{
	auto sqe{io_uring_get_sqe(ring.ring)};
	for(;sqe==nullptr;sqe=io_uring_get_sqe(ring.ring))
		submit(ring);
	io_uring_prep_accept(sqe,sio.soc,std::addressof(ep.storage.sock),std::addressof(ep.storage_size),0);//,std::addressof(ep.storage.sock),ep.storage_size);
	io_uring_sqe_set_data(sqe,callback.native_handle());
}

template<std::integral char_type>
inline void async_scatter_write_callback(io_uring_observer ring,basic_socket_io_observer<char_type> sio,std::span<io_scatter_t const> sp,io_uring_overlapped_observer callback,std::ptrdiff_t offset=0)
{
	basic_posix_io_observer<char_type> pio(sio);
	async_scatter_write_callback(ring,pio,sp,callback,offset);
}

template<std::integral char_type>
inline void async_scatter_read_callback(io_uring_observer ring,basic_socket_io_observer<char_type> sio,std::span<io_scatter_t const> sp,io_uring_overlapped_observer callback,std::ptrdiff_t offset=0)
{
	basic_posix_io_observer<char_type> pio(sio);
	async_scatter_read_callback(ring,pio,sp,callback,offset);
}


}
