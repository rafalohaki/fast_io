#pragma once

namespace fast_io
{

template<stream handletype,buffer_mode mde,typename decorators_type,std::size_t bfs>
inline constexpr void flush(basic_io_buffer<handletype,mde,decorators_type,bfs>& bios)
{
	if constexpr((mde&buffer_mode::out)==buffer_mode::out)
	{
		if constexpr(details::has_external_decorator_impl<decorators_type>)
			details::iobuf_output_flush_impl_deco(io_ref(bios.handle),io_deco_ref(external_decorator(bios.decorators)),bios.obuffer);
		else
			details::iobuf_output_flush_impl(io_ref(bios.handle),bios.obuffer);
	}
	if constexpr((mde&buffer_mode::in)==buffer_mode::in)
		bios.ibuffer.buffer_end=bios.ibuffer.buffer_curr=bios.ibuffer.buffer_begin;
}

template<stream handletype,buffer_mode mde,typename decorators_type,std::size_t bfs>
requires ((mde&buffer_mode::secure_clear)==buffer_mode::secure_clear)
inline constexpr void require_secure_clear(basic_io_buffer<handletype,mde,decorators_type,bfs>&){}
}