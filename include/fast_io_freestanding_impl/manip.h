#pragma once

namespace fast_io
{

namespace details
{

template<character_input_stream input,typename Func>
inline constexpr bool scan_std_string(input& in,std::basic_string<typename input::char_type>& str,Func&& dg)
{
	if constexpr(contiguous_input_stream<input>)
	{
		auto b{ibuffer_curr(in)};
		auto e{ibuffer_end(in)};
		for(;b!=e&&dg(*b);++b);
		if(b==e)
			return false;
		auto i{b};
		for(;i!=e&&!dg(*i);++i);
		str.clear();
		str.append(b,i);
		ibuffer_set_curr(in,i);
	}
	else if constexpr(buffer_input_stream<input>)
	{
		for(;;)
		{
			auto b{ibuffer_curr(in)};
			auto e{ibuffer_end(in)};
			for(;b!=e&&dg(*b);++b);
			ibuffer_set_curr(in,b);
			if(b==e)[[unlikely]]
			{
				if(!underflow(in))[[unlikely]]
					return false;
			}
			else
				break;
		}
		for(str.clear();;)
		{
			auto b{ibuffer_curr(in)};
			auto e{ibuffer_end(in)};
			auto i{b};
			for(;i!=e&&!dg(*i);++i);
			str.append(b,i);
			ibuffer_set_curr(in,i);
			if(i==e)[[unlikely]]
			{
				if(!underflow(in))[[unlikely]]
					break;
			}
			else
				break;
		}
		return true;
	}
	else
	{
		auto gen{igenerator(in)};
		auto i{begin(gen)};
		auto e{end(gen)};
		for(;i!=e&&dg(*i);++i);
		if(i==e)
			return false;
		for(str.clear();i!=e&&!dg(*i);++i)
			str.push_back(*i);
		return true;
	}
}

}

template<character_input_stream input,typename func>
inline constexpr bool scan_define(input& in,manip::space<std::basic_string<typename input::char_type>&,func&> str)
{
	return details::scan_std_string(in,str.reference,str.function);
}

template<character_input_stream input>
inline constexpr bool scan_define(input& in,std::basic_string<typename input::char_type> &str)
{
	constexpr details::is_none_space dg;
	return details::scan_std_string(in,str,[&](auto ch)
	{
		return !dg(ch);
	});
}

template<input_stream input>
inline void scan_define(input& in,manip::whole<std::basic_string<typename input::char_type>> r)
{
	r.reference.clear();
	fast_io::ostring_ref osf{r.reference};
//	fast_io::basic_ostring_ref<std::basic_string<typename input::char_type>> ostrf(r.reference);
	transmit(osf,in);
}

template<buffer_input_stream input>
inline bool scan_define(input& in,manip::line<std::basic_string<typename input::char_type>&> ref)
{
	auto& str{ref.reference};
	if constexpr(buffer_input_stream<input>)
	{
		if(ibuffer_curr(in)==ibuffer_end(in))[[unlikely]]
				if(!underflow(in))[[unlikely]]
					return false;
		for(str.clear();;)
		{
			auto b{ibuffer_curr(in)};
			auto i{b};
			auto e{ibuffer_end(in)};
			for(;i!=e&&*i!=u8'\n';++i);
			str.append(b,i);
			ibuffer_set_curr(in,i);
			if(i==e)[[unlikely]]
			{
				if(!underflow(in))[[unlikely]]
					break;
			}
			else
			{
				ibuffer_set_curr(in,i+1);
				break;
			}
		}
		return true;
	}
	else
	{
		auto gen{igenerator(in)};
		auto i{begin(gen)};
		auto e{end(gen)};
		if(i==e)[[unlikely]]
			return false;
		for(;i!=e&&*i!=u8'\n';++i)
			str.push_back(*i);
		if(i!=e)[[likely]]
			++i;
		return true;
	}
/*	auto i(begin(in));
	auto e(end(in));
	if(i==e)
		return false;
	for(ref.reference.clear();;)
	{
		auto i(begin(in));
		auto e(end(in));
		auto j(i);
		for(;j!=e&&*j!=u8'\n';++j);
		ref.reference.append(i,j);
		if(j!=e)
		{
			in+=j-i+1;
			return false;
		}
		if(!iflush(in))
			return false;
		ref.reference.reserve(ref.reference.size()<<1);
	}*/
}

/*template<character_input_stream input>
inline void getline(input& in,std::basic_string<typename input::char_type> &str)
{
	str.clear();
	for(decltype(get<true>(in)) ch;!(ch=get<true>(in)).second&&ch.first!=0xA;str.push_back(ch.first));
}


template<character_input_stream input>
inline void getcarriage(input& in,std::basic_string<typename input::char_type> &str)
{
	str.clear();
	for(decltype(get<true>(in)) ch;!(ch=get<true>(in)).second&&ch.first!=0xD;str.push_back(ch.first));
}

template<character_input_stream input>
inline void getwhole(input& in,std::basic_string<typename input::char_type> &str)
{
	str.clear();
	for(decltype(get<true>(in)) ch;!(ch=get<true>(in)).second;str.push_back(ch.first));
}


template<character_output_stream output,std::size_t indent_width,bool left,char8_t ch,typename T>
inline constexpr void print_define(output& out,manip::width<indent_width,left,ch,T const> a)
{
	basic_ostring<std::basic_string<typename output::char_type>> bas;
	print_freestanding(bas,a.reference);
	std::size_t const size(bas.str().size());
	if(size<indent_width)
	{
		if constexpr(left)
		{
			print_freestanding(out,bas.str());
			fill_nc(out,indent_width-size,ch);
		}	
		else
		{
			fill_nc(out,indent_width-size,ch);
			print_freestanding(out,bas.str());
		}
	}
	else
		print_freestanding(out,bas.str());
}
*/
template<output_stream output>
inline constexpr void print_define(output& out,std::endian e)
{
	switch(e)
	{
	case std::endian::little:
		print_freestanding(out,u8"little");
	break;
	case std::endian::big:
		print_freestanding(out,u8"big");
	break;
	default:
		print_freestanding(out,u8"unknown");
	}
}

}