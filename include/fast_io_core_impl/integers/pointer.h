#pragma once

namespace fast_io
{

template<output_stream output,std::integral array_value_type,std::size_t n>
requires (std::same_as<typename output::char_type,std::remove_cvref_t<array_value_type>>||
(std::same_as<typename output::char_type,char>&&std::same_as<std::remove_cvref_t<array_value_type>,char8_t>)
&&n!=0)	//array cannot be zero size. but we do the check too
constexpr void print_define(output& out,array_value_type (&s)[n])
{
	if constexpr(1<n)
	{
		if constexpr(n==2&&character_output_stream<output>)
			put(out,static_cast<typename output::char_type>(*s));
		else
			write(out,s,s+(n-1));
	}
}

/*
we do not accept char const* since we never know whether it has null terminator.
This avoids security vulneralbilities for
	char * str = 0;
	print(str);
Instead, we print out its pointer value

We extend print pointers to print contiguous_iterator. No we can write things like

std::vector<std::size_t> vec(100,2);
println("vec.begin():",vec.begin()," vec.end()",vec.end());
*/

template<typename Iter>
requires (std::contiguous_iterator<Iter>||std::is_pointer_v<std::remove_cvref_t<Iter>>||std::same_as<std::remove_cvref_t<Iter>,void*>)
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<Iter>)
{
	constexpr std::size_t sz{sizeof(std::uintptr_t)*2+4};
	return sz;
}

template<typename Iter,std::contiguous_iterator caiter,typename U>
requires (std::contiguous_iterator<Iter>||std::is_pointer_v<std::remove_cvref_t<Iter>>||std::same_as<std::remove_cvref_t<Iter>,void*>)
inline caiter print_reserve_define(io_reserve_type_t<Iter>,caiter iter,U&& v)
{
	constexpr std::size_t uisz{sizeof(std::uintptr_t)*2+2};
	constexpr std::size_t uisz1{uisz+2};
	details::my_copy_n(u8"0x",2,iter);
	if constexpr(std::is_pointer_v<std::remove_cvref_t<Iter>>)
		details::optimize_size::output_unsigned_dummy<uisz,16>(iter+2,bit_cast<std::uintptr_t>(v));	
	else
		details::optimize_size::output_unsigned_dummy<uisz,16>(iter+2,bit_cast<std::uintptr_t>(std::to_address(v)));
	return iter+uisz1;
}


template<std::integral char_type,std::integral array_value_type,std::size_t n>
requires (std::same_as<char_type,std::remove_cvref_t<array_value_type>>||
(std::same_as<char_type,char>&&std::same_as<std::remove_cvref_t<array_value_type>,char8_t>)
&&n!=0)	//C-style array cannot be zero size. but we do the check too
inline constexpr basic_io_scatter_t<char_type> print_scatter_define(print_scatter_type_t<char_type>,array_value_type const (&s)[n])
{
	if constexpr(std::same_as<char_type,std::remove_cvref_t<array_value_type>>)
		return {s,n-1};
	else
		return {reinterpret_cast<char const*>(s),n-1};
}

template<std::integral char_type,std::integral T>
requires (std::same_as<char_type,std::remove_cvref_t<T>>||
(std::same_as<char_type,char>&&std::same_as<std::remove_cvref_t<T>,char8_t>))
inline constexpr basic_io_scatter_t<char_type> print_scatter_define(print_scatter_type_t<char_type>,manip::chvw<T*> a)
{
	if constexpr(std::same_as<char_type,std::remove_cvref_t<T>>)
	{
#if __cpp_lib_is_constant_evaluated >= 201811L
		if(std::is_constant_evaluated())
		{
			std::basic_string_view<std::remove_cvref_t<T>> bsv(a.reference);
			return {bsv.data(),bsv.size()};
		}
		else
		{
#endif
			if constexpr(std::same_as<char,char_type>)
				return {a.reference,strlen(a.reference)};
			else if constexpr(std::same_as<char8_t,char_type>)
				return {a.reference,strlen(reinterpret_cast<char const*>(a.reference))};
			else
			{
				std::basic_string_view<std::remove_cvref_t<T>> bsv(a.reference);
				return {bsv.data(),bsv.size()};
			}
#if __cpp_lib_is_constant_evaluated >= 201811L
		}
#endif
	}
	else
		return {reinterpret_cast<char const*>(a.reference),strlen(reinterpret_cast<char const*>(a.reference))};
}


template<std::integral char_type,typename T>
requires (!std::is_pointer_v<std::remove_cvref_t<T>>&&!std::is_array_v<std::remove_cvref_t<T>>&&
(std::convertible_to<T,std::basic_string_view<char_type>>||(std::same_as<char_type,char>&&std::convertible_to<T,std::basic_string_view<char8_t>>)))
inline constexpr basic_io_scatter_t<char_type> print_scatter_define(print_scatter_type_t<char_type>,T&& convt_str)
{
	if constexpr(std::convertible_to<T,std::basic_string_view<char_type>>)
	{
#if __cpp_lib_is_constant_evaluated >= 201811L
		if(std::is_constant_evaluated())
		{
			std::basic_string_view<char_type> bsv(std::forward<T>(convt_str));
			return {bsv.data(),bsv.size()};
		}
		else
		{
#endif
			if constexpr(std::same_as<std::basic_string_view<char_type>,std::remove_cvref_t<T>>)
				return {convt_str.data(),convt_str.size()};
			else
			{
				std::basic_string_view<char_type> bsv(std::forward<T>(convt_str));
				return {bsv.data(),bsv.size()};
			}
#if __cpp_lib_is_constant_evaluated >= 201811L
		}
#endif
	}
	else
	{
		if constexpr(std::same_as<std::basic_string_view<char8_t>,std::remove_cvref_t<T>>)
			return {reinterpret_cast<char const*>(convt_str.data()),convt_str.size()};
		else
		{
			std::basic_string_view<char8_t> bsv(std::forward<T>(convt_str));
			return {reinterpret_cast<char const*>(bsv.data()),bsv.size()};
		}
	}
}

template<output_stream output>
inline constexpr void print_define(output& out,std::basic_string_view<typename output::char_type> str)
{
	write(out,str.data(),str.data()+str.size());
}

inline constexpr std::size_t print_reserve_size(io_reserve_type_t<std::nullptr_t>)
{
	constexpr std::size_t sz{9};
	return sz;
}

template<std::contiguous_iterator caiter>
inline constexpr caiter print_reserve_define(io_reserve_type_t<std::nullptr_t>,caiter iter,std::nullptr_t)
{
	details::copy_string_literal(u8"(nullptr)",iter);
	return iter+9;
}

}
