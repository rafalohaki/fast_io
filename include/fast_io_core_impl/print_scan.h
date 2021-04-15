#pragma once

namespace fast_io
{

template<std::integral char_type,typename T>
requires reserve_printable<char_type,T>
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<char_type,manip::line<T>>)
{
	constexpr std::size_t sz{print_reserve_size(io_reserve_type<char_type,std::remove_cvref_t<T>>)+1};
	return sz;
}

template<std::integral char_type,::fast_io::freestanding::random_access_iterator raiter,typename T,typename U>
requires reserve_printable<char_type,T>
inline constexpr raiter print_reserve_define(io_reserve_type_t<char_type,manip::line<T>>,raiter start,U a)
{
	auto it{print_reserve_define(io_reserve_type<char_type,std::remove_cvref_t<T>>,start,a.reference)};
	*it=u8'\n';
	return ++it;
}

}
