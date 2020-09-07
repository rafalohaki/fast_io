#pragma once
//#include<unordered_map>

namespace fast_io
{

// i think this should use coroutine. Let's wait until C++20
/*
template<buffer_input_stream input>
inline void get_http_header_split(input& in,std::basic_string<typename input::char_type> &str)
{
	str.clear();
	for(decltype(c(in)) ch;!(ch=get<true>(in)).second&&!details::isspace(ch.first)&ch.first!=0xD&ch.first!=0x3a;str.push_back(ch.first));
}

template<buffer_input_stream input>
inline constexpr auto scan_http_header(input& in)
{
	using string_type = std::basic_string<typename input::char_type>;
	std::unordered_map<string_type,string_type> results;
	for(string_type str,a,b;;)
	{
		getline(in,str);
		if(str.size()<2)
			return results;
		basic_istring_view<std::basic_string_view<typename input::char_type>> isv(str);
		get_http_header_split(isv,a);
		if(a.back()==0x3a)
			a.pop_back();
		getcarriage(isv,b);
		results.emplace(std::move(a),std::move(b));
	}
}*/
template<std::integral ch_type>
struct http_header_line
{
	std::basic_string_view<ch_type> key;
	std::basic_string_view<ch_type> value;
};

template<std::integral ch_type>
struct basic_http_status
{
	std::basic_string<ch_type> version;
	std::size_t code{};
};

using http_status = basic_http_status<char>;
using u8http_status = basic_http_status<char8_t>;

template<std::integral ch_type>
struct basic_http_request_status
{
	std::basic_string<ch_type> method;
	std::basic_string<ch_type> path;
	std::basic_string<ch_type> version;
};

using http_request_status = basic_http_request_status<char>;
using u8http_request_status = basic_http_request_status<char8_t>;


namespace details
{
template<typename T>
inline constexpr bool fastisspacerev2(T ch)
{
	std::make_unsigned_t<T> e(ch);
	e-=9;
	return e<5;
}
}

template<buffer_input_stream input>
inline void scan_define(input& in,basic_http_status<typename input::char_type>& s)
{
	scan(in,s.version,s.code);
	skip_line(in);
}

template<buffer_input_stream input>
inline void scan_define(input& in,basic_http_request_status<typename input::char_type>& s)
{
	scan(in,s.method,s.path,s.version);
	skip_line(in);
}

template<buffer_output_stream output>
inline void print_define(output& out,basic_http_status<typename output::char_type> const& s)
{
	print(out,s.version,u8" ",s.code);
}

template<buffer_output_stream output>
inline void print_define(output& out,basic_http_request_status<typename output::char_type> const& s)
{
	print(out,s.method,u8" ",s.path,u8" ",s.version);
}

#if __cpp_lib_coroutine >= 201707L

template<input_stream input>
requires (buffer_input_stream<input>||mutex_stream<input>)
inline generator<http_header_line<typename input::char_type>> scan_http_header(input& in)
{
	if constexpr(mutex_stream<input>)
	{
		details::lock_guard lg{in};
		decltype(auto) uh{in.unlocked_handle()};
		return scan_http_header(uh);
	}
	else
	{
		for(std::basic_string<typename input::char_type> str;;)
		{
			scan(in,line(str));
			if(str.size()<2)
				co_return;
			auto sz{str.find(u8':')};
			if(sz==std::string::npos)
#ifdef __cpp_exceptions
				throw fast_io_text_error("unknown http header line");
#else
				fast_terminate();
#endif
			std::size_t i{sz+1};
			for(;i!=str.size()&&str[i]==u8' ';++i);
			co_yield http_header_line<typename input::char_type>{
			std::basic_string_view<typename input::char_type>(str.data(),sz),
			std::basic_string_view<typename input::char_type>(str.cbegin()+i,str.cend())};
		}
	}
}
#endif
template<buffer_input_stream input>
inline constexpr void skip_http_header(input& in)
{
	while(1<skip_line(in));
}


}
