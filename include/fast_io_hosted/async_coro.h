#pragma once

namespace fast_io
{
//This is DAMN bullshit
struct task
{
struct promise_type
{
constexpr auto get_return_object() { return task{}; }
constexpr auto initial_suspend() { return std::suspend_never{}; }
constexpr auto final_suspend() { return std::suspend_never{}; }
void unhandled_exception() { std::terminate(); }
constexpr void return_void() {}
};
};


template<typename stm,bool write>
requires ((write&&async_output_stream<stm>)||(!write&&async_input_stream<stm>))
class async_io_coroutine
{
public:
	typename io_async_scheduler_t<stm>::type& scheduler;
	stm& stream;
	io_scatter_t scatter{};
	std::ptrdiff_t offset{};
	typename io_async_overlapped_t<stm>::type overlapped;
	std::size_t transferred{};
	constexpr bool await_ready() const { return false; }
	constexpr std::size_t await_resume() const { return transferred; }
	void await_suspend(std::coroutine_handle<> handle)
	{
		overlapped=typename io_async_overlapped_t<stm>::type(std::in_place,[handle,this](std::size_t calb)
		{
			this->transferred=calb;
			handle.resume();
		});
		if constexpr(write)
			async_scatter_read_callback(scheduler,stream,std::span<io_scatter_t,1>{std::addressof(scatter),1},overlapped,offset);
		else
			async_scatter_write_callback(scheduler,stream,std::span<io_scatter_t,1>{std::addressof(scatter),1},overlapped,offset);
	}
};

template<async_input_stream stm>
constexpr inline async_io_coroutine<stm,false> async_read(typename io_async_scheduler_t<stm>::type& scheduler,stm& sm,io_scatter_t sc,std::ptrdiff_t offset=0)
{
	return {scheduler,sm,sc,offset};
}

template<async_output_stream stm>
constexpr inline async_io_coroutine<stm,true> async_write(typename io_async_scheduler_t<stm>::type& scheduler,stm& sm,io_scatter_t sc,std::ptrdiff_t offset=0)
{
	return {scheduler,sm,sc,offset};
}


template<typename stm,bool write>
requires ((write&&async_scatter_output_stream<stm>)||(!write&&async_scatter_input_stream<stm>))
class async_scatter_io_coroutine
{
public:
	typename io_async_scheduler_t<stm>::type& scheduler;
	stm& stream;
	std::span<io_scatter_t> scatters;
	std::ptrdiff_t offset{};
	typename io_async_overlapped_t<stm>::type overlapped;
	std::size_t transferred{};
	constexpr bool await_ready() const { return false; }
	constexpr std::size_t await_resume() const { return transferred; }
	void await_suspend(std::coroutine_handle<> handle)
	{
		overlapped=typename io_async_overlapped_t<stm>::type(std::in_place,[handle,this](std::size_t calb)
		{
			this->transferred=calb;
			handle.resume();
		});
		if constexpr(write)
			async_scatter_read_callback(scheduler,stream,scatters,overlapped,offset);
		else
			async_scatter_write_callback(scheduler,stream,scatters,overlapped,offset);
	}
};


template<async_scatter_input_stream stm>
constexpr inline async_scatter_io_coroutine<stm,false> async_scatter_read(typename io_async_scheduler_t<stm>::type& scheduler,stm& sm,std::span<io_scatter_t> sc,std::ptrdiff_t offset=0)
{
	return {scheduler,sm,sc,offset};
}

template<async_scatter_output_stream stm>
constexpr inline async_scatter_io_coroutine<stm,true> async_scatter_write(typename io_async_scheduler_t<stm>::type& scheduler,stm& sm,std::span<io_scatter_t> sc,std::ptrdiff_t offset=0)
{
	return {scheduler,sm,sc,offset};
}

template<async_output_stream stm,bool line>
class async_print_coroutine
{
public:
	typename io_async_scheduler_t<stm>::type& scheduler;
	stm& stream;
	std::ptrdiff_t offset{};
	typename io_async_overlapped_t<stm>::type overlapped;
	std::size_t transferred{};
	internal_temporary_buffer<typename stm::char_type> buffer;
	io_scatter_t scatter{};
	template<typename ...Args>
	async_print_coroutine(typename io_async_scheduler_t<stm>::type& sh,std::ptrdiff_t off,stm& s,Args&& ...args):scheduler(sh),stream(s)
	{
		if constexpr(line)
			println(buffer,std::forward<Args>(args)...);
		else
			print(buffer,std::forward<Args>(args)...);
		scatter.base=buffer.beg_ptr;
		scatter.len=sizeof(*buffer.beg_ptr)*(buffer.end_ptr-buffer.beg_ptr);
	}
	constexpr bool await_ready() const { return false; }
	constexpr std::size_t await_resume() const { return transferred; }
	void await_suspend(std::coroutine_handle<> handle)
	{
		overlapped=typename io_async_overlapped_t<stm>::type(std::in_place,[handle,this](std::size_t calb)
		{
			this->transferred=calb;
			handle.resume();
		});
		async_scatter_write_callback(scheduler,stream,std::span<io_scatter_t,1>{std::addressof(scatter),1},overlapped,offset);
	}
};

template<async_output_stream stm,typename... Args>
constexpr inline async_print_coroutine<stm,true> async_print(typename io_async_scheduler_t<stm>::type& scheduler,stm& sm,Args&& ...args)
{
	return async_print_coroutine<stm,true>(scheduler,0,sm,std::forward<Args>(args)...);
}
template<async_output_stream stm,typename... Args>
constexpr inline async_print_coroutine<stm,true> async_println(typename io_async_scheduler_t<stm>::type& scheduler,stm& sm,Args&& ...args)
{
	return async_print_coroutine<stm,true>(scheduler,0,sm,std::forward<Args>(args)...);
}

template<async_output_stream stm,typename... Args>
constexpr inline async_print_coroutine<stm,true> async_print(typename io_async_scheduler_t<stm>::type& scheduler,std::ptrdiff_t offset,stm& sm,Args&& ...args)
{
	return async_print_coroutine<stm,true>(scheduler,offset,sm,std::forward<Args>(args)...);
}
template<async_output_stream stm,typename... Args>
constexpr inline async_print_coroutine<stm,true> async_println(typename io_async_scheduler_t<stm>::type& scheduler,std::ptrdiff_t offset,stm& sm,Args&& ...args)
{
	return async_print_coroutine<stm,true>(scheduler,offset,sm,std::forward<Args>(args)...);
}



}
