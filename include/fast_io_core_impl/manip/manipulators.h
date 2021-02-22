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

template<typename T>
struct blvw_t
{
	using manip_tag = manip_tag_t;
	T reference;
};

template<typename T>
struct comma_t
{
	using manip_tag = manip_tag_t;
	T reference;
};

template<typename T>
struct cdcvt_t
{
	using manip_tag = manip_tag_t;
	T reference;
};

template<typename T>
struct pos_sign_t
{
	using manip_tag = manip_tag_t;
	T reference;
};

enum class floating_representation
{
scientific,fixed,general
};

/*
Referenced from dragonbox paper
https://github.com/jk-jeon/dragonbox/blob/master/other_files/Dragonbox.pdf
*/

/*
https://github.com/jk-jeon/dragonbox
jkj::dragonbox::policy::rounding_mode::nearest_to_even: This is the default policy. Use round-to-nearest, tie-to-even rounding mode.

jkj::dragonbox::policy::rounding_mode::nearest_to_odd: Use round-to-nearest, tie-to-odd rounding mode.

jkj::dragonbox::policy::rounding_mode::nearest_toward_plus_infinity: Use round-to-nearest, tie-toward-plus-infinity rounding mode.

jkj::dragonbox::policy::rounding_mode::nearest_toward_minus_infinity: Use round-to-nearest, tie-toward-minus-infinity rounding mode.

jkj::dragonbox::policy::rounding_mode::nearest_toward_zero: Use round-to-nearest, tie-toward-zero rounding mode. This will produce the fastest code among all round-to-nearest rounding modes.

jkj::dragonbox::policy::rounding_mode::nearest_away_from_zero: Use round-to-nearest, tie-away-from-zero rounding mode.

jkj::dragonbox::policy::rounding_mode::nearest_to_even_static_boundary: Use round-to-nearest, tie-to-even rounding mode, but there will be completely independent code paths for even inputs and odd inputs. This will produce a bigger binary, but might run faster than jkj::dragonbox::policy::rounding_mode::nearest_to_even for some situation.

jkj::dragonbox::policy::rounding_mode::nearest_to_odd_static_boundary: Use round-to-nearest, tie-to-odd rounding mode, but there will be completely independent code paths for even inputs and odd inputs. This will produce a bigger binary, but might run faster than jkj::dragonbox::policy::rounding_mode::nearest_to_odd for some situation.

jkj::dragonbox::policy::rounding_mode::nearest_toward_plus_infinity_static_boundary: Use round-to-nearest, tie-toward-plus-infinity rounding mode, but there will be completely independent code paths for positive inputs and negative inputs. This will produce a bigger binary, but might run faster than jkj::dragonbox::policy::rounding_mode::nearest_toward_plus_infinity for some situation.

jkj::dragonbox::policy::rounding_mode::nearest_toward_minus_infinity_static_boundary: Use round-to-nearest, tie-toward-plus-infinity rounding mode, but there will be completely independent code paths for positive inputs and negative inputs. This will produce a bigger binary, but might run faster than jkj::dragonbox::policy::rounding_mode::nearest_toward_minus_infinity for some situation.

jkj::dragonbox::policy::rounding_mode::toward_plus_infinity: Use round-toward-plus-infinity rounding mode.

jkj::dragonbox::policy::rounding_mode::toward_minus_infinity: Use round-toward-minus-infinity rounding mode.

jkj::dragonbox::policy::rounding_mode::toward_zero: Use round-toward-zero rounding mode.

jkj::dragonbox::policy::rounding_mode::away_from_zero: Use away-from-zero rounding mode.

*/

enum class rounding_mode
{
nearest_to_even,
nearest_to_odd,
nearest_toward_plus_infinity,
nearest_toward_minus_infinity,
nearest_toward_zero,
nearest_away_from_zero,
nearest_to_even_static_boundary,
nearest_to_odd_static_boundary,
nearest_toward_plus_infinity_static_boundary,
nearest_toward_minus_infinity_static_boundary,
toward_plus_infinity,
toward_minus_infinity,
toward_zero,
away_from_zero,
};

template<typename T,floating_representation format,rounding_mode mode=rounding_mode::nearest_to_even,std::size_t base=10>
struct floating_format_t
{
	using manip_tag = manip_tag_t;
	T reference;
};

