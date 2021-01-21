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


template<typename char_type>
inline constexpr char_type* allocate_iobuf_space(std::size_t buffer_size,std::size_t aligmsz) noexcept
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
//Let std::bad_aloc to die.
		fast_terminate();
	}
#endif
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
				{
#if __cpp_lib_is_constant_evaluated >=201811L
					if(!std::is_constant_evaluated())
#endif
						secure_clear(obuffer.buffer_begin,sizeof(char_type)*buffer_size);
				}
				details::deallocate_iobuf_space<char_type>(obuffer.buffer_begin,buffer_size,buffer_alignment);
			}
		if constexpr((mode&buffer_mode::in)==buffer_mode::in)
			if(ibuffer.buffer_begin)
			{
				if constexpr((mode&buffer_mode::secure_clear)==buffer_mode::secure_clear)
				{
#if __cpp_lib_is_constant_evaluated >=201811L
					if(!std::is_constant_evaluated())
#endif
						secure_clear(ibuffer.buffer_begin,sizeof(char_type)*buffer_size);
				}
				details::deallocate_iobuf_space<char_type>(ibuffer.buffer_begin,buffer_size,buffer_alignment);
			}
	}
public:
	constexpr basic_io_buffer()=default;
	template<typename... Args>
	requires (sizeof...(Args)!=0)&&std::constructible_from<handle_type,Args...>
	explicit constexpr basic_io_buffer(Args&& ...args):handle(std::forward<Args>(args)...){}
	constexpr basic_io_buffer(basic_io_buffer const& other) requires std::copyable<handle_type>:handle(other.handle){}
	constexpr basic_io_buffer(basic_io_buffer const&)=delete;
	constexpr basic_io_buffer& operator=(basic_io_buffer const& other) requires std::copyable<handle_type>
	{
		close_throw_impl();
		if constexpr((mode&buffer_mode::in)==buffer_mode::in)
			ibuffer.buffer_curr=ibuffer.buffer_end;
		if constexpr((mode&buffer_mode::out)==buffer_mode::out)
			obuffer.buffer_curr=obuffer.buffer_begin;
		handle=other.handle;
		return *this;
	}
	constexpr basic_io_buffer& operator=(basic_io_buffer const&)=delete;
#if 0
	template<typename... Args>
	requires requires(Args&& ...args)
	{
		handle.reopen(std::forward<Args>(args)...);
	}
	constexpr void reopen(Args&& ...args)
	{
		handle.reopen(std::forward<Args>(args)...);
	}
#endif
	constexpr void close() requires requires()
	{
		handle.close();
	}
	{
		close_throw_impl();
		if constexpr((mode&buffer_mode::in)==buffer_mode::in)
			ibuffer.buffer_curr=ibuffer.buffer_end;
		if constexpr((mode&buffer_mode::out)==buffer_mode::out)
			obuffer.buffer_curr=obuffer.buffer_begin;
		handle.close();
	}
	constexpr basic_io_buffer(basic_io_buffer&& other) noexcept requires(std::movable<handle_type>):
		ibuffer(other.ibuffer),obuffer(other.obuffer),handle(std::move(other.handle))
	{
		other.ibuffer={};
		other.obuffer={};
	}
	constexpr basic_io_buffer(basic_io_buffer&&) noexcept=delete;
#if __cpp_constexpr_dynamic_alloc >= 201907L
	constexpr
#endif
	basic_io_buffer& operator=(basic_io_buffer&& other) noexcept requires(std::movable<handle_type>)
	{
		if(this==std::addressof(other))
			return *this;
		cleanup_impl();
		ibuffer=other.ibuffer;
		obuffer=other.obuffer;
		handle=std::move(other.handle);
		return *this;
	}
	constexpr basic_io_buffer& operator=(basic_io_buffer&&)=delete;
	constexpr void swap(basic_io_buffer&& other) noexcept requires std::swappable<handle_type>
	{
		std::ranges::swap(ibuffer,other.ibuffer);
		std::ranges::swap(obuffer,other.obuffer);
		std::ranges::swap(handle,other.handle);
	}
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

template<std::integral char_type,std::random_access_iterator Iter>
inline constexpr void iobuf_write_unhappy_nullptr_case_impl(basic_io_buffer_pointers<char_type>& obuffer,Iter first,Iter last,std::size_t buffer_size,std::size_t buffer_alignment)
{
	obuffer.buffer_end=(obuffer.buffer_curr=obuffer.buffer_begin=
	allocate_iobuf_space<char_type>(buffer_size,buffer_alignment))+buffer_size;
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
		write(t,first,last);
	else
		pointers.buffer_curr=non_overlapped_copy_n(first,new_remain_space,pointers.buffer_begin);
}

template<std::size_t buffer_size,std::size_t buffer_alignment,typename T,std::integral char_type,std::random_access_iterator Iter>
inline constexpr void iobuf_write_unhappy_decay_impl(T t,basic_io_buffer_pointers<char_type>& pointers,Iter first,Iter last)
{
	std::size_t const diff(static_cast<std::size_t>(last-first));
	if(pointers.buffer_begin==nullptr)
	{
		if(diff<buffer_size)
			iobuf_write_unhappy_nullptr_case_impl(pointers,first,last,buffer_size,buffer_alignment);
		else
			write(t,first,last);
		return;
	}
	iobuf_write_unhappy_decay_no_alloc_impl(t,pointers,first,last,buffer_size);
}

template<typename T,std::random_access_iterator Iter>
inline constexpr void iobuf_write_unhappy_impl(T& t,Iter first,Iter last)
{
	iobuf_write_unhappy_decay_impl<T::buffer_size,T::buffer_alignment>(io_ref(t.handle),t.obuffer,first,last);
}

}

template<stream handletype,buffer_mode mde,std::size_t bfs,std::size_t alignsz,std::random_access_iterator Iter>
requires (((mde&buffer_mode::out)==buffer_mode::out)&&details::allow_iobuf_punning<handletype,Iter>)
inline constexpr decltype(auto) write(basic_io_buffer<handletype,mde,bfs,alignsz>& bios,Iter first,Iter last)
{
	using iter_char_type = std::iter_value_t<Iter>;
	using char_type = typename handletype::char_type;
	if constexpr(std::same_as<iter_char_type,char_type>)
	{
		if constexpr(std::contiguous_iterator<Iter>&&!std::is_pointer_v<Iter>)
			write(bios,std::to_address(first),std::to_address(last));
		else if constexpr(std::random_access_iterator<Iter>)
		{
			std::size_t diff(static_cast<std::size_t>(last-first));
			std::size_t remain_space(bios.obuffer.buffer_end-bios.obuffer.buffer_curr);
			if(remain_space<diff)[[unlikely]]
			{
				details::iobuf_write_unhappy_impl(bios,first,last);
				return;
			}
			bios.obuffer.buffer_curr=details::non_overlapped_copy_n(first,diff,bios.obuffer.buffer_curr);
		}
/*
To do : forward_iterator. Support std::forward_list, std::list, std::set and std::unordered_set
*/
	}
	else
		write(bios,reinterpret_cast<char const*>(std::to_address(first)),
			reinterpret_cast<char const*>(std::to_address(last)));
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
		allocate_iobuf_space<char_type>(bfsz,almsz))+bfsz;
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