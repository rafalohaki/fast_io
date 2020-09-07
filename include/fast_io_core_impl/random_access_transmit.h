#pragma once

namespace fast_io
{
namespace details
{

#if defined(__linux__)||defined(__BSD_VISIBLE)
template<output_stream output,input_stream input>
inline constexpr std::common_type_t<std::size_t,std::uint64_t> zero_copy_random_access_transmit_impl(output& outp,input& inp,std::common_type_t<std::ptrdiff_t,std::int64_t> offset)
{
	auto ret(zero_copy_transmit<true,true>(outp,inp,offset));
	if(ret.second)
	{
		offset+=static_cast<std::common_type_t<std::ptrdiff_t,std::int64_t>>(ret.first);
		return ret.first+bufferred_transmit_impl(outp,inp);
	}
	return ret.first;
}

template<output_stream output,input_stream input>
inline constexpr std::common_type_t<std::size_t,std::uint64_t> zero_copy_random_access_transmit_impl(output& outp,input& inp,std::common_type_t<std::ptrdiff_t,std::int64_t> offset,std::common_type_t<std::size_t,std::uint64_t> sz)
{
	auto ret(zero_copy_transmit<true,true>(outp,inp,offset,sz)); 
	if(ret.second)
	{
		offset+=static_cast<std::common_type_t<std::ptrdiff_t,std::int64_t>>(ret.first);
		return ret.first+bufferred_transmit_impl(outp,inp,sz-ret.first);
	}
	return ret.first;
}
#endif

template<output_stream output,input_stream input,typename... Args>
inline constexpr auto random_access_transmit_impl(output& outp,input& inp,std::common_type_t<std::ptrdiff_t,std::int64_t> offset,Args&& ...args)
{
	if constexpr(mutex_stream<input>)
	{
		details::lock_guard lg{inp};
		decltype(auto) uh{inp.unlocked_handle()};
		return random_access_transmit_impl(outp,uh,std::forward<Args>(args)...);
	}
	else
	{
#ifdef __cpp_lib_is_constant_evaluated
		if (std::is_constant_evaluated())
		{
			seek(inp,offset);
			return bufferred_transmit_impl(outp,inp,std::forward<Args>(args)...);
		}
		else
		{
#endif
		if constexpr(zero_copy_output_stream<output>&&zero_copy_input_stream<input>)
		{
			if constexpr(buffer_input_stream<input>)
			{
				offset-=end(inp)-begin(inp);
				iclear(inp);
			}
			if constexpr(buffer_output_stream<output>)
				flush(outp);
#if defined(__linux__)||defined(__BSD_VISIBLE)
			return zero_copy_random_access_transmit_impl(outp,inp,offset,std::forward<Args>(args)...);
#else
			return zero_copy_transmit<true>(outp,inp,offset,std::forward<Args>(args)...);
#endif
		}
		else
		{
			seek(inp,offset);
			return bufferred_transmit_impl(outp,inp,std::forward<Args>(args)...);
		}
#ifdef __cpp_lib_is_constant_evaluated
		}
#endif
	}
}


}

template<output_stream output,input_stream input,std::integral sz_type,std::integral offset_type>
requires fast_io::random_access_stream<input>
inline constexpr void print_define(output& outp,manip::random_access_transmission<input,sz_type,offset_type> ref)
{
	ref.transmitted=static_cast<sz_type>(details::random_access_transmit_impl(outp,ref.reference,ref.offset));
}

template<output_stream output,input_stream input,std::integral sz_type,std::integral offset_type>
requires fast_io::random_access_stream<input>
inline constexpr void print_define(output& outp,manip::random_access_transmission_with_size<input,sz_type,offset_type> ref)
{
	ref.transmitted=static_cast<sz_type>(details::random_access_transmit_impl(outp,ref.reference,ref.offset,ref.bytes));
}

template<output_stream output,std::integral offset_type,input_stream input>
requires fast_io::random_access_stream<input>
inline constexpr std::common_type_t<std::size_t,std::uint64_t> random_access_transmit(output&& outp,offset_type offset,input&& in)
{
	std::common_type_t<std::size_t,std::uint64_t> transmitted{};
	print(outp,manip::random_access_transmission<input,offset_type,std::common_type_t<std::size_t,std::uint64_t>>(transmitted,offset,in));
	return transmitted;
}

template<output_stream output,std::integral offset_type,input_stream input,std::integral sz_type>
requires fast_io::random_access_stream<input>
inline constexpr sz_type random_access_transmit(output&& outp,offset_type offset,input&& in,sz_type bytes)
{
	sz_type transmitted{};
	print(outp,manip::random_access_transmission_with_size<input,offset_type,std::common_type_t<std::size_t,std::uint64_t>>(transmitted,offset,in,bytes));
	return transmitted;
}

}
