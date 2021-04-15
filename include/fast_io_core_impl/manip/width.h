#pragma once

namespace fast_io
{

namespace details
{

template<std::integral char_type,typename T>
requires (reserve_printable<char_type,std::remove_cvref_t<T>>||
	dynamic_reserve_printable<char_type,std::remove_cvref_t<T>>||
	scatter_type_printable<char_type,std::remove_cvref_t<T>>)
inline constexpr std::size_t print_reserve_size_width_impl(T w,std::size_t width) noexcept
{
	using value_type = std::remove_cvref_t<T>;
	if constexpr(reserve_printable<char_type,value_type>)
	{
		constexpr std::size_t sz{print_reserve_size(io_reserve_type<char_type,value_type>)};
		if(width<sz)
			return sz;
		return width;
	}
	else if constexpr(dynamic_reserve_printable<char_type,value_type>)
	{
		std::size_t const sz{print_reserve_size(io_reserve_type<char_type,value_type>,w)};
		if(width<sz)
			return sz;
		return width;
	}
	else if constexpr(scatter_type_printable<char_type,value_type>)
	{
		basic_io_scatter_t<char_type> const scatter{print_scatter_define(print_scatter_type<char_type>,w)};
		if(width<scatter.len)
			return scatter.len;
		return width;
	}
}

template<::fast_io::manipulators::width_mode wm,::fast_io::freestanding::random_access_iterator Iter>
inline constexpr Iter print_reserve_width_fill_unchecked(Iter first,Iter last,std::size_t width,std::ptrdiff_t diff) noexcept
{
	using char_type = ::fast_io::freestanding::iter_value_t<Iter>;
	if constexpr(wm==::fast_io::manipulators::width_mode::left)
	{
		width-=diff;
		if constexpr(std::same_as<char_type,char>)
			return my_fill_n(last,width,' ');
		else if constexpr(std::same_as<char_type,wchar_t>)
			return my_fill_n(last,width,L' ');
		else if constexpr(std::same_as<char_type,char16_t>)
			return my_fill_n(last,width,u' ');
		else if constexpr(std::same_as<char_type,char32_t>)
			return my_fill_n(last,width,U' ');
		else
			return my_fill_n(last,width,u8' ');
	}
	else
	{
		auto d_last{first+width};
		if constexpr(wm==::fast_io::manipulators::width_mode::middle)
		{
			std::size_t gap{width-diff};
			std::size_t gapdv2{gap>>1};
			auto md{first+gapdv2+diff};
			auto new_d_last{my_copy_backward(first,last,md)};
			if constexpr(std::same_as<char_type,char>)
			{
				my_fill(first,new_d_last,' ');
				my_fill(md,d_last,' ');
			}
			else if constexpr(std::same_as<char_type,wchar_t>)
			{
				my_fill(first,new_d_last,L' ');
				my_fill(md,d_last,L' ');
			}
			else if constexpr(std::same_as<char_type,char16_t>)
			{
				my_fill(first,new_d_last,u' ');
				my_fill(md,d_last,u' ');
			}
			else if constexpr(std::same_as<char_type,char32_t>)
			{
				my_fill(first,new_d_last,U' ');
				my_fill(md,d_last,U' ');
			}
			else
			{
				my_fill(first,new_d_last,u8' ');
				my_fill(md,d_last,u8' ');
			}
			return d_last;
		}
		else
		{
			auto new_d_last{my_copy_backward(first,last,d_last)};
			if constexpr(std::same_as<char_type,char>)
				my_fill(first,new_d_last,' ');
			else if constexpr(std::same_as<char_type,wchar_t>)
				my_fill(first,new_d_last,L' ');
			else if constexpr(std::same_as<char_type,char16_t>)
				my_fill(first,new_d_last,u' ');
			else if constexpr(std::same_as<char_type,char32_t>)
				my_fill(first,new_d_last,U' ');
			else
				my_fill(first,new_d_last,u8' ');
			return d_last;
		}
	}
}

template<::fast_io::manipulators::width_mode wm,::fast_io::freestanding::random_access_iterator Iter>
inline constexpr Iter print_reserve_width_ch_fill_unchecked(Iter first,Iter last,std::size_t width,::fast_io::freestanding::iter_value_t<Iter> ch,std::ptrdiff_t diff) noexcept
{
	if constexpr(wm==::fast_io::manipulators::width_mode::left)
	{
		return my_fill_n(last,diff,ch);
	}
	else
	{
		auto d_last{first+width};
		if constexpr(wm==::fast_io::manipulators::width_mode::middle)
		{
			std::size_t gap{width-diff};
			std::size_t gapdv2{gap>>1};
			auto md{first+gapdv2+diff};
			auto new_d_last{my_copy_backward(first,last,md)};
			my_fill(first,new_d_last,ch);
			my_fill(md,d_last,ch);
			return d_last;
		}
		else
		{
			auto new_d_last{my_copy_backward(first,last,d_last)};
			my_fill(first,new_d_last,ch);
			return d_last;
		}
	}
}

template<::fast_io::manipulators::width_mode wm,::fast_io::freestanding::random_access_iterator Iter>
inline constexpr Iter print_reserve_width_fill(Iter first,Iter last,std::size_t width) noexcept
{
	auto diff{last-first};
	if(diff<width)
		return print_reserve_width_fill_unchecked<wm>(first,last,width,diff);
	return last;
}

template<::fast_io::manipulators::width_mode wm,::fast_io::freestanding::random_access_iterator Iter>
inline constexpr Iter print_reserve_width_ch_fill(Iter first,Iter last,std::size_t width,::fast_io::freestanding::iter_value_t<Iter> ch) noexcept
{
	auto diff{last-first};
	if(diff<width)
		return print_reserve_width_ch_fill_unchecked<wm>(first,last,width,ch,diff);
	return last;
}

template<::fast_io::manipulators::width_mode wm,::fast_io::freestanding::random_access_iterator Iter,typename T>
requires (reserve_printable<::fast_io::freestanding::iter_value_t<Iter>,std::remove_cvref_t<T>>||
	dynamic_reserve_printable<::fast_io::freestanding::iter_value_t<Iter>,std::remove_cvref_t<T>>||
	scatter_type_printable<::fast_io::freestanding::iter_value_t<Iter>,std::remove_cvref_t<T>>)
inline constexpr Iter print_reserve_define_width_impl(Iter iter,manipulators::width_t<wm,T> t) noexcept
{
	using char_type = ::fast_io::freestanding::iter_value_t<Iter>;
	using value_type = std::remove_cvref_t<T>;
	if constexpr(reserve_printable<char_type,value_type>||dynamic_reserve_printable<char_type,value_type>)
		return print_reserve_width_fill<wm>(iter,print_reserve_define(io_reserve_type<char_type,value_type>,iter,t.reference),t.width);
	else if constexpr(scatter_type_printable<char_type,value_type>)
	{
		basic_io_scatter_t<char_type> const scatter{print_scatter_define(print_scatter_type<char_type>,t.reference)};
		return print_reserve_width_fill<wm>(iter,copy_scatter(scatter,iter),t.width);
	}
}

template<::fast_io::manipulators::width_mode wm,::fast_io::freestanding::random_access_iterator Iter,typename T>
requires (reserve_printable<::fast_io::freestanding::iter_value_t<Iter>,std::remove_cvref_t<T>>||
	dynamic_reserve_printable<::fast_io::freestanding::iter_value_t<Iter>,std::remove_cvref_t<T>>||
	scatter_type_printable<::fast_io::freestanding::iter_value_t<Iter>,std::remove_cvref_t<T>>)
inline constexpr Iter print_reserve_define_width_ch_impl(Iter iter,manipulators::width_ch_t<wm,T,::fast_io::freestanding::iter_value_t<Iter>> t) noexcept
{
	using char_type = ::fast_io::freestanding::iter_value_t<Iter>;
	using value_type = std::remove_cvref_t<T>;
	if constexpr(reserve_printable<char_type,value_type>||dynamic_reserve_printable<char_type,value_type>)
		return print_reserve_width_ch_fill<wm>(iter,print_reserve_define(io_reserve_type<char_type,value_type>,iter,t.reference),t.width,t.ch);
	else if constexpr(scatter_type_printable<char_type,value_type>)
	{
		basic_io_scatter_t<char_type> const scatter{print_scatter_define(print_scatter_type<char_type>,t.reference)};
		return print_reserve_width_ch_fill<wm>(iter,copy_scatter(scatter,iter),t.width,t.ch);
	}
}

}

template<std::integral char_type,manipulators::width_mode wm,typename T>
requires ((reserve_printable<char_type,std::remove_cvref_t<T>>||
	dynamic_reserve_printable<char_type,std::remove_cvref_t<T>>||
	scatter_type_printable<char_type,std::remove_cvref_t<T>>)&&(wm!=manipulators::width_mode::internal))
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<char_type,manipulators::width_t<wm,T>>,manipulators::width_t<wm,T> w) noexcept
{
	return details::print_reserve_size_width_impl<char_type,T>(w.reference,w.width);
}

