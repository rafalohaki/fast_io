#pragma once


namespace fast_io
{

class io_uring_observer
{
public:
	using native_handle_type = struct ::io_uring*;
	native_handle_type ring{};
	constexpr native_handle_type& native_handle() noexcept
	{
		return ring;
	}
	constexpr native_handle_type const& native_handle() const noexcept
	{
		return ring;
	}
	constexpr native_handle_type release() noexcept
	{
		auto temp{ring};
		ring=nullptr;
		return temp;
	}
};

class io_uring:public io_uring_observer
{
public:
	using native_handle_type = struct ::io_uring*;
	constexpr io_uring()=default;
	constexpr io_uring(native_handle_type handle):io_uring_observer{handle}
	{
	}
	io_uring(native_interface_t,unsigned entries,unsigned flags):io_uring_observer{new ::io_uring}
	{
		io_uring queue(this->native_handle());
		if(io_uring_queue_init(entries,this->native_handle(),flags)<0)[[unlikely]]
			throw_posix_error();
		queue.release();
	}
	io_uring(io_async_t):io_uring(native_interface,64,0){}
	io_uring(io_uring const&)=delete;
	io_uring& operator=(io_uring const&)=delete;
	constexpr io_uring(io_uring&& bmv) noexcept:io_uring_observer{bmv.release()}{}
#if __cpp_constexpr_dynamic_alloc >= 201907L
	constexpr
#endif
	io_uring& operator=(io_uring&& bmv) noexcept
	{
		if(this->native_handle()==bmv.native_handle())[[unlikely]]
			return *this;
		delete native_handle();
		native_handle()=bmv.release();
		return *this;
	}
	~io_uring()
	{
		if(this->native_handle()) [[likely]]
			io_uring_queue_exit(this->native_handle());
	}
};


}
