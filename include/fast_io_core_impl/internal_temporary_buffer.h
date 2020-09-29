#pragma once
//std::string is not usable in freestanding environments and cause problems for this library
//This is a very simple stuff for internal use

namespace fast_io
{

template<std::integral ch_type>
class internal_temporary_buffer
{
public:
	using char_type = ch_type;
	using pointer = char_type;
	using iterator = pointer;
	using const_iterator = pointer;
	using allocator_type = std::allocator<char_type>;
	std::array<char_type,4096> static_buffer;
	pointer *beg_ptr=static_buffer.data(),*end_ptr=static_buffer.data(),*capacity_ptr=static_buffer.data()+4096;
	constexpr internal_temporary_buffer()=default;
	internal_temporary_buffer(internal_temporary_buffer const&)=delete;
	internal_temporary_buffer& operator=(internal_temporary_buffer const&)=delete;
#if __cpp_constexpr_dynamic_alloc >= 201907L
	constexpr
#endif
	~internal_temporary_buffer()
	{
		if(beg_ptr!=static_buffer.data())
		{
			allocator_type alloc;
			std::allocator_traits<allocator_type>::deallocate(alloc,beg_ptr,capacity_ptr-beg_ptr);
		}
	}
};

template<std::integral ch_type>
[[nodiscard]] constexpr auto obuffer_begin(internal_temporary_buffer<ch_type>& ob)
{
	return ob.beg_ptr;
}

template<std::integral ch_type>
[[nodiscard]] constexpr auto obuffer_curr(internal_temporary_buffer<ch_type>& ob)
{
	return ob.end_ptr;
}

template<std::integral ch_type>
[[nodiscard]] constexpr auto obuffer_end(internal_temporary_buffer<ch_type>& ob)
{
	return ob.capacity_ptr;
}

template<std::integral ch_type>
inline constexpr void obuffer_set_curr(internal_temporary_buffer<ch_type>& ob,ch_type* ptr)
{
	ob.end_ptr=ptr;
}

namespace details::internal_temporary_buffer_impl
{

template<bool vector_buf,typename T>
inline constexpr void ogrow_impl(T& ob,std::size_t new_capacity)
{
	using char_type = typename T::char_type;
	std::allocator<char_type> alloc;
	using allocator_traits = std::allocator_traits<std::allocator<char_type>>;
	auto newp{allocator_traits::allocate(alloc,new_capacity)};
	details::non_overlapped_copy_n(ob.beg_ptr,static_cast<std::size_t>(ob.end_ptr-ob.beg_ptr),newp);
	std::size_t const current_size(ob.end_ptr-ob.beg_ptr);
	if constexpr(!vector_buf)
	{
		if(ob.beg_ptr!=ob.static_buffer.data())
		{
			std::allocator<char_type> alloc;
			allocator_traits::deallocate(alloc,ob.beg_ptr,ob.capacity_ptr-ob.beg_ptr);
		}
	}
	else
	{
		if(ob.beg_ptr)[[likely]]
		{
			std::allocator<char_type> alloc;
			allocator_traits::deallocate(alloc,ob.beg_ptr,ob.capacity_ptr-ob.beg_ptr);
		}
	}
	ob.end_ptr=(ob.beg_ptr=newp)+current_size;
	ob.capacity_ptr=ob.beg_ptr+new_capacity;
}

}

template<std::integral ch_type>
inline constexpr void ogrow(internal_temporary_buffer<ch_type>& ob,std::size_t new_capacity)
{
	details::internal_temporary_buffer_impl::ogrow_impl<false>(ob,new_capacity);
}

template<std::integral char_type>
inline constexpr void otakeover(internal_temporary_buffer<char_type>& ob,char_type* beg_ptr,char_type* end_ptr,char_type* cap_ptr)
{
	ob.beg_ptr=beg_ptr;
	ob.end_ptr=end_ptr;
	ob.capacity_ptr=cap_ptr;
}

template<std::integral char_type>
inline constexpr bool ocan_takeover(internal_temporary_buffer<char_type>& ob)
{
	return ob.end_ptr==ob.static_buffer.data();
}

namespace details::internal_temporary_buffer_impl
{

template<bool vector_buf,typename T,std::contiguous_iterator Iter>
inline constexpr void write_bad_case(T& ob,Iter cbegin,Iter cend,std::size_t to_write_chars)
{
	std::size_t new_capacity((ob.capacity_ptr-ob.beg_ptr)<<1);
	if(new_capacity<to_write_chars)
		new_capacity=to_write_chars;
	ogrow_impl<vector_buf>(ob,new_capacity);
	if(std::is_constant_evaluated())
		ob.end_ptr=std::copy(cbegin,cend,ob.end_ptr);
	else
	{
		std::size_t const csz(cend-cbegin);
		memcpy(ob.end_ptr,std::to_address(cbegin),csz*sizeof(*cbegin));
		ob.end_ptr+=csz;
	}
}

template<bool vector_buf,typename T,std::contiguous_iterator Iter>
inline constexpr void write_impl(T& ob,Iter cbegin,Iter cend)
{
	using char_type = typename T::char_type;
	if constexpr(std::same_as<char_type,std::remove_cvref_t<decltype(*cbegin)>>)
	{
		std::size_t to_write_chars(cend-cbegin);
		std::size_t remain_space(ob.capacity_ptr-ob.end_ptr);
		if(remain_space<to_write_chars)[[unlikely]]
		{
			details::internal_temporary_buffer_impl::write_bad_case<vector_buf>(ob,cbegin,cend,to_write_chars);
			return;
		}
		ob.end_ptr=details::non_overlapped_copy_n(cbegin,cend-cbegin,ob.end_ptr);
	}
	else
		write_impl<vector_buf>(ob,reinterpret_cast<char const*>(std::to_address(cbegin)),reinterpret_cast<char const*>(std::to_address(cend)));
}

template<bool vector_buf,typename T,std::integral ch_type>
inline constexpr void overflow_impl(T& ob,ch_type ch)
{
	ogrow_impl<vector_buf>(ob,(ob.capacity_ptr-ob.beg_ptr)<<1);
	*ob.end_ptr=ch;
	++ob.end_ptr;
}

}

template<std::integral ch_type>
inline constexpr void overflow(internal_temporary_buffer<ch_type>& ob,ch_type ch)
{
	details::internal_temporary_buffer_impl::overflow_impl<false>(ob,ch);
}

template<std::integral ch_type,std::contiguous_iterator Iter>
requires (std::same_as<ch_type,char>||
	std::same_as<ch_type,typename std::iterator_traits<Iter>::value_type>)
inline constexpr void write(internal_temporary_buffer<ch_type>& ob,Iter cbegin,Iter cend)
{
	details::internal_temporary_buffer_impl::write_impl<false>(ob,cbegin,cend);
}

template<std::integral ch_type>
inline constexpr void flush(internal_temporary_buffer<ch_type>&){}

template<std::integral ch_type>
inline constexpr auto oallocator(internal_temporary_buffer<ch_type>&)
{
	return std::allocator<ch_type>();
}

}
