#pragma once
#include"input_normal.h"
#include"input_deco.h"

namespace fast_io
{

template<zero_copy_input_stream handletype,
buffer_mode mde,
typename decorators,
std::size_t bfs>
requires ((mde&buffer_mode::in)==buffer_mode::in&&!details::has_internal_decorator_impl<decorators>)
inline constexpr decltype(auto) zero_copy_in_handle(basic_io_buffer<handletype,mde,decorators,bfs>& bios)
{
	return zero_copy_in_handle(bios.handle);
}

template<stream handletype,
buffer_mode mde,
typename decorators,
std::size_t bfs>
requires ((mde&buffer_mode::in)==buffer_mode::in)
inline constexpr auto ibuffer_begin(basic_io_buffer<handletype,mde,decorators,bfs>& bios) noexcept
{
	return bios.ibuffer.buffer_begin;
}

template<stream handletype,
buffer_mode mde,
typename decorators,
std::size_t bfs>
requires ((mde&buffer_mode::in)==buffer_mode::in)
inline constexpr auto ibuffer_curr(basic_io_buffer<handletype,mde,decorators,bfs>& bios) noexcept
{
	return bios.ibuffer.buffer_curr;
}

template<stream handletype,
buffer_mode mde,
typename decorators,
std::size_t bfs>
requires ((mde&buffer_mode::in)==buffer_mode::in)
inline constexpr auto ibuffer_end(basic_io_buffer<handletype,mde,decorators,bfs>& bios) noexcept
{
	return bios.ibuffer.buffer_end;
}

template<stream handletype,
buffer_mode mde,
typename decorators,
std::size_t bfs>
requires ((mde&buffer_mode::in)==buffer_mode::in)
inline constexpr void ibuffer_set_curr(basic_io_buffer<handletype,mde,decorators,bfs>& bios,typename basic_io_buffer<handletype,mde,decorators,bfs>::char_type* ptr) noexcept
{
	bios.ibuffer.buffer_curr=ptr;
}


template<stream handletype,
buffer_mode mde,
typename decoratorstype,
std::size_t bfs>
requires ((mde&buffer_mode::in)==buffer_mode::in)
inline constexpr bool underflow(basic_io_buffer<handletype,mde,decoratorstype,bfs>& bios)
{
	if constexpr(((mde&buffer_mode::out)==buffer_mode::out)&&
	((mde&buffer_mode::tie)==buffer_mode::tie))
	{
		if constexpr(details::has_external_decorator_impl<decoratorstype>)
			details::iobuf_output_flush_impl_deco(io_ref(bios.handle),external_decorator(bios.decorators),bios.obuffer,bfs);
		else
			details::iobuf_output_flush_impl(io_ref(bios.handle),bios.obuffer);
	}
	if constexpr(details::has_internal_decorator_impl<decoratorstype>)
		return details::underflow_impl_deco<bfs>(io_ref(bios.handle),internal_decorator(bios.decorators),bios.ibuffer);
	else
		return details::underflow_impl<bfs>(io_ref(bios.handle),bios.ibuffer);
}

namespace details
{


template<typename T,std::random_access_iterator Iter>
inline constexpr Iter iobuf_read_unhappy_impl(T& bios,Iter first,Iter last)
{
	if constexpr(((T::mode&buffer_mode::out)==buffer_mode::out)&&((T::mode&buffer_mode::tie)==buffer_mode::tie))
	{
		if constexpr(details::has_external_decorator_impl<typename T::decorators_type>)
			iobuf_output_flush_impl_deco(io_ref(bios.handle),io_deco_ref(external_decorator(bios.decorators)),bios.obuffer,T::buffer_size);
		else
			iobuf_output_flush_impl(io_ref(bios.handle),bios.obuffer);
	}
	first=non_overlapped_copy(bios.ibuffer.buffer_curr,bios.ibuffer.buffer_end,first);
	bios.ibuffer.buffer_curr=bios.ibuffer.buffer_end;
	if constexpr(details::has_internal_decorator_impl<typename T::decorators_type>)
		return iobuf_read_unhappy_decay_impl_deco(io_ref(bios.handle),io_deco_ref(internal_decorator(bios.decorators)),bios.ibuffer,first,last,T::buffer_size);
	else
		return iobuf_read_unhappy_decay_impl(io_ref(bios.handle),bios.ibuffer,first,last,T::buffer_size);
}

}

template<stream handletype,
buffer_mode mde,
typename decorators,
std::size_t bfs,std::random_access_iterator Iter>
requires (((mde&buffer_mode::in)==buffer_mode::in)&&details::allow_iobuf_punning<typename decorators::internal_type,Iter>)
inline constexpr Iter read(basic_io_buffer<handletype,mde,decorators,bfs>& bios,Iter first,Iter last)
{
	using iter_char_type = std::iter_value_t<Iter>;
	using char_type = typename decorators::internal_type;
	if constexpr(std::same_as<iter_char_type,char_type>)
	{
		if constexpr(std::contiguous_iterator<Iter>&&!std::is_pointer_v<Iter>)
		{
			return first+(read(bios,std::to_address(first),std::to_address(last))-std::to_address(first));
		}
		else
		{
			std::size_t diff(static_cast<std::size_t>(last-first));
			std::size_t remain_space(bios.ibuffer.buffer_end-bios.ibuffer.buffer_curr);
			if(remain_space<diff)[[unlikely]]
				return details::iobuf_read_unhappy_impl(bios,first,last);
			first=details::non_overlapped_copy_n(bios.ibuffer.buffer_curr,diff,first);
			bios.ibuffer.buffer_curr+=diff;
			return first;
		}
	}
	else
	{
		auto newb{reinterpret_cast<char*>(std::to_address(first))};
		auto ret{read(bios,newb,
			reinterpret_cast<char*>(std::to_address(last)))};
		return first+(ret-newb)/sizeof(*first);
	}
}

}
