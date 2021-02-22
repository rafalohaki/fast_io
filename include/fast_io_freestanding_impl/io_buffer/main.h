#pragma once

namespace fast_io
{

struct empty_decorator
{
};

template<std::integral ch_type,
typename internaltype=empty_decorator,
typename externaltype=empty_decorator>
struct basic_decorators
{
	using internal_type = ch_type;
	using internal_decorator_type=internaltype;
	using external_decorator_type=externaltype;
	[[no_unique_address]] internal_decorator_type internal_decorator;
	[[no_unique_address]] external_decorator_type external_decorator;
};

template<std::integral char_type,typename internaltype,typename externaltype>
requires (!std::same_as<internaltype,empty_decorator>)
inline constexpr internaltype& internal_decorator(basic_decorators<char_type,internaltype,externaltype>& decos) noexcept
{
	return decos.internal_decorator;
}

template<std::integral char_type,typename internaltype,typename externaltype>
requires (!std::same_as<externaltype,empty_decorator>)
inline constexpr externaltype& external_decorator(basic_decorators<char_type,internaltype,externaltype>& decos) noexcept
{
	return decos.external_decorator;
}

namespace details
{

template<typename T>
struct buffer_alloc_arr_ptr
{
	T* ptr{};
	std::size_t size{};
	constexpr buffer_alloc_arr_ptr() noexcept = default;
	constexpr buffer_alloc_arr_ptr(std::size_t sz) noexcept:ptr(allocate_iobuf_space<T>(sz)),size(sz){}

	buffer_alloc_arr_ptr(buffer_alloc_arr_ptr const&)=delete;
	buffer_alloc_arr_ptr& operator=(buffer_alloc_arr_ptr const&)=delete;

