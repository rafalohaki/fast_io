#pragma once

namespace fast_io
{

namespace details
{
template<std::integral char_type,bool iobuf=false>
inline constexpr std::size_t cal_buffer_size()
{
#ifdef FAST_IO_BUFFER_SIZE
	static_assert(sizeof(char_type)<=FAST_IO_BUFFER_SIZE);
#endif
	if constexpr(iobuf)
	{
		return 
#ifdef FAST_IO_BUFFER_SIZE
		FAST_IO_BUFFER_SIZE
#else
#if defined(__WINNT__) || defined(_MSC_VER)
	1048576
#else
	65536
#endif
#endif
		/sizeof(char_type);
	}
	else
	{
	return 
#ifdef FAST_IO_BUFFER_SIZE
	FAST_IO_BUFFER_SIZE	//avoid BUFSIZ macro since it is a cancer and often set incorrectly
#else
#ifdef FAST_IO_TRANSMIT_ON_STACK
	4096
#else
#if defined(__WINNT__) || defined(_MSC_VER)
	1048576
#else
	65536
#endif
#endif
#endif
	/sizeof(char_type);
	}
}

template<output_stream output,input_stream input>
inline constexpr std::uintmax_t bufferred_transmit_impl(output& outp,input& inp)
{
	std::uintmax_t transmitted_bytes{};
	if constexpr(buffer_input_stream<input>)
	{
		do
		{
			auto b{ibuffer_curr(inp)};
			auto e{std::to_address(ibuffer_end(inp))};
			if(b!=e)[[likely]]
			{
				std::size_t transmitted_this_round(static_cast<std::size_t>(e-b));
				write(outp,b,e);
				transmitted_bytes+=transmitted_this_round;
			}
		}
		while(underflow(inp));
		return transmitted_bytes;
	}
	else
	{
		using char_type = typename std::remove_cvref_t<input>::char_type;
		constexpr std::size_t buffer_size{cal_buffer_size<char_type>()};
#ifdef FAST_IO_TRANSMIT_ON_STACK
		std::array<char_type,buffer_size> array;
#else
		temp_unique_arr_ptr<char_type> ptr(buffer_size);
// we need to allocate it on the heap to avoid potential stack overflows
		std::span<char_type,buffer_size> array(ptr.data(),ptr.data()+buffer_size);
#endif
		conditional_secure_clear_guard<char_type,secure_clear_requirement_stream<std::remove_cvref_t<input>>> guard(array.data(),buffer_size);
		for(;;)
		{
			auto p(read(inp,array.data(),array.data()+array.size()));
			if(p==array.data())
				return transmitted_bytes;
			std::size_t transmitted_this_round(p-array.data());
			transmitted_bytes+=transmitted_this_round;
			write(outp,array.data(),p);
			if(!transmitted_this_round)
				return transmitted_bytes;
		}
	}
}

template<output_stream output,input_stream input>
inline constexpr std::uintmax_t bufferred_transmit_impl(output& outp,input& inp,std::uintmax_t bytes)
{
	std::uintmax_t transmitted_bytes{};
	if constexpr(buffer_input_stream<input>)
	{
		do
		{
			auto b(begin(inp));
			auto e(end(inp));
			if(b!=e)[[likely]]
			{
				std::size_t transmitted_this_round((e-b)*sizeof(*b));
				if(bytes<=transmitted_this_round)
				{
					write(outp,b,b+bytes);
					return transmitted_bytes+transmitted_this_round;
				}
				else
				{
					write(outp,b,e);
					transmitted_bytes+=transmitted_this_round;
					bytes-=transmitted_this_round;
				}
			}
		}
		while(underflow(inp));
		return transmitted_bytes;
	}
	else
	{
		using char_type = typename std::remove_cvref_t<input>::char_type;
		constexpr std::size_t buffer_size{cal_buffer_size<char_type>()};
#ifdef FAST_IO_TRANSMIT_ON_STACK
		std::array<char_type,buffer_size> array;
#else
		temp_unique_arr_ptr<char_type> ptr(buffer_size);
// we need to allocate it on the heap to avoid potential stack overflows
		std::span<char_type,buffer_size> array(ptr.data(),ptr.data()+buffer_size);
#endif
		conditional_secure_clear_guard<char_type,secure_clear_requirement_stream<std::remove_cvref_t<input>>> guard(array.data(),buffer_size);
		for(;bytes;)
		{
			std::size_t b(array.size());
			if(bytes<b)
				b=bytes;
			auto p(read(inp,array.data(),array.data()+b));
			if(p==array.data())
				return transmitted_bytes;
			std::size_t read_bytes(p-array.data());
			write(outp,array.data(),p);
			transmitted_bytes+=read_bytes;
			if(read_bytes!=b)
				return transmitted_bytes;
			bytes-=read_bytes;
		}
		return transmitted_bytes;
	}
}
#if defined(__linux__)||defined(__BSD_VISIBLE)
template<output_stream output,input_stream input>
inline constexpr std::uintmax_t zero_copy_transmit_impl(output& outp,input& inp)
{
	auto ret(zero_copy_transmit<false,true>(outp,inp,0));
	if(ret.second)
		return ret.first+bufferred_transmit_impl(outp,inp);
	return ret.first;
}

template<output_stream output,input_stream input>
inline constexpr std::uintmax_t zero_copy_transmit_impl(output& outp,input& inp,std::uintmax_t sz)
{
	auto ret(zero_copy_transmit<false,true>(outp,inp,sz,0)); 
	if(ret.second)
		return ret.first+bufferred_transmit_impl(outp,inp,sz-ret.first);
	return ret.first;
}
#endif

template<output_stream output,memory_map_input_stream input,typename... Args>
inline std::pair<bool,std::uintmax_t> memory_map_transmit_impl(output& outp,input& inp,Args&& ...args);

template<output_stream output,input_stream input,typename... Args>
inline constexpr auto transmit_impl(output& outp,input& inp,Args&& ...args)
{
	if constexpr(mutex_input_stream<input>)
	{
		typename input::lock_guard_type lg{mutex(inp)};
		decltype(auto) uh{unlocked_handle(inp)};
		return transmit_impl(outp,uh,std::forward<Args>(args)...);
	}
	else
	{
#ifdef __cpp_lib_is_constant_evaluated
		if (std::is_constant_evaluated())
			return bufferred_transmit_impl(outp,inp,std::forward<Args>(args)...);
		else
		{
#endif
		if constexpr(zero_copy_output_stream<output>&&zero_copy_input_stream<input>)
		{
			if constexpr(buffer_input_stream<input>)
			{
				write(outp,ibuffer_curr(inp),ibuffer_end(inp));
				ibuffer_set_curr(inp,ibuffer_end(inp));
			}
			if constexpr(buffer_output_stream<output>)
				flush(outp);
#if defined(__linux__)||defined(__BSD_VISIBLE)
			return zero_copy_transmit_impl(outp,inp,std::forward<Args>(args)...);
#else
			return zero_copy_transmit<false>(outp,inp,0,std::forward<Args>(args)...);
#endif
		}
		else if constexpr(memory_map_input_stream<input>)
		{
			auto [succ,size]=memory_map_transmit_impl(outp,inp,std::forward<Args>(args)...);
			if(succ)
				return size;
			else
				return bufferred_transmit_impl(outp,inp,std::forward<Args>(args)...);
		}
		else
			return bufferred_transmit_impl(outp,inp,std::forward<Args>(args)...);
#ifdef __cpp_lib_is_constant_evaluated
		}
#endif
	}
}
}
template<output_stream output,input_stream input,std::integral sz_type>
inline constexpr void print_define(output& outp,manip::transmission<input,sz_type> ref)
{
	ref.transmitted=static_cast<sz_type>(details::transmit_impl(outp,ref.reference));
}

template<output_stream output,input_stream input,std::integral sz_type>
inline constexpr void print_define(output& outp,manip::transmission_with_size<input,sz_type> ref)
{
	ref.transmitted=static_cast<sz_type>(details::transmit_impl(outp,ref.reference,ref.size));
}

template<output_stream output,input_stream input,std::integral sz_type>
inline constexpr sz_type transmit(output&& outp,input&& in,sz_type s)
{
	sz_type transmitted{};
	print(outp,manip::transmission_with_size<input,sz_type>{transmitted,in,s});
	return transmitted;
}

template<output_stream output,input_stream input>
inline constexpr std::uintmax_t transmit(output&& outp,input&& in)
{
	std::uintmax_t transmitted{};
	print(outp,manip::transmission<input,std::uintmax_t>{transmitted,in});
	return transmitted;
}

}
