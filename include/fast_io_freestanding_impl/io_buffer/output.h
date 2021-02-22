#pragma once
#include"output_normal.h"
#include"output_deco.h"

namespace fast_io
{

namespace details
{

template<typename T,std::random_access_iterator Iter>
inline constexpr void iobuf_write_unhappy_impl(T& t,Iter first,Iter last)
{
	if constexpr(has_external_decorator_impl<typename T::decorators_type>)
		iobuf_write_unhappy_decay_impl_deco<T::buffer_size>(io_ref(t.handle),external_decorator(t.decorators),t.obuffer,first,last);
	else
		iobuf_write_unhappy_decay_impl<T::buffer_size>(io_ref(t.handle),t.obuffer,first,last);
}

}

template<stream handletype,
buffer_mode mde,
typename decorators,
std::size_t bfs,std::random_access_iterator Iter>
requires (((mde&buffer_mode::out)==buffer_mode::out)&&details::allow_iobuf_punning<typename decorators::internal_type,Iter>)
inline constexpr void write(basic_io_buffer<handletype,mde,decorators,bfs>& bios,Iter first,Iter last)
{
	using iter_char_type = std::iter_value_t<Iter>;
	using char_type = typename decorators::internal_type;
	if constexpr(std::same_as<iter_char_type,char_type>)
	{
		if constexpr(std::contiguous_iterator<Iter>&&!std::is_pointer_v<Iter>)
			write(bios,std::to_address(first),std::to_address(last));
		else
		{
			std::size_t diff(static_cast<std::size_t>(last-first));
			std::size_t remain_space(bios.obuffer.buffer_end-bios.obuffer.buffer_curr);
			if(remain_space<diff)[[unlikely]]
			{
				details::iobuf_write_unhappy_impl(bios,first,last);
				return;
			}
			bios.obuffer.buffer_curr=details::non_overlapped_copy_n(first,diff,bios.obuffer.buffer_curr);
		}
/*
To do : forward_iterator. Support std::forward_list, std::list, std::set and std::unordered_set
*/
	}
	else
		write(bios,reinterpret_cast<char const*>(std::to_address(first)),
			reinterpret_cast<char const*>(std::to_address(last)));
}


template<stream handletype,
buffer_mode mde,
typename decorators,
std::size_t bfs>
requires ((mde&buffer_mode::out)==buffer_mode::out)
inline constexpr auto obuffer_begin(basic_io_buffer<handletype,mde,decorators,bfs>& bios) noexcept
{
	return bios.obuffer.buffer_begin;
}

template<stream handletype,
buffer_mode mde,
typename decorators,
std::size_t bfs>
requires ((mde&buffer_mode::out)==buffer_mode::out)
inline constexpr auto obuffer_curr(basic_io_buffer<handletype,mde,decorators,bfs>& bios) noexcept
{
	return bios.obuffer.buffer_curr;
}

template<stream handletype,
buffer_mode mde,
typename decorators,
std::size_t bfs>
requires ((mde&buffer_mode::out)==buffer_mode::out)
inline constexpr auto obuffer_end(basic_io_buffer<handletype,mde,decorators,bfs>& bios) noexcept
{
	return bios.obuffer.buffer_end;
}

template<stream handletype,
buffer_mode mde,
typename decorators,
std::size_t bfs>
requires ((mde&buffer_mode::out)==buffer_mode::out)
inline constexpr void obuffer_set_curr(basic_io_buffer<handletype,mde,decorators,bfs>& bios,typename basic_io_buffer<handletype,mde,decorators,bfs>::char_type* ptr) noexcept
{
	bios.obuffer.buffer_curr=ptr;
}

namespace details
{

template<typename T,std::integral char_type>
inline constexpr void iobuf_output_flush_impl(T handle,basic_io_buffer_pointers<char_type>& pointers)
{
	if(pointers.buffer_curr==pointers.buffer_begin)
		return;
	write(handle,pointers.buffer_begin,pointers.buffer_curr);
	pointers.buffer_curr=pointers.buffer_begin;
}

template<typename T,std::integral char_type>
inline constexpr void iobuf_overflow_impl(T handle,basic_io_buffer_pointers<char_type>& pointers,char_type ch,std::size_t bfsz)
{
	if(pointers.buffer_begin==nullptr)
	{
		pointers.buffer_end=(pointers.buffer_curr=pointers.buffer_begin=
		allocate_iobuf_space<char_type>(bfsz))+bfsz;
	}
	else
	{
		iobuf_output_flush_impl(handle,pointers);
	}
	*pointers.buffer_curr=ch;
	++pointers.buffer_curr;
}

template<typename T,typename decot,std::integral char_type>
inline constexpr void iobuf_output_flush_impl_deco(T handle,decot deco,basic_io_buffer_pointers<char_type>& pointers,std::size_t bfsz)
{
	if(pointers.buffer_curr==pointers.buffer_begin)
		return;
	write_with_deco(handle,deco,pointers.buffer_begin,pointers.buffer_curr,bfsz);
	pointers.buffer_curr=pointers.buffer_begin;
}

template<typename T,typename decot,std::integral char_type>
inline constexpr void iobuf_overflow_impl_deco(T handle,decot deco,basic_io_buffer_pointers<char_type>& pointers,char_type ch,std::size_t bfsz)
{
	if(pointers.buffer_begin==nullptr)
	{
		pointers.buffer_end=(pointers.buffer_curr=pointers.buffer_begin=
		allocate_iobuf_space<char_type>(bfsz))+bfsz;
	}
	else
	{
		iobuf_output_flush_impl_deco(handle,deco,pointers,bfsz);
	}
	*pointers.buffer_curr=ch;
	++pointers.buffer_curr;
}

}

template<stream handletype,
buffer_mode mde,
typename decorators,
std::size_t bfs>
requires ((mde&buffer_mode::out)==buffer_mode::out)
inline constexpr auto overflow(basic_io_buffer<handletype,mde,decorators,bfs>& bios,
	typename basic_io_buffer<handletype,mde,decorators,bfs>::char_type ch)
{
	if constexpr(details::has_external_decorator_impl<decorators>)
		details::iobuf_overflow_impl_deco(io_ref(bios.handle),external_decorator(bios.decorators),bios.obuffer,ch,bfs);
	else
		details::iobuf_overflow_impl(io_ref(bios.handle),bios.obuffer,ch,bfs);
}

template<zero_copy_output_stream handletype,
buffer_mode mde,
typename decorators,
std::size_t bfs>
requires ((mde&buffer_mode::out)==buffer_mode::out&&!details::has_internal_decorator_impl<decorators>)
inline constexpr decltype(auto) zero_copy_out_handle(basic_io_buffer<handletype,mde,decorators,bfs>& bios)
{
	return zero_copy_out_handle(bios.handle);
}

}