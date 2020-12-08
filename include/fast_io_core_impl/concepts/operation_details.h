#pragma once

namespace fast_io::details
{

template<typename T,typename output,typename input>
concept scan_reserve_transmit_impl = requires(T t,output out,input in)
{
	{scan_reserve_transmit(io_reserve_type<typename input::char_type,std::remove_cvref_t<T>>,out,in)}->std::convertible_to<bool>;
};

template<typename T,typename input>
concept space_scan_reserve_impl = requires(T t,typename input::char_type* ptr)
{
	{space_scan_reserve_define(io_reserve_type<typename input::char_type,std::remove_cvref_t<T>,true>,ptr,ptr,t)}->std::convertible_to<char8_t const*>;
};
/*
template<typename T,typename input>
concept space_scan_reserve_sized_transmit_impl = requires(T t)
{
	{space_scan_reserve_size(io_reserve_type<std::remove_cvref_t<T>>)}->std::convertible_to<std::size_t>;
	requires requires(typename input::char_type* ptr)
	{
		{space_scan_reserve_size_transmit(io_reserve_type<std::remove_cvref_t<T>>,ptr,t)}->std::convertible_to<bool>;
	};
};
*/

template<typename ch_type>
concept character = std::integral<ch_type>&&(std::same_as<ch_type,char>||std::same_as<ch_type,wchar_t>||
std::same_as<ch_type,char8_t>||std::same_as<ch_type,char32_t>);

}