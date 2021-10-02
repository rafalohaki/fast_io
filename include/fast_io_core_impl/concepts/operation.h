﻿#pragma once

namespace fast_io
{
template<typename input,typename T>
concept scanable=input_stream<input>&&requires(input& in,T t)
{
	{scan_define(in,t)}->std::convertible_to<bool>;
};

#if 0
template<typename char_type, typename T>
concept reserve_scanable = requires(char_type const* begin,char_type* result, T t)
{
	requires requires()
	{
		constexpr std::size_t sz{scan_reserve_size(io_reserve_type<char_type,T>)};
	}
	{scan_reserve_define(begin,begin,result,t,t,io_reserve_type<char_type,T>)}
	{scan_reserve_ctx_define(begin,begin,result,result,t,io_reserve_type<char_type,T>)}
};
#endif

template<typename char_type, typename T, bool contiguous_only = false>
concept context_scanable = requires(char_type const* begin, char_type const* end, T t)
{
	{ scan_context_define(scan_context<contiguous_only>, begin, end, t).iter }->std::convertible_to<char_type const*>;
	{ scan_context_define(scan_context<contiguous_only>, begin, end, t).code }->std::convertible_to<parse_code>;
};

template<typename char_type,typename T>
concept scanable_skipping = requires(scan_skip_type_t<T> t, char_type const* begin, char_type const* end)
{
	{ scan_skip_define(scan_skip_type<T>, begin, end) }->std::convertible_to<char_type const*>;
};

template<typename char_type,typename T>
concept skipper = requires(char_type const* begin, char_type const* end,T t)
{
	{ skip_define(begin, end, t) }->std::convertible_to<char_type const*>;
};


template<typename char_type, typename T>
concept contiguous_scanable = requires(char_type const* begin, char_type const* end, T t)
{
	{scan_contiguous_define(io_reserve_type<char_type,T>,begin,end,t)}->std::convertible_to<parse_result<char_type const*>>;
};
#if 0
//potentially useful
template<typename char_type, typename T>
concept contiguous_test_scanable = contiguous_scanable<char_type,T>&&requires(char_type ch,T t)
{
	{scan_contiguous_test(io_reserve_type<char_type,T>,ch)}->std::convertible_to<bool>;
};
#endif

template<typename char_type, typename T>
concept context_scanable2 = requires(char_type const* begin, char_type const* end, T t)
{
	requires requires(typename std::remove_cvref_t<decltype(scan_context_type(io_reserve_type<char_type,T>))>::type st)
	{
		{scan_context_define2(io_reserve_type<char_type,T>,st,begin,end,t)}->std::convertible_to<parse_result<char_type const*>>;
		{scan_context_eof_define(io_reserve_type<char_type,T>,st,t)}->std::convertible_to<parse_code>;
	};
};
#if 0
template<typename char_type, typename T>
concept contiguous_context_scanable = contiguous_scanable<char_type,T> && context_scanable2<char_type, T> && requires(char_type const* begin, char_type const* end, T t,parse_code code)
{
	scan_contiguous_context_verified_define(io_reserve_type<char_type,T>,scan_context_construct(io_reserve_type<char_type,T>,t),begin,end,t,code);
};
#endif

template<typename char_type,typename T>
concept reserve_printable=std::integral<char_type>&&requires(T t,char_type* ptr)
{
	{print_reserve_size(io_reserve_type<char_type,std::remove_cvref_t<T>>)}->std::convertible_to<std::size_t>;
	{print_reserve_define(io_reserve_type<char_type,std::remove_cvref_t<T>>,ptr,t)}->std::convertible_to<char_type*>;
};

template<typename char_type,typename T>
concept reserve_serializable=reserve_printable<char_type,T>&&requires(T t,char_type* ptr)
{
	{print_reserve_define(io_serial_type<char_type,std::remove_cvref_t<T>>,ptr,t)}->std::convertible_to<char_type*>;
};

template<typename char_type,typename T>
concept dynamic_reserve_printable=std::integral<char_type>&&requires(T t,char_type* ptr)
{
	{print_reserve_size(io_reserve_type<char_type,std::remove_cvref_t<T>>,t)}->std::convertible_to<std::size_t>;
	{print_reserve_define(io_reserve_type<char_type,std::remove_cvref_t<T>>,ptr,t)}->std::convertible_to<char_type*>;
};

template<typename char_type,typename T>
concept printable_internal_shift=requires(T t)
{
	{print_define_internal_shift(io_reserve_type<char_type,std::remove_cvref_t<T>>,t)}->std::same_as<std::size_t>;
};

template<typename char_type,typename T>
concept precise_reserve_printable=std::integral<char_type>&&(reserve_printable<char_type,T>||dynamic_reserve_printable<char_type,T>)&&requires(T t,char_type* ptr,std::size_t n)
{
	{print_reserve_precise_size(io_reserve_type<char_type,std::remove_cvref_t<T>>,t)}->std::convertible_to<std::size_t>;
	print_reserve_precise_define(io_reserve_type<char_type,std::remove_cvref_t<T>>,ptr,n,t);
};

template<typename char_type,typename T>
concept printable=requires(::fast_io::details::dummy_buffer_output_stream<char_type> out,T t)
{
	print_define(io_reserve_type<char_type,std::remove_cvref_t<T>>,out,t);
};

template<typename char_type,typename T>
concept scatter_printable=requires(char_type ch,T&& t)
{
	{print_scatter_define(io_reserve_type<char_type,std::remove_cvref_t<T>>,::fast_io::freestanding::forward<T>(t))}->std::same_as<basic_io_scatter_t<char_type>>;
};

template<typename T>
concept alias_scanable=requires(T&& t)
{
	scan_alias_define(io_alias,t);
};

template<typename T>
concept alias_printable=requires(T&& t)
{
	print_alias_define(io_alias,::fast_io::freestanding::forward<T>(t));
};

template<typename char_type,typename T>
concept status_io_scan_forwardable=std::integral<char_type>&&requires(T t)
{
	status_io_scan_forward(io_alias_type<char_type>,t);
};

template<typename char_type,typename T>
concept status_io_print_forwardable=std::integral<char_type>&&requires(T&& t)
{
	status_io_print_forward(io_alias_type<char_type>,::fast_io::freestanding::forward<T>(t));
};

template<typename io_device,typename... Args>
concept io_controllable=requires(io_device device,Args&& ...args)
{
	io_control(device,::fast_io::freestanding::forward<Args>(args)...);
};

struct manip_tag_t{};

template<typename T>
concept manipulator = requires(T t)
{
	typename T::manip_tag;
};

template<typename T>
struct parameter
{
	using manip_tag = manip_tag_t;
	T reference;
};

template<std::integral char_type,typename output,typename value_type>
requires (printable<char_type,std::remove_cvref_t<value_type>>&&std::is_trivially_copyable_v<output>)
constexpr void print_define(io_reserve_type_t<char_type,parameter<value_type>>,output out, parameter<value_type> wrapper)
{
	print_define(io_reserve_type<char_type,std::remove_cvref_t<value_type>>,out,wrapper.reference);
}

template<std::integral char_type,typename value_type>
requires reserve_printable<char_type,std::remove_cvref_t<value_type>>
constexpr std::size_t print_reserve_size(io_reserve_type_t<char_type,parameter<value_type>>)
{
	return print_reserve_size(io_reserve_type<char_type,std::remove_cvref_t<value_type>>);
}

template<std::integral char_type,typename value_type>
requires dynamic_reserve_printable<char_type,std::remove_cvref_t<value_type>>
constexpr std::size_t print_reserve_size(io_reserve_type_t<char_type,parameter<value_type>>,parameter<value_type> para)
{
	return print_reserve_size(io_reserve_type<char_type,std::remove_cvref_t<value_type>>,para.reference);
}

template<std::integral char_type,typename value_type,typename Iter>
requires (reserve_printable<char_type,std::remove_cvref_t<value_type>>||dynamic_reserve_printable<char_type,std::remove_cvref_t<value_type>>)
constexpr auto print_reserve_define(io_reserve_type_t<char_type,parameter<value_type>>,Iter begin,parameter<value_type> para)
{
	return print_reserve_define(io_reserve_type<char_type,std::remove_cvref_t<value_type>>,begin,para.reference);
}

template<std::integral char_type,typename value_type,typename Iter>
requires (printable_internal_shift<char_type,std::remove_cvref_t<value_type>>)
constexpr auto print_define_internal_shift(io_reserve_type_t<char_type,parameter<value_type>>,Iter begin,parameter<value_type> para)
{
	return print_define_internal_shift(io_reserve_type<char_type,std::remove_cvref_t<value_type>>,begin,para.reference);
}

template<std::integral char_type,typename value_type>
requires precise_reserve_printable<char_type,std::remove_cvref_t<value_type>>
constexpr std::size_t print_reserve_precise_size(io_reserve_type_t<char_type,parameter<value_type>>,parameter<value_type> para)
{
	return print_reserve_precise_size(io_reserve_type<char_type,std::remove_cvref_t<value_type>>,para.reference);
}

template<std::integral char_type,typename value_type,typename Iter>
requires precise_reserve_printable<char_type,std::remove_cvref_t<value_type>>
constexpr void print_reserve_precise_define(io_reserve_type_t<char_type,parameter<value_type>>,Iter begin,std::size_t n,parameter<value_type> para)
{
	print_reserve_precise_define(io_reserve_type<char_type,std::remove_cvref_t<value_type>>,begin,n,para.reference);
}

template<std::integral char_type,typename value_type>
requires scatter_printable<char_type,std::remove_cvref_t<value_type>>
constexpr auto print_scatter_define(io_reserve_type_t<char_type,parameter<value_type>>,parameter<value_type> para)
{
	return print_scatter_define(io_reserve_type<char_type,std::remove_cvref_t<value_type>>,para.reference);
}

namespace manipulators
{}

namespace mnp=manipulators;

}
