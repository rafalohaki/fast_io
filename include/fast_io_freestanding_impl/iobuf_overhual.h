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



template<typename char_type,std::size_t buffer_size,std::size_t aligmsz>
inline constexpr char_type* allocate_iobuf_space() noexcept
{
#if __cpp_exceptions
	try
	{
#endif
		return static_cast<char_type*>(operator new(buffer_size*sizeof(char_type),std::align_val_t{aligmsz}));
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
#if __cpp_exceptions
	try
	{
#endif
		operator delete(ptr,buffer_size*sizeof(char_type),std::align_val_t{aligmsz}));
	}
#if __cpp_exceptions
	}
	catch(...)
	{
		fast_terminate();
	}
#endif
}

}

template<stream handletype,buffer_flags mde,std::size_t bfs = io_default_buffer_size<typename handletype::char_type>,
	std::size_t alignmsz=
#ifdef FAST_IO_BUFFER_ALIGNMENT
	FAST_IO_BUFFER_ALIGNMENT
#else
	4096
#endif
	>
requires (details::constraint_buffer_flags<handletype>(mde)&&alignmsz!=0&&
	(alignmsz%(__STDCPP_DEFAULT_NEW_ALIGNMENT__)==0))
class basic_io_buffer
{
public:
	using handle_type = handletype;
	using char_type = typename handle_type::char_type;
	using pointer = char_type*;
	using const_pointer = char_type const*;
	inline static constexpr buffer_flags mode = mde;
	inline static constexpr size_type buffer_size = bfs;
	inline static constexpr size_type buffer_alignment = alignmsz;
#if __has_cpp_attribute(no_unique_address) >= 201803L
	[[no_unique_address]]
#endif
	std::conditional_t<(mode&buffer_flags::in)==buffer_flags::in,basic_buffer_pointers<pointer>,empty_buffer_pointers> ibuffer;

#if __has_cpp_attribute(no_unique_address) >= 201803L
	[[no_unique_address]]
#endif
	std::conditional_t<(mode&buffer_flags::out)==buffer_flags::out,basic_buffer_pointers<pointer>,empty_buffer_pointers> obuffer;

#if __has_cpp_attribute(no_unique_address) >= 201803L
	[[no_unique_address]]
#endif
	handle_type handle;

private:
	constexpr void close_throw_impl()
	{
		if(obuffer.beg!=obuffer.curr)
			write(handle,obuffer.beg,obuffer.curr);
	}
	constexpr void close_impl() noexcept
	{
		if constexpr((mode&buffer_flags::out)==buffer_flags::out)
		{
			if constexpr(noexcept(
				write(handle,obuffer.beg,obuffer.curr)
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
		if constexpr((mode&buffer_flags::out)==buffer_flags::out)
			if(obuffer.buffer_begin)
			{
				if constexpr((mode&buffer_flags::secure_clear)==buffer_flags::secure_clear)
					secure_clear(obuffer.buffer_begin,sizeof(char_type)*buffer_size);
				details::deallocate_iobuf_space<buffer_size,buffer_alignment>(obuffer.buffer_begin);
			}
		if constexpr((mode&buffer_flags::in)==buffer_flags::in)
			if(ibuffer.buffer_begin)
			{
				if constexpr((mode&buffer_flags::secure_clear)==buffer_flags::secure_clear)
					secure_clear(ibuffer.buffer_begin,sizeof(char_type)*buffer_size);
				details::deallocate_iobuf_space<buffer_size,buffer_alignment>(ibuffer.buffer_begin);
			}
	}
public:
	basic_io_buffer(basic_io_buffer const&)=delete;
	basic_io_buffer& operator=(basic_io_buffer const&)=delete;
#if __cpp_constexpr_dynamic_alloc >= 201907L
	constexpr
#endif
	~basic_io_buffer()
	{
		if constexpr((mode&buffer_flags::out)==buffer_flags::out)
			close_impl();
		cleanup_impl();
	}
};


namespace details
{

template<stream handletype,std::forward_iterator Iter>
concept allow_iobuf_punning = std::contiguous_iterator<Iter>&&
//temporary only allow contiguous_iterator before we finish this part
(std::same_as<typename handletype::char_type,std::iter_value_t<Iter>>||
(std::same_as<typename handletype::char_type,char>&&std::contiguous_iterator<Iter>));

template<typename T,std::forward_iterator Iter>
inline constexpr void iobuf_write_unhappy_decay_impl(T t,basic_buffer_pointers<typename T::char_type>& pointers,Iter first,Iter last)
{
	if constexpr(std::contiguous_iterator<Iter>)
	{
		if constexpr(scatter_output_stream<T>)
		{
			std::array<io_scatter_t,2> scatters{{pointers.buffer_begin,
				sizeof(*buffer.buffer_curr)*(buffer.buffer_curr-pointers.buffer_begin)},
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
inline constexpr void iobuf_write_unhappy_impl(T& t,Iter first,Iter last,std::ptrdiff_t dis)
{
	if(t.pointers.buffer_begin==nullptr&&dis<T::buffer_size)
	{
		t.pointers.buffer_end=(t.pointers.buffer_curr=t.pointers.buffer_begin=
			allocate_iobuf_space<typename T::char_type,T::buffer_size,T::buffer_alignment>())+T::buffer_size;
		t.pointers.buffer_curr=non_overlapped_copy(first,last,t.pointers.buffer_curr);
		return;
	}
	iobuf_write_unhappy_decay_impl(io_ref(t),t.pointers,first,last);
}


template<typename T,std::random_access_iterator Iter>
inline constexpr void iobuf_write_unhappy_impl(T& t,Iter first,Iter last)
{
	iobuf_write_unhappy_impl(t,first,last,last-first);
}

}

template<stream handletype,buffer_flags mde,std::size_t bfs,std::size_t alignsz,std::forward_iterator Iter>
requires (output_stream<handletype>&&details::allow_iobuf_punning<handletype,Iter>)
inline constexpr decltype(auto) write(basic_io_buffer<handletype,mde,bfs,alignsz>& bios,Iter first,Iter last)
{
	if constexpr((mde&buffer_flags::out)==buffer_flags::out)
	{
		using iter_char_type = std::iter_value_t<Iter>;
		if constexpr(std::same_as<iter_char_type,typename handle_type::char_type>)
		{
			auto diff(std::distance(first,last));
			auto remain_space(bios.obuffer.buffer_end-bios.obuffer.buffer_curr);
			if(remain_space<diff)
			{
				if constexpr(std::contiguous_iterator<Iter>)
					details::iobuf_write_unhappy_impl(t,std::to_address(first),std::to_address(last));
				else if constexpr(std::random_access_iterator<Iter>)
					details::iobuf_write_unhappy_impl(t,first,last);
				else
					details::iobuf_write_unhappy_impl(t,first,last,diff);
				return;
			}
			bios.obuffer.buffer_curr=details::non_overlapped_copy(first,last,bios.obuffer.buffer_curr);
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


template<stream handletype,buffer_flags mde,std::size_t bfs,std::size_t alignsz,std::forward_iterator Iter>
requires output_stream<handletype>
inline constexpr auto obuffer_begin(basic_io_buffer<handletype,mde,bfs,alignsz>& bios) noexcept
{
	if constexpr((mde&buffer_flags::out)==buffer_flags::out)
		return bios.obuffer.buffer_begin;
	else
		return obuffer_begin(bios.handle);
}


template<stream handletype,buffer_flags mde,std::size_t bfs,std::size_t alignsz,std::forward_iterator Iter>
requires output_stream<handletype>
inline constexpr auto obuffer_curr(basic_io_buffer<handletype,mde,bfs,alignsz>& bios) noexcept
{
	if constexpr((mde&buffer_flags::out)==buffer_flags::out)
		return bios.obuffer.buffer_curr;
	else
		return obuffer_curr(bios.handle);
}

template<stream handletype,buffer_flags mde,std::size_t bfs,std::size_t alignsz,std::forward_iterator Iter>
requires output_stream<handletype>
inline constexpr auto obuffer_end(basic_io_buffer<handletype,mde,bfs,alignsz>& bios) noexcept
{
	if constexpr((mde&buffer_flags::out)==buffer_flags::out)
		return bios.obuffer.buffer_end;
	else
		return obuffer_end(bios.handle);
}

namespace details
{
template<std::size_t bfsz,std::size_t almsz,typename T,std::integral char_type>
inline constexpr void iobuf_overflow_impl(T bios,basic_buffer_pointers<char_type>& pointers,typename T::char_type ch)
{
	if(pointers.buffer_begin==pointers.buffer_curr)[[likely]]
	{
		if(pointers.buffer_begin==nullptr)
		{
			t.pointers.buffer_end=(t.pointers.buffer_curr=t.pointers.buffer_begin=
			allocate_iobuf_space<char_type,bfsz,almsz>())+T::buffer_size;
		}
	}
	else
	{
		write(bios.handle,pointers.buffer_begin,pointers.buffer_curr);
		pointers.buffer_curr=pointers.buffer_begin;
	}
	*pointers.buffer_curr=ch;
	++pointers.buffer_curr;
}
}

template<stream handletype,buffer_flags mde,std::size_t bfs,std::size_t alignsz>
requires output_stream<handletype>
inline constexpr auto overflow(basic_io_buffer<handletype,mde,bfs,alignsz>& bios,
	std::conditional_t<(mde&buffer_flags::out)==buffer_flags::out,
	typename basic_io_buffer<handletype,mde,bfs,alignsz>::char_type,
	> ch)
{
	if constexpr((mde&buffer_flags::out)==buffer_flags::out)
		details::iobuf_overflow_impl<T::buffer_size,T::buffer_alignment>(io_ref(bios.handle),bios.obuffer,ch);
	else
		return overflow(bios.handle,ch);
}
}