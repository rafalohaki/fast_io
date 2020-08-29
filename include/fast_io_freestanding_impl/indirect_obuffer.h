#pragma once

namespace fast_io
{

template<std::integral ch_type,output_stream dest,typename func>
class basic_indirect_obuffer
{
public:
	using char_type = ch_type;
	using destination_type = dest;
	using function_type = func;
	destination_type destination;
	function_type function;
};

template<std::integral ch_type,output_stream dest,typename func,std::contiguous_iterator Iter>
inline constexpr auto write(basic_indirect_obuffer<ch_type,dest,func>& ob,Iter cbegin,Iter cend)
{
	return ob.function(ob.destination,cbegin,cend);
}

template<std::integral ch_type,output_stream dest,typename func>
class basic_indirect_obuffer_constructor_source_type:public basic_indirect_obuffer<ch_type,dest,func>
{
public:
	using char_type = ch_type;
	using destination_type = dest;
	using function_type = func;
	template<typename... Args>
	requires std::constructible_from<destination_type,Args...>
	constexpr basic_indirect_obuffer_constructor_source_type(Args&&...args):basic_indirect_obuffer<ch_type,dest,func>{.destination={std::forward<Args>(args)...},.function={}}{}
};

}