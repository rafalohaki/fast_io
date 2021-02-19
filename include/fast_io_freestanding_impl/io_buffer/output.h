#pragma once

namespace fast_io
{

namespace details
{

template<typename handletype,typename Iter>
concept allow_iobuf_punning = stream<handletype>&&std::forward_iterator<Iter>&&std::contiguous_iterator<Iter>&&
//temporary only allow contiguous_iterator before we finish this part
(std::same_as<typename handletype::char_type,std::iter_value_t<Iter>>||
(std::same_as<typename handletype::char_type,char>&&std::contiguous_iterator<Iter>));

template<std::integral char_type,std::random_access_iterator Iter>
inline constexpr void iobuf_write_unhappy_nullptr_case_impl(basic_io_buffer_pointers<char_type>& obuffer,Iter first,Iter last,std::size_t buffer_size,std::size_t buffer_alignment)
{
	obuffer.buffer_end=(obuffer.buffer_curr=obuffer.buffer_begin=
	allocate_iobuf_space<char_type>(buffer_size,buffer_alignment))+buffer_size;
	obuffer.buffer_curr=non_overlapped_copy(first,last,obuffer.buffer_curr);
}

template<typename T,typename decot,std::integral char_type,std::random_access_iterator Iter>
inline constexpr void iobuf_write_unhappy_decay_no_alloc_impl_deco(T t,decot& deco,basic_io_buffer_pointers<char_type>& pointers,Iter first,Iter last,std::size_t buffer_size)
{
	std::size_t const remain_space(pointers.buffer_end-pointers.buffer_curr);
	non_overlapped_copy_n(first,remain_space,pointers.buffer_curr);
	first+=remain_space;
	write_with_deco(t,deco,pointers.buffer_begin,pointers.buffer_end,buffer_size);
	pointers.buffer_curr=pointers.buffer_begin;
	std::size_t const new_remain_space(last-first);
	if(buffer_size<new_remain_space)
	{
		write_with_deco(t,deco,first,last,buffer_size);
	}
	else
		pointers.buffer_curr=non_overlapped_copy_n(first,new_remain_space,pointers.buffer_begin);

}

template<typename T,std::integral char_type,std::random_access_iterator Iter>
inline constexpr void iobuf_write_unhappy_decay_no_alloc_impl(T t,basic_io_buffer_pointers<char_type>& pointers,Iter first,Iter last,std::size_t buffer_size)
{
	std::size_t const remain_space(pointers.buffer_end-pointers.buffer_curr);
	non_overlapped_copy_n(first,remain_space,pointers.buffer_curr);
	first+=remain_space;
	write(t,pointers.buffer_begin,pointers.buffer_end);
	pointers.buffer_curr=pointers.buffer_begin;
	std::size_t const new_remain_space(last-first);
	if(buffer_size<new_remain_space)
	{
		write(t,first,last);
	}
	else
		pointers.buffer_curr=non_overlapped_copy_n(first,new_remain_space,pointers.buffer_begin);
}

template<std::size_t buffer_size,std::size_t buffer_alignment,typename T,std::integral char_type,std::random_access_iterator Iter>
inline constexpr void iobuf_write_unhappy_decay_impl(T t,basic_io_buffer_pointers<char_type>& pointers,Iter first,Iter last)
{
	std::size_t const diff(static_cast<std::size_t>(last-first));
	if(pointers.buffer_begin==nullptr)
	{
		if(diff<buffer_size)
			iobuf_write_unhappy_nullptr_case_impl(pointers,first,last,buffer_size,buffer_alignment);
		else
			write(t,first,last);
		return;
	}
	iobuf_write_unhappy_decay_no_alloc_impl(t,pointers,first,last,buffer_size);
}

template<std::size_t buffer_size,std::size_t buffer_alignment,typename T,typename decot,std::integral char_type,std::random_access_iterator Iter>
inline constexpr void iobuf_write_unhappy_decay_impl_deco(T t,decot& deco,basic_io_buffer_pointers<char_type>& pointers,Iter first,Iter last)
{
	std::size_t const diff(static_cast<std::size_t>(last-first));
	if(pointers.buffer_begin==nullptr)
	{
		if(diff<buffer_size)
			iobuf_write_unhappy_nullptr_case_impl(pointers,deco,first,last,buffer_size,buffer_alignment);
		else
			write_with_deco(t,deco,first,last,buffer_size);
		return;
	}
	iobuf_write_unhappy_decay_no_alloc_impl_deco(t,deco,pointers,first,last,buffer_size);
}


template<typename T,std::random_access_iterator Iter>
inline constexpr void iobuf_write_unhappy_impl(T& t,Iter first,Iter last)
{
	if constexpr(requires()
	{
		external(t.decorators);
	})
		iobuf_write_unhappy_decay_impl_deco<T::buffer_size,T::buffer_alignment>(io_ref(t.handle),external(t.decorators),t.obuffer,first,last);
	else
		iobuf_write_unhappy_decay_impl<T::buffer_size,T::buffer_alignment>(io_ref(t.handle),t.obuffer,first,last);
}

}

template<stream handletype,
typename decorators,
buffer_mode mde,std::size_t bfs,std::size_t alignsz,std::random_access_iterator Iter>
requires (((mde&buffer_mode::out)==buffer_mode::out)&&details::allow_iobuf_punning<handletype,Iter>)
inline constexpr void write(basic_io_buffer<handletype,decorators,mde,bfs,alignsz>& bios,Iter first,Iter last)
{
	using iter_char_type = std::iter_value_t<Iter>;
	using char_type = typename handletype::char_type;
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
typename decorators,
buffer_mode mde,std::size_t bfs,std::size_t alignsz>
requires ((mde&buffer_mode::out)==buffer_mode::out)
inline constexpr auto obuffer_begin(basic_io_buffer<handletype,decorators,mde,bfs,alignsz>& bios) noexcept
{
	return bios.obuffer.buffer_begin;
}


template<stream handletype,
typename decorators,
buffer_mode mde,std::size_t bfs,std::size_t alignsz>
requires ((mde&buffer_mode::out)==buffer_mode::out)
inline constexpr auto obuffer_curr(basic_io_buffer<handletype,decorators,mde,bfs,alignsz>& bios) noexcept
{
	return bios.obuffer.buffer_curr;
}

template<stream handletype,
typename decorators,
buffer_mode mde,std::size_t bfs,std::size_t alignsz>
requires ((mde&buffer_mode::out)==buffer_mode::out)
inline constexpr auto obuffer_end(basic_io_buffer<handletype,decorators,mde,bfs,alignsz>& bios) noexcept
{
	return bios.obuffer.buffer_end;
}

template<stream handletype,
typename decorators,
buffer_mode mde,std::size_t bfs,std::size_t alignsz>
requires ((mde&buffer_mode::out)==buffer_mode::out)
inline constexpr void obuffer_set_curr(basic_io_buffer<handletype,decorators,mde,bfs,alignsz>& bios,typename basic_io_buffer<handletype,decorators,mde,bfs,alignsz>::char_type* ptr) noexcept
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
inline constexpr void iobuf_overflow_impl(T handle,basic_io_buffer_pointers<char_type>& pointers,char_type ch,std::size_t bfsz,std::size_t almsz)
{
	if(pointers.buffer_begin==nullptr)
	{
		pointers.buffer_end=(pointers.buffer_curr=pointers.buffer_begin=
		allocate_iobuf_space<char_type>(bfsz,almsz))+bfsz;
	}
	else
	{
		iobuf_output_flush_impl(handle,pointers);
	}
	*pointers.buffer_curr=ch;
	++pointers.buffer_curr;
}

template<typename T,typename decot,std::integral char_type>
inline constexpr void iobuf_output_flush_impl_deco(T handle,decot& deco,basic_io_buffer_pointers<char_type>& pointers,std::size_t bfsz)
{
	if(pointers.buffer_curr==pointers.buffer_begin)
		return;
	write_with_deco(handle,deco,pointers.buffer_begin,pointers.buffer_curr,bfsz);
	pointers.buffer_curr=pointers.buffer_begin;
}

template<typename T,typename decot,std::integral char_type>
inline constexpr void iobuf_overflow_impl_deco(T handle,decot& deco,basic_io_buffer_pointers<char_type>& pointers,char_type ch,std::size_t bfsz,std::size_t almsz)
{
	if(pointers.buffer_begin==nullptr)
	{
		pointers.buffer_end=(pointers.buffer_curr=pointers.buffer_begin=
		allocate_iobuf_space<char_type>(bfsz,almsz))+bfsz;
	}
	else
	{
		iobuf_output_flush_impl(handle,deco,pointers,bfsz);
	}
	*pointers.buffer_curr=ch;
	++pointers.buffer_curr;
}

}

template<stream handletype,
typename decorators,
buffer_mode mde,std::size_t bfs,std::size_t alignsz>
requires ((mde&buffer_mode::out)==buffer_mode::out)
inline constexpr auto overflow(basic_io_buffer<handletype,decorators,mde,bfs,alignsz>& bios,
	typename basic_io_buffer<handletype,decorators,mde,bfs,alignsz>::char_type ch)
{
	if constexpr(requires()
	{
		external(bios.decorators);
	})
		details::iobuf_overflow_impl_deco(io_ref(bios.handle),external(bios.decorators),bios.obuffer,ch,bfs,alignsz);
	else
		details::iobuf_overflow_impl(io_ref(bios.handle),bios.obuffer,ch,bfs,alignsz);
}

template<zero_copy_output_stream handletype,
typename decorators,
buffer_mode mde,std::size_t bfs,std::size_t alignsz>
requires ((mde&buffer_mode::out)==buffer_mode::out)
inline constexpr decltype(auto) zero_copy_out_handle(basic_io_buffer<handletype,decorators,mde,bfs,alignsz>& bios)
{
	return zero_copy_out_handle(bios.handle);
}

}