	constexpr ~buffer_alloc_arr_ptr()
	{
		deallocate_iobuf_space<T>(ptr,size);
	}
};

template<typename T,typename decot,std::random_access_iterator Iter>
inline constexpr void write_with_deco(T t,decot deco,Iter first,Iter last,basic_io_buffer_pointers<typename decot::char_type>& pointers,std::size_t buffer_size)
{
	using char_type = typename T::char_type;
	using decot_no_cvref_t = std::remove_cvref_t<decot>;
	std::size_t internal_size{buffer_size};
	std::size_t diff{static_cast<std::size_t>(last-first)};
	if(diff<internal_size)
		internal_size=diff;

	auto required_size{deco_reserve_size(io_reserve_type<char_type,decot_no_cvref_t>,deco,internal_size)};
	buffer_alloc_arr_ptr<char_type> alloc_ptr;
	std::size_t this_round{};
	for(;first!=last;)
	{
		this_round=internal_size;
		diff=static_cast<std::size_t>(last-first);
		if(diff<this_round)
			this_round=diff;
		if constexpr(buffer_output_stream<T>)
		{
			auto ob_curr{obuffer_curr(t)};
			auto ob_end{obuffer_end(t)};
			std::size_t tbuffer_size(static_cast<std::size_t>(ob_end-ob_curr));
			if(this_round<tbuffer_size)
			{
				auto [srcit,destit]=deco_reserve_define(io_reserve_type<char_type,decot_no_cvref_t>,deco,first,first+this_round,ob_curr);
				first=srcit;
				obuffer_set_curr(t,destit);
				continue;
			}
		}
		if(alloc_ptr.ptr==nullptr)
			alloc_ptr.ptr=allocate_iobuf_space<char_type>(sz);
		write(t,alloc_ptr.ptr,deco_reserve_define(io_reserve_type<char_type,decot_no_cvref_t>,deco,first,first+this_round,alloc_ptr.ptr));
	}
}

template<typename decorators_type>
concept has_internal_decorator_impl = requires(decorators_type&& decos)
{
	internal_decorator(decos);
};

template<typename decorators_type>
concept has_external_decorator_impl = requires(decorators_type&& decos)
{
	external_decorator(decos);
};

}

template<stream handletype,
buffer_mode mde=buffer_mode::io|buffer_mode::secure_clear|buffer_mode::construct_decorator,
typename decoratorstypr=
basic_decorators<typename handletype::char_type>,
std::size_t bfs = io_default_buffer_size<typename decoratorstypr::internal_type>>
requires (details::constraint_buffer_mode<handletype>(mde))
class basic_io_buffer
{
public:
	using handle_type = handletype;
	using decorators_type = decoratorstypr;
	using external_char_type = typename handle_type::char_type;
	using char_type = typename decorators_type::internal_type;
	using pointer = char_type*;
	using const_pointer = char_type const*;
	inline static constexpr buffer_mode mode = mde;
	inline static constexpr std::size_t buffer_size = bfs;
#if __has_cpp_attribute(no_unique_address) >= 201803L
	[[no_unique_address]]
#endif
	std::conditional_t<(mode&buffer_mode::in)==buffer_mode::in,
	std::conditional_t<details::has_internal_decorator_impl<decorators_type>,basic_io_buffer_pointers_with_cap<char_type>,basic_io_buffer_pointers<char_type>>,
	empty_buffer_pointers> ibuffer;

#if __has_cpp_attribute(no_unique_address) >= 201803L
	[[no_unique_address]]
#endif
	std::conditional_t<(mode&buffer_mode::out)==buffer_mode::out,basic_io_buffer_pointers<char_type>,empty_buffer_pointers> obuffer;

#if __has_cpp_attribute(no_unique_address) >= 201803L
	[[no_unique_address]]
#endif
	std::conditional_t<((mode&buffer_mode::in)==buffer_mode::in)&&details::has_internal_decorator_impl<decorators_type>,
		basic_io_buffer_pointers<external_char_type>,empty_buffer_pointers> ibuffer_external;
#if 0
#if __has_cpp_attribute(no_unique_address) >= 201803L
	[[no_unique_address]]
#endif
	std::conditional_t<((mode&buffer_mode::out)==buffer_mode::out)&&details::has_external_decorator_impl<decorators_type>,
		basic_io_buffer_pointers<external_char_type>,empty_buffer_pointers> obuffer_external;
#endif

#if __has_cpp_attribute(no_unique_address) >= 201803L
	[[no_unique_address]]
#endif
	handle_type handle;
#if __has_cpp_attribute(no_unique_address) >= 201803L
	[[no_unique_address]]
#endif
	decorators_type decorators;
private:
	constexpr void close_throw_impl()
	{
		if(obuffer.buffer_begin!=obuffer.buffer_curr)
		{
			if constexpr(details::has_external_decorator_impl<decorators_type>)
			{
				details::write_with_deco(io_ref(handle),io_deco_ref(external_decorator(decorators)),obuffer.buffer_begin,obuffer.buffer_curr,bfs);
			}
			else
			{
				write(io_ref(handle),obuffer.buffer_begin,obuffer.buffer_curr);
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
		{
			if(obuffer.buffer_begin)
			{
				if constexpr((mode&buffer_mode::secure_clear)==buffer_mode::secure_clear)
				{
#if __cpp_lib_is_constant_evaluated >=201811L
					if(!std::is_constant_evaluated())
#endif
						secure_clear(obuffer.buffer_begin,sizeof(char_type)*buffer_size);
				}
				details::deallocate_iobuf_space<char_type>(obuffer.buffer_begin,buffer_size);
			}
		}
		if constexpr((mode&buffer_mode::in)==buffer_mode::in)
			if(ibuffer.buffer_begin)
			{
				if constexpr(details::has_internal_decorator_impl<decorators_type>)
				{
					std::size_t real_buffer_cap{static_cast<std::size_t>(ibuffer.buffer_cap-ibuffer.buffer_begin)};
					if constexpr((mode&buffer_mode::secure_clear)==buffer_mode::secure_clear)
					{
#if __cpp_lib_is_constant_evaluated >=201811L
						if(!std::is_constant_evaluated())
#endif
							secure_clear(ibuffer.buffer_begin,sizeof(char_type)*real_buffer_cap);
					}
					details::deallocate_iobuf_space<char_type>(ibuffer.buffer_begin,real_buffer_cap);

					std::size_t external_buffer_size{static_cast<std::size_t>(ibuffer_external.buffer_end-ibuffer_external.buffer_begin)};
					if constexpr((mode&buffer_mode::secure_clear)==buffer_mode::secure_clear)
					{
#if __cpp_lib_is_constant_evaluated >=201811L
						if(!std::is_constant_evaluated())
#endif
							secure_clear(ibuffer_external.buffer_begin,external_buffer_size*sizeof(external_char_type));
					}
					details::deallocate_iobuf_space<external_char_type>(ibuffer_external.buffer_begin,external_buffer_size);
				}
				else
				{
					if constexpr((mode&buffer_mode::secure_clear)==buffer_mode::secure_clear)
					{
#if __cpp_lib_is_constant_evaluated >=201811L
						if(!std::is_constant_evaluated())
#endif
							secure_clear(ibuffer.buffer_begin,sizeof(char_type)*buffer_size);
					}
					details::deallocate_iobuf_space<char_type>(ibuffer.buffer_begin,buffer_size);
				}
			}
	}
public:

	constexpr basic_io_buffer()=default;
	template<typename... Args>
	requires (((mode&buffer_mode::construct_decorator)!=buffer_mode::construct_decorator)&&std::constructible_from<handle_type,Args...>)
	explicit constexpr basic_io_buffer(Args&& ...args):handle(std::forward<Args>(args)...){}

	template<typename... Args>
	requires (((mode&buffer_mode::construct_decorator)==buffer_mode::construct_decorator)
	&&std::constructible_from<handle_type,Args...>)
	explicit constexpr basic_io_buffer(decorators_type&& decos,Args&& ...args):handle(std::forward<Args>(args)...),
		decorators(std::move(decos)){}

	constexpr basic_io_buffer(basic_io_buffer const& other) requires std::copyable<handle_type>:handle(other.handle),decorators(other.decorators){}
	constexpr basic_io_buffer(basic_io_buffer const&)=delete;
	constexpr basic_io_buffer& operator=(basic_io_buffer const& other) requires std::copyable<handle_type>
	{
		close_throw_impl();
		if constexpr((mode&buffer_mode::in)==buffer_mode::in)
			ibuffer.buffer_curr=ibuffer.buffer_end;
		if constexpr((mode&buffer_mode::out)==buffer_mode::out)
			obuffer.buffer_curr=obuffer.buffer_begin;
		handle=other.handle;
		decorators=other.decorators;
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
		{
			ibuffer.buffer_curr=ibuffer.buffer_end;
			if constexpr(((mode&buffer_mode::in)==buffer_mode::in)&&details::has_internal_decorator_impl<decorators_type>)
				ibuffer_external.buffer_curr=ibuffer_external.buffer_end;
		}
		if constexpr((mode&buffer_mode::out)==buffer_mode::out)
		{
			obuffer.buffer_curr=obuffer.buffer_begin;
			if constexpr(((mode&buffer_mode::out)==buffer_mode::out)&&details::has_external_decorator_impl<decorators_type>)
				obuffer_external.buffer_curr=obuffer_external.buffer_begin;
		}
		handle.close();
	}
	constexpr basic_io_buffer(basic_io_buffer&& other) noexcept requires(std::movable<handle_type>):
		ibuffer(other.ibuffer),obuffer(other.obuffer),ibuffer_external(other.ibuffer_external),obuffer_external(other.obuffer_external),handle(std::move(other.handle)),decorators(std::move(other.decorators))
	{
		other.ibuffer={};
		other.obuffer={};
		other.ibuffer_external={};
		other.obuffer_external={};
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
		other.ibuffer={};
		obuffer=other.obuffer;
		other.obuffer={};
		ibuffer_external=other.ibuffer_external;
		other.ibuffer_external={};
		obuffer_external=other.obuffer_external;
		other.obuffer_external={};
		handle=std::move(other.handle);
		decorators=std::move(other.decorators);
		return *this;
	}
	constexpr basic_io_buffer& operator=(basic_io_buffer&&)=delete;
	constexpr void swap(basic_io_buffer&& other) noexcept requires std::swappable<handle_type>
	{
		std::ranges::swap(ibuffer,other.ibuffer);
		std::ranges::swap(obuffer,other.obuffer);
		std::ranges::swap(ibuffer_external,other.ibuffer_external);
		std::ranges::swap(obuffer_external,other.obuffer_external);
		std::ranges::swap(handle,other.handle);
		std::ranges::swap(decorators,other.decorators);
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