#pragma once

namespace fast_io
{

template<input_stream input>
requires (!buffer_input_stream<input>)
struct single_character_input_buffer
{
public:
	using char_type = typename input::char_type;
	input& reference{};
	char_type single_character{};
	bool pos{};
	bool pos_end{};
};

template<input_stream input,typename... Args>
constexpr decltype(auto) read(single_character_input_buffer<input>& in,Args&& ... args)
{
	return read(in.reference,std::forward<Args>(args)...);
}

template<input_stream input>
constexpr auto ibuffer_begin(single_character_input_buffer<input>& in)
{
	return std::addressof(in.single_character);
}
template<input_stream input>
constexpr auto ibuffer_curr(single_character_input_buffer<input>& in)
{
	return std::addressof(in.single_character)+static_cast<std::size_t>(in.pos);
}
template<input_stream input>
constexpr auto ibuffer_end(single_character_input_buffer<input>& in)
{
	return std::addressof(in.single_character)+static_cast<std::size_t>(in.pos_end);
}

template<input_stream input>
constexpr void ibuffer_set_curr(single_character_input_buffer<input>& in,typename input::char_type* ptr)
{
	in.pos=(ptr!=std::addressof(in.single_character));
}

template<input_stream input>
constexpr bool underflow(single_character_input_buffer<input>& in)
{
	in.pos_end=(read(in.reference,std::addressof(in.single_character),std::addressof(in.single_character)+1)!=std::addressof(in.single_character));
	in.pos={};
	return in.pos_end;
}

template<input_stream input>
constexpr void avoid_scan_reserve(single_character_input_buffer<input>&){}

}