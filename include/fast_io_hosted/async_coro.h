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
	typename stm::async_scheduler_type& sch;
	stm& sm;
	Iter1 beg;
	Iter2 end;
	std::ptrdiff_t offset{};
	std::size_t transferred_size{};
	constexpr bool await_ready() const { return false; }
	constexpr Iter1 await_resume() const { return beg+transferred_size; }
	void await_suspend(std::coroutine_handle<> handle)
	{
		async_write_callback(sch,sm,beg,end,offset*sizeof(*beg),[handle,this](std::size_t calb)
		{
			this->transferred_size=calb/sizeof(*beg);
			handle.resume();
		});
	}
};

template<output_stream stm,std::input_or_output_iterator Iter1,
	std::input_or_output_iterator Iter2> async_write(typename stm::async_scheduler_type&,stm&,Iter1,Iter2,std::ptrdiff_t) ->
		async_write<stm, Iter1, Iter2>;
template<output_stream stm,std::input_or_output_iterator Iter1,
	std::input_or_output_iterator Iter2> async_write(typename stm::async_scheduler_type&,stm&,Iter1,Iter2) -> async_write<stm, Iter1, Iter2>;

template<output_stream stm>
class async_print
{
public:
	typename stm::async_scheduler_type& sch;
	stm& sm;
	std::ptrdiff_t offset{};
	internal_temporary_buffer<typename stm::char_type> buffer;
	std::size_t transferred_size{};
	template<typename ...Args>
	async_print(typename stm::async_scheduler_type& sh,stm& s,Args&& ...args):sch(sh),sm(s)
	{
		print(buffer,std::forward<Args>(args)...);
	}
	template<typename ...Args>
	async_print(typename stm::async_scheduler_type& sh,std::ptrdiff_t off,stm& s,Args&& ...args):sch(sh),sm(s),offset(off)
	{
		print(buffer,std::forward<Args>(args)...);
	}
	constexpr bool await_ready() const { return false; }
	constexpr std::size_t await_resume() const { return transferred_size; }
	void await_suspend(std::coroutine_handle<> handle)
	{
		async_write_callback(sch,sm,buffer.beg_ptr,buffer.end_ptr,offset*sizeof(typename stm::char_type),[handle,this](std::size_t bytes)
		{
			this->transferred_size=bytes/sizeof(typename stm::char_type);
			handle.resume();
		});
	}
};

template<output_stream stm,typename ...Args> async_print(typename stm::async_scheduler_type&,stm&,Args&& ...) -> async_print<stm>;
template<output_stream stm,typename ...Args> async_print(typename stm::async_scheduler_type&,std::ptrdiff_t,stm&,Args&& ...) -> async_print<stm>;



template<output_stream stm>
class async_println
{
public:
	typename stm::async_scheduler_type& sch;
	stm& sm;
	std::ptrdiff_t offset{};
	internal_temporary_buffer<typename stm::char_type> buffer;
	std::size_t transferred_size{};
	template<typename ...Args>
	async_println(typename stm::async_scheduler_type& sh,stm& s,Args&& ...args):sch(sh),sm(s)
	{
		println(buffer,std::forward<Args>(args)...);
	}
	template<typename ...Args>
	async_println(typename stm::async_scheduler_type& sh,std::ptrdiff_t off,stm& s,Args&& ...args):sch(sh),sm(s),offset(off)
	{
		println(buffer,std::forward<Args>(args)...);
	}
	constexpr bool await_ready() const { return false; }
	constexpr std::size_t await_resume() const { return transferred_size; }
	void await_suspend(std::coroutine_handle<> handle)
	{
		async_write_callback(sch,sm,buffer.beg_ptr,buffer.end_ptr,offset*sizeof(typename stm::char_type),[handle,this](std::size_t bytes)
		{
			this->transferred_size=bytes/sizeof(typename stm::char_type);
			handle.resume();
		});
	}
};

template<output_stream stm,typename ...Args> async_println(typename stm::async_scheduler_type&,stm&,Args&& ...) -> async_println<stm>;
template<output_stream stm,typename ...Args> async_println(typename stm::async_scheduler_type&,std::ptrdiff_t,stm&,Args&& ...) -> async_println<stm>;

template<typename stm>
class async_scatter_write
{
public:
	typename stm::async_scheduler_type& sch;
	stm& sm;
	std::span<io_scatter_t const> span;
	std::ptrdiff_t offset{};
	std::size_t transferred_size{};
	constexpr bool await_ready() const { return false; }
	constexpr std::size_t await_resume() const { return transferred_size; }
	void await_suspend(std::coroutine_handle<> handle)
	{
		async_scatter_write_callback(sch,sm,span,offset,[handle,this](std::size_t bytes)
		{
			this->transferred_size=bytes;
			handle.resume();
		});
	}
};

template<output_stream stm> async_scatter_write(typename stm::async_scheduler_type&,stm&,std::span<io_scatter_t const>,std::ptrdiff_t) -> async_scatter_write<stm>;
template<output_stream stm> async_scatter_write(typename stm::async_scheduler_type&,stm&,std::span<io_scatter_t const>) -> async_scatter_write<stm>;

}
