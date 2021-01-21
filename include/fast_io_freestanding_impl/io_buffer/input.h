#pragma once

namespace fast_io
{

template<zero_copy_input_stream handletype,buffer_mode mde,std::size_t bfs,std::size_t alignsz>
requires ((mde&buffer_mode::in)==buffer_mode::in)
inline constexpr decltype(auto) zero_copy_in_handle(basic_io_buffer<handletype,mde,bfs,alignsz>& bios)
{
	return zero_copy_in_handle(bios.handle);
}

template<stream handletype,buffer_mode mde,std::size_t bfs,std::size_t alignsz>
requires ((mde&buffer_mode::in)==buffer_mode::in)
inline constexpr auto ibuffer_begin(basic_io_buffer<handletype,mde,bfs,alignsz>& bios) noexcept
{
	return bios.ibuffer.buffer_begin;
}

template<stream handletype,buffer_mode mde,std::size_t bfs,std::size_t alignsz>
requires ((mde&buffer_mode::in)==buffer_mode::in)
inline constexpr auto ibuffer_curr(basic_io_buffer<handletype,mde,bfs,alignsz>& bios) noexcept
{
	return bios.ibuffer.buffer_curr;
}

template<stream handletype,buffer_mode mde,std::size_t bfs,std::size_t alignsz>
requires ((mde&buffer_mode::in)==buffer_mode::in)
inline constexpr auto ibuffer_end(basic_io_buffer<handletype,mde,bfs,alignsz>& bios) noexcept
{
	return bios.ibuffer.buffer_end;
}

template<stream handletype,buffer_mode mde,std::size_t bfs,std::size_t alignsz>
requires ((mde&buffer_mode::in)==buffer_mode::in)
inline constexpr void ibuffer_set_curr(basic_io_buffer<handletype,mde,bfs,alignsz>& bios,typename basic_io_buffer<handletype,mde,bfs,alignsz>::char_type* ptr) noexcept
{
	bios.ibuffer.buffer_curr=ptr;
}

namespace details
{

template<stream T,std::integral char_type>
inline constexpr bool underflow_rl_impl(T t,basic_io_buffer_pointers<char_type>& ibuffer,std::size_t bfsz,std::size_t alignsz)
{
	if(ibuffer.buffer_begin==nullptr)
		ibuffer.buffer_end=ibuffer.buffer_curr=ibuffer.buffer_begin=allocate_iobuf_space<char_type>(bfsz,alignsz);
	ibuffer.buffer_end=read(t,ibuffer.buffer_begin,ibuffer.buffer_begin+bfsz);
	ibuffer.buffer_curr=ibuffer.buffer_begin;
	return ibuffer.buffer_begin!=ibuffer.buffer_end;
}

template<std::size_t bfsz,std::size_t alignsz,stream T,std::integral char_type>
inline constexpr bool underflow_impl(T t,basic_io_buffer_pointers<char_type>& ibuffer)
{
	return underflow_rl_impl(t,ibuffer,bfsz,alignsz);
}


template<typename T,std::integral char_type,std::random_access_iterator Iter>
inline constexpr Iter iobuf_read_unhappy_decay_impl(T t,basic_io_buffer_pointers<char_type>& ibuffer,Iter first,Iter last,std::size_t buffer_size,std::size_t buffer_alignment)
{
	std::size_t iter_diff(static_cast<std::size_t>(last-first));
	if(buffer_size<=iter_diff)
		return read(t,first,last);
	if(ibuffer.buffer_begin==nullptr)
	{
		ibuffer.buffer_end=ibuffer.buffer_begin=
		allocate_iobuf_space<char_type>(buffer_size,buffer_alignment);
	}
	ibuffer.buffer_end=read(t,ibuffer.buffer_begin,ibuffer.buffer_begin+buffer_size);
	ibuffer.buffer_curr=ibuffer.buffer_begin;
	std::size_t diff(static_cast<std::size_t>(ibuffer.buffer_end-ibuffer.buffer_begin));
	if(diff<iter_diff)
		iter_diff=diff;
	first=non_overlapped_copy_n(ibuffer.buffer_curr,iter_diff,first);
	ibuffer.buffer_curr+=diff;
	return first;
}

template<typename T,std::random_access_iterator Iter>
inline constexpr Iter iobuf_read_unhappy_impl(T& bios,Iter first,Iter last)
{
	if constexpr(((T::mode&buffer_mode::out)==buffer_mode::out)&&((T::mode&buffer_mode::tie)==buffer_mode::tie))
		iobuf_output_flush_impl(io_ref(bios.handle),bios.ibuffer);
	first=non_overlapped_copy(bios.ibuffer.buffer_curr,bios.ibuffer.buffer_end,first);
	bios.ibuffer.buffer_curr=bios.ibuffer.buffer_end;
	return iobuf_read_unhappy_decay_impl(io_ref(bios.handle),bios.ibuffer,first,last,T::buffer_size,T::buffer_alignment);
}

}

template<stream handletype,buffer_mode mde,std::size_t bfs,std::size_t alignsz>
requires ((mde&buffer_mode::in)==buffer_mode::in)
inline constexpr bool underflow(basic_io_buffer<handletype,mde,bfs,alignsz>& bios)
{
	if constexpr(((mde&buffer_mode::out)==buffer_mode::out)&&
	((mde&buffer_mode::tie)==buffer_mode::tie))
		details::iobuf_output_flush_impl(io_ref(bios.handle),bios.obuffer);
	return details::underflow_impl<bfs,alignsz>(io_ref(bios.handle),bios.ibuffer);
}

template<stream handletype,buffer_mode mde,std::size_t bfs,std::size_t alignsz,std::random_access_iterator Iter>
requires (((mde&buffer_mode::in)==buffer_mode::in)&&details::allow_iobuf_punning<handletype,Iter>)
inline constexpr Iter read(basic_io_buffer<handletype,mde,bfs,alignsz>& bios,Iter first,Iter last)
{
	using iter_char_type = std::iter_value_t<Iter>;
	using char_type = typename handletype::char_type;
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