template<typename T,floating_representation format,rounding_mode mode=rounding_mode::nearest_to_even,std::size_t base=10>
struct floating_format_precision_t
{
	using manip_tag = manip_tag_t;
	T reference;
	std::size_t precision;
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




template<std::size_t bs,bool upper=false,typename T>
requires (2<=bs&&bs<=36&&(::fast_io::details::my_integral<T>||
	(std::is_pointer_v<T>||std::contiguous_iterator<T>)))
inline constexpr std::conditional_t<bs==10,
	parameter<std::conditional_t<std::is_pointer_v<T>||std::contiguous_iterator<T>,std::uintptr_t,
	std::make_unsigned_t<std::remove_cvref_t<T>>>>,
	base_t<bs,upper&&(10<bs),
	std::conditional_t<std::is_pointer_v<T>||std::contiguous_iterator<T>,std::uintptr_t,
	std::make_unsigned_t<std::remove_cvref_t<T>>>
	>> ubase(T reference) noexcept
{
	if constexpr(std::contiguous_iterator<T>)
		return {bit_cast<std::uintptr_t>(std::to_address(reference))};
	else if constexpr(std::is_pointer_v<T>)
		return {bit_cast<std::uintptr_t>(reference)};
	else
		return {static_cast<std::make_unsigned_t<std::remove_cvref_t<T>>>(reference)};
}


template<std::size_t bs,typename T>
requires (2<=bs&&bs<=36&&(::fast_io::details::my_integral<T>||
	(std::is_pointer_v<T>||std::contiguous_iterator<T>)))
inline constexpr std::conditional_t<bs==10,
	parameter<std::conditional_t<std::is_pointer_v<T>||std::contiguous_iterator<T>,std::uintptr_t,
	std::make_unsigned_t<std::remove_cvref_t<T>>>>,
	base_t<bs,10<bs,
	std::conditional_t<std::is_pointer_v<T>||std::contiguous_iterator<T>,std::uintptr_t,
	std::make_unsigned_t<std::remove_cvref_t<T>>>
	>> ubase_upper(T reference) noexcept
{
	if constexpr(std::contiguous_iterator<T>)
		return {bit_cast<std::uintptr_t>(std::to_address(reference))};
	else if constexpr(std::is_pointer_v<T>)
		return {bit_cast<std::uintptr_t>(reference)};
	else
		return {static_cast<std::make_unsigned_t<std::remove_cvref_t<T>>>(reference)};
}

template<typename T>
requires (::fast_io::details::my_integral<T>||
	(std::is_pointer_v<T>||std::contiguous_iterator<T>))
inline constexpr base_t<16,false,
	std::conditional_t<std::is_pointer_v<T>||std::contiguous_iterator<T>,std::uintptr_t,
	std::make_unsigned_t<std::remove_cvref_t<T>>>
	> uhex(T reference) noexcept
{
	if constexpr(std::contiguous_iterator<T>)
		return {bit_cast<std::uintptr_t>(std::to_address(reference))};
	else if constexpr(std::is_pointer_v<T>)
		return {bit_cast<std::uintptr_t>(reference)};
	else
		return {static_cast<std::make_unsigned_t<std::remove_cvref_t<T>>>(reference)};
}

template<typename T>
requires (::fast_io::details::my_integral<T>||
	(std::is_pointer_v<T>||std::contiguous_iterator<T>))
inline constexpr base_t<16,true,
	std::conditional_t<std::is_pointer_v<T>||std::contiguous_iterator<T>,std::uintptr_t,
	std::make_unsigned_t<std::remove_cvref_t<T>>>
	> uhex_upper(T reference) noexcept
{
	if constexpr(std::contiguous_iterator<T>)
		return {bit_cast<std::uintptr_t>(std::to_address(reference))};
	else if constexpr(std::is_pointer_v<T>)
		return {bit_cast<std::uintptr_t>(reference)};
	else
		return {static_cast<std::make_unsigned_t<std::remove_cvref_t<T>>>(reference)};
}


template<typename T>
requires (::fast_io::details::my_integral<T>||
	(std::is_pointer_v<T>||std::contiguous_iterator<T>))
inline constexpr base_t<2,false,
	std::conditional_t<std::is_pointer_v<T>||std::contiguous_iterator<T>,std::uintptr_t,
	std::make_unsigned_t<std::remove_cvref_t<T>>>
	> ubin(T reference) noexcept
{
	if constexpr(std::contiguous_iterator<T>)
		return {bit_cast<std::uintptr_t>(std::to_address(reference))};
	else if constexpr(std::is_pointer_v<T>)
		return {bit_cast<std::uintptr_t>(reference)};
	else
		return {static_cast<std::make_unsigned_t<std::remove_cvref_t<T>>>(reference)};
}

template<typename T>
requires (::fast_io::details::my_integral<T>||
	(std::is_pointer_v<T>||std::contiguous_iterator<T>))
inline constexpr base_t<8,false,
	std::conditional_t<std::is_pointer_v<T>||std::contiguous_iterator<T>,std::uintptr_t,
	std::make_unsigned_t<std::remove_cvref_t<T>>>
	> uoct(T reference) noexcept
{
	if constexpr(std::contiguous_iterator<T>)
		return {bit_cast<std::uintptr_t>(std::to_address(reference))};
	else if constexpr(std::is_pointer_v<T>)
		return {bit_cast<std::uintptr_t>(reference)};
	else
		return {static_cast<std::make_unsigned_t<std::remove_cvref_t<T>>>(reference)};
}

template<typename T>
requires (::fast_io::details::my_integral<T>||
	(std::is_pointer_v<T>||std::contiguous_iterator<T>))
inline constexpr parameter<
	std::conditional_t<std::is_pointer_v<T>||std::contiguous_iterator<T>,std::uintptr_t,
	std::make_unsigned_t<std::remove_cvref_t<T>>>
	> udec(T reference) noexcept
{
	if constexpr(std::contiguous_iterator<T>)
		return {bit_cast<std::uintptr_t>(std::to_address(reference))};
	else if constexpr(std::is_pointer_v<T>)
		return {bit_cast<std::uintptr_t>(reference)};
	else
		return {static_cast<std::make_unsigned_t<std::remove_cvref_t<T>>>(reference)};
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


/*
purify manipulator allows us ignoring status streams like imbued locales streams
*/
template<typename T>
struct purify_t
{
	using manip_tag = manip_tag_t;
	T reference;
};

template<typename output,typename value_type>
requires printable<output,std::remove_cvref_t<value_type>>
constexpr void print_define(output out, purify_t<value_type> wrapper)
{
	print_define(out,wrapper.reference);
}

template<std::integral char_type,typename value_type>
requires reserve_printable<char_type,std::remove_cvref_t<value_type>>
constexpr std::size_t print_reserve_size(io_reserve_type_t<char_type,purify_t<value_type>>)
{
	return print_reserve_size(io_reserve_type<char_type,std::remove_cvref_t<value_type>>);
}

template<std::integral char_type,typename value_type>
requires dynamic_reserve_printable<char_type,std::remove_cvref_t<value_type>>
constexpr std::size_t print_reserve_size(io_reserve_type_t<char_type,purify_t<value_type>>,purify_t<value_type> para)
{
	return print_reserve_size(io_reserve_type<char_type,std::remove_cvref_t<value_type>>,para.reference);
}

template<std::integral char_type,typename value_type,typename Iter>
requires (reserve_printable<char_type,std::remove_cvref_t<value_type>>||dynamic_reserve_printable<char_type,std::remove_cvref_t<value_type>>)
constexpr auto print_reserve_define(io_reserve_type_t<char_type,purify_t<value_type>>,Iter begin,purify_t<value_type> para)
{
	return print_reserve_define(io_reserve_type<char_type,std::remove_cvref_t<value_type>>,begin,para.reference);
}

template<std::integral char_type,typename value_type>
requires scatter_printable<char_type,std::remove_cvref_t<value_type>>
constexpr auto print_scatter_define(print_scatter_type_t<char_type>,purify_t<value_type> para)
{
	return print_scatter_define(print_scatter_type<char_type>,para.reference);
}

template<typename T>
inline constexpr auto purify(T&& ref) noexcept
{
	using no_cvref_t = std::remove_cvref_t<T>;
	if constexpr(alias_printable<std::remove_cvref_t<T>>)
		return purify_t<std::remove_cvref_t<decltype(print_alias_define(io_alias,ref))>>{print_alias_define(io_alias,ref)};
	else if constexpr(std::is_trivially_copyable_v<no_cvref_t>&&sizeof(no_cvref_t)<=sizeof(std::max_align_t))
		return purify_t<no_cvref_t>{ref};
	else
		return purify_t<std::remove_reference_t<T> const&>{ref};
}

inline constexpr blvw_t<bool> blvw(bool value) noexcept
{
	return {value};
}


template<typename T1,typename T2>
requires requires(T1&& t1,T2&& t2)
{
	std::same_as<decltype(print_alias_define(io_alias,std::forward<T1>(t1))),
	decltype(print_alias_define(io_alias,std::forward<T2>(t2)))>;
}
inline constexpr auto cond(bool con,T1&& t1,T2&& t2)
{
	if(con)
		return print_alias_define(io_alias,std::forward<T1>(t1));
	return print_alias_define(io_alias,std::forward<T2>(t2));
}

enum class width_mode
{
left,middle,right,internal
};

template<width_mode wm,typename T>
requires (static_cast<std::size_t>(wm)<4)
struct width_t
{
	using manip_tag = manip_tag_t;
	T reference;
	std::size_t width;
};

template<width_mode wm,typename T,std::integral char_type>
requires (static_cast<std::size_t>(wm)<4)
struct width_ch_t
{
	using manip_tag = manip_tag_t;
	T reference;
	std::size_t width;
	char_type ch;
};

template<typename T>
inline constexpr auto left_width(T&& t,std::size_t w) noexcept
{
	using value_type = std::remove_cvref_t<T>;
	if constexpr(alias_printable<std::remove_cvref_t<T>>)
		return width_t<width_mode::left,std::remove_cvref_t<decltype(print_alias_define(io_alias,t))>>{print_alias_define(io_alias,t),w};
	else if constexpr(std::is_trivially_copyable_v<value_type>&&sizeof(value_type)<=sizeof(std::max_align_t))
		return width_t<width_mode::left,value_type>{t,w};
	else
		return width_t<width_mode::left,std::remove_reference_t<T> const&>{t,w};
}

template<typename T>
inline constexpr auto middle_width(T&& t,std::size_t w) noexcept
{
	using value_type = std::remove_cvref_t<T>;
	if constexpr(alias_printable<std::remove_cvref_t<T>>)
		return width_t<width_mode::middle,std::remove_cvref_t<decltype(print_alias_define(io_alias,t))>>{print_alias_define(io_alias,t),w};
	else if constexpr(std::is_trivially_copyable_v<value_type>&&sizeof(value_type)<=sizeof(std::max_align_t))
		return width_t<width_mode::middle,value_type>{t,w};
	else
		return width_t<width_mode::middle,std::remove_reference_t<T> const&>{t,w};
}

template<typename T>
inline constexpr auto right_width(T&& t,std::size_t w) noexcept
{
	using value_type = std::remove_cvref_t<T>;
	if constexpr(alias_printable<std::remove_cvref_t<T>>)
		return width_t<width_mode::right,std::remove_cvref_t<decltype(print_alias_define(io_alias,t))>>{print_alias_define(io_alias,t),w};
	else if constexpr(std::is_trivially_copyable_v<value_type>&&sizeof(value_type)<=sizeof(std::max_align_t))
		return width_t<width_mode::right,value_type>{t,w};
	else
		return width_t<width_mode::right,std::remove_reference_t<T> const&>{t,w};
}

template<typename T>
inline constexpr auto internal_width(T&& t,std::size_t w) noexcept
{
	using value_type = std::remove_cvref_t<T>;
	if constexpr(alias_printable<std::remove_cvref_t<T>>)
		return width_t<width_mode::internal,std::remove_cvref_t<decltype(print_alias_define(io_alias,t))>>{print_alias_define(io_alias,t),w};
	else if constexpr(std::is_trivially_copyable_v<value_type>&&sizeof(value_type)<=sizeof(std::max_align_t))
		return width_t<width_mode::internal,value_type>{t,w};
	else
		return width_t<width_mode::internal,std::remove_reference_t<T> const&>{t,w};
}

template<width_mode mode,typename T>
inline constexpr auto width(T&& t,std::size_t w) noexcept
{
	using value_type = std::remove_cvref_t<T>;
	if constexpr(alias_printable<std::remove_cvref_t<T>>)
		return width_t<mode,std::remove_cvref_t<decltype(print_alias_define(io_alias,t))>>{print_alias_define(io_alias,t),w};
	else if constexpr(std::is_trivially_copyable_v<value_type>&&sizeof(value_type)<=sizeof(std::max_align_t))
		return width_t<mode,value_type>{t,w};
	else
		return width_t<mode,std::remove_reference_t<T> const&>{t,w};
}

template<typename T,std::integral char_type>
inline constexpr auto left_width(T&& t,std::size_t w,char_type ch) noexcept
{
	using value_type = std::remove_cvref_t<T>;
	if constexpr(alias_printable<std::remove_cvref_t<T>>)
		return width_ch_t<width_mode::left,std::remove_cvref_t<decltype(print_alias_define(io_alias,t))>,std::remove_cvref_t<char_type>>{print_alias_define(io_alias,t),w,ch};
	else if constexpr(std::is_trivially_copyable_v<value_type>&&sizeof(value_type)<=sizeof(std::max_align_t))
		return width_ch_t<width_mode::left,value_type,std::remove_cvref_t<char_type>>{t,w,ch};
	else
		return width_ch_t<width_mode::left,std::remove_reference_t<T> const,std::remove_cvref_t<char_type>>{t,w,ch};
}

template<typename T,std::integral char_type>
inline constexpr auto right_width(T&& t,std::size_t w,char_type ch) noexcept
{
	using value_type = std::remove_cvref_t<T>;
	if constexpr(alias_printable<std::remove_cvref_t<T>>)
		return width_ch_t<width_mode::right,std::remove_cvref_t<decltype(print_alias_define(io_alias,t))>,std::remove_cvref_t<char_type>>{print_alias_define(io_alias,t),w,ch};
	else if constexpr(std::is_trivially_copyable_v<value_type>&&sizeof(value_type)<=sizeof(std::max_align_t))
		return width_ch_t<width_mode::right,value_type,std::remove_cvref_t<char_type>>{t,w,ch};
	else
		return width_ch_t<width_mode::right,std::remove_reference_t<T> const&,std::remove_cvref_t<char_type>>{t,w,ch};
}

template<typename T,std::integral char_type>
inline constexpr auto middle_width(T&& t,std::size_t w,char_type ch) noexcept
{
	using value_type = std::remove_cvref_t<T>;
	if constexpr(alias_printable<std::remove_cvref_t<T>>)
		return width_ch_t<width_mode::middle,std::remove_cvref_t<decltype(print_alias_define(io_alias,t))>,std::remove_cvref_t<char_type>>{print_alias_define(io_alias,t),w,ch};
	else if constexpr(std::is_trivially_copyable_v<value_type>&&sizeof(value_type)<=sizeof(std::max_align_t))
		return width_ch_t<width_mode::middle,value_type,std::remove_cvref_t<char_type>>{t,w,ch};
	else
		return width_ch_t<width_mode::middle,std::remove_reference_t<T> const&,std::remove_cvref_t<char_type>>{t,w,ch};
}

template<typename T,std::integral char_type>
inline constexpr auto internal_width(T&& t,std::size_t w,char_type ch) noexcept
{
	using value_type = std::remove_cvref_t<T>;
	if constexpr(alias_printable<std::remove_cvref_t<T>>)
		return width_ch_t<width_mode::internal,std::remove_cvref_t<decltype(print_alias_define(io_alias,t))>,std::remove_cvref_t<char_type>>{print_alias_define(io_alias,t),w,ch};
	else if constexpr(std::is_trivially_copyable_v<value_type>&&sizeof(value_type)<=sizeof(std::max_align_t))
		return width_ch_t<width_mode::internal,value_type,std::remove_cvref_t<char_type>>{t,w,ch};
	else
		return width_ch_t<width_mode::internal,std::remove_reference_t<T> const&,std::remove_cvref_t<char_type>>{t,w,ch};
}

template<width_mode mode,typename T,std::integral char_type>
inline constexpr auto width(T&& t,std::size_t w,char_type ch) noexcept
{
	using value_type = std::remove_cvref_t<T>;
	if constexpr(alias_printable<std::remove_cvref_t<T>>)
		return width_ch_t<mode,std::remove_cvref_t<decltype(print_alias_define(io_alias,t))>,std::remove_cvref_t<char_type>>{print_alias_define(io_alias,t),w,ch};
	else if constexpr(std::is_trivially_copyable_v<value_type>&&sizeof(value_type)<=sizeof(std::max_align_t))
		return width_ch_t<mode,value_type,std::remove_cvref_t<char_type>>{t,w,ch};
	else
		return width_ch_t<mode,std::remove_reference_t<T> const&,std::remove_cvref_t<char_type>>{t,w,ch};
}

}