#pragma once

namespace fast_io::details
{
template<typename char_type>
inline constexpr char_type* allocate_iobuf_space(std::size_t buffer_size) noexcept
{
#if __cpp_constexpr >=201907L && __cpp_constexpr_dynamic_alloc >= 201907L && __cpp_lib_is_constant_evaluated >=201811L
	if(std::is_constant_evaluated())
	{
		return new char_type[buffer_size];
	}
	else
#endif
		return static_cast<char_type*>(operator new(intrinsics::cal_allocation_size_or_die<char_type>(buffer_size)));

}

template<bool nsecure_clear>
inline void deallocate_with_secure_clear(void* ptr,[[maybe_unused]] std::size_t buffer_bytes) noexcept
{
	if constexpr(nsecure_clear)
		secure_clear(ptr,buffer_bytes);
#if __cpp_sized_deallocation >= 201309L
	operator delete(ptr,buffer_bytes);
#else
	operator delete(ptr);
#endif
}

template<bool nsecure_clear,typename char_type>
inline constexpr void deallocate_iobuf_space(char_type* ptr,[[maybe_unused]] std::size_t buffer_size) noexcept
{
#if __cpp_constexpr >=201907L && __cpp_constexpr_dynamic_alloc >= 201907L && __cpp_lib_is_constant_evaluated >=201811L
	if(std::is_constant_evaluated())
	{
		delete[] ptr;
	}
	else
#endif
	{
		deallocate_with_secure_clear<nsecure_clear>(ptr,buffer_size*sizeof(char_type));
	}
}

template<typename T,bool nsecure_clear>
struct buffer_alloc_arr_ptr
{
	T* ptr{};
	std::size_t size{};
	constexpr buffer_alloc_arr_ptr() noexcept = default;
#if __cpp_constexpr >=201907L && __cpp_constexpr_dynamic_alloc >= 201907L && __cpp_lib_is_constant_evaluated >=201811L
	constexpr
#endif
	buffer_alloc_arr_ptr(std::size_t sz) noexcept:ptr(allocate_iobuf_space<T>(sz)),size(sz){}

	buffer_alloc_arr_ptr(buffer_alloc_arr_ptr const&)=delete;
	buffer_alloc_arr_ptr& operator=(buffer_alloc_arr_ptr const&)=delete;
	constexpr T* get() noexcept
	{
		return ptr;
	}
	constexpr T const* get() const noexcept
	{
		return ptr;
	}
	constexpr T& operator[](std::size_t pos) noexcept
	{
		return ptr[pos];
	}
	constexpr T const& operator[](std::size_t pos) const noexcept
	{
		return ptr[pos];
	}
#if __cpp_constexpr >=201907L && __cpp_constexpr_dynamic_alloc >= 201907L && __cpp_lib_is_constant_evaluated >=201811L
	constexpr
#endif
	~buffer_alloc_arr_ptr()
	{
		deallocate_iobuf_space<nsecure_clear,T>(ptr,size);
	}
};

template<typename char_type>
using local_operator_new_array_ptr=buffer_alloc_arr_ptr<char_type,false>;

}
