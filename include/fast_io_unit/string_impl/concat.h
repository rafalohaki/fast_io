#pragma once

namespace fast_io
{
//potential constexpr in the future if std::string can be constexpr

namespace details::decay
{

template<bool line,typename ptr_type,typename T,typename... Args>
inline constexpr ptr_type print_reserve_define_chain_impl(ptr_type p,T t,Args ...args)
{
	using char_type = ::fast_io::freestanding::iter_value_t<ptr_type>;
	if constexpr(sizeof...(Args)==0)
	{
		p=print_reserve_define(io_reserve_type<char_type,std::remove_cvref_t<T>>,p,t);
		if constexpr(line)
		{
			*p=sign_ch<u8'\n',char_type>;
			++p;
		}
		return p;
	}
	else
		return print_reserve_define_chain_impl<line>(print_reserve_define(io_reserve_type<char_type,std::remove_cvref_t<T>>,p,t),args...);
}

template<std::integral char_type,typename T,typename... Args>
inline constexpr std::size_t calculate_scatter_dynamic_reserve_size_with_scatter([[maybe_unused]] T t,Args... args)
{
	if constexpr(dynamic_reserve_printable<char_type,T>)
	{
		std::size_t res{print_reserve_size(io_reserve_type<char_type,T>,t)};
		if constexpr(sizeof...(Args)==0)
			return res;
		else
			return ::fast_io::details::intrinsics::add_or_overflow_die(res,calculate_scatter_dynamic_reserve_size_with_scatter<char_type>(args...));
	}
	else if constexpr(scatter_type_printable<char_type,T>)
	{
		std::size_t res{print_scatter_define(print_scatter_type<char_type>,t).len};
		if constexpr(sizeof...(Args)==0)
			return res;
		else
			return ::fast_io::details::intrinsics::add_or_overflow_die(res,calculate_scatter_dynamic_reserve_size_with_scatter<char_type>(args...));
	}
	else
	{
		if constexpr(sizeof...(Args)==0)
			return 0;
		else
			return calculate_scatter_dynamic_reserve_size_with_scatter<char_type>(args...);
	}
}

template<bool line,typename ptr_type,typename T,typename... Args>
inline constexpr ptr_type print_reserve_define_chain_scatter_impl(ptr_type p,T t,Args ...args)
{
	using char_type = ::fast_io::freestanding::iter_value_t<ptr_type>;
	if constexpr(dynamic_reserve_printable<char_type,T>||reserve_printable<char_type,T>)
		p = print_reserve_define(io_reserve_type<char_type,std::remove_cvref_t<T>>,p,t);
	else
	{
		basic_io_scatter_t<char_type> sc{print_scatter_define(print_scatter_type<char_type>,t)};
		p = non_overlapped_copy_n(sc.base,sc.len,p);
	}
	if constexpr(sizeof...(Args)==0)
	{
		if constexpr(line)
		{
			*p=sign_ch<u8'\n',char_type>;
			++p;
		}
		return p;
	}
	else
		return print_reserve_define_chain_scatter_impl<line>(p,args...);
}

template<std::integral ch_type,typename T>
inline constexpr basic_io_scatter_t<ch_type> print_scatter_define_extract_one(T t)
{
	return print_scatter_define<ch_type>(print_scatter_type<ch_type>,t);
}

template<bool line,typename T,typename... Args>
inline constexpr T basic_concat_decay_impl(Args ...args)
{
	using ch_type = typename T::value_type;
	if constexpr(sizeof...(Args)==0)
	{
		if constexpr(line)
		{
			return T(1,sign_ch<u8'\n',ch_type>);
		}
		else
		{
			return {};
		}
	}
	else if constexpr(((reserve_printable<ch_type,Args>||scatter_type_printable<ch_type,Args>||dynamic_reserve_printable<ch_type,Args>)&&...))
	{
		constexpr std::size_t sz{calculate_scatter_reserve_size<ch_type,Args...>()};
		if constexpr(line)
			static_assert(sz!=SIZE_MAX,"overflow\n");
		constexpr std::size_t sz_with_line{sz+static_cast<std::size_t>(line)};
		if constexpr((reserve_printable<ch_type,Args>&&...))
		{
			T str;
#ifndef _LIBCPP_VERSION
//libc++ uses big/small sso implementation, the local_cap might not make sense. avoid it
			constexpr std::size_t local_cap{string_hack::local_capacity<T>()};
			if constexpr(sz<=local_cap)
#endif
				str.reserve(sz_with_line);
			set_basic_string_ptr(str,print_reserve_define_chain_impl<line>(str.data(),args...));
			return str;
		}
		else
		{
			if constexpr((!line)&&sizeof...(args)==1&&(scatter_printable<ch_type,Args>&&...))
			{
				basic_io_scatter_t<ch_type> scatter{print_scatter_define_extract_one<ch_type>(scatter)};
				return T(scatter.base,scatter.len);
			}
			else
			{
				std::size_t total_size{::fast_io::details::intrinsics::add_or_overflow_die(sz_with_line,calculate_scatter_dynamic_reserve_size_with_scatter<ch_type>(args...))};
				T str;
				str.reserve(total_size);
				set_basic_string_ptr(str,print_reserve_define_chain_scatter_impl<line>(str.data(),args...));
				return str;
			}
		}
	}
	else
	{
		T str;
		basic_ostring_ref<ch_type,typename T::traits_type,typename T::allocator_type> ref{__builtin_addressof(str)};
		print_freestanding_decay_normal(str,args...);
		return str;
	}
}
}

template<typename T,typename ...Args>
inline constexpr T basic_concat_decay(Args ...args)
{
	return details::decay::basic_concat_decay_impl<false,T>(args...);
}

template<typename T,typename ...Args>
inline constexpr T basic_concat(Args&& ...args)
{
	using char_type = typename T::value_type;
	return details::decay::basic_concat_decay_impl<false,T>(io_print_forward<char_type>(io_print_alias(args))...);
}

template<typename ...Args>
inline
#if __cpp_lib_constexpr_string >= 201907L
constexpr
#endif
std::string concat(Args&& ...args)
{
	return details::decay::basic_concat_decay_impl<false,std::string>(io_print_forward<char>(io_print_alias(args))...);
}

template<typename ...Args>
inline
#if __cpp_lib_constexpr_string >= 201907L
constexpr
#endif
std::wstring wconcat(Args&& ...args)
{
	return details::decay::basic_concat_decay_impl<false,std::wstring>(io_print_forward<wchar_t>(io_print_alias(args))...);
}

template<typename ...Args>
inline
#if __cpp_lib_constexpr_string >= 201907L
constexpr
#endif
std::u8string u8concat(Args&& ...args)
{
	return details::decay::basic_concat_decay_impl<false,std::u8string>(io_print_forward<char8_t>(io_print_alias(args))...);
}

template<typename ...Args>
inline
#if __cpp_lib_constexpr_string >= 201907L
constexpr
#endif
std::u16string u16concat(Args&& ...args)
{
	return details::decay::basic_concat_decay_impl<false,std::u16string>(io_print_forward<char16_t>(io_print_alias(args))...);
}

template<typename ...Args>
inline
#if __cpp_lib_constexpr_string >= 201907L
constexpr
#endif
std::u32string u32concat(Args&& ...args)
{
	return details::decay::basic_concat_decay_impl<false,std::u32string>(io_print_forward<char32_t>(io_print_alias(args))...);
}

template<typename T,typename ...Args>
inline constexpr T basic_concatln(Args&& ...args)
{
	using char_type = typename T::value_type;
	return details::decay::basic_concat_decay_impl<true,T>(io_print_forward<char_type>(io_print_alias(args))...);
}

template<typename ...Args>
inline
#if __cpp_lib_constexpr_string >= 201907L
constexpr
#endif
std::string concatln(Args&& ...args)
{
	return details::decay::basic_concat_decay_impl<true,std::string>(io_print_forward<char>(io_print_alias(args))...);
}

template<typename ...Args>
inline
#if __cpp_lib_constexpr_string >= 201907L
constexpr
#endif
std::wstring wconcatln(Args&& ...args)
{
	return details::decay::basic_concat_decay_impl<true,std::wstring>(io_print_forward<wchar_t>(io_print_alias(args))...);
}

template<typename ...Args>
inline
#if __cpp_lib_constexpr_string >= 201907L
constexpr
#endif
std::u8string u8concatln(Args&& ...args)
{
	return details::decay::basic_concat_decay_impl<true,std::u8string>(io_print_forward<char8_t>(io_print_alias(args))...);
}

template<typename ...Args>
inline
#if __cpp_lib_constexpr_string >= 201907L
constexpr
#endif
std::u16string u16concatln(Args&& ...args)
{
	return details::decay::basic_concat_decay_impl<true,std::u16string>(io_print_forward<char16_t>(io_print_alias(args))...);
}

template<typename ...Args>
inline
#if __cpp_lib_constexpr_string >= 201907L
constexpr
#endif
std::u32string u32concatln(Args&& ...args)
{
	return details::decay::basic_concat_decay_impl<true,std::u32string>(io_print_forward<char32_t>(io_print_alias(args))...);
}

}
