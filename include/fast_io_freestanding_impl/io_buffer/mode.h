#pragma once

namespace fast_io
{

enum class buffer_mode
{
in=1<<0,
out=1<<1,
tie=1<<2,
io=in|out|tie,
secure_clear=1<<3
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
inline constexpr char_type* allocate_iobuf_space(std::size_t buffer_size,std::size_t aligmsz) noexcept
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
		constexpr std::size_t max_size{SIZE_MAX/sizeof(char_type)};
		if(buffer_size>max_size)
			fast_terminate();
		return static_cast<char_type*>(operator new(buffer_size*sizeof(char_type),std::align_val_t{aligmsz}));
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
inline constexpr void deallocate_iobuf_space(char_type* ptr,[[maybe_unused]] std::size_t buffer_size,std::size_t aligmsz) noexcept
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
		operator delete(ptr,buffer_size*sizeof(char_type),std::align_val_t{aligmsz});
#else
		operator delete(ptr,std::align_val_t{aligmsz});
#endif
	}
}

template<typename T>
struct iobuf_char_type_impl
{
	using type = T;
};

template<stream handletype,buffer_mode mde>
inline constexpr auto get_iobuf_char_type() noexcept
{
	if constexpr(((mde&buffer_mode::out)==buffer_mode::out)&&
		((mde&buffer_mode::in)==buffer_mode::in))
	{
		if constexpr(decorated_output_stream<handletype>)
		{
			using output_decorator_type = typename handletype::output_decorator_type;
			static_assert(std::same_as<
				typename output_decorator_type::to_type,
				typename handletype::char_type>);
			if constexpr(decorated_input_stream<handletype>)
			{
				using input_decorator_type = typename handletype::input_decorator_type;
				static_assert(std::same_as<typename input_decorator_type::from_type,typename handletype::char_type>);
				static_assert(std::same_as<typename input_decorator_type::to_type,typename output_decorator_type::from_type>);
				return iobuf_char_type_impl<typename output_decorator_type::from_type>{};
			}
			else
			{
				static_assert(std::same_as<typename handletype::char_type,
					typename output_decorator_type::from_type>);
				static_assert(std::same_as<typename handletype::char_type,
					typename output_decorator_type::to_type>);
				return iobuf_char_type_impl<typename handletype::char_type>{};
			}
		}
		else if constexpr(decorated_input_stream<handletype>)
		{
			using input_decorator_type = typename handletype::input_decorator_type;
			static_assert(std::same_as<typename handletype::char_type,
				typename input_decorator_type::to_type>);
			static_assert(std::same_as<typename handletype::char_type,
				typename input_decorator_type::from_type>);
			return iobuf_char_type_impl<typename handletype::char_type>{};
		}
		else
			return iobuf_char_type_impl<typename handletype::char_type>{};
	}
	else if constexpr((mde&buffer_mode::out)==buffer_mode::out)
	{
		if constexpr(decorated_output_stream<handletype>)
		{
			using output_decorator_type = typename handletype::output_decorator_type;
			static_assert(std::same_as<typename handletype::char_type,
				typename output_decorator_type::to_type>);
			return iobuf_char_type_impl<typename output_decorator_type::from_type>{};
		}
		else
		{
			return iobuf_char_type_impl<typename handletype::char_type>{};
		}
	}
	else if constexpr((mde&buffer_mode::in)==buffer_mode::in)
	{
		if constexpr(decorated_input_stream<handletype>)
		{
			using input_decorator_type = typename handletype::input_decorator_type;
			static_assert(std::same_as<typename handletype::char_type,
				typename input_decorator_type::to_type>);
			return iobuf_char_type_impl<typename input_decorator_type::to_type>{};
		}
		else
			return iobuf_char_type_impl<typename handletype::char_type>{};
	}
}

}

}