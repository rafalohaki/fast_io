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
			return '0';
		else if constexpr(ch==u8'{')
			return L'{';
		else
			return L'}';
	}
	else
		return static_cast<char_type>(ch);
}

#if 0

template<output_stream output,typename ...Args>
inline constexpr void unsafe_rt_fprint_fallback(output out,Args ...args)
{
	using char_type = typename output::char_type;
	if constexpr((scatter_output_stream<output>&&((scatter_printable<typename output::char_type,Args>||reserve_printable<typename output::char_type,Args>||dynamic_reserve_printable<typename output::char_type,Args>)&&...)))
	{
		std::array<io_scatter_t,(sizeof...(Args))+static_cast<std::size_t>(line)> scatters;
		if constexpr((scatter_printable<typename output::char_type,Args>&&...))
		{
			scatter_print_recursive<typename output::char_type>(scatters.data(),args...);
			scatter_write(out,scatters);
		}
		else if constexpr(((scatter_printable<char_type,Args>||
			reserve_printable<char_type,Args>)&&...))
		{
			std::array<char_type,calculate_scatter_reserve_size<char_type,Args...>()> array;
			scatter_print_with_reserve_recursive(array.data(),scatters.data(),args...);
			scatter_write(out,scatters);
		}
		else
		{
			std::array<char_type,calculate_scatter_reserve_size<char_type,Args...>()> array;
			local_operator_new_array_ptr<char_type> new_ptr(calculate_scatter_dynamic_reserve_size<char_type>(args...));
			scatter_print_with_dynamic_reserve_recursive(scatters.data(),array.data(),new_ptr.ptr,args...);
			scatter_write(out,scatters);
		}
	}
	else
	{
		using internal_buffer_type = internal_temporary_buffer<typename output::char_type>;
		internal_buffer_type buffer;
		auto ref{io_ref(buffer)};
		print_controls_line<line,pci>(ref,args...);
		write(out,buffer.beg_ptr,buffer.end_ptr);
	}
}

#endif

}

template<typename output,typename... Args>
requires ((output_stream<output>||status_output_stream<output>)&&(std::is_trivially_copyable_v<output>&&(std::is_trivially_copyable_v<Args>&&...))&&(sizeof...(Args)!=0))
inline constexpr void unsafe_rt_fprint_freestanding_decay(output out,
	std::basic_string_view<typename output::char_type> view,
	Args ...args)
{
	if constexpr(status_output_stream<output>)
	{
		unsafe_rt_fprint_status_define(out,view,args...);
	}
	else if constexpr(mutex_stream<output>)
	{
		details::lock_guard lg{out};
		decltype(auto) dout{out.unlocked_handle()};
		unsafe_rt_fprint_freestanding_decay(io_ref(dout),view,args...);
	}
	else if constexpr(buffer_output_stream<output>)
	{
		using char_type = typename output::char_type;
		using unsigned_char_type = std::make_unsigned_t<char_type>;
		auto it{view.data()};
		auto ed{view.data()+view.size()};
		constexpr std::size_t maximum_allowed_args{std::max(static_cast<std::size_t>(100),static_cast<std::size_t>(sizeof...(Args)*static_cast<std::size_t>(10)))};
		std::size_t current_parameter{};
		std::size_t swap_parameters{};
		constexpr auto left_bracket{details::get_bracket<char_type,u8'{'>()};
		constexpr auto right_bracket{details::get_bracket<char_type,u8'}'>()};
		constexpr unsigned_char_type zeroch(static_cast<unsigned_char_type>(details::get_bracket<char_type,u8'0'>()));
		for(;it!=ed;)
		{
			auto next_it{fast_io::details::my_trivial_find(it,ed,left_bracket)};
			std::ptrdiff_t diff{ed-next_it};
			if(diff<2)
				break;
			auto next_ch{next_it[1]};
			if(next_ch==left_bracket)
			{
				write(out,it,next_it+1);
				it=next_it+2;
				continue;
			}
			else if(next_ch==right_bracket)
			{
				if(current_parameter==sizeof...(Args))
					write(out,it,next_it+2);
				else
				{
					write(out,it,next_it);
					if constexpr(sizeof...(Args)==1)
						((details::decay::print_control(out,args)),...);
					else
					{
						details::print_para_at_pos(out,current_parameter,args...);
					}
					++current_parameter;
				}
				it=next_it+2;
				continue;
			}
			if(diff==2||swap_parameters==maximum_allowed_args)
				break;
			if constexpr(sizeof...(Args)==1)
			{
				if((next_ch!=zeroch)|(next_it[2]!=right_bracket))
					break;
				write(out,it,next_it);
				((details::decay::print_control(out,args)),...);
				++swap_parameters;
				it=next_it+3;
			}
			else if constexpr(sizeof...(Args)<11)
			{
				constexpr unsigned_char_type max_sz{static_cast<unsigned_char_type>(sizeof...(Args))};
				if(next_it[2]!=right_bracket)
					break;
				unsigned_char_type rn(static_cast<unsigned_char_type>(next_ch)-zeroch);
				if(rn<max_sz)[[likely]]
				{
					write(out,it,next_it);
					details::print_para_at_pos(out,static_cast<std::size_t>(rn),args...);
					++swap_parameters;
					it=next_it+3;
				}
				else
					break;
			}
			else
			{
				std::size_t n;
				auto res{scan_context_define(scan_context<true>,next_it+1,ed,parameter<std::size_t&>(n))};
				if((res.code!=std::errc{})|(res.iter==ed||*res.iter!=right_bracket))
					break;
				if(sizeof...(Args)<=n)
					break;
				write(out,it,next_it);
				details::print_para_at_pos(out,static_cast<std::size_t>(n),args...);
				++swap_parameters;
				it=res.iter+1;
			}
		}
		if(it!=ed)
			write(out,it,ed);
	}
	else
	{

	}
}

template<typename output,typename... Args>
requires ((output_stream<output>||status_output_stream<output>)&&(sizeof...(Args)!=0))
inline constexpr void unsafe_rt_fprint(output&& out,std::basic_string_view<typename std::remove_cvref_t<output>::char_type> view,Args&& ...args)
{
	unsafe_rt_fprint_freestanding_decay(io_ref(out),view,io_forward(io_print_alias<typename std::remove_cvref_t<output>::char_type>(args))...);
}


}