#pragma once

namespace fast_io
{

#if 0
template<std::integral char_type,typename T>
struct lc_reserve_type_t
{
explicit constexpr lc_reserve_type_t() noexcept = default;
};
template<std::integral char_type,typename T>
inline constexpr lc_reserve_type_t<char_type,T> lc_reserve_type{};

template<typename char_type,typename T>
concept lc_dynamic_reserve_printable = std::integral<char_type>&&
	requires(T t,basic_lc_all<char_type> const* all,char_type* ptr)
{
	{print_reserve_size(lc_reserve_type<char_type,std::remove_cvref_t<T>>,all,t)}->std::convertible_to<std::size_t>;
	{print_reserve_define(lc_reserve_type<char_type,std::remove_cvref_t<T>>,all,ptr,t)}->std::convertible_to<char_type*>;
};

template<std::integral char_type,typename value_type,typename Iter>
requires lc_dynamic_reserve_printable<char_type,std::remove_cvref_t<value_type>>
inline constexpr auto print_reserve_size(lc_reserve_type_t<char_type,parameter<value_type>>,basic_lc_all<char_type> const* __restrict all,parameter<value_type> para)
{
	return print_reserve_size(lc_reserve_type<char_type,std::remove_cvref_t<value_type>>,all,para.reference);
}

template<std::integral char_type,typename value_type,typename Iter>
requires lc_dynamic_reserve_printable<char_type,std::remove_cvref_t<value_type>>
inline constexpr auto print_reserve_define(lc_reserve_type_t<char_type,parameter<value_type>>,basic_lc_all<char_type> const* __restrict all,Iter begin,parameter<value_type> para)
{
	return print_reserve_define(lc_reserve_type<char_type,std::remove_cvref_t<value_type>>,all,begin,para.reference);
}
#else
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
#endif
template<typename output,typename T>
concept lc_printable = output_stream<output>&&requires(basic_lc_all<typename output::char_type> const* all,output out,T t)
{
	print_define(all,out,t);
};

namespace details::decay
{

template<std::integral char_type,typename T,typename... Args>
#if __has_cpp_attribute(gnu::sysv_abi) && !defined(_MSC_VER)
[[gnu::sysv_abi]]
#endif
inline constexpr std::size_t calculate_lc_scatter_dynamic_reserve_size(
	basic_lc_all<char_type> const* __restrict all,T t,Args... args)
{
	if constexpr(lc_dynamic_reserve_printable<char_type,T>)
	{
#if 0
		std::size_t res{print_reserve_size(lc_reserve_type<char_type,T>,all,t)};
#else
		std::size_t res{print_reserve_size(all,t)};
#endif
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
#if __has_cpp_attribute(gnu::sysv_abi) && !defined(_MSC_VER)
[[gnu::sysv_abi]]
#endif
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
#if 0
		auto end_ptr = print_reserve_define(lc_reserve_type<char_type,T>,all,dynamic_buffer_ptr,t);
#else
		auto end_ptr = print_reserve_define(all,dynamic_buffer_ptr,t);
#endif
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
#if __has_cpp_attribute(gnu::sysv_abi) && !defined(_MSC_VER)
[[gnu::sysv_abi]]
#endif
inline constexpr void lc_print_control_reserve_bad_path(basic_lc_all<typename output::char_type> const* __restrict lc,output out,T t,std::size_t size)
{
#if 0
	using value_type = std::remove_cvref_t<T>;
#endif
	using char_type = typename output::char_type;
	local_operator_new_array_ptr<char_type> ptr(size);
#if 0
	auto it{print_reserve_define(lc_reserve_type<char_type,value_type>,lc,ptr.ptr,t)};
#else
	auto it{print_reserve_define(lc,ptr.ptr,t)};
#endif
	if constexpr(line)
	{
		if constexpr(std::same_as<char,char_type>)
			*it='\n';
		else if constexpr(std::same_as<wchar_t,char_type>)
			*it=L'\n';
		else
			*it=u8'\n';
		++it;
	}
	write(out,ptr.ptr,it);
}

template<bool line,output_stream output,typename T>
requires (std::is_trivially_copyable_v<output>&&std::is_trivially_copyable_v<T>)
#if __has_cpp_attribute(gnu::sysv_abi) && !defined(_MSC_VER)
[[gnu::sysv_abi]]
#endif
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
					if constexpr(std::same_as<char,char_type>)
						*curr='\n';
					else if constexpr(std::same_as<wchar_t,char_type>)
						*curr=L'\n';
					else
						*curr=u8'\n';
					++curr;
					obuffer_set_curr(out,curr);
				}
				else
				{
					write(out,scatter.base,scatter.base+scatter.len);
					if constexpr(std::same_as<char,char_type>)
						put(out,'\n');
					else if constexpr(std::same_as<wchar_t,char_type>)
						put(out,L'\n');
					else
						put(out,u8'\n');
				}
			}
			else
			{
				write(out,scatter.base,scatter.base+scatter.len);
				if constexpr(std::same_as<char,char_type>)
					put(out,'\n');
				else if constexpr(std::same_as<wchar_t,char_type>)
					put(out,L'\n');
				else
					put(out,u8'\n');
			}
		}
		else
		{
			write(out,scatter.base,scatter.base+scatter.len);
		}
	}
	else if constexpr(lc_dynamic_reserve_printable<char_type,value_type>)
	{
#if 0
		std::size_t sz{print_reserve_size(lc_reserve_type<char_type,value_type>,lc,t)};
#else
		std::size_t sz{print_reserve_size(lc,t)};
#endif
		if constexpr(line)
			++sz;
		if constexpr(buffer_output_stream<output>)
		{
			auto bcurr{obuffer_curr(out)};
			auto bend{obuffer_end(out)};
			std::ptrdiff_t const diff(bend-bcurr);
			if(static_cast<std::ptrdiff_t>(sz)<diff)[[likely]]
			{
				//To check whether this affects performance.
#if 0
				auto it{print_reserve_define(lc_reserve_type<char_type,value_type>,lc,bcurr,t)};
#else
				auto it{print_reserve_define(lc,bcurr,t)};
#endif
				if constexpr(line)
				{
					if constexpr(std::same_as<char,char_type>)
						*it='\n';
					else if constexpr(std::same_as<wchar_t,char_type>)
						*it=L'\n';
					else
						*it=u8'\n';
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
			if constexpr(std::same_as<char_type,char>)
				put(out,'\n');
			else if constexpr(std::same_as<char_type,wchar_t>)
				put(out,L'\n');
			else
				put(out,u8'\n');
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
#if __has_cpp_attribute(gnu::sysv_abi) && !defined(_MSC_VER)
[[gnu::sysv_abi]]
#endif
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
#if __has_cpp_attribute(gnu::sysv_abi) && !defined(_MSC_VER)
[[gnu::sysv_abi]]
#endif
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
		std::array<io_scatter_t,sizeof...(Args)+static_cast<std::size_t>(ln)> scatters;
		if constexpr(((scatter_printable<char_type,Args>||lc_scatter_printable<char_type,Args>)&&...))
		{
			lc_scatter_print_recursive(lc,scatters.data(),args...);
			if constexpr(ln)
			{
				if constexpr(std::same_as<char_type,char>)
				{
					char_type ch('\n');
					scatters.back()={std::addressof(ch),sizeof(ch)};
					scatter_write(out,{scatters.data(),scatters.size()});
				}
				else if constexpr(std::same_as<char_type,wchar_t>)
				{
					char_type ch(L'\n');
					scatters.back()={std::addressof(ch),sizeof(ch)};
					scatter_write(out,{scatters.data(),scatters.size()});
				}
				else
				{
					char_type ch(u8'\n');
					scatters.back()={std::addressof(ch),sizeof(ch)};
					scatter_write(out,{scatters.data(),scatters.size()});
				}
			}
			else
				scatter_write(out,{scatters.data(),scatters.size()});
		}
		else
		{
			std::array<char_type,calculate_scatter_reserve_size<char_type,Args...>()> array;
			local_operator_new_array_ptr<char_type> new_ptr(calculate_lc_scatter_dynamic_reserve_size<char_type>(lc,args...));
			lc_scatter_print_with_dynamic_reserve_recursive(lc,scatters.data(),array.data(),new_ptr.ptr,args...);
			if constexpr(ln)
			{
				if constexpr(std::same_as<char_type,char>)
				{
					char_type ch('\n');
					scatters.back()={std::addressof(ch),sizeof(ch)};
					scatter_write(out,{scatters.data(),scatters.size()});
				}
				else if constexpr(std::same_as<char_type,wchar_t>)
				{
					char_type ch(L'\n');
					scatters.back()={std::addressof(ch),sizeof(ch)};
					scatter_write(out,{scatters.data(),scatters.size()});
				}
				else
				{
					char_type ch(u8'\n');
					scatters.back()={std::addressof(ch),sizeof(ch)};
					scatter_write(out,{scatters.data(),scatters.size()});
				}
			}
			else
				scatter_write(out,{scatters.data(),scatters.size()});
		}
	}
	else
	{
		dynamic_io_buffer<typename output::char_type> buffer;
		auto ref{io_ref(buffer)};
		lc_print_controls_line<line>(lc,ref,args...);
		write(out,buffer.beg_ptr,buffer.end_ptr);
	}
}

template<bool ln,output_stream output,typename... Args>
#if __has_cpp_attribute(gnu::sysv_abi) && !defined(_MSC_VER)
[[gnu::sysv_abi]]
#endif
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
			if constexpr(std::same_as<char_type,char>)
			{
				char_type ch('\n');
				put(out,ch);
			}
			else if constexpr(std::same_as<char_type,wchar_t>)
			{
				char_type ch(L'\n');
				put(out,ch);
			}
			else
			{
				char_type ch(u8'\n');
				put(out,ch);
			}
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
#if __has_cpp_attribute(gnu::sysv_abi) && !defined(_MSC_VER)
[[gnu::sysv_abi]]
#endif
inline constexpr void print_status_define(lc_imbuer<output> imb,Args... args)
{
	details::decay::lc_print_status_define_further_decay<false>(imb.all,imb.handle,args...);
}

template<output_stream output,typename... Args>
#if __has_cpp_attribute(gnu::sysv_abi) && !defined(_MSC_VER)
[[gnu::sysv_abi]]
#endif
inline constexpr void println_status_define(lc_imbuer<output> imb,Args... args)
{
	details::decay::lc_print_status_define_further_decay<true>(imb.all,imb.handle,args...);
}

}