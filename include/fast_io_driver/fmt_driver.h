#pragma once
#include"../fast_io.h"
#include<fmt/format.h>

namespace fast_io
{

template<std::integral char_type,typename... Args>
class fmt_vformat
{
public:
	std::basic_string_view<char_type> format_str;
	std::tuple<Args&&...> args; 
};

template<std::integral char_type,typename... Args>
constexpr auto fmt_format(std::basic_string_view<char_type> bsv,Args&& ...args)
{
	return fmt_vformat<char_type,Args...>(bsv,forward_as_tuple(args...));
}

template<std::integral char_type,std::size_t N,typename... Args>
constexpr auto fmt_format(char_type (&bsv)[N],Args&& ...args)
{
	return fmt_vformat<std::remove_cvref_t<char_type>,Args&&...>(bsv,{std::forward<Args>(args)...});
}

template<output_stream output,std::integral char_type,typename... Args>
inline void print_define(output& out,fmt_vformat<char_type,Args...> vfmt)
{
	std::apply([&](auto&& ...args)
	{
		fmt::basic_memory_buffer<char_type> buffer;
		fmt::format_to(buffer,vfmt.format_str,std::forward<Args>(args)...);
		write(out,buffer.data(),buffer.data()+buffer.size());
	},vfmt.args);
}

}

template<typename T,typename... Args>
inline constexpr void fmt_fprint(T&& out,Args&& ...args)
{
	if constexpr(fast_io::output_stream<std::remove_cvref_t<T>>)
	{
		fmt::basic_memory_buffer<typename std::remove_cvref_t<T>::char_type> buffer;
		fmt::format_to(buffer,std::forward<Args>(args)...);
		write(out,buffer.data(),buffer.data()+buffer.size());		
	}
	else
	{
		fmt_fprint(fast_io::c_stdout(),std::forward<T>(out),std::forward<Args>(args)...);
	}
}