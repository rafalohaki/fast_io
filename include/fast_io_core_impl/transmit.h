#pragma once

namespace fast_io
{
/*
Referenced from
https://eklitzke.org/efficient-file-copying-on-linux
In response to my last post about dd, a friend of mine noticed that GNU cp always uses a 128 KB buffer size when copying a regular file; this is also the buffer size used by GNU cat. If you use strace to watch what happens when copying a file, you should see a lot of 128 KB read/write sequences:
*/
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
	131072
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
	131072
#endif
#endif
	/sizeof(char_type);
	}
}

template<output_stream output,input_stream input>
inline constexpr std::uintmax_t bufferred_transmit_impl(output& outp,input& inp)
{
#if 0
	if constexpr(contiguous_output_stream<output>)
	{
		using input_char_type = typename std::remove_cvref_t<input>::char_type;
		using output_char_type = typename std::remove_cvref_t<output>::char_type;
		if constexpr(std::same_as<input_char_type,output_char_type>)
		{
			auto curr{obuffer_curr(outp)};
			auto ed{obuffer_end(outp)};
			for(auto ip{curr};;)
			{
				auto readed{read(inp,ip,ed)};
				obuffer_set_curr(outp,readed);
				if((readed==ip)|(readed==ed))
					return readed-curr;
				ip=readed;
			}
		}
		else
		{
			static_assert(std::same_as<output_char_type,char>,"only char allows punning");
			typedef input_char_type 
#ifndef _MSC_VER
			__attribute__((__may_alias__))
#endif
			alias_input_char_type;
			auto raw_curr{obuffer_curr(outp)};
			auto raw_end{obuffer_end(outp)};
			alias_input_char_type* curr{reinterpret_cast<alias_input_char_type*>(raw_curr)};
			alias_input_char_type* ed{curr+(raw_end-raw_curr)/sizeof(input_char_type)};
			for(auto ip{curr};;)
			{
				auto readed{read(inp,ip,ed)};
				obuffer_set_curr(outp,reinterpret_cast<char*>(readed));
				if((readed==ip)|(readed==ed))
					return readed-curr;
				ip=readed;
			}
		}
	}
	else
#endif
	if constexpr(buffer_input_stream<input>)
	{
		std::uintmax_t transmitted_chars{};
		do
		{
			auto b{ibuffer_curr(inp)};
			auto e{ibuffer_end(inp)};
			if(b!=e)[[likely]]
			{
				std::size_t transmitted_this_round(static_cast<std::size_t>(e-b));
				write(outp,b,e);
				transmitted_chars+=transmitted_this_round;
			}
		}
		while(ibuffer_underflow(inp));
		return transmitted_chars;
	}
	else
	{
		std::uintmax_t transmitted_chars{};
		using char_type = typename std::remove_cvref_t<input>::char_type;
		constexpr std::size_t buffer_size{cal_buffer_size<char_type>()};
#ifdef FAST_IO_TRANSMIT_ON_STACK
		::fast_io::freestanding::array<char_type,buffer_size> array;
		conditional_secure_clear_guard<char_type,secure_clear_requirement_stream<std::remove_cvref_t<input>>> guard(array.data(),buffer_size);
		auto array_start{array.data()};
#else
		buffer_alloc_arr_ptr<char_type,secure_clear_requirement_stream<std::remove_cvref_t<input>>> ptr(buffer_size);
		auto array_start{ptr.ptr};
// we need to allocate it on the heap to avoid potential stack overflows
#endif
		for(auto array_end{array_start+buffer_size};;)
		{
			auto p(read(inp,array_start,array_end));
			if(p==array_start)
				return transmitted_chars;
			std::size_t transmitted_this_round(p-array_start);
			transmitted_chars+=transmitted_this_round;
			write(outp,array_start,p);
			if(!transmitted_this_round)
				return transmitted_chars;
		}
	}
}

