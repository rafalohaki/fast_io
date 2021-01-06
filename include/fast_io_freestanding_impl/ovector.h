#pragma once

namespace fast_io
{
#ifndef _MSC_VER
template<output_stream output,typename vect_type = std::vector<output>>
class basic_ovector:public vect_type
{
public:
	using char_type = typename output::char_type;
	using vector_type = vect_type;
};

template<output_stream output,typename vect_type = std::vector<typename output::char_type>,std::contiguous_iterator Iter>
requires (std::same_as<typename output::char_type,std::iter_value_t<Iter>>||std::same_as<typename output::char_type,char>)
inline constexpr void write(basic_ovector<output,vect_type>& vec,Iter begin,Iter end)
{
	for(auto& e : vec)
		write(e,begin,end);
}
#endif
}
