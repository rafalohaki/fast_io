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

template<typename Iter>
requires (std::contiguous_iterator<Iter>||std::is_pointer_v<std::remove_cvref_t<Iter>>||std::same_as<std::remove_cvref_t<Iter>,void*>)
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<Iter>)
{
	constexpr std::size_t sz{sizeof(std::uintptr_t)*2+4};
	return sz;
}

template<std::contiguous_iterator caiter>
inline constexpr caiter print_reserve_define(io_reserve_type_t<void*>,caiter iter,void* v)
{
	constexpr std::size_t uisz{sizeof(std::uintptr_t)*2+2};
	constexpr std::size_t uisz1{uisz+2};
	details::copy_string_literal(u8"0x",iter);
	details::optimize_size::output_unsigned_dummy<uisz,16>(iter+2,bit_cast<std::uintptr_t>(v));	
	return iter+uisz1;
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

template<output_stream output,std::integral char_type>
requires (std::same_as<typename output::char_type,char_type>)
inline constexpr void print_define(output out,basic_io_scatter_t<char_type> iosc)
{
	write(out,iosc.base,iosc.base+iosc.len);
}

template<output_stream output,std::integral char_type>
requires (std::same_as<typename output::char_type,char_type>)
inline constexpr basic_io_scatter_t<char_type> print_scatter_define(output out,basic_io_scatter_t<char_type> iosc)
{
	return iosc;
}

template<std::integral char_type,std::size_t n>
requires(n!=0)
inline constexpr auto print_alias_define(io_alias_type_t<char_type[n]>,char_type const(&s)[n])
{
	if constexpr(n==1||n==2)
		return chvw(*s);
	else
		return basic_io_scatter_t<char_type>{s,n-1};
}

template<std::integral char_type>
inline constexpr auto print_alias_define(io_alias_type_t<std::basic_string_view<char_type>>,std::basic_string_view<char_type> svw)
{
	return basic_io_scatter_t<char_type>{svw.data(),svw.size()};
}

template<std::integral char_type,typename T>
inline constexpr auto print_alias_define(io_alias_type_t<manip::chvw<T*>>,manip::chvw<T*> svw)
{
	std::basic_string_view<std::remove_cvref_t<T>> bsv{svw.reference};
	return basic_io_scatter_t<char_type>{bsv.data(),bsv.size()};
}

template<std::contiguous_iterator Iter>
inline constexpr void* print_alias_define(io_alias_type_t<Iter>,Iter it)
{
	return std::to_address(it);
}

template<typename T>
inline constexpr void* print_alias_define(io_alias_type_t<T*>,T* ptr)
{
	return ptr;
}


}
