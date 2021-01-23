#pragma once

namespace fast_io
{

namespace details
{
/*
format string vulnerablities are hard to avoid when you deal with format string

We have to put restrictions on them and mark them as unsafe.
*/

template<typename output,typename U,typename... Args>
inline constexpr void lc_print_para_at_pos(basic_lc_all<typename output::char_type> const* __restrict lc,output out,std::size_t sz,U u,Args ...args)
{
	using char_type = typename output::char_type;
	if(sz==0)
		decay::lc_print_control<false>(lc,out,u);
	if constexpr(sizeof...(Args)!=0)
	{
		if constexpr(((lc_scatter_printable<char_type,Args>||
		lc_dynamic_reserve_printable<char_type,Args>||
		lc_printable<output,Args>)||...))
			lc_print_para_at_pos(lc,out,sz-1,args...);
		else
			print_para_at_pos(out,sz-1,args...);
	}
}


template<typename output,typename... Args>
requires (sizeof...(Args)!=0)
inline constexpr void lc_unsafe_fprint_freestanding_decay_impl(basic_lc_all<typename output::char_type> const* __restrict lc,
	output out,
	std::basic_string_view<typename output::char_type> view,
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
			((details::decay::lc_print_control(lc,out,args)),...);
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
			lc_print_para_at_pos(lc,out,pos,args...);
		},fprint_args_num_para<sizeof...(Args)<11?fprint_args_num_para_enum::less_than_11:fprint_args_num_para_enum::other>{sizeof...(Args)});
	}
}



template<output_stream output,typename... Args>
inline constexpr void lc_unsafe_fprint_fallback(basic_lc_all<typename output::char_type> const* __restrict lc,output out,
	std::basic_string_view<typename output::char_type> view,Args... args)
{
	using char_type = typename output::char_type;
	if constexpr((((!lc_dynamic_reserve_printable<char_type,Args>&&
	!lc_printable<io_reference_wrapper<
		internal_temporary_buffer<char_type>>,Args>&&!lc_scatter_printable<char_type,Args>))&&...))
	{
		unsafe_fprint_freestanding_decay(out,view,args...);
	}
	else if constexpr(scatter_output_stream<output>&&
	((reserve_printable<char_type,Args>
		||dynamic_reserve_printable<char_type,Args>
		||lc_dynamic_reserve_printable<char_type,Args>
		||scatter_printable<char_type,Args>
		||lc_scatter_printable<char_type,Args>
		)&&...))
	{
		std::array<io_scatter_t,sizeof...(Args)> scatters;
		if constexpr(((scatter_printable<char_type,Args>||lc_scatter_printable<char_type,Args>)&&...))
		{
			decay::lc_scatter_print_recursive(lc,scatters.data(),args...);
			scatter_fprint_write<sizeof...(Args)>(out,view,scatters.data());
		}
		else
		{
			std::array<char_type,decay::calculate_scatter_reserve_size<char_type,Args...>()> array;
			local_operator_new_array_ptr<char_type> new_ptr(decay::calculate_lc_scatter_dynamic_reserve_size<char_type>(lc,args...));
			decay::lc_scatter_print_with_dynamic_reserve_recursive(lc,scatters.data(),array.data(),new_ptr.ptr,args...);
			scatter_fprint_write<sizeof...(Args)>(out,view,scatters.data());
		}
	}
	else
	{
		internal_temporary_buffer<typename output::char_type> buffer;
		auto ref{io_ref(buffer)};
		lc_unsafe_fprint_freestanding_decay_impl(lc,ref,view,args...);
		write(out,buffer.beg_ptr,buffer.end_ptr);
	}

}

template<output_stream output,typename... Args>
inline constexpr void lc_unsafe_fprint_status_define_further_decay(basic_lc_all<typename output::char_type> const* __restrict lc,
	output out,
	std::basic_string_view<typename output::char_type> view,
	Args... args)
{
	if constexpr(mutex_stream<output>)
	{
		lock_guard lg{out};
		decltype(auto) dout{out.unlocked_handle()};
		lc_unsafe_fprint_status_define_further_decay(lc,io_ref(dout),view,args...);
	}
	else if constexpr(buffer_output_stream<output>)
	{
		lc_unsafe_fprint_freestanding_decay_impl(lc,out,view,args...);
	}
	else
		lc_unsafe_fprint_fallback(lc,out,view,args...);
}
}

template<output_stream output,typename... Args>
requires (sizeof...(Args)!=0)
inline constexpr void unsafe_fprint_status_define(lc_imbuer<output> imb,std::basic_string_view<typename output::char_type> view,Args... args)
{
	details::lc_unsafe_fprint_status_define_further_decay(imb.all,imb.handle,view,args...);
}

}