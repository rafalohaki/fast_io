#pragma once

namespace fast_io::manipulators
{

template<std::size_t base,bool uppercase,typename T>
struct base_t
{
	using manip_tag = manip_tag_t;
	T reference;
};

template<std::size_t bs,::fast_io::details::my_integral T>
requires (2<=bs&&bs<=36)
inline constexpr base_t<bs,false,T&> base_get(T& reference) noexcept
{
	return {reference};
}

template<::fast_io::details::my_integral T>
inline constexpr base_t<16,false,T&> hex_get(T& reference) noexcept
{
	return {reference};
}

template<std::size_t bs,::fast_io::details::my_integral T>
requires (2<=bs&&bs<=36)
inline constexpr base_t<bs,false,T> base(T reference) noexcept
{
	return {reference};
}

template<std::size_t bs,::fast_io::details::my_integral T>
requires (2<=bs&&bs<=36)
inline constexpr base_t<bs,true,T> base_upper(T reference) noexcept
{
	return {reference};
}

template<::fast_io::details::my_integral T>
inline constexpr base_t<16,false,T> hex(T reference) noexcept
{
	return {reference};
}

template<::fast_io::details::my_integral T>
inline constexpr base_t<16,true,T> hex_upper(T reference) noexcept
{
	return {reference};
}

}