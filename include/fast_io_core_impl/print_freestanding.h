#pragma once

namespace fast_io
{

namespace details::decay
{

template<std::integral char_type,typename T>
inline constexpr void scatter_print_recursive(io_scatter_t* arr,T t)
{
	*arr=print_scatter_define(print_scatter_type<char_type>,t);
}

template<std::integral char_type,typename T,typename... Args>
inline constexpr void scatter_print_recursive(io_scatter_t* arr,T t, Args ...args)
{
	*arr=print_scatter_define(print_scatter_type<char_type>,t);
	scatter_print_recursive<char_type>(arr+1,args...);
}

template<std::integral char_type,typename T>
inline constexpr std::size_t calculate_scatter_reserve_size_unit()
{
	using real_type = std::remove_cvref_t<T>;
	if constexpr(reserve_printable<char_type,real_type>)
	{
		constexpr std::size_t sz{print_reserve_size(io_reserve_type<char_type,real_type>)};
		return sz;
	}
	else
		return 0;
}

template<std::integral char_type,typename T>
inline constexpr std::size_t calculate_scatter_reserve_size()
{
	return calculate_scatter_reserve_size_unit<char_type,T>();
}

template<std::integral char_type,typename T,typename... Args>
requires (sizeof...(Args)!=0)
inline constexpr std::size_t calculate_scatter_reserve_size()
{
	return calculate_scatter_reserve_size_unit<char_type,T>()+
		calculate_scatter_reserve_size<char_type,Args...>();
}

template<std::integral char_type,typename T>
inline constexpr void scatter_print_with_reserve_recursive_unit(char_type*& start_ptr,
		io_scatter_t* arr,T t)
{
	using real_type = std::remove_cvref_t<T>;
	if constexpr(scatter_printable<char_type,real_type>)
	{
		*arr=print_scatter_define(print_scatter_type<char_type>,t);
	}
	else
	{
		auto end_ptr = print_reserve_define(io_reserve_type<char_type,real_type>,start_ptr,t);
		*arr={start_ptr,(end_ptr-start_ptr)*sizeof(*start_ptr)};
		start_ptr=end_ptr;
	}
}



template<std::integral char_type,typename T>
inline constexpr void scatter_print_with_reserve_recursive(char_type* ptr,
		io_scatter_t* arr,T t)
{
	scatter_print_with_reserve_recursive_unit(ptr,arr,t);
}

template<std::integral char_type,typename T,typename... Args>
inline constexpr void scatter_print_with_reserve_recursive(char_type* ptr,
	io_scatter_t* arr,T t, Args ...args)
{
	scatter_print_with_reserve_recursive_unit(ptr,arr,t);
	scatter_print_with_reserve_recursive(ptr,arr+1,args...);
}

template<std::integral char_type,typename T>
requires scatter_type_printable<char_type,T>
inline constexpr auto extract_one_scatter(T t)
{
	return print_scatter_define(print_scatter_type<char_type>,t);
}

template<bool line=false,output_stream output,typename T>
requires (std::is_trivially_copyable_v<output>&&std::is_trivially_copyable_v<T>)
inline constexpr void print_control(output out,T t)
{
	using char_type = typename output::char_type;
	using value_type = std::remove_cvref_t<T>;
	if constexpr(reserve_printable<char_type,value_type>)
	{
		constexpr std::size_t size{print_reserve_size(io_reserve_type<char_type,value_type>)+static_cast<std::size_t>(line)};
		if constexpr(contiguous_output_stream<output>)
		{
			if constexpr(line)
			{
				auto it{print_reserve_define(io_reserve_type<char_type,value_type>,obuffer_curr(out),t)};
				*it=u8'\n';
				obuffer_set_curr(out,++it);
			}
			else
			{
				obuffer_set_curr(out,print_reserve_define(io_reserve_type<char_type,value_type>,obuffer_curr(out),t));
			}
		}
		else if constexpr(buffer_output_stream<output>)
		{
			auto bcurr{obuffer_curr(out)};
			if(bcurr+size<obuffer_end(out))[[likely]]
			{
				if constexpr(line)
				{
					auto it{print_reserve_define(io_reserve_type<char_type,value_type>,obuffer_curr(out),t)};
					*it=u8'\n';
					obuffer_set_curr(out,++it);
				}
				else
				{
					obuffer_set_curr(out,print_reserve_define(io_reserve_type<char_type,value_type>,bcurr,t));
				}
			}
			else
			{
				std::array<char_type,size> array;
				if constexpr(line)
				{
					auto it{print_reserve_define(io_reserve_type<char_type,value_type>,array.data(),t)};
					*it=u8'\n';
					write(out,array.data(),++it);
				}
				else
					write(out,array.data(),print_reserve_define(io_reserve_type<char_type,value_type>,array.data(),t));
			}
		}
		else if constexpr(reserve_output_stream<output>)
		{
			if constexpr(std::is_pointer_v<std::remove_cvref_t<decltype(oreserve(out,size))>>)
			{
				auto ptr{oreserve(out,size)};
				if(ptr==nullptr)[[unlikely]]
				{
					std::array<char_type,size> array;
					if constexpr(line)
					{
						auto it{print_reserve_define(io_reserve_type<char_type,value_type>,array.data(),t)};
						*it=u8'\n';
						write(out,array.data(),++it);
					}
					else
					{
						write(out,array.data(),print_reserve_define(io_reserve_type<char_type,value_type>,array.data(),t));
					}
					return;
				}
				if constexpr(line)
				{
					auto it{print_reserve_define(io_reserve_type<char_type,value_type>,ptr,t)};
					*it=u8'\n';
					orelease(out,++it);
				}
				else
					orelease(out,print_reserve_define(io_reserve_type<char_type,value_type>,ptr,t));
			}
			else
			{
				if constexpr(line)
				{
					auto it{print_reserve_define(io_reserve_type<char_type,value_type>,oreserve(out,size),t)};
					*it=u8'\n';
					orelease(out,++it);
				}
				else
					orelease(out,print_reserve_define(io_reserve_type<char_type,value_type>,oreserve(out,size),t));
			}
		}
		else
		{
			std::array<char_type,size> array;
			if constexpr(line)
			{
				auto it{print_reserve_define(io_reserve_type<char_type,value_type>,array.data(),t)};
				*it=u8'\n';
				write(out,array.data(),++it);
			}
			else
				write(out,array.data(),print_reserve_define(io_reserve_type<char_type,value_type>,array.data(),t));
		}
	}
	else if constexpr(printable<output,value_type>)
	{
		print_define(out,t);
		if constexpr(line)
			put(out,u8'\n');
	}
	else
	{
		static_assert(printable<output,value_type>,"type not printable");
	}
}

template<typename T,typename U>
concept test_print_control = requires(T t,U arg)
{
	print_control(t,arg);
};

template<bool line,output_stream output,typename ...Args>
inline constexpr void print_fallback(output out,Args ...args)
{
	if constexpr(scatter_output_stream<output>&&((scatter_printable<typename output::char_type,Args>||reserve_printable<typename output::char_type,Args>)&&...))
	{
		std::array<io_scatter_t,(sizeof...(Args))+static_cast<std::size_t>(line)> scatters;
		if constexpr((scatter_printable<typename output::char_type,Args>&&...))
		{
			scatter_print_recursive<typename output::char_type>(scatters.data(),args...);
			if constexpr(line)
			{
				typename output::char_type ch(u8'\n');
				scatters.back()={std::addressof(ch),sizeof(ch)};
				scatter_write(out,scatters);
			}
			else
				scatter_write(out,scatters);
		}
		else
		{
			std::array<typename output::char_type,calculate_scatter_reserve_size<typename output::char_type,Args...>()+static_cast<std::size_t>(line)> array;
			scatter_print_with_reserve_recursive(array.data(),scatters.data(),args...);
			if constexpr(line)
			{
				typename output::char_type ch(u8'\n');
				scatters.back()={std::addressof(ch),sizeof(ch)};
				scatter_write(out,scatters);
			}
			else
				scatter_write(out,scatters);
		}
	}
	else
	{
		using internal_buffer_type = internal_temporary_buffer<typename output::char_type>;
		internal_buffer_type buffer;
		auto ref{io_ref(buffer)};
		if constexpr((test_print_control<decltype(ref),Args>&&...))
		{
			if constexpr(line)
			{
				if constexpr((sizeof...(Args)==1)&&(reserve_printable<typename output::char_type,Args>&&...))
				{
					((print_control<true>(ref,args)),...);
				}
				else
				{
					((print_control(ref,args)),...);
					put(buffer,u8'\n');
				}
			}
			else
			{
				(print_control(ref,args),...);
			}
			write(out,buffer.beg_ptr,buffer.end_ptr);
		}
		else if constexpr(buffer_output_stream<internal_buffer_type>)
		{
			static_assert(!buffer_output_stream<internal_buffer_type>,
			"\n\n\tThis type is not defined for printing. Please consider define it with print_define or print_reserve_define."
			"\n\tSee wiki page https://github.com/expnkx/fast_io/wiki/0018.-custom-type\n");
		}
	}
}

}

template<output_stream output,typename ...Args>
requires (std::is_trivially_copyable_v<output>&&(std::is_trivially_copyable_v<Args>&&...))
inline constexpr void print_freestanding_decay(output out,Args ...args)
{
	if constexpr(mutex_stream<output>)
	{
		details::lock_guard lg{out};
		decltype(auto) dout{out.unlocked_handle()};
		print_freestanding_decay(io_ref(dout),args...);
	}
	else if constexpr(status_output_stream<output>)
		print_status_define(out,args...);
	else if constexpr(((printable<output,Args>||reserve_printable<typename output::char_type,Args>)&&...)&&(sizeof...(Args)==1||buffer_output_stream<output>))
	{
		if constexpr(sizeof...(Args)==1||(!maybe_buffer_output_stream<output>))
		{
			(details::decay::print_control(out,args),...);
		}
		else
		{
			if constexpr(sizeof...(Args)!=1)
			{
				if(!obuffer_is_active(out))[[unlikely]]
				{
					details::decay::print_fallback<false>(out,args...);
					return;
				}
			}
			(details::decay::print_control(out,args),...);
		}
	}
	else
		details::decay::print_fallback<false>(out,args...);
}


template<std::integral char_type,typename T>
inline constexpr decltype(auto) io_print_alias(T&& t)
{
	if constexpr(alias_printable<char_type,std::remove_cvref_t<T>>)
		return print_alias_define(io_alias<char_type>,std::forward<T>(t));
	else
		return std::forward<T>(t);
}

template<output_stream output,typename ...Args>
inline constexpr void print_freestanding(output&& out,Args&& ...args)
{
	print_freestanding_decay(io_ref(out),io_forward(io_print_alias<typename std::remove_cvref_t<output>::char_type>(args))...);
}


template<output_stream output,typename ...Args>
inline constexpr void println_freestanding_decay(output out,Args ...args)
{
	if constexpr(mutex_stream<output>)
	{
		details::lock_guard lg{out};
		decltype(auto) dout{out.unlocked_handle()};
		println_freestanding_decay(io_ref(dout),args...);
	}
	else if constexpr(status_output_stream<output>)
		println_status_define(out,args...);
	else if constexpr((sizeof...(Args)==1&&(reserve_printable<typename output::char_type,Args>&&...))||
	((printable<output,Args>&&...)&&buffer_output_stream<output>))
	{
		using char_type = typename std::remove_cvref_t<output>::char_type;
		if constexpr((sizeof...(Args)==1)&&(reserve_printable<typename output::char_type,Args>&&...))
			((details::decay::print_control<true>(out,args)),...);
		else
		{
			if constexpr(maybe_buffer_output_stream<output>)
			{
				if(!obuffer_is_active(out))[[unlikely]]
				{
					details::decay::print_fallback<true>(out,args...);
					return;
				}
			}
			if constexpr(sizeof...(Args)==1&&(scatter_type_printable<char_type,Args>&&...))
			{
				auto curr=obuffer_curr(out);
				auto end=obuffer_end(out);
				auto scatter=details::decay::extract_one_scatter<char_type>(args...);
				std::size_t const len{scatter.len};
				if(curr+(len+1)<end)[[likely]]
				{
					details::non_overlapped_copy_n(scatter.base,len,curr);
					curr[len]=u8'\n';
					obuffer_set_curr(out,curr+(len+1));
				}
				else
				{
					write(out,scatter.base,scatter.base+len);
					put(out,u8'\n');
				}
			}
			else
			{
				((details::decay::print_control(out,args)),...);
				put(out,u8'\n');
			}
		}
	}
	else
		details::decay::print_fallback<true>(out,args...);
}

template<output_stream output,typename ...Args>
inline constexpr void println_freestanding(output&& out,Args&& ...args)
{
	println_freestanding_decay(io_ref(out),io_forward(io_print_alias<typename std::remove_cvref_t<output>::char_type>(args))...);
}


}