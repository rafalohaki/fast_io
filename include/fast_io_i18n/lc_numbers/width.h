#pragma once

namespace fast_io
{

namespace details
{

template<std::integral char_type,typename T>
requires (reserve_printable<char_type,std::remove_cvref_t<T>>||
	dynamic_reserve_printable<char_type,std::remove_cvref_t<T>>||
	scatter_type_printable<char_type,std::remove_cvref_t<T>>)
inline constexpr std::size_t lc_print_reserve_size_width_impl(basic_lc_all<char_type> const* all,T w,std::size_t width) noexcept
{
	using value_type = std::remove_cvref_t<T>;
	if constexpr(lc_dynamic_reserve_printable<char_type,value_type>)
	{
		std::size_t const sz{print_reserve_size(all,w)};
		if(width<sz)
			return sz;
		return width;
	}
	else if constexpr(lc_scatter_type_printable<char_type,value_type>)
	{
		basic_io_scatter_t<char_type> const scatter{print_scatter_define(all,w)};
		if(width<scatter.len)
			return scatter.len;
		return width;
	}
}


template<::fast_io::manipulators::width_mode wm,std::random_access_iterator Iter,typename T>
requires (reserve_printable<std::iter_value_t<Iter>,std::remove_cvref_t<T>>||
	dynamic_reserve_printable<std::iter_value_t<Iter>,std::remove_cvref_t<T>>||
	scatter_type_printable<std::iter_value_t<Iter>,std::remove_cvref_t<T>>)
inline constexpr Iter lc_print_reserve_define_width_impl(basic_lc_all<std::iter_value_t<Iter>> const* __restrict all,Iter iter,manipulators::width_t<wm,T> t) noexcept
{
	using char_type = std::iter_value_t<Iter>;
	using value_type = std::remove_cvref_t<T>;
	if constexpr(lc_dynamic_reserve_printable<char_type,value_type>)
		return print_reserve_width_fill<wm>(iter,print_reserve_define(all,iter,t.reference),t.width);
	else if constexpr(scatter_type_printable<char_type,value_type>)
	{
		basic_io_scatter_t<char_type> const scatter{print_scatter_define(print_scatter_type<char_type>,t.reference)};
		return print_reserve_width_fill<wm>(iter,copy_scatter(scatter,iter),t.width);
	}
}

}

template<std::integral char_type,manipulators::width_mode wm,typename T>
requires ((lc_dynamic_reserve_printable<char_type,std::remove_cvref_t<T>>||
	lc_scatter_type_printable<char_type,std::remove_cvref_t<T>>)&&(wm!=manipulators::width_mode::internal))
inline constexpr std::size_t print_reserve_size(basic_lc_all<char_type> const* __restrict all,manipulators::width_t<wm,T> w) noexcept
{
	return details::lc_print_reserve_size_width_impl<char_type,T>(all,w.reference,w.width);
}

template<std::random_access_iterator Iter,manipulators::width_mode wm,typename T>
requires ((lc_dynamic_reserve_printable<std::iter_value_t<Iter>,std::remove_cvref_t<T>>||
	lc_scatter_type_printable<std::iter_value_t<Iter>,std::remove_cvref_t<T>>)&&(wm!=manipulators::width_mode::internal))
inline constexpr Iter print_reserve_define(basic_lc_all<std::iter_value_t<Iter>> const* __restrict all,Iter iter,manipulators::width_t<wm,T> w) noexcept
{
	return details::lc_print_reserve_define_width_impl<wm>(all,iter,w);
}


}