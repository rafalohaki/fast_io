#pragma once

namespace fast_io
{

template<std::integral char_type,typename T>
inline constexpr decltype(auto) io_scan_alias(T&& t)
{
	if constexpr(alias_scannable<char_type,std::remove_cvref_t<T>>)
		return scan_alias_define(io_alias<char_type>,std::forward<T>(t));
	else
		return std::forward<T>(t);
}

template<output_stream output,typename ...Args>
inline constexpr void scan_freestanding(input&& in,Args&& ...args)
{
	scan_freestanding_decay(io_ref(in),io_forward(io_scan_alias<typename std::remove_cvref_t<output>::char_type>(args))...);
}

}