template<output_stream output,input_stream input>
inline constexpr std::uintmax_t bufferred_transmit_impl(output& outp,input& inp,std::uintmax_t chars)
{
#if 0
	if constexpr(contiguous_output_stream<output>)
	{
		using input_char_type = typename std::remove_cvref_t<input>::char_type;
		using output_char_type = typename std::remove_cvref_t<output>::char_type;
		if constexpr(std::same_as<input_char_type,output_char_type>)
		{
			auto curr{obuffer_curr(outp)};
			auto ed{obuffer_end(outp)};
			if(chars<static_cast<std::size_t>(ed-curr))
				ed=chars;
			for(auto ip{curr};;)
			{
				auto readed{read(inp,ip,ed)};
				obuffer_set_curr(outp,readed);
				if((readed==ip)|(readed==ed))
					return readed-curr;
				ip=readed;
			}
		}
		else
		{
			static_assert(std::same_as<output_char_type,char>,"only char allows punning");
			typedef input_char_type 
#if __has_cpp_attribute(gnu::may_alias)
			[[gnu::may_alias]]
#endif
			alias_input_char_type;
			auto raw_curr{obuffer_curr(outp)};
			auto raw_end{obuffer_end(outp)};
			alias_input_char_type* curr{reinterpret_cast<alias_input_char_type*>(raw_curr)};
			alias_input_char_type* ed{curr+(raw_end-raw_curr)/sizeof(input_char_type)};
			if(chars<ed-curr)
				ed=chars;
			for(auto ip{curr};;)
			{
				auto readed{read(inp,ip,ed)};
				obuffer_set_curr(outp,reinterpret_cast<char*>(readed));
				if((readed==ip)|(readed==ed))
					return readed-curr;
				ip=readed;
			}
		}
	}
	else
#endif
	if constexpr(buffer_input_stream<input>)
	{
		std::uintmax_t transmitted_chars{};
		do
		{
			auto b(ibuffer_curr(inp));
			auto e(ibuffer_end(inp));
			if(b!=e)[[likely]]
			{
				std::size_t transmitted_this_round{static_cast<std::size_t>(e-b)};
				if(chars<=transmitted_this_round)
				{
					write(outp,b,b+chars);
					return transmitted_chars+chars;
				}
				else
				{
					write(outp,b,e);
					transmitted_chars+=transmitted_this_round;
					chars-=transmitted_this_round;
				}
			}
		}
		while(ibuffer_underflow(inp));
		return transmitted_chars;
	}
	else
	{
		std::uintmax_t transmitted_chars{};
		using char_type = typename std::remove_cvref_t<input>::char_type;
		constexpr std::size_t buffer_size{cal_buffer_size<char_type>()};
#ifdef FAST_IO_TRANSMIT_ON_STACK
		::fast_io::freestanding::array<char_type,buffer_size> array;
		conditional_secure_clear_guard<char_type,secure_clear_requirement_stream<std::remove_cvref_t<input>>> guard(array.data(),buffer_size);
		auto array_start{array.data()};
#else
		buffer_alloc_arr_ptr<char_type,secure_clear_requirement_stream<std::remove_cvref_t<input>>> ptr(buffer_size);
		auto array_start{ptr.ptr};
// we need to allocate it on the heap to avoid potential stack overflows
#endif
		for(;chars;)
		{
			std::size_t b(buffer_size);
			if(chars<b)
				b=chars;
			auto p(read(inp,array_start,array_start+b));
			if(p==array_start)
				return transmitted_chars;
			std::size_t read_chars(p-array_start);
			write(outp,array_start,p);
			transmitted_chars+=read_chars;
			if(read_chars!=b)
				return transmitted_chars;
			chars-=read_chars;
		}
		return transmitted_chars;
	}
}
#if 0
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

template<output_stream output,input_stream input,typename... Args>
inline constexpr auto transmit_impl(output& outp,input& inp,Args&& ...args)
{
	if constexpr(mutex_stream<input>)
	{
		details::lock_guard lg{inp};
		decltype(auto) uh{inp.unlocked_handle()};
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


#if 0
#if defined(__linux__)
		if constexpr(zero_copy_output_stream<output>&&zero_copy_input_stream<input>)
		{
			if constexpr(buffer_input_stream<input>)
			{
				write(outp,ibuffer_curr(inp),ibuffer_end(inp));
				ibuffer_set_curr(inp,ibuffer_end(inp));
			}
			if constexpr(buffer_output_stream<output>)
				flush(outp);

			return zero_copy_transmit_impl(outp,inp,std::forward<Args>(args)...);
#else
			return zero_copy_transmit<false>(outp,inp,0,std::forward<Args>(args)...);
		}
		else
#endif
#endif
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
	print_freestanding(std::forward<output>(outp),manip::transmission_with_size<input,sz_type>{transmitted,in,s});
	return transmitted;
}

template<output_stream output,input_stream input>
inline constexpr std::uintmax_t transmit(output&& outp,input&& in)
{
	std::uintmax_t transmitted{};
	print_freestanding(std::forward<output>(outp),manip::transmission<input,std::uintmax_t>{transmitted,in});
	return transmitted;
}

}
