#pragma once

namespace fast_io
{

template<stream handletype,buffer_mode mde=buffer_mode::io|buffer_mode::secure_clear,std::size_t bfs = io_default_buffer_size<typename handletype::char_type>,
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

}