template<::fast_io::freestanding::random_access_iterator Iter,manipulators::width_mode wm,typename T>
requires ((reserve_printable<::fast_io::freestanding::iter_value_t<Iter>,std::remove_cvref_t<T>>||
	dynamic_reserve_printable<::fast_io::freestanding::iter_value_t<Iter>,std::remove_cvref_t<T>>||
	scatter_type_printable<::fast_io::freestanding::iter_value_t<Iter>,std::remove_cvref_t<T>>)&&(wm!=manipulators::width_mode::internal))
inline constexpr Iter print_reserve_define(io_reserve_type_t<::fast_io::freestanding::iter_value_t<Iter>,manipulators::width_t<wm,T>>,Iter iter,manipulators::width_t<wm,T> w) noexcept
{
	return details::print_reserve_define_width_impl<wm>(iter,w);
}


template<std::integral char_type,manipulators::width_mode wm,typename T>
requires ((reserve_printable<char_type,std::remove_cvref_t<T>>||
	dynamic_reserve_printable<char_type,std::remove_cvref_t<T>>||
	scatter_type_printable<char_type,std::remove_cvref_t<T>>)&&(wm!=manipulators::width_mode::internal))
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<char_type,manipulators::width_ch_t<wm,T,char_type>>,manipulators::width_ch_t<wm,T,char_type> w) noexcept
{
	return details::print_reserve_size_width_impl<char_type,T>(w.reference,w.width);
}

template<::fast_io::freestanding::random_access_iterator Iter,manipulators::width_mode wm,typename T>
requires ((reserve_printable<::fast_io::freestanding::iter_value_t<Iter>,std::remove_cvref_t<T>>||
	dynamic_reserve_printable<::fast_io::freestanding::iter_value_t<Iter>,std::remove_cvref_t<T>>||
	scatter_type_printable<::fast_io::freestanding::iter_value_t<Iter>,std::remove_cvref_t<T>>)&&(wm!=manipulators::width_mode::internal))
inline constexpr Iter print_reserve_define(io_reserve_type_t<::fast_io::freestanding::iter_value_t<Iter>,manipulators::width_ch_t<wm,T,::fast_io::freestanding::iter_value_t<Iter>>>,Iter iter,manipulators::width_ch_t<wm,T,::fast_io::freestanding::iter_value_t<Iter>> w) noexcept
{
	return details::print_reserve_define_width_ch_impl<wm>(iter,w);
}

}