#pragma once

namespace fast_io::details
{

template<typename char_type,typename Iter>
concept allow_iobuf_punning = std::contiguous_iterator<Iter>&&
//temporary only allow contiguous_iterator before we finish this part
(std::same_as<char_type,std::iter_value_t<Iter>>||
(std::same_as<char_type,char>&&std::contiguous_iterator<Iter>));

template<std::integral char_type,std::random_access_iterator Iter>
inline constexpr void iobuf_write_unhappy_nullptr_case_impl(basic_io_buffer_pointers<char_type>& obuffer,Iter first,Iter last,std::size_t buffer_size)
{
	obuffer.buffer_end=(obuffer.buffer_curr=obuffer.buffer_begin=
	allocate_iobuf_space<char_type>(buffer_size))+buffer_size;
	obuffer.buffer_curr=non_overlapped_copy(first,last,obuffer.buffer_curr);
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

template<std::size_t buffer_size,typename T,std::integral char_type,std::random_access_iterator Iter>
inline constexpr void iobuf_write_unhappy_decay_impl(T t,basic_io_buffer_pointers<char_type>& pointers,Iter first,Iter last)
{
	std::size_t const diff(static_cast<std::size_t>(last-first));
	if(pointers.buffer_begin==nullptr)
	{
		if(diff<buffer_size)
			iobuf_write_unhappy_nullptr_case_impl(pointers,first,last,buffer_size);
		else
			write(t,first,last);
		return;
	}
	iobuf_write_unhappy_decay_no_alloc_impl(t,pointers,first,last,buffer_size);
}

}