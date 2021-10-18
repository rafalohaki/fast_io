#pragma once

namespace fast_io
{

template<std::integral char_type,typename T>
requires reserve_printable<char_type,T>
inline constexpr std::size_t print_reserve_size(io_reserve_type_t<char_type,std::complex<T>>)
{
	return 2*print_reserve_size(io_reserve_type<char_type,T>)+3;
}

namespace details
{
template<typename T,::fast_io::freestanding::random_access_iterator Iter>
inline constexpr Iter print_reserve_complex_impl(Iter iter,std::complex<T> c) noexcept
{
	using char_type = ::fast_io::freestanding::iter_value_t<Iter>;
	*iter=char_literal_v<u8'(',char_type>;
	++iter;
	iter=print_reserve_define(io_reserve_type<char_type,T>,iter,std::real(c));
	*iter=char_literal_v<u8',',char_type>;
	++iter;
	iter=print_reserve_define(io_reserve_type<char_type,T>,iter,std::imag(c));
	*iter=char_literal_v<u8')',char_type>;
	++iter;
	return iter;
}
}

template<std::integral char_type,typename T,::fast_io::freestanding::random_access_iterator Iter>
requires reserve_printable<char_type,T>
inline constexpr Iter print_reserve_define(io_reserve_type_t<char_type,std::complex<T>>,Iter iter,std::complex<T> c) noexcept
{
	return details::print_reserve_complex_impl(iter,c);
}

}
