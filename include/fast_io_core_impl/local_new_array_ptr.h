#pragma once

namespace fast_io::details
{

template<std::integral char_type>
struct local_operator_new_array_ptr
{
	std::size_t size;
	char_type* ptr;
#if __cpp_constexpr >=201907L && __cpp_constexpr_dynamic_alloc >= 201907L && __cpp_lib_is_constant_evaluated >=201811L
	constexpr
#endif
	local_operator_new_array_ptr(std::size_t sz):size(sz)
	{
#if __cpp_constexpr >=201907L && __cpp_constexpr_dynamic_alloc >= 201907L && __cpp_lib_is_constant_evaluated >=201811L
		if(std::is_constant_evaluated())
		{
			ptr=new char_type[sz];
		}
		else
#endif
		{
			if constexpr(alignof(char_type)>=alignof(std::max_align_t))
				ptr=static_cast<char_type*>(operator new(sz*sizeof(char_type),std::align_val_t{alignof(char_type)}));
			else
				ptr=static_cast<char_type*>(operator new(sz*sizeof(char_type)));
		}
	}
	local_operator_new_array_ptr(local_operator_new_array_ptr const&)=delete;
	local_operator_new_array_ptr& operator=(local_operator_new_array_ptr const&)=delete;
#if __cpp_constexpr >=201907L && __cpp_constexpr_dynamic_alloc >= 201907L && __cpp_lib_is_constant_evaluated >=201811L
	constexpr
#endif
	~local_operator_new_array_ptr()
	{
#if __cpp_constexpr >=201907L && __cpp_constexpr_dynamic_alloc >= 201907L && __cpp_lib_is_constant_evaluated >=201811L
		if(std::is_constant_evaluated())
		{
			delete[] ptr;
		}
		else
#endif
		{
#if __cpp_sized_deallocation >= 201309
			if constexpr(alignof(char_type)>=alignof(std::max_align_t))
				operator delete(ptr,size*sizeof(char_type),std::align_val_t{alignof(char_type)});
			else
				operator delete(ptr,size*sizeof(char_type));
#else
			if constexpr(alignof(char_type)>=alignof(std::max_align_t))
				operator delete(ptr,std::align_val_t{alignof(char_type)});
			else
				operator delete(ptr);
#endif
		}
	}
};
}