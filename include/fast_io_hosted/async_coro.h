#pragma once

namespace fast_io
{
//This is DAMN bullshit
struct task
{
struct promise_type
{
auto get_return_object() { return task{}; }
auto initial_suspend() { return std::suspend_never{}; }
auto final_suspend() { return std::suspend_never{}; }
void unhandled_exception() { std::terminate(); }
void return_void() {}
};
};
template<output_stream stm,std::input_or_output_iterator Iter1,std::input_or_output_iterator Iter2>
class async_write
{
public:
	stm& sm;
	Iter1 beg;
	Iter2 end;
	std::size_t transferred_size{};
	constexpr bool await_ready() const { return false; }
	constexpr Iter1 await_resume() const { return beg+transferred_size; }
	void await_suspend(std::coroutine_handle<> handle)
	{
		async_write_callback(sm,beg,end,[handle,this](std::size_t calb)
		{
			this->transferred_size=calb;
			handle.resume();
		});
	}
};

template<output_stream stm,std::input_or_output_iterator Iter1,
	std::input_or_output_iterator Iter2> async_write(stm& sm,Iter1 b, Iter2 e) -> async_write<stm,Iter1, Iter2>;

template<output_stream stm>
class async_print
{
public:
	stm& sm;
	internal_temporary_buffer<typename stm::char_type> buffer;
	template<typename ...Args>
	async_print(stm& s,Args&& ...args):sm(s)
	{
		print(buffer,std::forward<Args>(args)...);
	}
	constexpr bool await_ready() const { return false; }
	constexpr void await_resume() const {}
	void await_suspend(std::coroutine_handle<> handle)
	{
		async_write_callback(sm,buffer.beg_ptr,buffer.end_ptr,[handle,this](std::size_t calb)
		{
			handle.resume();
		});
	}
};
template<output_stream stm,typename ...Args> async_print(stm&,Args&& ...) -> async_print<stm>;

template<output_stream stm>
class async_println
{
public:
	stm& sm;
	internal_temporary_buffer<typename stm::char_type> buffer;
	template<typename ...Args>
	async_println(stm& s,Args&& ...args):sm(s)
	{
		println(buffer,std::forward<Args>(args)...);
	}
	constexpr bool await_ready() const { return false; }
	constexpr void await_resume() const {}
	void await_suspend(std::coroutine_handle<> handle)
	{
		async_write_callback(sm,buffer.beg_ptr,buffer.end_ptr,[handle,this](std::size_t calb)
		{
			handle.resume();
		});
	}
};

template<output_stream stm,typename ...Args> async_println(stm&,Args&& ...) -> async_println<stm>;

template<typename stm>
class async_scatter_write
{
public:
	typename stm::async_scheduler_type& sch;
	stm& sm;
	std::span<io_scatter_t const> span;
	std::size_t transferred_size{};
	async_scatter_write(typename stm::async_scheduler_type& sh,stm& s,std::span<io_scatter_t const> sp):sch(sh),sm(s),span(sp){}
	constexpr bool await_ready() const { return false; }
	constexpr std::size_t await_resume() const { return transferred_size; }
	void await_suspend(std::coroutine_handle<> handle)
	{
		async_scatter_write_callback(sch,sm,span,[handle,this](std::size_t bytes)
		{
			this->transferred_size=bytes;
			handle.resume();
		});
	}
};

template<output_stream stm> async_scatter_write(typename stm::async_scheduler_type&,stm&,std::span<io_scatter_t const>) -> async_scatter_write<stm>;


}
