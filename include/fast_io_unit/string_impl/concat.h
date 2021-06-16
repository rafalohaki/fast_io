#pragma once

namespace fast_io
{
//potential constexpr in the future if std::string can be constexpr



namespace details
{

template <typename T>
struct is_std_string
{
inline static constexpr bool value{};
};
//https://stackoverflow.com/questions/51862465/check-if-a-type-is-stdbasic-stringt-in-compile-time-in-c
template <typename T, typename Traits, typename Alloc>
struct is_std_string<std::basic_string<T, Traits, Alloc>>
{
inline static constexpr bool value{true};
};

template<bool ln,typename T,typename U,typename... Args>
inline constexpr bool test_one() noexcept
{
	using no_cvref=std::remove_cvref_t<std::invoke_result_t<decltype(io_print_alias<typename T::value_type,U>),U&&>>;
	if constexpr(reserve_printable<typename T::value_type,no_cvref>)
	{
		constexpr auto size{print_reserve_size(io_reserve_type<typename T::value_type,no_cvref>)+static_cast<std::size_t>(ln)};
		return string_hack::local_capacity<T>()<size;
	}
	else
		return false;
}

template<typename T,bool ln,typename U>
inline constexpr T deal_with_one(U t)
{
	using value_type = typename T::value_type;
	using no_cvref = std::remove_cvref_t<U>;
	constexpr auto size{print_reserve_size(io_reserve_type<typename T::value_type,no_cvref>)+static_cast<std::size_t>(ln)};
	if constexpr(!ln&&std::same_as<no_cvref,::fast_io::manipulators::chvw_t<typename T::value_type>>)
		return T(1,t.reference);
	else
	{
		::fast_io::freestanding::array<value_type,size> array;
		if constexpr(ln)
		{
			auto p {print_reserve_define(io_reserve_type<typename T::value_type,no_cvref>,array.data(),t)};
			if constexpr(details::exec_charset_is_ebcdic<value_type>())
				*p=0x25;
			else
				*p=u8'\n';
			return T(array.data(),++p);
		}
		else
		{
			return T(array.data(),
			print_reserve_define(io_reserve_type<typename T::value_type,no_cvref>,array.data(),t));
		}
	}
}

template<typename T,typename U,typename... Args>
inline constexpr bool test_first_is_string_rvalue_reference()
{
	if constexpr(std::is_rvalue_reference_v<U>)
	{
		using no_cvref_t = std::remove_cvref_t<U>;
		if constexpr(is_std_string<no_cvref_t>::value&&std::same_as<no_cvref_t,T>)
			return true;
	}
	return false;
}



template<std::integral char_type,typename T>
inline constexpr std::size_t scatter_concat_recursive(basic_io_scatter_t<char_type>* arr,T t)
{
	*arr=print_scatter_define(print_scatter_type<char_type>,t);
	return arr->len;
}

template<std::integral char_type,typename T,typename... Args>
inline constexpr std::size_t scatter_concat_recursive(basic_io_scatter_t<char_type>* arr,T t, Args ...args)
{
	*arr=print_scatter_define(print_scatter_type<char_type>,t);
	return scatter_concat_recursive<char_type>(arr+1,args...)+arr->len;
}


template<std::integral char_type,typename T>
inline constexpr std::size_t scatter_concat_with_reserve_recursive_unit(char_type*& start_ptr,
		basic_io_scatter_t<char_type>* arr,T t)
{
	using real_type = std::remove_cvref_t<T>;
	if constexpr(scatter_type_printable<char_type,real_type>)
		*arr=print_scatter_define(print_scatter_type<char_type>,t);
	else
	{
		auto end_ptr = print_reserve_define(io_reserve_type<char_type,real_type>,start_ptr,t);
		*arr={start_ptr,static_cast<std::size_t>(end_ptr-start_ptr)};
		start_ptr=end_ptr;
	}
	return arr->len;
}

template<std::integral char_type,typename T>
inline constexpr std::size_t scatter_concat_with_reserve_recursive(char_type* ptr,
		basic_io_scatter_t<char_type>* arr,T t)
{
	return scatter_concat_with_reserve_recursive_unit(ptr,arr,t);
}

template<std::integral char_type,typename T,typename... Args>
inline constexpr std::size_t scatter_concat_with_reserve_recursive(char_type* ptr,
	basic_io_scatter_t<char_type>* arr,T t, Args ...args)
{
	std::size_t const res{scatter_concat_with_reserve_recursive_unit(ptr,arr,t)};
	return res+scatter_concat_with_reserve_recursive(ptr,arr+1,args...);
}

template<bool line,typename char_type,std::size_t arg_number>
inline constexpr auto deal_with_scatters(::fast_io::freestanding::array<basic_io_scatter_t<char_type>,arg_number>& scatters,std::size_t sz)
{
	std::basic_string<char_type> str;
	if constexpr(line)
		str.reserve(sz+1);
	else
		str.reserve(sz);
	basic_ostring_ref<char_type> oref{&str};
	auto it(str.data());
	for(auto const& e : scatters)
		it=details::non_overlapped_copy_n(e.base,e.len,it);
	if constexpr(line)
	{
		if constexpr(details::exec_charset_is_ebcdic<char_type>())
			*it=0x25;
		else
			*it=u8'\n';
		++it;
	}
	obuffer_set_curr(oref,it);
	return str;
}

template<std::integral char_type,typename T,typename... Args>
inline constexpr char_type* concat_with_reserve_recursive(char_type* ptr,T t,Args... args)
{
	using real_type = std::remove_cvref_t<T>;
	ptr=print_reserve_define(io_reserve_type<char_type,real_type>,ptr,t);
	if constexpr(sizeof...(Args)!=0)
		ptr=concat_with_reserve_recursive(ptr,args...);
	return ptr;
}

template<bool line,typename char_type,std::size_t arg_number>
inline constexpr void deal_with_scatters_string(std::basic_string<char_type>& str,::fast_io::freestanding::array<basic_io_scatter_t<char_type>,arg_number>& scatters,std::size_t sz)
{
	if constexpr(line)
		str.reserve(sz+1+str.size());
	else
		str.reserve(sz+str.size());
	basic_ostring_ref<char_type> oref{&str};
	auto it(str.data());
	for(auto const& e : scatters)
		it=details::non_overlapped_copy_n(e.base,e.len,it);
	if constexpr(line)
	{
		if constexpr(details::exec_charset_is_ebcdic<char_type>())
			*it=0x25;
		else
			*it=u8'\n';
		++it;
	}
	obuffer_set_curr(oref,it);
}

template<bool line,std::integral char_type,typename ...Args>
inline constexpr auto concat_fallback(Args ...args)
{
	if constexpr(((scatter_type_printable<char_type,Args>||reserve_printable<char_type,Args>)&&...))
	{
		if constexpr(((reserve_printable<char_type,Args>)&&...))
		{
			::fast_io::freestanding::array<char_type,decay::calculate_scatter_reserve_size<char_type,Args...>()+static_cast<std::size_t>(line)> array;
			auto ptr{concat_with_reserve_recursive<char_type>(array.data(),args...)};
			if constexpr(line)
			{
				if constexpr(details::exec_charset_is_ebcdic<char_type>())
					*ptr=0x25;
				else
					*ptr=u8'\n';
				++ptr;
			}
			return std::basic_string<char_type>(array.data(),ptr);
		}
		else
		{
			::fast_io::freestanding::array<basic_io_scatter_t<char_type>,sizeof...(Args)> scatters;
			if constexpr(((scatter_type_printable<char_type,Args>)&&...))
				return deal_with_scatters<line,char_type>(scatters,scatter_concat_recursive<char_type>(
					scatters.data(),args...));
			else
			{
				constexpr std::size_t sca_sz{decay::calculate_scatter_reserve_size<char_type,Args...>()};
				::fast_io::freestanding::array<char_type,sca_sz> array;
				return deal_with_scatters<line,char_type>(scatters,scatter_concat_with_reserve_recursive<char_type>(
					array.data(),scatters.data(),args...));
			}
		}
	}
	else
	{
		std::basic_string<char_type> bas;
		basic_ostring_ref<char_type> itb{&bas};
		if constexpr(line)
			println_freestanding_decay(io_ref(itb),args...);
		else
			print_freestanding_decay(io_ref(itb),args...);
		return bas;
	}
}


template<bool line,std::integral char_type,typename U,typename... Args>
inline constexpr decltype(auto) deal_with_first_is_string_rvalue_reference_decay(U&& u,Args ...args)
{
	if constexpr(((scatter_type_printable<char_type,Args>||reserve_printable<char_type,Args>)&&...))
	{
		::fast_io::freestanding::array<basic_io_scatter_t<char_type>,sizeof...(Args)> scatters;
		if constexpr(((scatter_type_printable<char_type,Args>)&&...))
			deal_with_scatters_string<line>(u,scatters,scatter_concat_recursive<char_type>(
				scatters.data(),args...));
		else
		{
			constexpr std::size_t sca_sz{decay::calculate_scatter_reserve_size<char_type,Args...>()};
			::fast_io::freestanding::array<char_type,sca_sz> array;
			deal_with_scatters_string<line>(u,scatters,scatter_concat_with_reserve_recursive<char_type>(
				array.data(),scatters.data(),args...));
		}
	}
	else
	{
		basic_ostring_ref<char_type> t{&u};
		{
			if constexpr(line)
				println_freestanding_decay(io_ref(t),args...);
			else
				print_freestanding_decay(io_ref(t),args...);
		}
	}
}

template<bool line,std::integral char_type,typename U,typename... Args>
inline constexpr decltype(auto) deal_with_first_is_string_rvalue_reference(U&& u,Args&& ...args)
{
	if constexpr(sizeof...(Args)==0)
	{
		if constexpr(line)
		{
			if constexpr(details::exec_charset_is_ebcdic<char_type>())
				u.push_back(0x25);
			else
				u.push_back(u8'\n');
		}
	}
	else
	{
		deal_with_first_is_string_rvalue_reference_decay<line,char_type>(std::forward<U>(u),io_forward(io_print_alias<char_type>(args))...);
	}
	return std::forward<U>(u);
}

}

template<typename T=std::string,typename... Args>
inline constexpr T concat(Args&& ...args)
{
	if constexpr(sizeof...(Args)==0)
		return {};
	else if constexpr(sizeof...(Args)==1&&details::test_one<false,T,Args...>())
		return details::deal_with_one<T,false>(io_forward(io_print_alias<typename T::value_type>(args))...);
	else
	{
		if constexpr(details::test_first_is_string_rvalue_reference<T,Args&&...>())
			return details::deal_with_first_is_string_rvalue_reference<false,typename T::value_type>(std::forward<Args>(args)...);
		else
			return details::concat_fallback<false,typename T::value_type>(io_forward(io_print_alias<typename T::value_type>(args))...);
	}
}

template<typename T=std::string,typename... Args>
inline constexpr T concatln(Args&& ...args)
{
	if constexpr(sizeof...(Args)==0)
	{
		if constexpr(details::exec_charset_is_ebcdic<typename T::value_type>())
			return T(1,0x25);
		else
			return T(1,u8'\n');
	}
	else if constexpr(sizeof...(Args)==1&&details::test_one<true,T,Args...>())
	{
		return details::deal_with_one<T,true>(io_forward(io_print_alias<typename T::value_type>(args))...);
	}
	else
	{
		if constexpr(details::test_first_is_string_rvalue_reference<T,Args&&...>())
			return details::deal_with_first_is_string_rvalue_reference<true,typename T::value_type>(std::forward<Args>(args)...);
		else
			return details::concat_fallback<true,typename T::value_type>(io_forward(io_print_alias<typename T::value_type>(args))...);
	}
}

template<typename T,typename... Args>
inline
#if __cpp_lib_constexpr_string >= 201907L
constexpr
#endif
void in_place_to(T& t,Args&& ...args)
{
	std::string str;
	ostring_ref ref{&str};
	print_freestanding(ref,std::forward<Args>(args)...);
	basic_istring_view<char> is(str);
	if(!scan_freestanding(is,t))
		throw_parse_code(parse_code::partial);
	//No Decoration?
}

template<typename... Args>
inline
#if __cpp_lib_constexpr_string >= 201907L
constexpr
#endif
void in_place_to(std::string& t,Args&& ...args)
{
	t.clear();
	ostring_ref ref{&t};
	print_freestanding(ref,std::forward<Args>(args)...);
}

template<typename T,typename... Args>
inline constexpr T to(Args&& ...args)
{
	if constexpr(details::is_std_string<T>::value)
	{
		return fast_io::concat<T>(std::forward<Args>(args)...);
	}
	else
	{
		T t;
		in_place_to(t,std::forward<Args>(args)...);
		return t;
	}
}


}
