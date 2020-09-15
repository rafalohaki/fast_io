#pragma once

namespace fast_io
{

template<typename input,typename T>
concept space_scanable=input_stream<input>&&requires(input& in,T&& t)
{
	space_scan_define(in,std::forward<T>(t));
};

template<typename input,typename T>
concept scanable=input_stream<input>&&requires(input& in,T&& t)
{
	scan_define(in,std::forward<T>(t));
};

template<typename T,typename output,typename input>
concept reserve_scanable=(details::scan_reserve_transmit_impl<T,output,input>||scanable<T,input>)&&requires(T t,char8_t const* ptr,input in)
{
	scan_reserve_skip(io_reserve_type<std::remove_cvref_t<T>>,in);
	{scan_reserve_define(io_reserve_type<std::remove_cvref_t<T>,true>,ptr,ptr,t)}->std::convertible_to<char8_t const*>;
};

template<typename T,typename output,typename input>
concept reserve_space_scanable=(details::scan_reserve_transmit_impl<T,output,input>||space_scanable<T,input>)&&
	requires(T t,char8_t const* ptr)
{
	{space_scan_reserve_define(io_reserve_type<std::remove_cvref_t<T>,true>,ptr,ptr,t)}->std::convertible_to<char8_t const*>;
};

template<typename T,typename output,typename input>
concept general_reserve_scanable=reserve_space_scanable<T,output,input>||reserve_scanable<T,output,input>;

template<typename T>
concept reserve_size_scanable=requires()
{
	{scan_reserve_size(io_reserve_type<std::remove_cvref_t<T>>)}->std::convertible_to<std::size_t>;
};

template<typename input>
concept reserve_scan_avoidance = requires(input in)
{
	avoid_scan_reserve(in);
};

template<typename T>
concept reserve_printable=requires(T&& t,char8_t* ptr)
{
	{print_reserve_size(io_reserve_type<std::remove_cvref_t<T>>)}->std::convertible_to<std::size_t>;
	{print_reserve_define(io_reserve_type<std::remove_cvref_t<T>>,ptr,t)}->std::convertible_to<char8_t*>;
};

template<typename T>
concept reverse_reserve_printable=reserve_printable<T>&&requires(T&& t,char8_t* ptr)
{
	{print_reverse_reserve_define(io_reserve_type<std::remove_cvref_t<T>>,ptr,t)}->std::convertible_to<char8_t*>;
};

template<typename T>
concept reserve_print_testable=requires(T&& t)
{
	{print_reserve_test<static_cast<std::size_t>(0)>(io_reserve_type<std::remove_cvref_t<T>>,std::forward<T>(t))}->std::convertible_to<bool>;
}&&reserve_printable<T>;

template<typename output,typename T>
concept printable=output_stream<output>&&requires(output& out,T&& t)
{
	print_define(out,std::forward<T>(t));
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

template<typename input,typename T>
concept general_scanable=space_scanable<input,T>||scanable<input,T>;

template<typename output,typename T>
concept general_printable=reserve_printable<T>||printable<output,T>;


template<typename T>
concept alias_scanable=requires(T&& t)
{
	scan_alias_define(std::forward<T>(t));
};

template<typename T>
concept alias_printable=requires(T&& t)
{
	print_alias_define(io_alias_type<std::remove_cvref_t<T>>,std::forward<T>(t));
};

template<typename io_device,typename... Args>
concept io_controllable=requires(io_device& device,Args&& ...args)
{
	io_control(device,std::forward<Args>(args)...);
};

struct manip_tag_t{};

template<typename T>
concept manipulator = std::same_as<typename T::manip_tag,manip_tag_t>&&requires(T t)
{
	{t.reference};
};

}