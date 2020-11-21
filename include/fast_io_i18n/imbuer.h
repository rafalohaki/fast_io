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
inline constexpr auto imbue(i18n_locale& loc,stm&& out) noexcept
{
	using char_type = std::remove_cvref_t<stm>::char_type;
	return imbue(get_all<char_type>(loc.loc),out);
}

template<std::integral char_type,typename T>
struct lc_reserve_type_t
{
};
template<std::integral char_type,typename T>
inline constexpr lc_reserve_type_t<char_type,T> lc_reserve_type{};

template<typename char_type,typename T>
concept lc_reserve_printable = std::integral<char_type>&&
	requires(T t,basic_lc_all<char_type> const* all,char_type* ptr)
{
	{print_reserve_size(lc_reserve_type<char_type,std::remove_cvref_t<T>>)}->std::convertible_to<std::size_t>;
	{print_reserve_define(lc_reserve_type<char_type,std::remove_cvref_t<T>>,all,ptr,t)}->std::convertible_to<char_type*>;
};

template<typename output,typename T>
concept lc_printable = output_stream<output>&&requires(basic_lc_all<typename output::char_type> const* all,output out,T t)
{
	print_define(all,out,t);
};
#if 0
namespace details
{

template<bool ln,output_stream output,typename T>
requires (std::is_trivially_copyable_v<output>&&std::is_trivially_copyable_v<T>)
inline constexpr void lc_print_control(basic_lc_all<typename output::char_type> const* lc,output out,T t)
{
	using char_type = typename output::char_type;
	using value_type = std::remove_cvref_t<T>;	
}

template<bool ln,output_stream output,typename... Args>
inline constexpr void lc_print_impl(basic_lc_all<typename output::char_type> const* lc,output out,Args... args)
{
	if constexpr((!(lc_printable<output,Args>||lc_reserve_printable<typename output::char_type,Args>))&&...))
	{
		
	}
}

template<bool ln,output_stream output,typename... Args>
inline constexpr void lc_print_status_define_further_decay(basic_lc_all<typename output::char_type> const* lc,output out,Args... args)
{
	if constexpr(mutex_stream<output>)
	{
		lock_guard lg{out};
		decltype(auto) dout{out.unlocked_handle()};
		details::lc_print_status_define_further_decay<ln>(lc,io_ref(dout),args...);
	}
	else if constexpr(
		((printable<output,Args>||reserve_printable<typename output::char_type,Args>
		||lc_printable<output,Args>||lc_reserve_printable<typename output::char_type,Args>
		)&&...)&&
		(sizeof...(Args)==1||buffer_output_stream<output>))
	{
		if constexpr(sizeof...(Args)==1||(!maybe_buffer_output_stream<output>))
		{
			(lc_print_control(lc,out,args),...);
		}
		else
		{
			if constexpr(sizeof...(Args)!=1)
			{
				if(!obuffer_is_active(out))[[unlikely]]
				{
					lc_print_fallback<false>(lc,out,args...);
					return;
				}
			}
			(lc_print_control(lc,out,args),...);
		}
	}
	else
		lc_print_fallback(lc,out,args...);
}

}

template<output_stream output,typename... Args>
inline constexpr void print_status_define(lc_imbuer<output> imb,Args... args)
{
	details::lc_print_status_define_further_decay<false>(imb.all,imb.handle,args...);
}

template<output_stream output,typename... Args>
inline constexpr void println_status_define(lc_imbuer<output> imb,Args... args)
{
	details::lc_print_status_define_further_decay<true>(imb.all,imb.handle,args...);
}
#endif
}