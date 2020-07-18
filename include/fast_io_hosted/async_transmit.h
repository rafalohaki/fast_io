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
}

template<async_output_stream output,async_input_stream input>
inline details::async_transmit_async_output_from_async_input_impl<output,input>
	async_transmit(typename io_async_scheduler_t<output>::type& out_sh,output& out,
		typename io_async_scheduler_t<input>::type& in_sh,input& in)
{
	return {out_sh,out,in_sh,in};
}
/*
template<async_output_stream output,input_stream input>
inline details::async_transmit_async_output_from_async_input_impl<output,input>
	async_sync_transmit(typename io_async_scheduler_t<output>::type& out_sh,output& out,input& in)
{
	return {out_sh,out,in_sh,in};
}
*/
}