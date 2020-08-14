#pragma once

namespace fast_io
{

template<typename input>
class icrtp{};

template<typename input>
requires requires(input& inp)
{
	inp.icrtp_handle();
}
constexpr decltype(auto) icrtp_handle(icrtp<input>& inp)
{
	return static_cast<input&>(inp).icrtp_handle();
}

template<typename input>
requires requires(icrtp<input>& inp)
{
	icrtp_handle(inp);
}
using icrtp_handle_type=std::remove_cvref_t<decltype(icrtp_handle(*reinterpret_cast<input*>(0)))>;

template<typename input,typename... Args>
requires input_stream<icrtp_handle_type<input>>
constexpr decltype(auto) read(icrtp<input>& in,Args&& ...args)
{
	return read(icrtp_handle(in),std::forward<Args>(args)...);
}

template<typename input>
requires character_input_stream<icrtp_handle_type<input>>
constexpr decltype(auto) igenerator(icrtp<input>& in)
{
	return igenerator(icrtp_handle(in));
}
template<typename input>
requires buffer_input_stream<icrtp_handle_type<input>>
constexpr decltype(auto) underflow(icrtp<input>& in)
{
	return underflow(icrtp_handle(in));
}

template<typename input>
requires buffer_input_stream<icrtp_handle_type<input>>
constexpr decltype(auto) ibuffer_begin(icrtp<input>& in)
{
	return ibuffer_begin(icrtp_handle(in));
}

template<typename input>
requires buffer_input_stream<icrtp_handle_type<input>>
constexpr decltype(auto) ibuffer_curr(icrtp<input>& in)
{
	return ibuffer_curr(icrtp_handle(in));
}

template<typename input>
requires buffer_input_stream<icrtp_handle_type<input>>
constexpr decltype(auto) ibuffer_end(icrtp<input>& in)
{
	return ibuffer_end(icrtp_handle(in));
}

template<typename input>
requires capacity_available_buffer_input_stream<icrtp_handle_type<input>>
constexpr decltype(auto) ibuffer_cap(icrtp<input>& in)
{
	return ibuffer_cap(icrtp_handle(in));
}

template<typename input,typename... Args>
requires buffer_input_stream<icrtp_handle_type<input>>
constexpr decltype(auto) ibuffer_set_curr(icrtp<input>& in,Args&& ...args)
{
	return ibuffer_set_curr(icrtp_handle(in),std::forward<Args>(args)...);
}

template<typename input>
requires refill_buffer_input_stream<icrtp_handle_type<input>>
constexpr decltype(auto) irefill(icrtp<input>& in)
{
	return irefill(icrtp_handle(in));
}

template<typename input>
requires zero_copy_input_stream<icrtp_handle_type<input>>
constexpr decltype(auto) zero_copy_in_handle(icrtp<input>& in)
{
	return zero_copy_in_handle(icrtp_handle(in));
}

template<typename input>
requires memory_map_input_stream<icrtp_handle_type<input>>
constexpr decltype(auto) memory_map_in_handle(icrtp<input>& in)
{
	return memory_map_in_handle(icrtp_handle(in));
}
template<typename input,typename... Args>
requires scatter_input_stream<icrtp_handle_type<input>>
constexpr decltype(auto) scatter_read(icrtp<input>& in,Args&& ...args)
{
	return scatter_read(icrtp_handle(in),std::forward<Args>(args)...);
}

template<typename input>
requires requires(input inp)
{
	ibuffer_initialize(inp);
}
constexpr void ibuffer_initialize(icrtp<input>& inp)
{
	ibuffer_initialize(inp);
}


}