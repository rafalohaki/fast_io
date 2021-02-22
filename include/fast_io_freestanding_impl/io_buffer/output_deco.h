#pragma once

namespace fast_io::details
{



template<typename handle_type,typenam decotype>
inline constexpr auto iobuf_write_unhappy_nullptr_case_impl_deco(handle_type t,
	decotype deco,
	typename decotype::char_type const* first,
	typename decotype::char_type const* last,
	std::size_t bfsz)
{
	using char_type = typename T::char_type;
	using decot_no_cvref_t = std::remove_cvref_t<decot>;
	std::size_t required_size{deco_reserve_size(io_reserve_type<char_type,decot_no_cvref_t>,deco,bfsz)};
	buffer_alloc_arr_ptr<char_type> alloc_ptr(allocate_iobuf_space<char_type>(sz));	
	for(;first!=last;++first)
	{
		this_round=internal_size;
		diff=static_cast<std::size_t>(last-first);
		if(diff<this_round)
			this_round=diff;
		auto to_write_first{first+this_round};
		auto [srcit,destit]=
			deco_reserve_define(io_reserve_type<char_type,decot_no_cvref_t>,
				deco,first,to_write_first,alloc_ptr.ptr);
		if(srcit==first)
			break;
		first=srcit;
		write(t,alloc_ptr.ptr,destit);
	}
	return first;
}

template<typename handle_type,typenam decotype>
inline constexpr auto iobuf_write_unhappy_decay_no_alloc_impl_deco(handle_type t,
	decotype deco,
	typename decotype::char_type const* first,
	typename decotype::char_type const* last,
	std::size_t bfsz)
{
	using char_type = typename T::char_type;
	using decot_no_cvref_t = std::remove_cvref_t<decot>;
	std::size_t required_size{deco_reserve_size(io_reserve_type<char_type,decot_no_cvref_t>,deco,bfsz)};
	buffer_alloc_arr_ptr<char_type> alloc_ptr(allocate_iobuf_space<char_type>(sz));	
	for(;first!=last;++first)
	{
		this_round=internal_size;
		diff=static_cast<std::size_t>(last-first);
		if(diff<this_round)
			this_round=diff;
		auto to_write_first{first+this_round};
		auto [srcit,destit]=
			deco_reserve_define(io_reserve_type<char_type,decot_no_cvref_t>,
				deco,first,to_write_first,alloc_ptr.ptr);
		if(srcit==first)
			break;
		first=srcit;
		write(t,alloc_ptr.ptr,destit);
	}
	return first;
}



template<std::size_t buffer_size,typename T,typenam decotype,std::integral char_type,std::random_access_iterator Iter>
inline constexpr void iobuf_write_unhappy_decay_impl_deco(T t,decotype deco,basic_io_buffer_pointers<char_type>& pointers,Iter first,Iter last)
{
	std::size_t const diff(static_cast<std::size_t>(last-first));
	if(pointers.buffer_begin==nullptr)
	{
		if(buffer_size<=diff)
			iobuf_write_nullptr_case(t,deco,pointers,first,last,buffer_size);
		else
		{
			auto mv{iobuf_write_unhappy_nullptr_case_impl_deco(t,deco,pointers,first,last,buffer_size)};
			if(mv==last)
				return;
			std::size_t unwritten{static_cast<std::size_t>(last-first)};
			std::size_t bfsz{buffer_size};
			if(bfsz<unwritten)
				bfsz=unwritten;
			pointers.buffer_end=((pointers.buffer_curr=(pointers.buffer_begin=allocate_iobuf_space<char_type>(bfsz)))+bfsz);
			pointers.buffer_curr=nonoverlapped_copy_n(first,unwritten,pointers.buffer_curr);
		}
		return;
	}
	iobuf_write_unhappy_decay_no_alloc_impl_deco(t,pointers,first,last,buffer_size);
}


}