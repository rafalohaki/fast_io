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

constexpr buffer_mode operator&(buffer_mode x, buffer_mode y) noexcept
{
using utype = typename std::underlying_type<buffer_mode>::type;
return static_cast<buffer_mode>(static_cast<utype>(x) & static_cast<utype>(y));
}

constexpr buffer_mode operator|(buffer_mode x, buffer_mode y) noexcept
{
using utype = typename std::underlying_type<buffer_mode>::type;
return static_cast<buffer_mode>(static_cast<utype>(x) | static_cast<utype>(y));
}

constexpr buffer_mode operator^(buffer_mode x, buffer_mode y) noexcept
{
using utype = typename std::underlying_type<buffer_mode>::type;
return static_cast<buffer_mode>(static_cast<utype>(x) ^ static_cast<utype>(y));
}

constexpr buffer_mode operator~(buffer_mode x) noexcept
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
	if(secure_clear_requirement_stream<handle_type>&&((mode&buffer_mode::secure_clear)!=buffer_mode::secure_clear))
		return false;
	return true;
}



template<typename char_type,std::size_t buffer_size,std::size_t aligmsz>
inline constexpr char_type* allocate_iobuf_space() noexcept
{
#if __cpp_exceptions
	try
	{
#endif
#if __cpp_constexpr >=201907L && __cpp_constexpr_dynamic_alloc >= 201907L && __cpp_lib_is_constant_evaluated >=201811L
		if(std::is_constant_evaluated())
		{
			return new char_type[buffer_size];
		}
		else
#endif
		{
			return static_cast<char_type*>(operator new(buffer_size*sizeof(char_type),std::align_val_t{aligmsz}));
		}
#if __cpp_exceptions
	}
	catch(...)
	{
		fast_terminate();
	}
#endif
}

template<typename char_type,std::size_t buffer_size,std::size_t aligmsz>
inline constexpr void deallocate_iobuf_space(char_type* ptr) noexcept
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

template<stream handletype,buffer_mode mde,std::size_t bfs = io_default_buffer_size<typename handletype::char_type>,
	std::size_t alignmsz=
#ifdef FAST_IO_BUFFER_ALIGNMENT
	FAST_IO_BUFFER_ALIGNMENT
#else
	4096
#endif
	>
requires (details::constraint_buffer_mode<handletype>(mde)&&alignmsz!=0&&
	(alignmsz%(__STDCPP_DEFAULT_NEW_ALIGNMENT__)==0))
class basic_io_buffer
{
public:
	using handle_type = handletype;
	using char_type = typename decltype(details::get_iobuf_char_type<handle_type,mde>())::type;
	using pointer = char_type*;
	using const_pointer = char_type const*;
	inline static constexpr buffer_mode mode = mde;
	inline static constexpr std::size_t buffer_size = bfs;
	inline static constexpr std::size_t buffer_alignment = alignmsz;
#if __has_cpp_attribute(no_unique_address) >= 201803L
	[[no_unique_address]]
#endif
	std::conditional_t<(mode&buffer_mode::in)==buffer_mode::in,basic_io_buffer_pointers<char_type>,empty_buffer_pointers> ibuffer;

#if __has_cpp_attribute(no_unique_address) >= 201803L
	[[no_unique_address]]
#endif
	std::conditional_t<(mode&buffer_mode::out)==buffer_mode::out,basic_io_buffer_pointers<char_type>,empty_buffer_pointers> obuffer;

#if __has_cpp_attribute(no_unique_address) >= 201803L
	[[no_unique_address]]
#endif
	handle_type handle;

private:
	constexpr void close_throw_impl()
	{
		if(obuffer.buffer_begin!=obuffer.buffer_curr)
			write(handle,obuffer.buffer_begin,obuffer.buffer_curr);
	}
	constexpr void close_impl() noexcept
	{
		if constexpr((mode&buffer_mode::out)==buffer_mode::out)
		{
			if constexpr(noexcept(
				write(handle,obuffer.buffer_begin,obuffer.buffer_curr)
			))
			{
				close_throw_impl();
			}
			else
			{
#ifdef __cpp_exceptions
				try
				{
#endif
					close_throw_impl();
#ifdef __cpp_exceptions
				}
				catch(...)
				{
				}
#endif
			}
		}
	}
	constexpr void cleanup_impl() noexcept
	{
		if constexpr((mode&buffer_mode::out)==buffer_mode::out)
			if(obuffer.buffer_begin)
			{
				if constexpr((mode&buffer_mode::secure_clear)==buffer_mode::secure_clear)
					secure_clear(obuffer.buffer_begin,sizeof(char_type)*buffer_size);
				details::deallocate_iobuf_space<char_type,buffer_size,buffer_alignment>(obuffer.buffer_begin);
			}
		if constexpr((mode&buffer_mode::in)==buffer_mode::in)
			if(ibuffer.buffer_begin)
			{
				if constexpr((mode&buffer_mode::secure_clear)==buffer_mode::secure_clear)
					secure_clear(ibuffer.buffer_begin,sizeof(char_type)*buffer_size);
				details::deallocate_iobuf_space<char_type,buffer_size,buffer_alignment>(ibuffer.buffer_begin);
			}
	}
public:
	constexpr basic_io_buffer()=default;
	template<typename... Args>
	requires (sizeof...(Args)!=0)&&std::constructible_from<handle_type,Args...>
	explicit constexpr basic_io_buffer(Args&& ...args):handle(std::forward<Args>(args)...){}
	basic_io_buffer(basic_io_buffer const&)=delete;
	basic_io_buffer& operator=(basic_io_buffer const&)=delete;
#if __cpp_constexpr_dynamic_alloc >= 201907L
	constexpr
#endif
	~basic_io_buffer()
	{
		if constexpr((mode&buffer_mode::out)==buffer_mode::out)
			close_impl();
		cleanup_impl();
	}
};


