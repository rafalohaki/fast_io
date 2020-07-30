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


template<async_input_stream stm,std::input_or_output_iterator Iter1,std::input_or_output_iterator Iter2>
class async_read
{
public:
	typename io_async_scheduler_t<stm>::type& sch;
	stm& sm;
	Iter1 beg;
	Iter2 end;
	std::ptrdiff_t offset{};
	std::size_t transferred_size{};
	typename io_async_overlapped_t<stm>::type overlapped;
	constexpr bool await_ready() const { return false; }
	constexpr Iter1 await_resume() const { return beg+transferred_size; }
	void await_suspend(std::coroutine_handle<> handle)
	{
		overlapped=typename io_async_overlapped_t<stm>::type(std::in_place,[handle,this](std::size_t calb)
		{
			this->transferred_size=calb/sizeof(*beg);
			handle.resume();
		});
		async_read_callback(sch,sm,beg,end,overlapped,offset*sizeof(*beg));
	}
};

template<async_input_stream stm,std::input_or_output_iterator Iter1,
	std::input_or_output_iterator Iter2> async_read(typename io_async_scheduler_t<stm>::type&,stm&,Iter1,Iter2,std::ptrdiff_t)
		-> async_read<stm, Iter1, Iter2>;
template<async_input_stream stm,std::input_or_output_iterator Iter1,
	std::input_or_output_iterator Iter2> async_read(typename io_async_scheduler_t<stm>::typee&,stm&,Iter1,Iter2)
		-> async_read<stm, Iter1, Iter2>;




template<async_output_stream stm,std::input_or_output_iterator Iter1,std::input_or_output_iterator Iter2>
class async_write
{
public:
	typename io_async_scheduler_t<stm>::type& sch;
	stm& sm;
	Iter1 beg;
	Iter2 end;
	std::ptrdiff_t offset{-1};
	std::size_t transferred_size{};
	typename io_async_overlapped_t<stm>::type overlapped;
	constexpr bool await_ready() const { return false; }
	constexpr Iter1 await_resume() const { return beg+transferred_size; }
	void await_suspend(std::coroutine_handle<> handle)
	{
		overlapped=typename io_async_overlapped_t<stm>::type(std::in_place,[handle,this](std::size_t calb)
		{
			this->transferred_size=calb/sizeof(*beg);
			handle.resume();
		});
		async_write_callback(sch,sm,beg,end,overlapped,offset*sizeof(*beg));
	}
};

template<async_output_stream stm,std::input_or_output_iterator Iter1,
	std::input_or_output_iterator Iter2> async_write(typename io_async_scheduler_t<stm>::type&,stm&,Iter1,Iter2,std::ptrdiff_t) ->
		async_write<stm, Iter1, Iter2>;
template<async_output_stream stm,std::input_or_output_iterator Iter1,
	std::input_or_output_iterator Iter2> async_write(typename io_async_scheduler_t<stm>::type&,stm&,Iter1,Iter2) -> async_write<stm, Iter1, Iter2>;

template<async_output_stream stm>
class async_print
{
public:
	typename stm::async_scheduler_type& sch;
	stm& sm;
	std::ptrdiff_t offset{-1};
	internal_temporary_buffer<typename stm::char_type> buffer;
	std::size_t transferred_size{};
	typename io_async_overlapped_t<stm>::type overlapped;
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
		overlapped=typename io_async_overlapped_t<stm>::type(std::in_place,[handle,this](std::size_t calb)
		{
			this->transferred_size=calb/sizeof(typename stm::char_type);
			handle.resume();
		});
		async_write_callback(sch,sm,buffer.beg_ptr,buffer.end_ptr,overlapped,offset*sizeof(typename stm::char_type));
	}
};

template<async_output_stream stm,typename ...Args> async_print(typename io_async_scheduler_t<stm>::type&,stm&,Args&& ...) -> async_print<stm>;
template<async_output_stream stm,typename ...Args> async_print(typename io_async_scheduler_t<stm>::type&,std::ptrdiff_t,stm&,Args&& ...) -> async_print<stm>;

template<async_output_stream stm>
class async_println
{
public:
	typename stm::async_scheduler_type& sch;
	stm& sm;
	std::ptrdiff_t offset{-1};
	internal_temporary_buffer<typename stm::char_type> buffer;
	std::size_t transferred_size{};
	typename io_async_overlapped_t<stm>::type overlapped;
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
		overlapped=typename io_async_overlapped_t<stm>::type(std::in_place,[handle,this](std::size_t calb)
		{
			this->transferred_size=calb/sizeof(typename stm::char_type);
			handle.resume();
		});
		async_write_callback(sch,sm,buffer.beg_ptr,buffer.end_ptr,overlapped,offset*sizeof(typename stm::char_type));
	}
};

template<async_output_stream stm,typename ...Args> async_println(typename io_async_scheduler_t<stm>::type&,stm&,Args&& ...) -> async_println<stm>;
template<async_output_stream stm,typename ...Args> async_println(typename io_async_scheduler_t<stm>::type&,std::ptrdiff_t,stm&,Args&& ...) -> async_println<stm>;

template<typename stm>
class async_scatter_write
{
public:
	typename stm::async_scheduler_type& sch;
	stm& sm;
	std::span<io_scatter_t const> span;
	std::ptrdiff_t offset{};
	std::size_t transferred_size{};
	typename io_async_overlapped_t<stm>::type overlapped;
	constexpr bool await_ready() const { return false; }
	constexpr std::size_t await_resume() const { return transferred_size; }
	void await_suspend(std::coroutine_handle<> handle)
	{
		overlapped=typename io_async_overlapped_t<stm>::type(std::in_place,[handle,this](std::size_t calb)
		{
			this->transferred_size=calb;
			handle.resume();
		});
		async_scatter_write_callback(sch,sm,span,overlapped,offset);
	}
};

template<async_output_stream stm> async_scatter_write(typename io_async_scheduler_t<stm>::type&,stm&,std::span<io_scatter_t const>,std::ptrdiff_t) -> async_scatter_write<stm>;
template<async_output_stream stm> async_scatter_write(typename io_async_scheduler_t<stm>::type&,stm&,std::span<io_scatter_t const>) -> async_scatter_write<stm>;

}
