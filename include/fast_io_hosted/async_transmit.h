#pragma once

namespace fast_io
{

namespace details
{
template<async_output_stream output,async_input_stream input>
class async_transmit_async_output_from_async_input_impl
{
public:
	typename io_async_scheduler_t<output>::type& out_sch;
	output& out;
	typename io_async_scheduler_t<input>::type& in_sch;
	input& in;
	using char_type = typename std::remove_cvref_t<input>::char_type;
	static inline constexpr std::size_t buffer_size{details::cal_buffer_size<char_type>()};
	std::uintmax_t transferred{};
	std::unique_ptr<char_type[]> uptr{new char_type[buffer_size]};
	typename io_async_overlapped_t<output>::type output_overlapped;
	typename io_async_overlapped_t<input>::type input_overlapped;
	
	constexpr bool await_ready() const { return false; }
	constexpr std::uintmax_t await_resume() const { return transferred; }
	void await_suspend(std::coroutine_handle<> handle)
	{
		output_overlapped=typename io_async_overlapped_t<output>::type(std::in_place,[this](std::size_t calb)
		{
			async_read_callback(this->in_sch,this->in,uptr.get(),uptr.get()+buffer_size,this->input_overlapped);
		});
		input_overlapped=typename io_async_overlapped_t<input>::type(std::in_place,[handle,this](std::size_t calb)
		{
			this->transferred+=calb/sizeof(typename input::char_type);
			if(calb==0)
				handle.resume();
			else
				async_write_callback(out_sch,out,uptr.get(),uptr.get()+calb,this->output_overlapped,-1);
		});
		async_read_callback(this->in_sch,this->in,uptr.get(),uptr.get()+buffer_size,this->input_overlapped);
	}
};

template<async_output_stream output,input_stream input>
class async_transmit_async_output_from_sync_input_impl
{
public:
	typename io_async_scheduler_t<output>::type& out_sch;
	output& out;
	input& in;
	using char_type = typename std::remove_cvref_t<input>::char_type;
	static inline constexpr std::size_t buffer_size{details::cal_buffer_size<char_type>()};
	std::uintmax_t transferred{};
	std::unique_ptr<char_type[]> uptr{new char_type[buffer_size]};
	typename io_async_overlapped_t<output>::type output_overlapped;
	
	constexpr bool await_ready() const { return false; }
	constexpr std::uintmax_t await_resume() const { return transferred; }
	void await_suspend(std::coroutine_handle<> handle)
	{
		output_overlapped=typename io_async_overlapped_t<output>::type(std::in_place,[handle,this](std::size_t)
		{
			auto it{read(this->in,uptr.get(),uptr.get()+buffer_size)};
			if(it==uptr.get())
				handle.resume();
			else
			{
				std::size_t this_round(it-uptr.get());
				this->transferred+=it-uptr.get();
				async_write_callback(this->out_sch,this->out,uptr.get(),uptr.get()+this_round,this->output_overlapped,-1);
			}
		});
		auto it{read(in,uptr.get(),uptr.get()+buffer_size)};
		if(it!=uptr.get())[[likely]]
		{
			std::size_t this_round(it-uptr.get());
			this->transferred+=this_round;
			async_write_callback(this->out_sch,this->out,uptr.get(),uptr.get()+this_round,this->output_overlapped,-1);
		}
	}
};

template<async_output_stream output,buffer_input_stream input>
class async_transmit_async_output_from_sync_buffer_input_impl
{
public:
	typename io_async_scheduler_t<output>::type& out_sch;
	output& out;
	input& in;
	using char_type = typename std::remove_cvref_t<input>::char_type;
	static inline constexpr std::size_t buffer_size{details::cal_buffer_size<char_type>()};
	std::uintmax_t transferred{};
	typename io_async_overlapped_t<output>::type output_overlapped;
	
	constexpr bool await_ready() const { return false; }
	constexpr std::uintmax_t await_resume() const { return transferred; }
	void await_suspend(std::coroutine_handle<> handle)
	{
		output_overlapped=typename io_async_overlapped_t<output>::type(std::in_place,[handle,this](std::size_t)
		{
			if(underflow(in))
			{
				auto b{ibuffer_curr(in)};
				auto e{ibuffer_end(in)};
				this->transferred+=e-b;
				async_write_callback(this->out_sch,this->out,b,e,this->output_overlapped,-1);
			}
			else
				handle.resume();
		});
		auto b{ibuffer_curr(in)};
		auto e{ibuffer_end(in)};
		if(b!=e)[[likely]]
		{
			transferred+=e-b;
			async_write_callback(this->out_sch,this->out,b,e,this->output_overlapped,-1);
		}
		else if(underflow(in))
		{
			auto b{ibuffer_curr(in)};
			auto e{ibuffer_end(in)};
			transferred+=e-b;
			async_write_callback(this->out_sch,this->out,b,e,this->output_overlapped,-1);
		}
		else
			handle.resume();
	}
};

template<output_stream output,async_input_stream input>
class async_transmit_sync_output_from_async_input_impl
{
public:
	output& out;
	typename io_async_scheduler_t<input>::type& in_sch;
	input& in;
	using char_type = typename std::remove_cvref_t<input>::char_type;
	static inline constexpr std::size_t buffer_size{details::cal_buffer_size<char_type>()};
	std::uintmax_t transferred{};
	std::unique_ptr<char_type[]> uptr{new char_type[buffer_size]};
	typename io_async_overlapped_t<input>::type input_overlapped;

	constexpr bool await_ready() const { return false; }
	constexpr std::uintmax_t await_resume() const { return transferred; }
	void await_suspend(std::coroutine_handle<> handle)
	{
		input_overlapped=typename io_async_overlapped_t<input>::type(std::in_place,[handle,this](std::size_t calb)
		{
			this->transferred+=calb/sizeof(typename input::char_type);
			if(calb==0)
				handle.resume();
			else
			{
				write(out,uptr.get(),uptr.get()+calb);
				async_read_callback(this->in_sch,this->in,uptr.get(),uptr.get()+buffer_size,this->input_overlapped);
			}
		});
		async_read_callback(this->in_sch,this->in,uptr.get(),uptr.get()+buffer_size,this->input_overlapped);
	}
};


}

template<async_output_stream output,async_input_stream input>
inline details::async_transmit_async_output_from_async_input_impl<output,input>
	async_transmit(typename io_async_scheduler_t<output>::type& out_sh,output& out,
		typename io_async_scheduler_t<input>::type& in_sh,input& in)
{
	return {out_sh,out,in_sh,in};
}

template<async_output_stream output,input_stream input>
inline auto async_sync_transmit(typename io_async_scheduler_t<output>::type& out_sh,output& out,input& in)
{
	if constexpr(buffer_input_stream<input>)
		return details::async_transmit_async_output_from_sync_buffer_input_impl<output,input>{out_sh,out,in};
	else
		return details::async_transmit_async_output_from_sync_input_impl<output,input>{out_sh,out,in};
}

template<output_stream output,async_input_stream input>
inline details::async_transmit_sync_output_from_async_input_impl<output,input>
	sync_async_transmit(output& out,typename io_async_scheduler_t<output>::type& in_sh,input& in)
{
	return {out,in_sh,in};
}

}
