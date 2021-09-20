#pragma once

namespace fast_io
{

template<typename char_type,typename T>
concept lc_dynamic_reserve_printable = std::integral<char_type>&&
	requires(T t,basic_lc_all<char_type> const* all,char_type* ptr,std::size_t size)
{
	{print_reserve_size(all,t)}->std::convertible_to<std::size_t>;
	{print_reserve_define(all,ptr,t)}->std::convertible_to<char_type*>;
};

template<std::integral char_type,typename value_type>
requires lc_dynamic_reserve_printable<char_type,std::remove_cvref_t<value_type>>
inline constexpr auto print_reserve_size(basic_lc_all<char_type> const* __restrict all,parameter<value_type> para)
{
	return print_reserve_size(all,para.reference);
}

template<std::integral char_type,typename value_type,typename Iter>
requires lc_dynamic_reserve_printable<char_type,std::remove_cvref_t<value_type>>
inline constexpr auto print_reserve_define(basic_lc_all<char_type> const* __restrict all,Iter begin,parameter<value_type> para)
{
	return print_reserve_define(all,begin,para.reference);
}
template<typename char_type,typename T>
concept lc_scatter_printable=requires(basic_lc_all<char_type> const* all,T t)
{
	{print_scatter_define(all,t)}->std::convertible_to<io_scatter_t>;
};

template<typename char_type,typename T>
concept lc_scatter_type_printable=lc_scatter_printable<char_type,T>&&requires(basic_lc_all<char_type> const* all,T t)
{
	{print_scatter_define(all,t)}->std::convertible_to<basic_io_scatter_t<char_type>>;
};

template<typename output,typename T>
concept lc_printable = output_stream<output>&&requires(basic_lc_all<typename output::char_type> const* all,output out,T t)
{
	print_define(all,out,t);
};

namespace details::decay
{

template<std::integral char_type,typename T,typename... Args>
inline constexpr std::size_t calculate_lc_scatter_dynamic_reserve_size(
	basic_lc_all<char_type> const* __restrict all,T t,Args... args)
{
	if constexpr(lc_dynamic_reserve_printable<char_type,T>)
	{
		std::size_t res{print_reserve_size(all,t)};
		if constexpr(sizeof...(Args)==0)
			return res;
		else
			return ::fast_io::details::intrinsics::add_or_overflow_die(res,calculate_lc_scatter_dynamic_reserve_size(all,args...));
	}
	else if constexpr(!reserve_printable<char_type,T>&&
		dynamic_reserve_printable<char_type,T>)
	{
		std::size_t res{print_reserve_size(io_reserve_type<char_type,T>,t)};
		if constexpr(sizeof...(Args)==0)
			return res;
		else
			return ::fast_io::details::intrinsics::add_or_overflow_die(res,calculate_lc_scatter_dynamic_reserve_size(all,args...));
	}
	else
	{
		if constexpr(sizeof...(Args)==0)
			return 0;
		else
			return calculate_lc_scatter_dynamic_reserve_size(all,args...);
	}
}

template<std::integral char_type,typename T,typename... Args>
inline constexpr void lc_scatter_print_with_dynamic_reserve_recursive(
	basic_lc_all<char_type> const* __restrict all,
	io_scatter_t* __restrict arr,
	char_type* __restrict ptr,
	char_type* __restrict dynamic_buffer_ptr,T t, Args ...args)
{
	if constexpr(lc_scatter_printable<char_type,T>)
	{
		*arr=print_scatter_define(all,t);
	}
	else if constexpr(lc_dynamic_reserve_printable<char_type,T>)
	{
		auto end_ptr = print_reserve_define(all,dynamic_buffer_ptr,t);
		*arr={dynamic_buffer_ptr,(end_ptr-dynamic_buffer_ptr)*sizeof(*dynamic_buffer_ptr)};
		if constexpr(sizeof...(Args)!=0)
			dynamic_buffer_ptr = end_ptr;
	}
	else if constexpr(scatter_type_printable<char_type,T>)
		*arr=print_scatter_define(print_scatter_type<char_type>,t);
	else if constexpr(reserve_printable<char_type,T>)
	{
		auto end_ptr = print_reserve_define(io_reserve_type<char_type,T>,ptr,t);
		*arr={ptr,(end_ptr-ptr)*sizeof(*ptr)};
		if constexpr(sizeof...(Args)!=0)
			ptr=end_ptr;
	}
	else
	{
		auto end_ptr = print_reserve_define(io_reserve_type<char_type,T>,dynamic_buffer_ptr,t);
		*arr={dynamic_buffer_ptr,static_cast<std::size_t>(end_ptr-dynamic_buffer_ptr)*sizeof(*dynamic_buffer_ptr)};
		if constexpr(sizeof...(Args)!=0)
			dynamic_buffer_ptr = end_ptr;
	}
	if constexpr(sizeof...(Args)!=0)
	{
		if constexpr(((!lc_dynamic_reserve_printable<char_type,Args>&&!lc_scatter_printable<char_type,Args>)&&...))
			scatter_print_with_dynamic_reserve_recursive(arr+1,ptr,dynamic_buffer_ptr,args...);
		else
			lc_scatter_print_with_dynamic_reserve_recursive(all,arr+1,ptr,dynamic_buffer_ptr,args...);
	}
}

template<bool line,output_stream output,typename T>
inline constexpr void lc_print_control_reserve_bad_path(basic_lc_all<typename output::char_type> const* __restrict lc,output out,T t,std::size_t size)
{
	using char_type = typename output::char_type;
	if constexpr(line)
	{
		if(SIZE_MAX==size)
			fast_terminate();
		++size;
	}
	local_operator_new_array_ptr<char_type> ptr(size);
	auto it{print_reserve_define(lc,ptr.ptr,t)};
	if constexpr(line)
	{
		*it=char_literal_v<u8'\n',char_type>;
		++it;
	}
	write(out,ptr.ptr,it);
}

template<bool line,output_stream output,typename T>
requires (std::is_trivially_copyable_v<output>&&std::is_trivially_copyable_v<T>)
inline constexpr void lc_print_control(basic_lc_all<typename output::char_type> const* __restrict lc,output out,T t)
{
	using char_type = typename output::char_type;
	using value_type = std::remove_cvref_t<T>;
	if constexpr(lc_scatter_type_printable<char_type,value_type>)
	{
		basic_io_scatter_t<char_type> scatter{print_scatter_define(lc,t)};
		if constexpr(line)
		{
			if constexpr(buffer_output_stream<output>)
			{
				auto curr=obuffer_curr(out);
				auto end=obuffer_end(out);
				std::size_t const len{scatter.len};
				std::ptrdiff_t const diff(end-curr-1);
				if(static_cast<std::ptrdiff_t>(len)<diff)[[likely]]
				{
					curr=non_overlapped_copy_n(scatter.base,len,curr);
					*curr=char_literal_v<u8'\n',char_type>;
					++curr;
					obuffer_set_curr(out,curr);
				}
				else
				{
					write(out,scatter.base,scatter.base+scatter.len);
					put(out,char_literal_v<u8'\n',char_type>);
				}
			}
			else
			{
				write(out,scatter.base,scatter.base+scatter.len);
				put(out,char_literal_v<u8'\n',char_type>);
			}
		}
		else
		{
			write(out,scatter.base,scatter.base+scatter.len);
		}
	}
	else if constexpr(lc_dynamic_reserve_printable<char_type,value_type>)
	{
		std::size_t sz{print_reserve_size(lc,t)};
		if constexpr(buffer_output_stream<output>)
		{
			auto bcurr{obuffer_curr(out)};
			auto bend{obuffer_end(out)};
			std::ptrdiff_t diff(bend-bcurr);
			if constexpr(line)
			{
				--diff;
			}
			if(static_cast<std::ptrdiff_t>(sz)<diff)[[likely]]
			{
				//To check whether this affects performance.
				auto it{print_reserve_define(lc,bcurr,t)};
				if constexpr(line)
				{
					*it=char_literal_v<u8'\n',char_type>;
					++it;
				}	
				obuffer_set_curr(out,it);
			}
			else
				lc_print_control_reserve_bad_path<line>(lc,out,t,sz);
		}
		else
			lc_print_control_reserve_bad_path<line>(lc,out,t,sz);
	}
	else if constexpr(lc_printable<output,value_type>)
	{
		print_define(lc,out,t);
		if constexpr(line)
		{
			put(out,char_literal_v<u8'\n',char_type>);
		}
	}
	else
		print_control<line>(out,t);
}

template<bool line,output_stream output,typename T,typename... Args>
inline constexpr void lc_print_controls_line(basic_lc_all<typename output::char_type> const* __restrict lc,output out,T t,Args... args)
{
	if constexpr(sizeof...(Args)==0)
	{
		lc_print_control<line>(lc,out,t);
	}
	else
	{
		using char_type = typename output::char_type;
		lc_print_control<false>(lc,out,t);
		if constexpr(((lc_scatter_printable<char_type,Args>||
		lc_dynamic_reserve_printable<char_type,Args>||
		lc_printable<output,Args>)||...))
			lc_print_controls_line<line>(lc,out,args...);
		else
			print_controls_line<line>(out,args...);
	}
}

template<std::integral char_type,typename T,typename... Args>
inline constexpr void lc_scatter_print_recursive(basic_lc_all<char_type> const* __restrict lc,io_scatter_t* arr,T t, Args ...args)
{
	if constexpr(lc_scatter_printable<char_type,T>)
		*arr=print_scatter_define(lc,t);
	else
		*arr=print_scatter_define(print_scatter_type<char_type>,t);
	if constexpr(sizeof...(Args)!=0)
	{
		if constexpr(((!lc_scatter_printable<char_type,Args>)&&...))
			scatter_print_recursive<char_type>(arr+1,args...);
		else
			lc_scatter_print_recursive(lc,arr+1,args...);
	}
}

template<bool ln,output_stream output,typename... Args>
inline constexpr void lc_print_fallback(basic_lc_all<typename output::char_type> const* __restrict lc,output out,Args... args)
{
	using char_type = typename output::char_type;
	if constexpr((((!lc_dynamic_reserve_printable<char_type,Args>&&
	!lc_printable<io_reference_wrapper<
		dynamic_io_buffer<char_type>>,Args>&&!lc_scatter_printable<char_type,Args>))&&...))
	{
		print_freestanding_decay_normal<ln>(out,args...);
	}
	else if constexpr(scatter_output_stream<output>&&
	((reserve_printable<char_type,Args>
		||dynamic_reserve_printable<char_type,Args>
		||lc_dynamic_reserve_printable<char_type,Args>
		||scatter_printable<char_type,Args>
		||lc_scatter_printable<char_type,Args>
		)&&...))
	{
		::fast_io::freestanding::array<io_scatter_t,sizeof...(Args)+static_cast<std::size_t>(ln)> scatters;
		if constexpr(((scatter_printable<char_type,Args>||lc_scatter_printable<char_type,Args>)&&...))
		{
			lc_scatter_print_recursive(lc,scatters.data(),args...);
			if constexpr(ln)
			{
				constexpr char_type ch(char_literal_v<u8'\n',char_type>);
				scatters.back()={__builtin_addressof(ch),sizeof(ch)};
				scatter_write(out,{scatters.data(),scatters.size()});
			}
			else
				scatter_write(out,{scatters.data(),scatters.size()});
		}
		else
		{
			::fast_io::freestanding::array<char_type,calculate_scatter_reserve_size<char_type,Args...>()> array;
			local_operator_new_array_ptr<char_type> new_ptr(calculate_lc_scatter_dynamic_reserve_size<char_type>(lc,args...));
			lc_scatter_print_with_dynamic_reserve_recursive(lc,scatters.data(),array.data(),new_ptr.ptr,args...);
			if constexpr(ln)
			{
				constexpr char_type ch(char_literal_v<u8'\n',char_type>);
				scatters.back()={__builtin_addressof(ch),sizeof(ch)};
				scatter_write(out,{scatters.data(),scatters.size()});
			}
			else
				scatter_write(out,{scatters.data(),scatters.size()});
		}
	}
	else
	{
		dynamic_io_buffer<typename output::char_type> buffer;
		auto ref{io_ref(buffer)};
		lc_print_controls_line<ln>(lc,ref,args...);
		write(out,buffer.beg_ptr,buffer.end_ptr);
	}
}

template<bool ln,output_stream output,typename... Args>
inline constexpr void lc_print_status_define_further_decay(basic_lc_all<typename output::char_type> const* __restrict lc,output out,Args... args)
{
	using char_type = typename output::char_type;
	if constexpr(sizeof...(Args)==0&&!ln)
		return;
	else if constexpr(mutex_stream<output>)
	{
		lock_guard lg{out};
		decltype(auto) dout{out.unlocked_handle()};
		lc_print_status_define_further_decay<ln>(lc,io_ref(dout),args...);
	}
	else if constexpr(buffer_output_stream<output>)
	{
		if constexpr(sizeof...(Args)==0&&ln)
		{
			put(out,char_literal_v<u8'\n',char_type>);
		}
		else
		{
			if constexpr(ln)
				lc_print_controls_line<ln>(lc,out,args...);
			else
				(lc_print_control<ln>(lc,out,args),...);
		}
	}
	else if constexpr(sizeof...(Args)==1&&!ln
	&&((printable<output,Args>||
	scatter_type_printable<char_type,Args>||lc_scatter_type_printable<char_type,Args>||
	lc_dynamic_reserve_printable<char_type,Args>||
	reserve_printable<char_type,Args>||dynamic_reserve_printable<char_type,Args>
	)&&...)
	)
	{
		(lc_print_control<false>(lc,out,args),...);
	}
	else if constexpr(sizeof...(Args)==1&&ln&&((lc_dynamic_reserve_printable<char_type,Args>)&&...))
	{
		(lc_print_control<true>(lc,out,args),...);
	}
	else
		lc_print_fallback<ln>(lc,out,args...);
}

}

template<output_stream output,typename... Args>
inline constexpr void print_status_define(lc_imbuer<output> imb,Args... args)
{
	::fast_io::details::decay::lc_print_status_define_further_decay<false>(imb.all,imb.handle,args...);
}

template<output_stream output,typename... Args>
inline constexpr void println_status_define(lc_imbuer<output> imb,Args... args)
{
	::fast_io::details::decay::lc_print_status_define_further_decay<true>(imb.all,imb.handle,args...);
}

}