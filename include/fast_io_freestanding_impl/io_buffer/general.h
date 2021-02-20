#pragma once

namespace fast_io
{

template<stream handletype,typename decorators_type,buffer_mode mde,std::size_t bfs,std::size_t alignsz>
inline constexpr void flush(basic_io_buffer<handletype,decorators_type,mde,bfs,alignsz>& bios)
{
	if constexpr((mde&buffer_mode::out)==buffer_mode::out)
	{
		if constexpr(details::has_external_decorator_impl<decorators_type>)
			details::iobuf_output_flush_impl_deco(io_ref(bios.handle),io_deco_ref(external(bios.decorators)),bios.obuffer);
		else
			details::iobuf_output_flush_impl(io_ref(bios.handle),bios.obuffer);
	}
	if constexpr((mde&buffer_mode::in)==buffer_mode::in)
		bios.ibuffer.buffer_end=bios.ibuffer.buffer_curr=bios.ibuffer.buffer_begin;
}
#if 0
template<stream handletype,typename internaldecorator,typename externaldecorator,buffer_mode mde,std::size_t bfs,std::size_t alignsz>
inline constexpr void require_secure_clear(basic_io_buffer<handletype,internaldecorator,externaldecorator,mde,bfs,alignsz>&){}
#endif
}