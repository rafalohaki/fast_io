#pragma once

namespace fast_io
{

template<zero_copy_input_stream handletype,buffer_mode mde,std::size_t bfs,std::size_t alignsz>
requires ((mde&buffer_mode::in)==buffer_mode::in)
inline constexpr decltype(auto) zero_copy_in_handle(basic_io_buffer<handletype,mde,bfs,alignsz>& bios)
{
	return zero_copy_in_handle(bios.handle);
}

}