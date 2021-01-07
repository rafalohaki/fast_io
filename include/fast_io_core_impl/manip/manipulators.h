#pragma once

namespace fast_io::manipulators
{

template<std::size_t base,bool uppercase,typename T>
struct base_t
{
	using manip_tag = manip_tag_t;
	T reference;
};

template<typename T>
struct chvw_t
{
	using manip_tag = manip_tag_t;
	T reference;
};

template<std::size_t bs,::fast_io::details::my_integral T>
requires (2<=bs&&bs<=36)
inline constexpr std::conditional_t<bs==10,parameter<T&>,base_t<bs,false,T&>> base_get(T& reference) noexcept
{
	return {reference};
}

template<::fast_io::details::my_integral T>
inline constexpr base_t<16,false,T&> hex_get(T& reference) noexcept
{
	return {reference};
}

template<::fast_io::details::my_integral T>
inline constexpr parameter<T&> dec_get(T& reference) noexcept
{
	return {reference};
}

template<::fast_io::details::my_integral T>
inline constexpr base_t<8,false,T&> oct_get(T& reference) noexcept
{
	return {reference};
}

template<::fast_io::details::my_integral T>
inline constexpr base_t<2,false,T&> bin_get(T& reference) noexcept
{
	return {reference};
}

template<std::size_t bs,bool upper=false,typename T>
requires (2<=bs&&bs<=36&&(::fast_io::details::my_integral<T>||
	(std::is_pointer_v<T>||std::contiguous_iterator<T>)))
inline constexpr std::conditional_t<bs==10,
	parameter<std::conditional_t<std::is_pointer_v<T>||std::contiguous_iterator<T>,std::uintptr_t,
	std::remove_cvref_t<T>>>,
	base_t<bs,upper&&(10<bs),
	std::conditional_t<std::is_pointer_v<T>||std::contiguous_iterator<T>,std::uintptr_t,
	std::remove_cvref_t<T>>
	>> base(T reference) noexcept
{
	if constexpr(std::contiguous_iterator<T>)
		return {bit_cast<std::uintptr_t>(std::to_address(reference))};
	else if constexpr(std::is_pointer_v<T>)
		return {bit_cast<std::uintptr_t>(reference)};
	else
		return {reference};
}


template<std::size_t bs,typename T>
requires (2<=bs&&bs<=36&&(::fast_io::details::my_integral<T>||
	(std::is_pointer_v<T>||std::contiguous_iterator<T>)))
inline constexpr std::conditional_t<bs==10,
	parameter<std::conditional_t<std::is_pointer_v<T>||std::contiguous_iterator<T>,std::uintptr_t,
	std::remove_cvref_t<T>>>,
	base_t<bs,10<bs,
	std::conditional_t<std::is_pointer_v<T>||std::contiguous_iterator<T>,std::uintptr_t,
	std::remove_cvref_t<T>>
	>> base_upper(T reference) noexcept
{
	if constexpr(std::contiguous_iterator<T>)
		return {bit_cast<std::uintptr_t>(std::to_address(reference))};
	else if constexpr(std::is_pointer_v<T>)
		return {bit_cast<std::uintptr_t>(reference)};
	else
		return {reference};
}

template<typename T>
requires (::fast_io::details::my_integral<T>||
	(std::is_pointer_v<T>||std::contiguous_iterator<T>))
inline constexpr base_t<16,false,
	std::conditional_t<std::is_pointer_v<T>||std::contiguous_iterator<T>,std::uintptr_t,
	std::remove_cvref_t<T>>
	> hex(T reference) noexcept
{
	if constexpr(std::contiguous_iterator<T>)
		return {bit_cast<std::uintptr_t>(std::to_address(reference))};
	else if constexpr(std::is_pointer_v<T>)
		return {bit_cast<std::uintptr_t>(reference)};
	else
		return {reference};
}

template<typename T>
requires (::fast_io::details::my_integral<T>||
	(std::is_pointer_v<T>||std::contiguous_iterator<T>))
inline constexpr base_t<16,true,
	std::conditional_t<std::is_pointer_v<T>||std::contiguous_iterator<T>,std::uintptr_t,
	std::remove_cvref_t<T>>
	> hex_upper(T reference) noexcept
{
	if constexpr(std::contiguous_iterator<T>)
		return {bit_cast<std::uintptr_t>(std::to_address(reference))};
	else if constexpr(std::is_pointer_v<T>)
		return {bit_cast<std::uintptr_t>(reference)};
	else
		return {reference};
}


template<typename T>
requires (::fast_io::details::my_integral<T>||
	(std::is_pointer_v<T>||std::contiguous_iterator<T>))
inline constexpr base_t<2,false,
	std::conditional_t<std::is_pointer_v<T>||std::contiguous_iterator<T>,std::uintptr_t,
	std::remove_cvref_t<T>>
	> bin(T reference) noexcept
{
	if constexpr(std::contiguous_iterator<T>)
		return {bit_cast<std::uintptr_t>(std::to_address(reference))};
	else if constexpr(std::is_pointer_v<T>)
		return {bit_cast<std::uintptr_t>(reference)};
	else
		return {reference};
}

template<typename T>
requires (::fast_io::details::my_integral<T>||
	(std::is_pointer_v<T>||std::contiguous_iterator<T>))
inline constexpr base_t<8,false,
	std::conditional_t<std::is_pointer_v<T>||std::contiguous_iterator<T>,std::uintptr_t,
	std::remove_cvref_t<T>>
	> oct(T reference) noexcept
{
	if constexpr(std::contiguous_iterator<T>)
		return {bit_cast<std::uintptr_t>(std::to_address(reference))};
	else if constexpr(std::is_pointer_v<T>)
		return {bit_cast<std::uintptr_t>(reference)};
	else
		return {reference};
}

template<typename T>
requires (::fast_io::details::my_integral<T>||
	(std::is_pointer_v<T>||std::contiguous_iterator<T>))
inline constexpr parameter<
	std::conditional_t<std::is_pointer_v<T>||std::contiguous_iterator<T>,std::uintptr_t,
	std::remove_cvref_t<T>>
	> dec(T reference) noexcept
{
	if constexpr(std::contiguous_iterator<T>)
		return {bit_cast<std::uintptr_t>(std::to_address(reference))};
	else if constexpr(std::is_pointer_v<T>)
		return {bit_cast<std::uintptr_t>(reference)};
	else
		return {reference};
}

template<std::integral T>
inline constexpr chvw_t<T> chvw(T ch)
{
	return {ch};
}

template<std::integral T>
inline constexpr chvw_t<T const*> chvw(T const* ch)
{
	return {ch};
}

}