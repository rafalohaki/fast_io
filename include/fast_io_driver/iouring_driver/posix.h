#pragma once

namespace fast_io
{

namespace details
{

struct io_uring_overlapped_base
{
	virtual void invoke(io_uring_cqe*) noexcept = 0;
	virtual ~io_uring_overlapped_base()=default;
};

enum class uring_operation_callbacks
{
callback_bytes
};

template<typename T,uring_operation_callbacks cabs>
struct io_uring_overlapped_data:io_uring_overlapped_base
{
	T callback;
	template<typename... Args>
	requires std::constructible_from<T,Args...>
	io_uring_overlapped_data(std::in_place_t,Args&& ...args):callback(std::forward<Args>(args)...){}
	void invoke(io_uring_cqe* cqe) noexcept override
	{
//		if constexpr(cabs==uring_operation_callbacks::callback_bytes)
//			callback(cqe,);
	}
};

}

template<std::integral char_type,typename Func>
inline void async_scatter_write_callback(io_uring_observer ring, basic_posix_io_observer<char_type> piob,std::span<io_scatter_t const> span,Func&& callback)
{
	std::unique_ptr<details::io_uring_overlapped_base> uptr(new details::io_uring_overlapped_data<Func,details::uring_operation_callbacks::callback_bytes>(std::in_place,std::forward<Func>(callback)));
	auto sqe{io_uring_get_sqe(ring.ring)};
	if(sqe==nullptr)
		throw_posix_error();
	io_uring_prep_writev(sqe,piob.fd,reinterpret_cast<details::iovec_may_alias const*>(span.data()),span.size(),0);
	io_uring_sqe_set_data(sqe,uptr.get());
	if(io_uring_submit(ring.ring)<0)
		throw_posix_error();
	uptr.release();
}

}
