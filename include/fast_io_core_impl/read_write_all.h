#pragma once

namespace fast_io
{

template<input_stream input,std::input_iterator Iter>
[[nodiscard]] inline constexpr Iter read_all(input& inp,Iter begin,Iter end)
{
	if constexpr(contiguous_buffer_input_stream<input>)
		return read(inp,begin,end);
	else
	{
		for(Iter iter{begin};(iter = read(inp,begin,end))!=end;begin = iter)
			if(iter==begin)[[unlikely]]
				return iter;
		return end;
	}
}


template<output_stream output,std::input_iterator Iter>
inline constexpr void write_all(output& outp,Iter begin,Iter end)
{
	if constexpr(std::same_as<decltype(write(outp,begin,end)),void>)
	{
		write(outp,begin,end);
	}
	else
		for(Iter iter{begin};(iter = write(outp,begin,end))!=end;begin=iter);
}

}