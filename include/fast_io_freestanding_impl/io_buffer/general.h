#pragma once

namespace fast_io
{

template<stream handletype,buffer_mode mde,std::size_t bfs,std::size_t alignsz>
inline constexpr void flush(basic_io_buffer<handletype,mde,bfs,alignsz>& bios)
{
	if constexpr((mde&buffer_mode::out)==buffer_mode::out)
		details::iobuf_output_flush_impl(io_ref(bios.handle),bios.obuffer);
	if constexpr((mde&buffer_mode::in)==buffer_mode::in)
		bios.ibuffer.buffer_end=bios.ibuffer.buffer_curr=bios.ibuffer.buffer_begin;
}

template<stream handletype,buffer_mode mde,std::size_t bfs,std::size_t alignsz>
inline constexpr void require_secure_clear(basic_io_buffer<handletype,mde,bfs,alignsz>&){}

}