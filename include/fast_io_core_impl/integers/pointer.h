#pragma once

namespace fast_io
{


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

template<std::integral char_type>
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<char_type,void const*>) noexcept
{
	constexpr std::size_t sz{sizeof(std::uintptr_t)*2+2};
	return sz;
}

namespace details
{
template<std::random_access_iterator caiter>
inline constexpr caiter print_reserve_address_impl(caiter iter,std::uintptr_t value) noexcept
{
	using char_type = std::iter_value_t<caiter>;
	constexpr std::size_t uisz{sizeof(std::uintptr_t)*2};
	if constexpr(std::same_as<char,char_type>)
		iter=details::copy_string_literal("0x",iter);
	else if constexpr(std::same_as<wchar_t,char_type>)
		iter=details::copy_string_literal(L"0x",iter);
	else if constexpr(std::same_as<char16_t,char_type>)
		iter=details::copy_string_literal(u"0x",iter);
	else if constexpr(std::same_as<char32_t,char_type>)
		iter=details::copy_string_literal(U"0x",iter);
	else
		iter=details::copy_string_literal(u8"0x",iter);
#ifdef FAST_IO_OPTIMIZE_SIZE
	details::optimize_size::output_unsigned_dummy<uisz,16>(iter,value);
	return iter+=uisz;
#else
	constexpr auto table(details::get_shared_inline_constexpr_base_table<char_type,16,false,false>().data());
	auto bg{iter};
	for(auto ptr{iter+=uisz};ptr!=bg;)
	{
		non_overlapped_copy_n(table[value&255u].data(),2,ptr-=2);
		value>>=8;
	}
	return iter;
#endif
}
#if 0
template<std::integral char_type>
inline constexpr auto zeros_generator() noexcept
{
	char_type zero(u8'0');
	char_type x(u8'x');
	if constexpr(std::same_as<char_type,char>)
	{
		zero='0';
		x='x';
	}
	else if constexpr(std::same_as<char_type,wchar_t>)
	{
		zero=L'0';
		x=L'x';
	}
	std::array<char_type,sizeof(std::uintptr_t)*2+2> arr;
	for(auto& e : arr)
		e=zero;
	arr[1]=x;
	return arr;
}
#endif
}

template<std::integral char_type,std::random_access_iterator caiter>
inline constexpr caiter print_reserve_define(io_reserve_type_t<char_type,void const*>,caiter iter,void const* v) noexcept
{
	return details::print_reserve_address_impl(iter,bit_cast<std::uintptr_t>(v));
}

template<std::integral char_type>
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<char_type,std::nullptr_t>) noexcept
{
	constexpr std::size_t sz{sizeof(std::uintptr_t)*2+4};
	return sz;
}

template<std::integral char_type,std::random_access_iterator caiter>
inline constexpr caiter print_reserve_define(io_reserve_type_t<char_type,std::nullptr_t>,caiter iter,std::nullptr_t) noexcept
{
/*
Should match the output of libstdc++ iostream
*/
	if constexpr(std::same_as<char_type,char>)
		return details::copy_string_literal("nullptr",iter);
	else if constexpr(std::same_as<char_type,wchar_t>)
		return details::copy_string_literal(L"nullptr",iter);
	else if constexpr(std::same_as<char_type,char16_t>)
		return details::copy_string_literal(u"nullptr",iter);
	else if constexpr(std::same_as<char_type,char32_t>)
		return details::copy_string_literal(U"nullptr",iter);
	else
		return details::copy_string_literal(u8"nullptr",iter);
}

#if 0
template<output_stream output,std::integral char_type>
requires (std::same_as<typename output::char_type,char_type>)
inline constexpr void print_define(output out,basic_io_scatter_t<char_type> iosc) noexcept
{
	write(out,iosc.base,iosc.base+iosc.len);
}
#endif

template<std::integral char_type>
inline constexpr basic_io_scatter_t<char_type> print_scatter_define(print_scatter_type_t<char_type>,basic_io_scatter_t<char_type> iosc) noexcept
{
	return iosc;
}

template<std::integral char_type,std::size_t n>
inline constexpr auto print_alias_define(io_alias_t,char_type const(&s)[n]) noexcept
{
	if constexpr(n==2)
		return manipulators::chvw_t<char_type>{*s};
	else
		return basic_io_scatter_t<char_type>{s,n-1};
}

template<std::integral char_type>
inline constexpr basic_io_scatter_t<char_type> print_alias_define(io_alias_t,std::basic_string_view<char_type> svw) noexcept
{
	return {svw.data(),svw.size()};
}

template<std::integral T>
inline constexpr auto print_alias_define(io_alias_t,manipulators::chvw_t<T const*> svw) noexcept
{
	std::basic_string_view<T> bsv{svw.reference};
	return basic_io_scatter_t<T>{bsv.data(),bsv.size()};
}

template<typename Iter>
requires (std::contiguous_iterator<Iter>||std::is_pointer_v<Iter>)
inline constexpr void const* print_alias_define(io_alias_t,Iter it) noexcept
{
	if constexpr(std::is_pointer_v<std::remove_cvref_t<Iter>>)
	{
		if constexpr(std::convertible_to<std::remove_cvref_t<Iter>,void const*>)
			return it;
		else
			return bit_cast<void const*>(it);
	}
	else
		return std::to_address(it);
}

template<std::integral char_type,std::integral pchar_type>
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<char_type,manipulators::chvw_t<pchar_type>>) noexcept
{
	return 1;
}

template<std::integral char_type,std::integral pchar_type,std::contiguous_iterator caiter,typename T>
inline constexpr caiter print_reserve_define(io_reserve_type_t<char_type,manipulators::chvw_t<pchar_type>>,caiter iter,T ch) noexcept
{
	*iter=static_cast<pchar_type>(ch.reference);
	return ++iter;
}

}
