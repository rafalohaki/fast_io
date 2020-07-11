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
inline constexpr bool test_one()
{
	using no_cvref = std::remove_cvref_t<U>;
	if constexpr(!reserve_printable<no_cvref>)
		return false;
	else
	{
		constexpr auto size{print_reserve_size(print_reserve_type<no_cvref>)+static_cast<std::size_t>(ln)};
		return string_hack::local_capacity<T>()<size;
	}
}

template<typename T,bool ln,typename U>
inline constexpr T deal_with_one(U&& t)
{
	using value_type = typename T::value_type;
	using no_cvref = std::remove_cvref_t<U>;

	constexpr auto size{print_reserve_size(print_reserve_type<no_cvref>)+static_cast<std::size_t>(ln)};
	std::array<value_type,size> array;
	if constexpr(ln)
	{
		auto p {print_reserve_define(print_reserve_type<no_cvref>,array.data(),std::forward<U>(t))};
		*p=u8'\n';
		return T(array.data(),++p);
	}
	else
	{
		return T(array.data(),
		print_reserve_define(print_reserve_type<no_cvref>,array.data(),std::forward<U>(t)));
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
inline constexpr std::size_t scatter_concat_recursive(io_scatter_t* arr,T&& t)
{
	*arr=print_scatter_define(print_scatter_type<char_type>,std::forward<T>(t));
	return arr->len;
}

template<std::integral char_type,typename T,typename... Args>
inline constexpr std::size_t scatter_concat_recursive(io_scatter_t* arr,T&& t, Args&& ...args)
{
	*arr=print_scatter_define(print_scatter_type<char_type>,std::forward<T>(t));
	return scatter_concat_recursive<char_type>(arr+1,std::forward<Args>(args)...)+arr->len;
}


template<std::integral char_type,typename T>
inline constexpr std::size_t scatter_concat_with_reserve_recursive_unit(char_type*& start_ptr,
		io_scatter_t* arr,T&& t)
{
	if constexpr(scatter_printable<char_type,T>)
		*arr=print_scatter_define(print_scatter_type<char_type>,std::forward<T>(t));
	else
	{
		using real_type = std::remove_cvref_t<T>;
		auto end_ptr = print_reserve_define(print_reserve_type<real_type>,start_ptr,std::forward<T>(t));
		*arr={start_ptr,(end_ptr-start_ptr)*sizeof(*start_ptr)};
		start_ptr=end_ptr;
	}
	return arr->len;
}

template<std::integral char_type,typename T>
inline constexpr std::size_t scatter_concat_with_reserve_recursive(char_type* ptr,
		io_scatter_t* arr,T&& t)
{
	return scatter_concat_with_reserve_recursive_unit(ptr,arr,std::forward<T>(t));
}

template<std::integral char_type,typename T,typename... Args>
inline constexpr std::size_t scatter_concat_with_reserve_recursive(char_type* ptr,
	io_scatter_t* arr,T&& t, Args&& ...args)
{
	std::size_t const res{scatter_concat_with_reserve_recursive(ptr,arr,std::forward<T>(t))};
	return res+scatter_concat_with_reserve_recursive(ptr,arr+1,std::forward<Args>(args)...);
}

template<bool line,typename char_type,std::size_t arg_number>
inline constexpr auto deal_with_scatters(std::array<io_scatter_t,arg_number>& scatters,std::size_t sz)
{
	sz/=sizeof(char_type);
	std::basic_string<char_type> str;
	if constexpr(line)
		str.reserve(sz+1);
	else
		str.reserve(sz);
	fast_io::ostring_ref oref{str};
	auto it(str.data());
	for(auto const& e : scatters)
	{
		memcpy(it,e.base,e.len);
		it+=e.len/sizeof(char_type);
	}
	if constexpr(line)
	{
		*it=u8'\n';
		++it;
	}
	obuffer_set_curr(oref,it);
	return str;
}

template<bool line,typename char_type,std::size_t arg_number>
inline constexpr void deal_with_scatters_string(std::basic_string<char_type>& str,std::array<io_scatter_t,arg_number>& scatters,std::size_t sz)
{
	sz/=sizeof(char_type);
	if constexpr(line)
		str.reserve(sz+1+str.size());
	else
		str.reserve(sz+str.size());
	fast_io::ostring_ref oref{str};
	auto it(str.data());
	for(auto const& e : scatters)
	{
		memcpy(it,e.base,e.len);
		it+=e.len/sizeof(char_type);
	}
	if constexpr(line)
	{
		*it=u8'\n';
		++it;
	}
	obuffer_set_curr(oref,it);
}

template<bool line,std::integral char_type,typename ...Args>
inline constexpr auto concat_fallback(Args&& ...args)
{
	if constexpr(((scatter_printable<char_type,Args>||reserve_printable<Args>)&&...))
	{
		std::array<io_scatter_t,sizeof...(Args)> scatters;
		if constexpr(((scatter_printable<char_type,Args>)&&...))
			return deal_with_scatters<line,char_type>(scatters,scatter_concat_recursive<char_type>(
				scatters.data(),std::forward<Args>(args)...));
		else
		{
			constexpr std::size_t sca_sz{calculate_scatter_reserve_size<Args...>()};
			std::array<char_type,sca_sz> array;
			return deal_with_scatters<line,char_type>(scatters,scatter_concat_with_reserve_recursive<char_type>(
				array.data(),scatters.data(),std::forward<Args>(args)...));
		}
	}
	else
	{
		std::basic_string<char_type> bas;
		fast_io::ostring_ref itb(bas);
		if constexpr(line)
			println(itb,std::forward<Args>(args)...);
		else
			print(itb,std::forward<Args>(args)...);
		return bas;
	}
}


template<bool line,std::integral char_type,typename U,typename... Args>
inline constexpr decltype(auto) deal_with_first_is_string_rvalue_reference(U&& u,Args&& ...args)
{
	if constexpr(sizeof...(Args)==0)
	{
		if constexpr(line)
			u.push_back(u8'\n');
	}
	else
	{
		if constexpr(((scatter_printable<char_type,Args>||reserve_printable<Args>)&&...))
		{
			std::array<io_scatter_t,sizeof...(Args)> scatters;
			if constexpr(((scatter_printable<char_type,Args>)&&...))
				deal_with_scatters_string<line>(u,scatters,scatter_concat_recursive<char_type>(
					scatters.data(),std::forward<Args>(args)...));
			else
			{
				constexpr std::size_t sca_sz{calculate_scatter_reserve_size<Args...>()};
				std::array<char_type,sca_sz> array;
				deal_with_scatters_string<line>(u,scatters,scatter_concat_with_reserve_recursive<char_type>(
					array.data(),scatters.data(),std::forward<Args>(args)...));
			}
		}
		else
		{
			ostring_ref t{u};
			{
				if constexpr(line)
					println(t,std::forward<Args>(args)...);
				else
					print(t,std::forward<Args>(args)...);
			}
		}
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
	{
		return details::deal_with_one<T,false>(std::forward<Args>(args)...);
	}
	else
	{
		if constexpr(details::test_first_is_string_rvalue_reference<T,Args&&...>())
			return details::deal_with_first_is_string_rvalue_reference<false,typename T::value_type>(std::forward<Args>(args)...);
		else
			return details::concat_fallback<false,typename T::value_type>(std::forward<Args>(args)...);
	}
}

template<typename T=std::string,typename... Args>
inline constexpr T concatln(Args&& ...args)
{
	if constexpr(sizeof...(Args)==0)
		return T(1,u8'\n');
	else if constexpr(sizeof...(Args)==1&&details::test_one<true,T,Args...>())
	{
		return details::deal_with_one<T,true>(std::forward<Args>(args)...);
	}
	else
	{
		if constexpr(details::test_first_is_string_rvalue_reference<T,Args&&...>())
			return details::deal_with_first_is_string_rvalue_reference<true,typename T::value_type>(std::forward<Args>(args)...);
		else
			return details::concat_fallback<true,typename T::value_type>(std::forward<Args>(args)...);
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
	ostring_ref ref{str};
	print(ref,std::forward<Args>(args)...);
	istring_view is(str);
	scan(is,t);		//Todo. no_decoration
}

template<typename... Args>
inline
#if __cpp_lib_constexpr_string >= 201907L
constexpr
#endif
void in_place_to(std::string& t,Args&& ...args)
{
	t.clear();
	ostring_ref ref{t};
	print(ref,std::forward<Args>(args)...);
}

template<typename T,typename... Args>
inline constexpr T to(Args&& ...args)
{
	if constexpr(details::is_std_string<T>::value)
	{
		return fast_io::concat<T>(std::forward<Args>(args)...);
	}
	else if constexpr(std::same_as<T,std::runtime_error>||
		std::same_as<T,std::logic_error>||std::same_as<T,std::domain_error>||
		std::same_as<T,std::invalid_argument>||std::same_as<T,std::length_error>||
		std::same_as<T,std::out_of_range>||std::same_as<T,std::range_error>||
		std::same_as<T,std::overflow_error>||std::same_as<T,std::underflow_error>)
		return T(fast_io::concat(std::forward<Args>(args)...));
	else
	{
		T t;
		in_place_to(t,std::forward<Args>(args)...);
		return t;
	}
}
}
