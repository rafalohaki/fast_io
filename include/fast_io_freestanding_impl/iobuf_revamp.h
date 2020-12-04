#pragma once

namespace fast_io
{

enum class buffer_flags
{
in=1<<0,
out=1<<1,
tie=1<<2,
io=in|out|tie,
secure_clear=1<<3
};

constexpr buffer_flags operator&(buffer_flags x, buffer_flags y) noexcept
{
using utype = typename std::underlying_type<buffer_flags>::type;
return static_cast<buffer_flags>(static_cast<utype>(x) & static_cast<utype>(y));
}

constexpr buffer_flags operator|(buffer_flags x, buffer_flags y) noexcept
{
using utype = typename std::underlying_type<buffer_flags>::type;
return static_cast<buffer_flags>(static_cast<utype>(x) | static_cast<utype>(y));
}

constexpr buffer_flags operator^(buffer_flags x, buffer_flags y) noexcept
{
using utype = typename std::underlying_type<buffer_flags>::type;
return static_cast<buffer_flags>(static_cast<utype>(x) ^ static_cast<utype>(y));
}

constexpr buffer_flags operator~(buffer_flags x) noexcept
{
using utype = typename std::underlying_type<buffer_flags>::type;
return static_cast<buffer_flags>(~static_cast<utype>(x));
}

inline constexpr buffer_flags& operator&=(buffer_flags& x, buffer_flags y) noexcept{return x=x&y;}

inline constexpr buffer_flags& operator|=(buffer_flags& x, buffer_flags y) noexcept{return x=x|y;}

inline constexpr buffer_flags& operator^=(buffer_flags& x, buffer_flags y) noexcept{return x=x^y;}


template<typename char_type>
inline constexpr std::size_t io_default_buffer_size = details::cal_buffer_size<char_type,true>();

template<typename pointer_type>
struct basic_buffer_pointers
{
	pointer_type beg{},curr{},ed{};
};

struct empty_buffer_pointers
{};

namespace details
{
template<stream handle_type>
inline 
#if __cpp_consteval >= 201811L
consteval
#else
constexpr
#endif
bool constraint_buffer_flags(buffer_flags mode) noexcept
{
	if(((mode&buffer_flags::in)==buffer_flags::in)&&(!input_stream<handle_type>))
		return false;
	if(((mode&buffer_flags::out)==buffer_flags::out)&&(!output_stream<handle_type>))
		return false;
	if(secure_clear_requirement_stream<handle_type>&&((mode&buffer_flags::secure_clear)!=buffer_flags::secure_clear))
		return false;
	return true;
}

template<typename allocator_type,typename allocator_type::size_type buffer_size>
struct allocator_guard
{
	using allocator_traits = std::allocator_traits<allocator_type>;
	[[no_unique_address]] std::conditional_t<std::is_empty_v<allocator_type>,allocator_type,allocator_type&> alloc;
	[[no_unique_address]] allocator_type::pointer ptr;
	constexpr ~allocator_guard()
	{
		if(ptr)
			allocator_traits::deallocate(alloc,ptr,buffer_size);
	}
};
}

template<stream handletype,
	buffer_flags mde,
	typename Decorator = void,
	typename Allocator = io_aligned_allocator<typename handletype::char_type>,
	std::size_t bfs = io_default_buffer_size<typename handletype::char_type>>
requires (details::constraint_buffer_flags<handletype>(mde))
class basic_io_buffer
{
public:
	using handle_type = handletype;
	using allocator_type = Allocator;
	using allocator_traits = std::allocator_traits<allocator_type>;
	using decorator_type = Decorator;
	using char_type = typename allocator_traits::value_type;
	using pointer = typename allocator_traits::pointer;
	using const_pointer = typename allocator_traits::const_pointer;
	using size_type = typename allocator_traits::size_type;
	using difference_type = typename allocator_traits::difference_type;
	using reference = char_type&;
	using const_reference = char_type const&;
	inline static constexpr buffer_flags mode = mde;
	inline static constexpr size_type buffer_size = bfs;
	[[no_unique_address]] std::conditional_t<(mode&buffer_flags::in)==buffer_flags::in,basic_buffer_pointers<pointer>,empty_buffer_pointers> ibuffer;
	[[no_unique_address]] std::conditional_t<(mode&buffer_flags::out)==buffer_flags::out,basic_buffer_pointers<pointer>,empty_buffer_pointers> obuffer;
	struct compound_type
	{
		[[no_unique_address]] handle_type handle;
		[[no_unique_address]] std::conditional_t<std::same_as<Decorator,void>,empty_buffer_pointers,Decorator> decorator;
		[[no_unique_address]] allocator_type allocator;
	};
	[[no_unique_address]] compound_type compound;
	constexpr basic_io_buffer()=default;
	explicit constexpr basic_io_buffer(compound_type cpd) noexcept
		:compound(std::move(cpd)){}

private:
	constexpr void close_throw_impl()
	{
		if(obuffer.beg!=obuffer.curr)
			write(compound.handle,obuffer.beg,obuffer.curr);
	}
	constexpr void close_impl() noexcept
	{
		if constexpr((mode&buffer_flags::out)==buffer_flags::out)
		{
			if constexpr(noexcept(
				write(compound.handle,obuffer.beg,obuffer.curr)
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
		if constexpr((mode&buffer_flags::secure_clear)==buffer_flags::secure_clear)
		{
			if constexpr((mode&buffer_flags::out)==buffer_flags::out)
				if(obuffer.beg)
				{
					secure_clear(std::to_address(obuffer.beg),sizeof(char_type)*buffer_size);
					allocator_traits::deallocate(compound.allocator,obuffer.beg,buffer_size);
				}
			if constexpr((mode&buffer_flags::in)==buffer_flags::in)
				if(ibuffer.beg)
				{
					secure_clear(std::to_address(ibuffer.beg),sizeof(char_type)*buffer_size);
					allocator_traits::deallocate(compound.allocator,ibuffer.beg,buffer_size);
				}
		}
		else
		{
			if constexpr((mode&buffer_flags::out)==buffer_flags::out)
				if(obuffer.beg)
					allocator_traits::deallocate(compound.allocator,obuffer.beg,buffer_size);
			if constexpr((mode&buffer_flags::in)==buffer_flags::in)
				if(ibuffer.beg)
					allocator_traits::deallocate(compound.allocator,ibuffer.beg,buffer_size);
		}
	}
public:

	constexpr basic_io_buffer(basic_io_buffer&& other) noexcept:
		ibuffer(other.ibuffer),obuffer(other.obuffer),compound(std::move(other.compound))
	{
		other.ibuffer={};
		other.obuffer={};
	}

	constexpr basic_io_buffer(basic_io_buffer&& other, allocator_type const& alloc)
		noexcept(allocator_traits::is_always_equal) :
		compound{std::move(other.compound.handle),std::move(other.compound.decorator),allocator(alloc)}
	{
		if constexpr(!allocator_traits::is_always_equal)
		{
			if(alloc!=other.compound.allocator)
			{
				if constexpr((mode&buffer_flags::in)==buffer_flags::in)
				{
					{
						std::size_t const sz(other.ibuffer.ed-other.ibuffer.curr);
						if(sz)
						{
							ibuffer.beg=allocator_traits::allocate(compound.allocator,buffer_size);
							ibuffer.ed=ibuffer.beg+buffer_size;
							ibuffer.curr=non_overlapped_copy_n(other.ibuffer.curr,sz,ibuffer.beg);
						}
					}
					if constexpr((mode&buffer_flags::out)==buffer_flags::out)
					{
						std::size_t sz(other.obuffer.ed-other.obuffer.curr);
						if(sz)
						{
							details::allocator_guard<allocator_type,buffer_size> gd{compound.allocator,ibuffer.beg};
							obuffer.curr=obuffer.beg=allocator_traits::allocate(compound.allocator,buffer_size);
							obuffer.ed=non_overlapped_copy_n(other.obuffer.beg,sz,obuffer.beg);
						}
					}
				}
				else
				{
					if constexpr((mode&buffer_flags::out)==buffer_flags::out)
					{
						std::size_t sz(other.obuffer.ed-other.obuffer.curr);
						if(sz)
						{
							obuffer.curr=obuffer.beg=allocator_traits::allocate(compound.allocator,buffer_size);
							obuffer.ed=non_overlapped_copy_n(other.obuffer.beg,sz,obuffer.beg);
						}
					}
				}
				return;
			}
		}
		ibuffer=other.ibuffer;
		obuffer=other.obuffer;
		other.ibuffer={};
		other.obuffer={};
	}

	constexpr basic_io_buffer(basic_io_buffer const& other) requires(std::copyable<handle_type>&&std::copyable<decorator_type>):
			compound{other.handle,other.decorator,allocator_traits::select_on_container_copy_construction(other.alloc)}
	{}

	constexpr basic_io_buffer(basic_io_buffer const& other, allocator_type const& alloc) requires(std::copyable<handle_type>&&std::copyable<decorator_type>):
			compound{other.handle,other.decorator,alloc}
	{}

	constexpr std::size_t max_size() noexcept
	{
		return allocator_traits::max_size();
	}

	constexpr void close() requires(closable_stream<handle_type>)
	{
		close_throw_impl();
		compound.handle.close();
		if constexpr((mode&buffer_flags::in)==buffer_flags::in)
			ibuffer.ed=ibuffer.curr=ibuffer.beg;
		if constexpr((mode&buffer_flags::out)==buffer_flags::out)
			obuffer.curr=obuffer.beg;
	}
	constexpr basic_io_buffer& operator=(basic_io_buffer const& other) requires(std::copyable<handle_type>&&std::copyable<decorator_type>)
	{
		close_throw_impl();
		if constexpr(allocator_traits::propagate_on_container_copy_assignment::value)
		{
			if(other.allocator!=this->allocator)
			{
				cleanup_impl();
				if constexpr((mode&buffer_flags::in)==buffer_flags::in)
					ibuffer={};
				if constexpr((mode&buffer_flags::out)==buffer_flags::out)
					obuffer={};
			}
			else
			{
				if constexpr((mode&buffer_flags::in)==buffer_flags::in)
				{
					if(ibuffer.beg)
						ibuffer.ed=ibuffer.curr=ibuffer.beg;
				}
				if constexpr((mode&buffer_flags::out)==buffer_flags::out)
				{
					if(obuffer.beg)
						obuffer.curr=obuffer.beg;
				}
			}
		}
		else
		{
			if constexpr((mode&buffer_flags::in)==buffer_flags::in)
				ibuffer.ed=ibuffer.curr=ibuffer.beg;
			if constexpr((mode&buffer_flags::out)==buffer_flags::out)
				obuffer.curr=obuffer.beg;
		}
		compound=other.compound;
		return *this;
	}

	constexpr basic_io_buffer& operator=(basic_io_buffer&& other)
		noexcept(allocator_traits::propagate_on_container_move_assignment::value
			||allocator_traits::is_always_equal::value)
	{
		if(std::addressof(other)==this)
			return *this;
		close_impl();
		if constexpr(!allocator_traits::propagate_on_container_move_assignment::value
			&&!allocator_traits::is_always_equal::value)
		{
			if(other.compound.allocator!=compound.allocator)
			{
				if constexpr((mode&buffer_flags::in)==buffer_flags::in)
				{
					{
						std::size_t const sz(other.ibuffer.ed-other.ibuffer.curr);
						if(sz)
						{
							ibuffer.beg=allocator_traits::allocate(compound.allocator,buffer_size);
							ibuffer.ed=ibuffer.beg+buffer_size;
							ibuffer.curr=non_overlapped_copy_n(other.ibuffer.curr,sz,ibuffer.beg);
						}
					}
					if constexpr((mode&buffer_flags::out)==buffer_flags::out)
					{
						std::size_t sz(other.obuffer.ed-other.obuffer.curr);
						if(sz)
						{
							details::allocator_guard<allocator_type,buffer_size> gd{compound.allocator,ibuffer.beg};
							obuffer.curr=obuffer.beg=allocator_traits::allocate(compound.allocator,buffer_size);
							obuffer.ed=non_overlapped_copy_n(other.obuffer.beg,sz,obuffer.beg);
						}
					}
				}
				else
				{
					if constexpr((mode&buffer_flags::out)==buffer_flags::out)
					{
						std::size_t sz(other.obuffer.ed-other.obuffer.curr);
						if(sz)
						{
							obuffer.curr=obuffer.beg=allocator_traits::allocate(compound.allocator,buffer_size);
							obuffer.ed=non_overlapped_copy_n(other.obuffer.beg,sz,obuffer.beg);
						}
					}
				}
				compound=std::move(other.compound);
				return *this;
			}
		}
		compound=std::move(other.compound);
		obuffer=std::move(other.obuffer);
		ibuffer=std::move(other.ibuffer);
		other.obuffer={};
		other.ibuffer={};
		return *this;
	}
	constexpr void swap(basic_io_buffer& other) noexcept(allocator_traits::propagate_on_container_swap::value||allocator_traits::is_always_equal::value)
	{
/*
If std::allocator_traits<allocator_type>::propagate_on_container_swap::value is true, then the allocators are exchanged using an unqualified call to non-member swap. Otherwise, they are not swapped (and if get_allocator() != other.get_allocator(), the behavior is undefined).

WTF is this garbage?

*/
		if constexpr(std::allocator_traits<allocator_type>::propagate_on_container_swap::value)
		{
			if(other.compound.allocator!=this->compound.allocator)
				return;
		}
		using std::swap;
		swap(compound.handle,other.compound.handle);
		swap(compound.decorator,other.compound.decorator);
		swap(compound.allocator,other.compound.allocator);
		swap(obuffer,other.obuffer);
		swap(ibuffer,other.ibuffer);
	}
	constexpr ~basic_io_buffer()
	{
		close_impl();
		cleanup_impl();
	}
};

template<stream handletype,buffer_flags mde,typename Decorator,typename Allocator,std::size_t buffer_size>
inline constexpr void swap(basic_io_buffer<handletype,mde,Decorator,Allocator,buffer_size>& lhs,basic_io_buffer<handletype,mde,Decorator,Allocator,buffer_size>& rhs)
	noexcept(noexcept(lhs.swap(rhs)))
{
	lhs.swap(rhs);
}



}