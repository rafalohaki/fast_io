#pragma once

namespace fast_io
{

template<refill_buffer_input_stream src,dynamic_buffer_output_stream indire,typename func>
class basic_indirect_ibuffer
{
public:
	using source_buffer_type = src;
	using indirect_buffer_type = indire;
	using function_type = func;
	using char_type = typename indirect_buffer_type::char_type;
	source_buffer_type source;
	indirect_buffer_type indirect;
	function_type function;
	std::size_t current_position{};
};

template<refill_buffer_input_stream src,dynamic_buffer_output_stream indire,typename func>
class basic_indirect_ibuffer_constructor_source_type:public basic_indirect_ibuffer<src,indire,func>
{
public:
	using source_buffer_type = src;
	using indirect_buffer_type = indire;
	using function_type = func;
	using char_type = typename indirect_buffer_type::char_type;
	template<typename... Args>
	requires std::constructible_from<source_buffer_type,Args...>
	constexpr basic_indirect_ibuffer_constructor_source_type(Args&& ...args):basic_indirect_ibuffer<src,indire,func>{.source={std::forward<Args>(args)...},.indirect={},.function={}}
	{}
};

template<refill_buffer_input_stream src,dynamic_buffer_output_stream indire,typename function>
inline constexpr auto ibuffer_begin(basic_indirect_ibuffer<src,indire,function>& in)
{
	return obuffer_begin(in.indirect);
}

template<refill_buffer_input_stream src,dynamic_buffer_output_stream indire,typename function>
inline constexpr auto ibuffer_curr(basic_indirect_ibuffer<src,indire,function>& in)
{
	return obuffer_begin(in.indirect)+in.current_position;
}

template<refill_buffer_input_stream src,dynamic_buffer_output_stream indire,typename function>
inline constexpr auto ibuffer_end(basic_indirect_ibuffer<src,indire,function>& in)
{
	return obuffer_curr(in.indirect);
}

template<refill_buffer_input_stream src,dynamic_buffer_output_stream indire,typename function>
inline constexpr auto ibuffer_set_curr(basic_indirect_ibuffer<src,indire,function>& in,typename indire::char_type* ptr)
{
	in.current_position=ptr-obuffer_begin(in.indirect);
}

template<refill_buffer_input_stream src,dynamic_buffer_output_stream indire,typename function>
inline constexpr bool underflow(basic_indirect_ibuffer<src,indire,function>& in)
{
	if(!irefill(in.source))[[unlikely]]
		return false;
	in.current_position=0;
	obuffer_set_curr(in.indirect,obuffer_begin(in.indirect));
	if constexpr(std::same_as<decltype(in.function(in.indirect,ibuffer_begin(in.source),ibuffer_end(in.source))),void>)
	{
		auto ed{ibuffer_end(in.source)};
		in.function(in.indirect,ibuffer_begin(in.source),ed);
		ibuffer_set_curr(in.source,ed);
	}
	else
		ibuffer_set_curr(in.source,in.function(in.indirect,ibuffer_begin(in.source),ibuffer_end(in.source)));
	return true;
}

template<refill_buffer_input_stream src,dynamic_buffer_output_stream indire,typename function>
inline constexpr bool irefill(basic_indirect_ibuffer<src,indire,function>& in)
{
	if(!irefill(in.source))[[unlikely]]
		return false;
	auto b{ibuffer_begin(in)};
	auto c{ibuffer_curr(in)};
	auto e{ibuffer_end(in)};
	if(c!=e)
	{
		obuffer_set_curr(in.indirect,std::copy(c,e,b));
		in.current_position=0;
	}
	if constexpr(std::same_as<decltype(in.function(in.indirect,ibuffer_begin(in.source),ibuffer_end(in.source))),void>)
	{
		auto ed{ibuffer_end(in.source)};
		in.function(in.indirect,ibuffer_begin(in.source),ed);
		ibuffer_set_curr(in.source,ed);
	}
	else
		ibuffer_set_curr(in.source,in.function(in.indirect,ibuffer_begin(in.source),ibuffer_end(in.source)));
	return true;
}

namespace details::indirect_ibuffer
{
template<typename T,std::contiguous_iterator Iter>
inline constexpr Iter read_cold(T& in,Iter begin,Iter end)
{
	while(underflow(in))
	{
		auto bg{ibuffer_begin(in)};
		auto curr{bg+in.current_position};
		auto ed{ibuffer_end(in)};
		std::size_t to_read{end-begin};
		std::size_t available_in_buffer{ed-curr};
		std::size_t read_this_round(std::min(to_read,available_in_buffer));
		non_overlapped_copy_n(curr,read_this_round,begin);
		begin+=read_this_round;
		if(to_read<=available_in_buffer)[[likely]]
		{
 			in.current_position+=to_read;
			break;
		}
	}
	return begin;
}

}

template<refill_buffer_input_stream src,dynamic_buffer_output_stream indire,typename function,std::contiguous_iterator Iter>
requires (std::same_as<std::iter_value_t<Iter>,typename indire::char_type>||std::same_as<typename indire::char_type,char>)
inline constexpr Iter read(basic_indirect_ibuffer<src,indire,function>& in,Iter begin,Iter end)
{
	if constexpr(std::same_as<std::iter_value_t<Iter>,typename indire::char_type>)
	{
		auto bg{ibuffer_begin(in)};
		auto curr{bg+in.current_position};
		auto ed{ibuffer_end(in)};
		std::size_t to_read{end-begin};
		if(ed<curr+to_read)[[unlikely]]
			return details::indirect_ibuffer::read_cold(in,begin,end);
		details::non_overlapped_copy_n(curr,to_read,begin);
		return end;
	}
	else
	{
		auto b{reinterpret_cast<char*>(std::to_address(begin))};
		auto e{reinterpret_cast<char*>(std::to_address(end))};
		auto it{read(in,b,e)};
		return begin+(it-b)/sizeof(*begin);
	}
}

}