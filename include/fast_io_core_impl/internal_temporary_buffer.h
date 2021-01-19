#pragma once
//std::string is not usable in freestanding environments and cause problems for this library
//This is a very simple stuff for internal use

namespace fast_io
{

namespace details
{
#ifdef _WIN32
extern "C" void* __stdcall GetProcessHeap() noexcept;
extern "C" void* __stdcall HeapAlloc(void*,std::uint32_t,std::size_t) noexcept;
extern "C" int __stdcall HeapFree(void*,std::uint32_t,void*) noexcept;
extern "C" void* __stdcall HeapReAlloc(void*,std::uint32_t,void*,std::size_t) noexcept;
#else
extern "C" void* malloc(std::size_t) noexcept;
extern "C" void free(void*) noexcept;
extern "C" void* realloc(void*,std::size_t) noexcept;
#endif
inline void* sys_allocate(std::size_t n) noexcept
{
	auto ptr
#ifdef _WIN32
	{HeapAlloc(GetProcessHeap(),0,n)};
#else
	{malloc(n)};
#endif
	if(ptr==nullptr)[[unlikely]]
		fast_terminate();
	return ptr;
}

inline void sys_deallocate(void* ptr,[[maybe_unused]] std::size_t n) noexcept
{
	if(ptr)[[likely]]
	{
#ifdef _WIN32
		if(!HeapFree(GetProcessHeap(),0,ptr))[[unlikely]]
			fast_terminate();
#else
		free(ptr);
#endif
	}
}

inline void* sys_reallocate(std::size_t n,void const* ptr) noexcept
{
#ifdef _WIN32
	if(ptr)
	{
		auto new_ptr{HeapReAlloc(GetProcessHeap(),0,const_cast<void*>(ptr),n)};
		if(new_ptr==nullptr)[[unlikely]]
			fast_terminate();
		return new_ptr;
	}
	else
		return sys_allocate(n);
#else
	auto new_ptr{realloc(const_cast<void*>(ptr),n)};
	if(new_ptr==nullptr)[[unlikely]]
		fast_terminate();
	return new_ptr;
#endif
}


}

template<typename T>
struct noexcept_allocator
{
	using value_type = T;
	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;
	[[nodiscard]] inline
	#if __cpp_lib_is_constant_evaluated >= 201811L && __cpp_constexpr_dynamic_alloc >= 201907L
		constexpr
	#endif
	T* allocate(std::size_t n) noexcept
	{
	#if __cpp_lib_is_constant_evaluated >= 201811L && __cpp_constexpr_dynamic_alloc >= 201907L
		if(std::is_constant_evaluated())
			return new T[n];
		else
	#endif
			return reinterpret_cast<T*>(details::sys_allocate(n*sizeof(T)));
	}

	[[nodiscard]] inline
	#if __cpp_lib_is_constant_evaluated >= 201811L && __cpp_constexpr_dynamic_alloc >= 201907L
		constexpr
	#endif
	T* allocate(std::size_t n,void const* hint) noexcept
	{
	#if __cpp_lib_is_constant_evaluated >= 201811L && __cpp_constexpr_dynamic_alloc >= 201907L
		if(std::is_constant_evaluated())
			return new T[n];
		else
	#endif
			return reinterpret_cast<T*>(details::sys_reallocate(n*sizeof(T),hint));
	}
	inline
	#if __cpp_lib_is_constant_evaluated >= 201811L && __cpp_constexpr_dynamic_alloc >= 201907L
		constexpr
	#endif
	void deallocate(T* p, [[maybe_unused]] std::size_t n) noexcept
	{
	#if __cpp_lib_is_constant_evaluated >= 201811L && __cpp_constexpr_dynamic_alloc >= 201907L
		if(std::is_constant_evaluated())
			delete[] p;
		else
	#endif
			return details::sys_deallocate(p,n*sizeof(T));
	}
};

template<std::integral ch_type>
class internal_temporary_buffer
{
public:
	using char_type = ch_type;
	using pointer = char_type*;
	using iterator = pointer;
	using const_iterator = pointer;
	using allocator_type = std::allocator<char_type>;
	std::array<char_type,4096> static_buffer;
	pointer beg_ptr=static_buffer.data(),end_ptr=static_buffer.data(),capacity_ptr=static_buffer.data()+4096;
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
			using allocator_traits = std::allocator_traits<std::allocator<char_type>>;
			allocator_traits::deallocate(alloc,beg_ptr,capacity_ptr-beg_ptr);
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
	std::size_t const current_size(ob.end_ptr-ob.beg_ptr);
	non_overlapped_copy_n(ob.beg_ptr,current_size,newp);
	if constexpr(!vector_buf)
	{
		if(ob.beg_ptr!=ob.static_buffer.data())
		{
			allocator_traits::deallocate(alloc,ob.beg_ptr,ob.capacity_ptr-ob.beg_ptr);
		}
	}
	else
	{
		if(ob.beg_ptr)[[likely]]
		{
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
	std::size_t new_capacity(static_cast<std::size_t>(ob.capacity_ptr-ob.beg_ptr)<<1);
	to_write_chars+=ob.end_ptr-ob.beg_ptr;
	if(new_capacity<to_write_chars)
		new_capacity=to_write_chars;
	ogrow_impl<vector_buf>(ob,new_capacity);
	std::size_t const csz(cend-cbegin);
	details::non_overlapped_copy_n(cbegin,csz,ob.end_ptr);
	ob.end_ptr+=csz;
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
		details::non_overlapped_copy_n(cbegin,to_write_chars,ob.end_ptr);
		ob.end_ptr+=to_write_chars;
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
inline constexpr void overflow(internal_temporary_buffer<ch_type>& ob,ch_type ch) noexcept
{
	details::internal_temporary_buffer_impl::overflow_impl<false>(ob,ch);
}

template<std::integral ch_type,std::contiguous_iterator Iter>
requires (std::same_as<ch_type,char>||
	std::same_as<ch_type,typename std::iterator_traits<Iter>::value_type>)
inline constexpr void write(internal_temporary_buffer<ch_type>& ob,Iter cbegin,Iter cend) noexcept
{
	details::internal_temporary_buffer_impl::write_impl<false>(ob,cbegin,cend);
}

template<std::integral ch_type>
inline constexpr void flush(internal_temporary_buffer<ch_type>&) noexcept{}

template<std::integral ch_type>
inline constexpr auto oallocator(internal_temporary_buffer<ch_type>&) noexcept
{
	return std::allocator<ch_type>();
}

}
