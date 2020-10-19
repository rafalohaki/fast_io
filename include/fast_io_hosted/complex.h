#pragma once

namespace fast_io
{

template<std::integral char_type,typename T>
requires reserve_printable<char_type,T>
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<char_type,std::complex<T>>)
{
	return 2*print_reserve_size(io_reserve_type<char_type,T>)+3;
}

template<std::integral char_type,typename T,std::random_access_iterator Iter>
requires reserve_printable<char_type,T>
inline constexpr Iter print_reserve_define(io_reserve_type_t<char_type,std::complex<T>>,Iter iter,std::complex<T> c)
{
	*iter=u8'(';
	++iter;
	*(iter=print_reserve_define(io_reserve_type<char_type,T>,iter,std::real(c)))=u8',';
	++iter;
	*(iter=print_reserve_define(io_reserve_type<char_type,T>,iter,std::imag(c)))=u8')';
	return ++iter;
}

}