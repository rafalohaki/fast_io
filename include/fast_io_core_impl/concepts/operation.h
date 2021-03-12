#pragma once

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
	{ scan_context_define(scan_context<contiguous_only>, begin, end, t).code }->std::convertible_to<std::errc>;
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
concept secure_reserve_printable=std::integral<char_type>&&requires(T t,char_type* ptr)
{
	{print_reserve_size(io_secure_reserve_type<char_type,std::remove_cvref_t<T>>)}->std::convertible_to<std::size_t>;
	{print_reserve_define(io_secure_reserve_type<char_type,std::remove_cvref_t<T>>,ptr,t)}->std::convertible_to<char_type*>;
};

template<typename char_type,typename T>
concept dynamic_secure_reserve_printable=std::integral<char_type>&&requires(T t,char_type* ptr)
{
	{print_reserve_size(io_secure_reserve_type<char_type,std::remove_cvref_t<T>>,t)}->std::convertible_to<std::size_t>;
	{print_reserve_define(io_secure_reserve_type<char_type,std::remove_cvref_t<T>>,ptr,t)}->std::convertible_to<char_type*>;
};

template<typename char_type,typename T>
concept dynamic_reserve_serializable=dynamic_reserve_printable<char_type,T>&&requires(T t,char_type* ptr)
{
	{print_reserve_define(io_serial_type<char_type,std::remove_cvref_t<T>>,ptr,t)}->std::convertible_to<char_type*>;
};

template<typename char_type,typename T>
concept reverse_reserve_printable=std::integral<char_type>&&reserve_printable<char_type,T>&&requires(T t,char_type* ptr)
{
	{print_reverse_reserve_define(io_reserve_type<char_type,std::remove_cvref_t<T>>,ptr,t)}->std::convertible_to<char_type*>;
};

template<typename char_type,typename T>
concept reverse_dynamic_reserve_printable=std::integral<char_type>&&dynamic_reserve_printable<char_type,T>&&requires(T t,char_type* ptr)
{
	{print_reverse_reserve_define(io_reserve_type<char_type,std::remove_cvref_t<T>>,ptr,t)}->std::convertible_to<char_type*>;
};

template<typename output,typename T>
concept printable=output_stream<output>&&requires(output out,T t)
{
	print_define(out,t);
};
template<typename char_type,typename T>
concept scatter_printable=requires(char_type ch,T&& t)
{
	{print_scatter_define(print_scatter_type<char_type>,std::forward<T>(t))}->std::convertible_to<io_scatter_t>;
};

template<typename char_type,typename T>
concept scatter_type_printable=scatter_printable<char_type,T>&&requires(char_type ch,T&& t)
{
	{print_scatter_define(print_scatter_type<char_type>,std::forward<T>(t))}->std::convertible_to<basic_io_scatter_t<char_type>>;
};

template<typename output,typename T>
concept general_printable=reserve_printable<typename output::char_type,T>||printable<output,T>;

template<typename char_type,typename T>
concept alias_type_scanable=std::integral<char_type>&&requires(T&& t)
{
	scan_alias_define(io_alias_type<char_type>,std::forward<T>(t));
};

template<typename T>
concept alias_scanable=requires(T&& t)
{
	scan_alias_define(io_alias,std::forward<T>(t));
};

template<typename T>
concept alias_printable=requires(T&& t)
{
	print_alias_define(io_alias,std::forward<T>(t));
};
template<typename io_device,typename... Args>
concept io_controllable=requires(io_device device,Args&& ...args)
{
	io_control(device,std::forward<Args>(args)...);
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

template<typename output,typename value_type>
requires printable<output,std::remove_cvref_t<value_type>>
constexpr void print_define(output out, parameter<value_type> wrapper)
{
	print_define(out,wrapper.reference);
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

template<std::integral char_type,typename value_type>
requires scatter_printable<char_type,std::remove_cvref_t<value_type>>
constexpr auto print_scatter_define(print_scatter_type_t<char_type>,parameter<value_type> para)
{
	return print_scatter_define(print_scatter_type<char_type>,para.reference);
}

}