namespace details
{

template<typename handletype,typename Iter>
concept allow_iobuf_punning = stream<handletype>&&std::forward_iterator<Iter>&&std::contiguous_iterator<Iter>&&
//temporary only allow contiguous_iterator before we finish this part
(std::same_as<typename handletype::char_type,std::iter_value_t<Iter>>||
(std::same_as<typename handletype::char_type,char>&&std::contiguous_iterator<Iter>));

template<typename T,std::forward_iterator Iter>
inline constexpr void iobuf_write_unhappy_decay_impl(T t,basic_io_buffer_pointers<typename T::char_type>& pointers,Iter first,Iter last)
{
	if constexpr(std::contiguous_iterator<Iter>)
	{
		if constexpr(scatter_output_stream<T>)
		{
			io_scatter_t scatters[2]{{pointers.buffer_begin,
				sizeof(*pointers.buffer_curr)*(pointers.buffer_curr-pointers.buffer_begin)},
				{first,(last-first)*sizeof(*last)}};
			scatter_write(t,scatters);
			pointers.buffer_curr=pointers.buffer_begin;
		}
		else
		{
			if(pointers.buffer_begin!=pointers.buffer_curr)
			{
				write(t,pointers.buffer_begin,pointers.buffer_curr);
				pointers.buffer_curr=pointers.buffer_begin;
			}
			write(t,first,last);
		}
	}
	else
	{
		//Todo: deal with non contiguous iterator
	}
}

template<typename T,std::forward_iterator Iter>
inline constexpr void iobuf_write_unhappy_impl(T& t,Iter first,Iter last,std::size_t dis)
{
	if(t.obuffer.buffer_begin==nullptr&&dis<T::buffer_size)
	{
		t.obuffer.buffer_end=(t.obuffer.buffer_curr=t.obuffer.buffer_begin=
			allocate_iobuf_space<typename T::char_type,T::buffer_size,T::buffer_alignment>())+T::buffer_size;
		t.obuffer.buffer_curr=non_overlapped_copy(first,last,t.obuffer.buffer_curr);
		return;
	}
	iobuf_write_unhappy_decay_impl(io_ref(t.handle),t.obuffer,first,last);
}


template<typename T,std::random_access_iterator Iter>
inline constexpr void iobuf_write_unhappy_impl(T& t,Iter first,Iter last)
{
	iobuf_write_unhappy_impl(t,first,last,static_cast<std::size_t>(last-first));
}

}

