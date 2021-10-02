#pragma once


namespace fast_io
{

template<typename T,typename P>
concept status_transmitable = requires(T&& t,P&& p)
{
	status_transmit_define(io_alias,::fast_io::freestanding::forward<T>(t),::fast_io::freestanding::forward<P>(p));
};

template<typename T,typename P>
concept zero_copy_transmitable = zero_copy_output_stream<T>&&zero_copy_input_stream<P>&&requires(T t,P p)
{
	zero_copy_transmit_define(io_alias,zero_copy_out_handle(t),zero_copy_in_handle(t));
};

namespace details
{

template<std::integral char_type>
inline constexpr std::size_t calculate_transmit_buffer_size() noexcept
{
#ifdef FAST_IO_BUFFER_SIZE
	static_assert(sizeof(char_type)>=FAST_IO_BUFFER_SIZE);
	static_assert(FAST_IO_BUFFER_SIZE<SIZE_MAX);
	return FAST_IO_BUFFER_SIZE/sizeof(char_type);
#else
	if constexpr(sizeof(std::size_t)<=sizeof(std::uint_least16_t))
	{
		return 4096/sizeof(char_type);	
	}
	else
	{
		return 131072/sizeof(char_type);
	}
#endif
}

template<std::integral char_type>
inline constexpr std::size_t transmit_buffer_size_cache{calculate_transmit_buffer_size<char_type>()};

}

template<output_stream output,input_stream input>
inline constexpr std::uintmax_t raw_transmit_decay(output outs,input ins)
{
	if constexpr(contiguous_input_stream<input>)
	{
		auto curr{ibuffer_curr(ins)};
		auto ed{ibuffer_end(ins)};
		write(outs,curr,ed);
		auto diff{ed-curr};
		ibuffer_set_curr(ins,ed);
		return static_cast<std::uintmax_t>(static_cast<std::size_t>(diff));
	}
	else if constexpr(buffer_input_stream<input>)
	{
		std::uintmax_t chars{};
		do
		{
			auto curr{ibuffer_curr(ins)};
			auto ed{ibuffer_end(ins)};
			if(curr==ed)[[unlikely]]
				continue;
			write(outs,curr,ed);
			chars+=static_cast<std::size_t>(ed-curr);
			ibuffer_set_curr(ins,ed);
		}
		while(ibuffer_underflow(ins));
		return chars;
	}
	else
	{
		using input_char_type = typename input::char_type;
		std::uintmax_t chars{};
		constexpr std::size_t buffer_size{details::transmit_buffer_size_cache<input_char_type>};
		details::buffer_alloc_arr_ptr<input_char_type,false> array_ptr(buffer_size);
		auto array_start{array_ptr.ptr},array_end{array_ptr.ptr+buffer_size};
		for(;;)
		{
			auto ed{read(ins,array_start,array_end)};
			if(array_start==ed)[[unlikely]]
				break;
			write(outs,array_start,ed);
			chars+=static_cast<std::size_t>(ed-array_start);
		}
		return chars;
	}
}

template<output_stream output,input_stream input>
inline constexpr decltype(auto) transmit_decay(output outs,input ins)
{
	if constexpr(mutex_stream<input>)
	{
		io_lock_guard lg{ins};
		decltype(auto) uh{ins.unlocked_handle()};
		return transmit_decay(outs,io_ref(uh));
	}
	else if constexpr(mutex_stream<output>)
	{
		io_lock_guard lg{outs};
		decltype(auto) uh{outs.unlocked_handle()};
		return transmit_decay(io_ref(uh),ins);
	}
	else if constexpr(zero_copy_transmitable<output,input>)
	{
		if constexpr(buffer_output_stream<output>&&!flush_output_stream<output>)
			return raw_transmit_decay(outs,ins);
		else
		{
			std::uintmax_t chars{};
			if constexpr(buffer_input_stream<input>)
			{
				auto curr{ibuffer_curr(ins)};
				auto ed{ibuffer_end(ins)};
				write(outs,curr,ed);
				chars+=static_cast<std::size_t>(ed-curr);
				ibuffer_set_curr(ins,ed);
			}
			if constexpr(buffer_output_stream<output>)
				flush(outs);
			return chars+zero_copy_transmit_define(io_alias,zero_copy_out_handle(outs),zero_copy_in_handle(ins));
		}
	}
	else if constexpr(output_stream<output>&&input_stream<input>)
		return raw_transmit_decay(outs,ins);
	else
	{
		constexpr bool no{output_stream<output>&&input_stream<input>};
		static_assert(no,"transmit must happen to output stream from input stream");
	}
}

template<typename output,typename input>
#if __has_cpp_attribute(gnu::always_inline)
[[gnu::always_inline]]
#elif __has_cpp_attribute(msvc::forceinline)
[[msvc::forceinline]]
#endif
inline constexpr decltype(auto) transmit(output&& outs,input&& ins)
{
	if constexpr(status_transmitable<output,input>)
		return status_transmit_define(io_alias,::fast_io::freestanding::forward<output>(outs),::fast_io::freestanding::forward<input>(ins));
	else
		return transmit_decay(fast_io::io_ref(outs),fast_io::io_ref(ins));
}

}
