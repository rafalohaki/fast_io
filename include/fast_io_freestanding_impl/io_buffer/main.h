#pragma once

namespace fast_io
{

template<std::integral ch_type>
struct empty_decorator
{
	using char_type = ch_type;
};

namespace details
{

template<typename T>
struct buffer_alloc_arr_ptr
{
	T* ptr{};
	std::size_t size{};
	constexpr buffer_alloc_arr_ptr() noexcept = default;
	constexpr buffer_alloc_arr_ptr(std::size_t sz) noexcept:ptr(allocate_iobuf_space<T>(size,64)),size(sz){}

	buffer_alloc_arr_ptr(buffer_alloc_arr_ptr const&)=delete;
	buffer_alloc_arr_ptr& operator=(buffer_alloc_arr_ptr const&)=delete;

	constexpr ~buffer_alloc_arr_ptr()
	{
		deallocate_iobuf_space<T>(ptr,size,64);
	}
};

template<typename T,typename decot,std::random_access_iterator Iter>
inline constexpr void write_with_deco(T t,decot& deco,Iter first,Iter last,std::size_t buffer_size)
{
	using char_type = typename T::char_type;
	using decot_no_cvref_t = std::remove_cvref_t<decot>;
	std::size_t internal_size{buffer_size};
	std::size_t diff{static_cast<std::size_t>(last-first)};
	if(diff<internal_size)
		internal_size=diff;
	buffer_alloc_arr_ptr<char_type> alloc_ptr{deco_reserve_size(io_reserve_type<char_type,decot_no_cvref_t>,diff)};
	for(;first!=last;)
	{
		std::size_t this_round{internal_size};
		if(static_cast<std::size_t>(last-first)<this_round)
			this_round=static_cast<std::size_t>(last-first);
 		write(t,alloc_ptr.ptr,deco_reserve_define(io_reserve_type<char_type,decot_no_cvref_t>,deco,first,first+this_round,alloc_ptr.ptr));
		first+=this_round;
	}
}
}

template<stream handletype,
typename internaldecorator=empty_decorator<typename handletype::char_type>,
typename externaldecorator=empty_decorator<typename handletype::char_type>,
buffer_mode mde=buffer_mode::io|buffer_mode::secure_clear,std::size_t bfs = io_default_buffer_size<typename handletype::char_type>,
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
	using internal_decorator_type = internaldecorator;
	using external_decorator_type = externaldecorator;
	using char_type = typename internaldecorator::char_type;
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
#if __has_cpp_attribute(no_unique_address) >= 201803L
	[[no_unique_address]]
#endif
	internal_decorator_type internal_decorator;
#if __has_cpp_attribute(no_unique_address) >= 201803L
	[[no_unique_address]]
#endif
	external_decorator_type external_decorator;
private:
	constexpr void close_throw_impl()
	{
		if(obuffer.buffer_begin!=obuffer.buffer_curr)
		{
			if constexpr(std::same_as<external_decorator_type,empty_decorator<typename handletype::char_type>>)
			{
				write_with_deco(handle,external_decorator,obuffer.buffer_begin,obuffer.buffer_curr,bfs);
			}
			else
			{
				write(handle,obuffer.buffer_begin,obuffer.buffer_curr);
			}
		}
	}
	constexpr void close_impl() noexcept
	{
		if constexpr((mode&buffer_mode::out)==buffer_mode::out)
		{
#if __cpp_exceptions
			try
			{
#endif
				close_throw_impl();
#if __cpp_exceptions
			}
			catch(...)
			{
			}
#endif
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
	requires (std::is_empty_v<internal_decorator_type>&&std::is_empty_v<external_decorator_type>
	&&(sizeof...(Args)!=0)&&std::constructible_from<handle_type,Args...>)
	explicit constexpr basic_io_buffer(Args&& ...args):handle(std::forward<Args>(args)...){}

	template<typename indtype,
	typename endtype,
	typename... Args>
	requires (!std::is_empty_v<internal_decorator_type>&&!std::is_empty_v<external_decorator_type>
	&&std::constructible_from<internal_decorator_type,indtype>
	&&std::constructible_from<external_decorator_type,endtype>
	&&std::constructible_from<handle_type,Args...>)
	explicit constexpr basic_io_buffer(indtype&& ideco,
	endtype&& edeco,Args&& ...args):handle(std::forward<Args>(args)...),
		internal_decorator_type(std::forward<indtype>(ideco)),
		external_decorator_type(std::forward<endtype>(edeco)){}

	template<typename indtype,typename... Args>
	requires (!std::is_empty_v<internal_decorator_type>&&std::is_empty_v<external_decorator_type>
	&&std::constructible_from<internal_decorator_type,indtype>
	&&std::constructible_from<handle_type,Args...>)
	explicit constexpr basic_io_buffer(indtype&& ideco,Args&& ...args):handle(std::forward<Args>(args)...),
		internal_decorator_type(std::forward<indtype>(ideco)){}

	template<typename endtype,typename... Args>
	requires (std::is_empty_v<internal_decorator_type>&&!std::is_empty_v<external_decorator_type>
	&&std::constructible_from<external_decorator_type,endtype>
	&&std::constructible_from<handle_type,Args...>)
	explicit constexpr basic_io_buffer(endtype&& edeco,Args&& ...args):handle(std::forward<Args>(args)...),
		external_decorator_type(std::forward<endtype>(edeco)){}


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

}