template<stream handletype,buffer_mode mde,std::size_t bfs,std::size_t alignsz,std::forward_iterator Iter>
requires (output_stream<handletype>&&details::allow_iobuf_punning<handletype,Iter>)
inline constexpr decltype(auto) write(basic_io_buffer<handletype,mde,bfs,alignsz>& bios,Iter first,Iter last)
{
	if constexpr((mde&buffer_mode::out)==buffer_mode::out)
	{
		using iter_char_type = std::iter_value_t<Iter>;
		if constexpr(std::same_as<iter_char_type,typename handletype::char_type>)
		{
			auto diff(std::distance(first,last));
			auto remain_space(bios.obuffer.buffer_end-bios.obuffer.buffer_curr);
			if(remain_space<diff)[[unlikely]]
			{
				if constexpr(std::contiguous_iterator<Iter>)
					details::iobuf_write_unhappy_impl(bios,std::to_address(first),std::to_address(last));
				else if constexpr(std::random_access_iterator<Iter>)
					details::iobuf_write_unhappy_impl(bios,first,last);
				else
					details::iobuf_write_unhappy_impl(bios,first,last,diff);
				return;
			}
			else [[likely]]
				bios.obuffer.buffer_curr=details::non_overlapped_copy_n(first,diff,bios.obuffer.buffer_curr);
		}
		else
			write(bios,reinterpret_cast<char const*>(std::to_address(first)),
				reinterpret_cast<char const*>(std::to_address(last)));
	}
	else
	{
		return write(bios.handle,first,last);
	}
}


template<stream handletype,buffer_mode mde,std::size_t bfs,std::size_t alignsz>
requires ((mde&buffer_mode::out)==buffer_mode::out)
inline constexpr auto obuffer_begin(basic_io_buffer<handletype,mde,bfs,alignsz>& bios) noexcept
{
	return bios.obuffer.buffer_begin;
}


template<stream handletype,buffer_mode mde,std::size_t bfs,std::size_t alignsz>
requires ((mde&buffer_mode::out)==buffer_mode::out)
inline constexpr auto obuffer_curr(basic_io_buffer<handletype,mde,bfs,alignsz>& bios) noexcept
{
	return bios.obuffer.buffer_curr;
}

template<stream handletype,buffer_mode mde,std::size_t bfs,std::size_t alignsz>
requires ((mde&buffer_mode::out)==buffer_mode::out)
inline constexpr auto obuffer_end(basic_io_buffer<handletype,mde,bfs,alignsz>& bios) noexcept
{
	return bios.obuffer.buffer_end;
}

template<stream handletype,buffer_mode mde,std::size_t bfs,std::size_t alignsz>
requires ((mde&buffer_mode::out)==buffer_mode::out)
inline constexpr void obuffer_set_curr(basic_io_buffer<handletype,mde,bfs,alignsz>& bios,typename basic_io_buffer<handletype,mde,bfs,alignsz>::char_type* ptr) noexcept
{
	bios.obuffer.buffer_curr=ptr;
}

namespace details
{
template<std::size_t bfsz,std::size_t almsz,typename T,std::integral char_type>
inline constexpr void iobuf_overflow_impl(T handle,basic_io_buffer_pointers<char_type>& pointers,typename T::char_type ch)
{
	if(pointers.buffer_begin==nullptr)
	{
		pointers.buffer_end=(pointers.buffer_curr=pointers.buffer_begin=
		allocate_iobuf_space<char_type,bfsz,almsz>())+bfsz;
	}
	else
	{
		write(handle,pointers.buffer_begin,pointers.buffer_curr);
		pointers.buffer_curr=pointers.buffer_begin;
	}
	*pointers.buffer_curr=ch;
	++pointers.buffer_curr;
}
}

template<stream handletype,buffer_mode mde,std::size_t bfs,std::size_t alignsz>
requires ((mde&buffer_mode::out)==buffer_mode::out)
inline constexpr auto overflow(basic_io_buffer<handletype,mde,bfs,alignsz>& bios,
	typename basic_io_buffer<handletype,mde,bfs,alignsz>::char_type ch)
{
	details::iobuf_overflow_impl<bfs,alignsz>(io_ref(bios.handle),bios.obuffer,ch);
}
}