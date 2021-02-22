#pragma once

namespace fast_io
{

enum class buffer_mode
{
in=1<<0,
out=1<<1,
tie=1<<2,
io=in|out|tie,
secure_clear=1<<3,
construct_decorator=1<<4
};

inline constexpr buffer_mode operator&(buffer_mode x, buffer_mode y) noexcept
{
using utype = typename std::underlying_type<buffer_mode>::type;
return static_cast<buffer_mode>(static_cast<utype>(x) & static_cast<utype>(y));
}

inline constexpr buffer_mode operator|(buffer_mode x, buffer_mode y) noexcept
{
using utype = typename std::underlying_type<buffer_mode>::type;
return static_cast<buffer_mode>(static_cast<utype>(x) | static_cast<utype>(y));
}

inline constexpr buffer_mode operator^(buffer_mode x, buffer_mode y) noexcept
{
using utype = typename std::underlying_type<buffer_mode>::type;
return static_cast<buffer_mode>(static_cast<utype>(x) ^ static_cast<utype>(y));
}

inline constexpr buffer_mode operator~(buffer_mode x) noexcept
{
using utype = typename std::underlying_type<buffer_mode>::type;
return static_cast<buffer_mode>(~static_cast<utype>(x));
}

inline constexpr buffer_mode& operator&=(buffer_mode& x, buffer_mode y) noexcept{return x=x&y;}

inline constexpr buffer_mode& operator|=(buffer_mode& x, buffer_mode y) noexcept{return x=x|y;}

inline constexpr buffer_mode& operator^=(buffer_mode& x, buffer_mode y) noexcept{return x=x^y;}


template<typename char_type>
inline constexpr std::size_t io_default_buffer_size = details::cal_buffer_size<char_type,true>();

struct empty_buffer_pointers
{};

template<typename T>
struct basic_io_buffer_pointers
{
	using value_type = T;
	using pointer = T*;
	pointer buffer_begin{},buffer_curr{},buffer_end{};
};

template<typename T>
struct basic_io_buffer_pointers_with_cap
{
	using value_type = T;
	using pointer = T*;
	pointer buffer_begin{},buffer_curr{},buffer_end{},buffer_cap{};
};

namespace details
{
template<stream handle_type>
inline 
#if __cpp_consteval >= 201811L
consteval
#else
constexpr
#endif
bool constraint_buffer_mode(buffer_mode mode) noexcept
{
	if(((mode&buffer_mode::in)==buffer_mode::in)&&(!input_stream<handle_type>))
		return false;
	if(((mode&buffer_mode::out)==buffer_mode::out)&&(!output_stream<handle_type>))
		return false;
	if constexpr(secure_clear_requirement_stream<handle_type>)
		if((mode&buffer_mode::secure_clear)!=buffer_mode::secure_clear)
			return false;
	return true;
}


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
	{
#if __cpp_exceptions
	try
	{
#endif
//capable for AVX512
		return static_cast<char_type*>(operator new(intrinsics::cal_allocation_size_or_die<char_type>(buffer_size)));
#if __cpp_exceptions
	}
	catch(...)
	{
		fast_terminate();
	}
#endif
	}
}

template<typename char_type>
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
#if __cpp_sized_deallocation >= 201309L
		operator delete(ptr,buffer_size*sizeof(char_type));
#else
		operator delete(ptr);
#endif
	}
}

}

}