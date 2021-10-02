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
namespace manipulators
{
template<typename T>
struct chvw_t
{
	using manip_tag = manip_tag_t;
	T reference;
};

template<std::integral T>
inline constexpr chvw_t<T> chvw(T ch) noexcept
{
	return {ch};
}

template<std::integral T>
inline constexpr chvw_t<T const*> chvw(T const* ch) noexcept
{
	return {ch};
}
#if 0
template<std::integral T>
inline constexpr basic_io_scatter_t<T> chvw(T const* ch,std::size_t n) noexcept
{
	return {ch,
#if defined(__has_builtin)
#if __has_builtin(__builtin_strnlen)
	__builtin_strnlen(ch,n)
#else
	::std::strnlen(ch,n)
#endif
#else
	::std::strnlen(ch,n)
#endif
	};
}
#endif
}

template<std::integral char_type>
inline constexpr basic_io_scatter_t<char_type> print_scatter_define(io_reserve_type_t<char_type,basic_io_scatter_t<char_type>>,basic_io_scatter_t<char_type> iosc) noexcept
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
inline constexpr basic_io_scatter_t<char_type> print_alias_define(io_alias_t,::fast_io::freestanding::basic_string_view<char_type> svw) noexcept
{
	return {svw.data(),svw.size()};
}

template<std::integral T>
inline constexpr auto print_alias_define(io_alias_t,manipulators::chvw_t<T const*> svw) noexcept
{
	::fast_io::freestanding::basic_string_view<T> bsv{svw.reference};
	return basic_io_scatter_t<T>{bsv.data(),bsv.size()};
}

template<std::integral char_type,std::integral pchar_type>
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<char_type,manipulators::chvw_t<pchar_type>>) noexcept
{
	return 1;
}

template<std::integral char_type,std::integral pchar_type,::fast_io::freestanding::contiguous_iterator caiter,typename T>
inline constexpr caiter print_reserve_define(io_reserve_type_t<char_type,manipulators::chvw_t<pchar_type>>,caiter iter,T ch) noexcept
{
	using unsigned_char_type = std::make_unsigned_t<char_type>;
	*iter=static_cast<char_type>(static_cast<unsigned_char_type>(ch.reference));
	return ++iter;
}

}
