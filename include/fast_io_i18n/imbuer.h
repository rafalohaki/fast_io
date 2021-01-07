#pragma once

namespace fast_io
{

template<stream stm>
requires (value_based_stream<stm>&&requires(stm sm)
{
	{io_value_handle(sm)}->std::same_as<stm>;
})
struct lc_imbuer
{
	using handle_type = stm;
	using char_type = typename handle_type::char_type;
	using lc_all_type = basic_lc_all<char_type>;
	lc_all_type const* all{};
	[[no_unique_address]] handle_type handle{};
};

template<stream stm>
inline constexpr lc_imbuer<stm> io_value_handle(lc_imbuer<stm> sm) noexcept
{
	return sm;
}

template<stream stm>
requires (std::is_lvalue_reference_v<stm>||std::is_trivially_copyable_v<stm>)
inline constexpr auto imbue(basic_lc_all<typename std::remove_cvref_t<stm>::char_type> const* all,stm&& out) noexcept
{
	using char_type = typename std::remove_cvref_t<stm>::char_type;
	if constexpr(value_based_stream<stm>)
		return lc_imbuer{all,io_value_handle(out)};
	else
		return lc_imbuer{all,io_ref(out)};
}

template<stream stm>
requires (std::is_lvalue_reference_v<stm>||std::is_trivially_copyable_v<stm>)
inline constexpr auto imbue(l10n& loc,stm&& out) noexcept
{
	using char_type = typename std::remove_cvref_t<stm>::char_type;
	return imbue(get_all<char_type>(loc.loc),out);
}

template<std::integral char_type,typename T>
struct lc_reserve_type_t
{
explicit constexpr lc_reserve_type_t() noexcept = default;
};
template<std::integral char_type,typename T>
inline constexpr lc_reserve_type_t<char_type,T> lc_reserve_type{};

template<typename char_type,typename T>
concept lc_dynamic_reserve_printable = std::integral<char_type>&&
	requires(T t,basic_lc_all<char_type> const* all,char_type* ptr,std::size_t size)
{
	{print_reserve_size(lc_reserve_type<char_type,std::remove_cvref_t<T>>,all,size)}->std::convertible_to<std::size_t>;
	{print_reserve_define(lc_reserve_type<char_type,std::remove_cvref_t<T>>,all,ptr,t,size)}->std::convertible_to<char_type*>;
};

template<typename output,typename T>
concept lc_printable = output_stream<output>&&requires(basic_lc_all<typename output::char_type> const* all,output out,T t)
{
	print_define(all,out,t);
};

template<std::integral char_type,typename value_type,typename Iter>
requires dynamic_reserve_printable<char_type,std::remove_cvref_t<value_type>>
inline constexpr auto print_reserve_size(io_reserve_type_t<char_type,parameter<value_type>>,basic_lc_all<char_type> const* __restrict all,parameter<value_type> para)
{
	return print_reserve_size(io_reserve_type<char_type,std::remove_cvref_t<value_type>>,all,para.reference);
}

template<std::integral char_type,typename value_type,typename Iter>
requires dynamic_reserve_printable<char_type,std::remove_cvref_t<value_type>>
inline constexpr auto print_reserve_define(io_reserve_type_t<char_type,parameter<value_type>>,Iter begin,basic_lc_all<char_type> const* __restrict all,parameter<value_type> para,std::size_t size)
{
	return print_reserve_define(io_reserve_type<char_type,std::remove_cvref_t<value_type>>,all,begin,para.reference,size);
}

namespace details::decay
{

template<bool line,output_stream output,typename T>
inline constexpr void lc_print_control_reserve_bad_path(basic_lc_all<typename output::char_type> const* __restrict lc,output out,T t,std::size_t size)
{
	using value_type = std::remove_cvref_t<T>;
	using char_type = typename output::char_type;
	std::size_t sizep1{size};
	if constexpr(line)
		++sizep1;
	local_operator_new_array_ptr<char_type> ptr(sizep1);
	auto it{print_reserve_define(lc_reserve_type<char_type,value_type>,lc,ptr.ptr,t,size)};
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
inline constexpr void lc_print_control(basic_lc_all<typename output::char_type> const* __restrict lc,output out,T t)
{
	using char_type = typename output::char_type;
	using value_type = std::remove_cvref_t<T>;
	if constexpr(lc_dynamic_reserve_printable<char_type,value_type>)
	{
		std::size_t sz{print_reserve_size(lc_reserve_type<char_type,value_type>,lc,t)};
		if constexpr(buffer_output_stream<output>)
		{
			auto bcurr{obuffer_curr(out)};
			auto bend{obuffer_end(out)};
			std::ptrdiff_t const diff(bend-bcurr);
			std::size_t sizep1{sz};
			if constexpr(line)
			{
				++sizep1;
			}
			if(static_cast<std::ptrdiff_t>(sizep1)<diff)[[likely]]
			{
				//To check whether this affects performance.
				auto it{print_reserve_define(lc_reserve_type<char_type,value_type>,lc,bcurr,t,sz)};
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
				lc_print_control_reserve_bad_path<line>(lc,out,t);
		}
		else
			lc_print_control_reserve_bad_path<line>(lc,out,t);
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

template<output_stream output,typename T,typename... Args>
inline constexpr void lc_print_controls_line(basic_lc_all<typename output::char_type> const* __restrict lc,output out,T t,Args... args)
{
	if constexpr(sizeof...(Args)==0)
	{
		lc_print_control<true>(lc,out,t);
	}
	else
	{
		lc_print_control<false>(lc,out,t);
		lc_print_controls_line(lc,out,args...);
	}
}

template<bool ln,output_stream output,typename... Args>
inline constexpr void lc_print_fallback(basic_lc_all<typename output::char_type> const* __restrict lc,output out,Args... args)
{
	if constexpr((!lc_dynamic_reserve_printable<char_type,Args>&&
	(!lc_printable<io_reference_wrapper<
		internal_temporary_buffer<typename output::char_type>>,char_type>))&&...)
	{
		print_fallback<line>(out,args...);
	}
	else if constexpr(scatter_output_stream<output>&&
	((reserve_printable<char_type,Args>
		||dynamic_reserve_printable<char_type,Args>
		||lc_dynamic_reserve_printable<char_type,Args>
		||scatter_printable<char_type,Args>
		)&&...))
	{
		std::array<io_scatter_t,sizeof...(Args)+static_cast<std::size_t>(ln)> scatters;
		std::array<char_type,calculate_scatter_reserve_size<char_type,Args...>()> array;
		scatter_print_with_reserve_recursive(array.data(),scatters.data(),args...);
		


		if constexpr(ln)
		{
			if constexpr(std::same_as<char_type,char>)
			{
				char_type ch('\n');
				scatters.back()={std::addressof(ch),sizeof(ch)};
				scatter_write(out,scatters);
			}
			else if constexpr(std::same_as<char_type,wchar_t>)
			{
				char_type ch(L'\n');
				scatters.back()={std::addressof(ch),sizeof(ch)};
				scatter_write(out,scatters);
			}
			else
			{
				char_type ch(u8'\n');
				scatters.back()={std::addressof(ch),sizeof(ch)};
				scatter_write(out,scatters);
			}
		}
		else
			scatter_write(out,scatters);
	}
	else
	{
		internal_temporary_buffer<typename output::char_type> buffer;
		auto ref{io_ref(buffer)};
		if constexpr(!ln)
			(lc_print_control<false>(lc,ref,args),...);
		else
			lc_print_controls_line(lc,ref,args...);
		write(out,buffer.beg_ptr,buffer.end_ptr);
	}
}

template<bool ln,output_stream output,typename... Args>
inline constexpr void lc_print_status_define_further_decay(basic_lc_all<typename output::char_type> const* __restrict lc,output out,Args... args)
{
	using char_type = typename output::char_type;
	if constexpr(mutex_stream<output>)
	{
		lock_guard lg{out};
		decltype(auto) dout{out.unlocked_handle()};
		details::lc_print_status_define_further_decay<ln>(lc,io_ref(dout),args...);
	}
	else if constexpr(
		(buffer_output_stream<output>||((!ln)&&sizeof...(Args)==1))
		&&((printable<output,Args>||reserve_printable<char_type,Args>
		||dynamic_reserve_printable<char_type,Args>
		||lc_printable<output,Args>||lc_dynamic_reserve_printable<char_type,Args>
		||scatter_type_printable<char_type,Args>
		)&&...)
		)
	{
		if constexpr(!ln)
			(lc_print_control<false>(lc,out,args),...);
		else
			lc_print_controls_line(lc,out,args...);
	}
	else
		lc_print_fallback<ln>(lc,out,args...);
}

}

template<output_stream output,typename... Args>
inline constexpr void print_status_define(lc_imbuer<output> imb,Args... args)
{
	details::decay::lc_print_status_define_further_decay<false>(imb.all,imb.handle,args...);
}

template<output_stream output,typename... Args>
inline constexpr void println_status_define(lc_imbuer<output> imb,Args... args)
{
	details::decay::lc_print_status_define_further_decay<true>(imb.all,imb.handle,args...);
}

}