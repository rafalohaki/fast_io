#pragma once

namespace fast_io
{

/*
format string vulnerablities are hard to avoid when you deal with format string

We have to put restrictions on them and mark them as unsafe.
*/

namespace details
{

template<typename T,typename U,typename... Args>
inline constexpr void print_para_at_pos(T out,std::size_t sz,U u,Args ...args)
{
	if(sz==0)
		decay::print_control(out,u);
	if constexpr(sizeof...(Args)!=0)
		print_para_at_pos(out,sz-1,args...);
}

template<std::integral char_type,char8_t ch>
inline constexpr char_type get_bracket() noexcept
{
	static_assert(ch==u8'{'||ch==u8'}'||ch==u8'0');
	if constexpr(std::same_as<char,char_type>)
	{
		if constexpr(ch==u8'0')
			return '0';
		else if constexpr(ch==u8'{')
			return '{';
		else
			return '}';
	}
	else if constexpr(std::same_as<wchar_t,char_type>)
	{
		if constexpr(ch==u8'0')
			return L'0';
		else if constexpr(ch==u8'{')
			return L'{';
		else
			return L'}';
	}
	else
		return static_cast<char_type>(ch);
}

enum class fprint_args_num_para_enum
{
one,less_than_11,other
};

template<fprint_args_num_para_enum ag>
struct fprint_args_num_para
{
#if __has_cpp_attribute(no_unique_address)
	[[no_unique_address]]
#endif
	std::conditional_t<ag==fprint_args_num_para_enum::one,empty,std::size_t> arg_num;
};

template<std::integral char_type,typename write_functor,typename functor,fprint_args_num_para_enum ag>
inline constexpr void unsafe_fprint_impl(char_type* it,char_type* ed,write_functor write_func,functor func,
[[maybe_unused]] fprint_args_num_para<ag> arg)
{

	using unsigned_char_type = std::make_unsigned_t<char_type>;
	std::size_t current_parameter{};
	std::size_t swap_parameters{};
	constexpr auto left_bracket{details::get_bracket<char_type,u8'{'>()};
	constexpr auto right_bracket{details::get_bracket<char_type,u8'}'>()};
	constexpr unsigned_char_type zeroch(static_cast<unsigned_char_type>(details::get_bracket<char_type,u8'0'>()));
	std::size_t maximum_allowed_args;
	std::size_t args_num;
	if constexpr(ag==fprint_args_num_para_enum::one)
	{
		maximum_allowed_args=100;
		args_num=1;
	}
	else if constexpr(ag==fprint_args_num_para_enum::less_than_11)
	{
		maximum_allowed_args=100;
		args_num=arg.arg_num;
	}
	else
	{
		args_num=arg.arg_num;
		maximum_allowed_args=static_cast<std::size_t>(args_num*static_cast<std::size_t>(10));
	}
	for(;it!=ed;)
	{
		auto next_it{::fast_io::freestanding::find(it,ed,left_bracket)};
		std::ptrdiff_t diff{ed-next_it};
		if(diff<2)
			break;
		auto next_ch{next_it[1]};
		if(next_ch==left_bracket)
		{
			write_func(it,next_it+1);
			it=next_it+2;
			continue;
		}
		else if(next_ch==right_bracket)
		{
			if(current_parameter==args_num)
				write_func(it,next_it+2);
			else
			{
				write_func(it,next_it);
				if constexpr(ag==fprint_args_num_para_enum::one)
				{
					func();
				}
				else
				{
					func(current_parameter);
				}
				++current_parameter;
			}
			it=next_it+2;
			continue;
		}
		if(diff==2||swap_parameters==maximum_allowed_args)
			break;
		if constexpr(ag==fprint_args_num_para_enum::one)
		{
			if((next_ch!=zeroch)|(next_it[2]!=right_bracket))
				break;
			write_func(it,next_it);
			func();
			++swap_parameters;
			it=next_it+3;
		}
		else if constexpr(ag==fprint_args_num_para_enum::less_than_11)
		{
			unsigned_char_type const max_sz{static_cast<unsigned_char_type>(args_num)};
			if(next_it[2]!=right_bracket)
				break;
			unsigned_char_type rn(static_cast<unsigned_char_type>(next_ch)-zeroch);
			if(rn<max_sz)[[likely]]
			{
				write_func(it,next_it);
				func(static_cast<std::size_t>(rn));
				++swap_parameters;
				it=next_it+3;
			}
			else
				break;
		}
		else
		{
			std::size_t n;
			auto res{scan_context_define(scan_context<true>,next_it+1,ed,parameter<std::size_t&>{n})};
			if((res.code!=parse_code::ok)|(res.iter==ed||*res.iter!=right_bracket))
				break;
			if(args_num<=n)
				break;
			write_func(it,next_it);
			func(static_cast<std::size_t>(n));
			++swap_parameters;
			it=res.iter+1;
		}
	}
	if(it!=ed)
	{
		write_func(it,ed);
	}
}

template<typename output,typename... Args>
requires (sizeof...(Args)!=0)
inline constexpr void unsafe_fprint_freestanding_decay_impl(output out,
	::fast_io::freestanding::basic_string_view<typename output::char_type> view,
	Args ...args)
{
	using char_type = typename output::char_type;
	if constexpr(sizeof...(Args)==1)
	{
		unsafe_fprint_impl(view.data(),view.data()+view.size(),
			[out](char_type const* beg_ptr,char_type const* end_ptr)
		{
			write(out,beg_ptr,end_ptr);
		},[&]()
		{
			((details::decay::print_control(out,args)),...);
		},fprint_args_num_para<fprint_args_num_para_enum::one>{});
	}
	else
	{
		unsafe_fprint_impl(view.data(),view.data()+view.size(),
			[out](char_type const* beg_ptr,char_type const* end_ptr)
		{
			write(out,beg_ptr,end_ptr);
		},[&](std::size_t pos)
		{
			print_para_at_pos(out,pos,args...);
		},fprint_args_num_para<sizeof...(Args)<11?fprint_args_num_para_enum::less_than_11:fprint_args_num_para_enum::other>{sizeof...(Args)});
	}
}

template<fprint_args_num_para_enum ag,std::integral char_type>
inline constexpr io_scatter_t* scatter_fprint_impl(
	::fast_io::freestanding::basic_string_view<char_type> view,
	io_scatter_t* scatters,
	io_scatter_t* new_scatters,fprint_args_num_para<ag> arg)
{
	if constexpr(ag==fprint_args_num_para_enum::one)
	{
		unsafe_fprint_impl(view.data(),view.data()+view.size(),
			[&new_scatters](char_type const* beg_ptr,char_type const* end_ptr)
		{
			*new_scatters={beg_ptr,(end_ptr-beg_ptr)*sizeof(char_type)};
			++new_scatters;
		},[&]()
		{
			*new_scatters=*scatters;
			++new_scatters;
		},arg);
	}
	else
	{
		unsafe_fprint_impl(view.data(),view.data()+view.size(),
			[&new_scatters](char_type const* beg_ptr,char_type const* end_ptr)
		{
			*new_scatters={beg_ptr,(end_ptr-beg_ptr)*sizeof(char_type)};
			++new_scatters;
		},[&](std::size_t pos)
		{
			*new_scatters=scatters[pos];
			++new_scatters;
		},arg);
	}
	return new_scatters;
}

template<std::size_t args_num,output_stream output,typename ...Args>
inline constexpr void scatter_fprint_write(output out,
	::fast_io::freestanding::basic_string_view<typename output::char_type> view,
	io_scatter_t* scatters)
{
	constexpr std::size_t maximum_allowed_args{::fast_io::freestanding::max(static_cast<std::size_t>(100),args_num*static_cast<std::size_t>(10))+args_num};
	constexpr std::size_t n{maximum_allowed_args*2+1};
	io_scatter_t new_scatters[n];
	if constexpr(args_num==1)
	{
		auto res{scatter_fprint_impl(view,scatters,new_scatters,fprint_args_num_para<fprint_args_num_para_enum::one>{})};
		scatter_write(out,{new_scatters,res-new_scatters});
	}
	else
	{
		auto res{scatter_fprint_impl(view,scatters,new_scatters,
			fprint_args_num_para<args_num<11?
			fprint_args_num_para_enum::less_than_11:fprint_args_num_para_enum::other>{args_num})};
		scatter_write(out,{new_scatters,res-new_scatters});
	}
}


template<output_stream output,typename ...Args>
inline constexpr void unsafe_fprint_fallback(output out,::fast_io::freestanding::basic_string_view<typename output::char_type> view,Args ...args)
{
	using char_type = typename output::char_type;
	if constexpr((scatter_output_stream<output>&&((scatter_printable<typename output::char_type,Args>||reserve_printable<typename output::char_type,Args>||dynamic_reserve_printable<typename output::char_type,Args>)&&...)))
	{
		constexpr std::size_t n{sizeof...(Args)};
		io_scatter_t scatters[n];
		if constexpr((scatter_printable<typename output::char_type,Args>&&...))
		{
			decay::scatter_print_recursive<typename output::char_type>(scatters,args...);
			scatter_fprint_write<sizeof...(Args)>(out,view,scatters);
		}
		else if constexpr(((scatter_printable<char_type,Args>||
			reserve_printable<char_type,Args>)&&...))
		{
			constexpr std::size_t m{decay::calculate_scatter_reserve_size<char_type,Args...>()};
			char_type array[m];
			decay::scatter_print_with_reserve_recursive(array,scatters,args...);
			scatter_fprint_write<sizeof...(Args)>(out,view,scatters);
		}
		else
		{
			constexpr std::size_t m{decay::calculate_scatter_reserve_size<char_type,Args...>()};
			char_type array[m];
			local_operator_new_array_ptr<char_type> new_ptr(decay::calculate_scatter_dynamic_reserve_size<char_type>(args...));
			decay::scatter_print_with_dynamic_reserve_recursive(scatters,array,new_ptr.ptr,args...);
			scatter_fprint_write<sizeof...(Args)>(out,view,scatters);
		}
	}
	else
	{
		dynamic_io_buffer<typename output::char_type> buffer;
		auto ref{io_ref(buffer)};
		unsafe_fprint_freestanding_decay_impl(ref,view,args...);
	}
}


}

template<typename output,typename... Args>
requires ((output_stream<output>||status_output_stream<output>)&&(std::is_trivially_copyable_v<output>&&(std::is_trivially_copyable_v<Args>&&...))&&(sizeof...(Args)!=0))
inline constexpr void unsafe_fprint_freestanding_decay(output out,
	::fast_io::freestanding::basic_string_view<typename output::char_type> view,
	Args ...args)
{
	if constexpr(status_output_stream<output>)
	{
		unsafe_fprint_status_define(out,view,args...);
	}
	else if constexpr(mutex_stream<output>)
	{
		details::lock_guard lg{out};
		decltype(auto) dout{out.unlocked_handle()};
		unsafe_fprint_freestanding_decay(io_ref(dout),view,args...);
	}
	else if constexpr(buffer_output_stream<output>)
	{
		details::unsafe_fprint_freestanding_decay_impl(out,view,args...);
	}
	else
	{
		details::unsafe_fprint_fallback(out,view,args...);
	}
}

template<typename output,typename... Args>
requires ((output_stream<output>||status_output_stream<output>)&&(sizeof...(Args)!=0))
inline constexpr void unsafe_fprint(output&& out,::fast_io::freestanding::basic_string_view<typename std::remove_cvref_t<output>::char_type> view,Args&& ...args)
{
	unsafe_fprint_freestanding_decay(io_ref(out),view,io_print_forward<typename std::remove_cvref_t<output>::char_type>(io_print_alias(args))...);
}


}