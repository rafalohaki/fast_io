#pragma once

namespace fast_io
{

template<typename T,std::size_t alignment=4096>
struct io_aligned_allocator
{
	using value_type = T;
	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;
	[[nodiscard]] inline
	#if __cpp_lib_is_constant_evaluated >= 201811L && __cpp_constexpr_dynamic_alloc >= 201907L
		constexpr
	#endif
	T* allocate(std::size_t n)
	{
	#if __cpp_lib_is_constant_evaluated >= 201811L && __cpp_constexpr_dynamic_alloc >= 201907L
		if(std::is_constant_evaluated())
			return new T[n];
		else
	#endif
	#if __cpp_sized_deallocation >=	201309L && __cpp_aligned_new >= 201606L
			return static_cast<T*>(operator new(n*sizeof(T),std::align_val_t{alignment}));
	#else
			return new T[n];
	#endif
	}
	inline
	#if __cpp_lib_is_constant_evaluated >= 201811L && __cpp_constexpr_dynamic_alloc >= 201907L
		constexpr
	#endif
	void deallocate(T* p, [[maybe_unused]] std::size_t n) noexcept
	{
	#if __cpp_lib_is_constant_evaluated >= 201811L && __cpp_constexpr_dynamic_alloc >= 201907L
		if(std::is_constant_evaluated())
			delete[] p;
		else
	#endif
	#if __cpp_sized_deallocation >=	201309L && __cpp_aligned_new >= 201606L
			operator delete(p,n*sizeof(T),std::align_val_t{alignment});
	#else
			delete[] p;
	#endif
	}
};

template<std::integral CharT,std::size_t buffer_size = ((
#if defined(__WINNT__) || defined(_MSC_VER)
1048576
#else
65536
#endif
<sizeof(CharT))?1:
#if defined(__WINNT__) || defined(_MSC_VER)
1048576
#else
65536
#endif
/sizeof(CharT)),typename Allocator = io_aligned_allocator<CharT>>
requires (buffer_size!=0)
class basic_buf_handler
{
	Allocator alloc;
public:
	using char_type = CharT;
	using allocator_type = Allocator;
	char_type *beg{},*curr{},*end{};
	basic_buf_handler()=default;
	basic_buf_handler& operator=(basic_buf_handler const&)=delete;
	basic_buf_handler(basic_buf_handler const&)=delete;
	static constexpr std::size_t size = buffer_size;
	basic_buf_handler(basic_buf_handler&& m) noexcept:beg(m.beg),curr(m.curr),end(m.end)
	{
		m.end=m.curr=m.beg=nullptr;
	}
	basic_buf_handler& operator=(basic_buf_handler&& m) noexcept
	{
		if(std::addressof(m)!=this)[[likely]]
		{
			if(m.beg)
				alloc.deallocate(beg,buffer_size);
			beg=m.beg;
			curr=m.curr;
			end=m.end;
			m.end=m.curr=m.beg=nullptr;
		}
		return *this;
	}
	inline void init_space()
	{
		end=curr=beg=alloc.allocate(buffer_size);
	}
	inline void release()
	{
		if(beg)[[likely]]
			alloc.deallocate(beg,buffer_size);
		end=curr=beg=nullptr;
	}
	~basic_buf_handler()
	{
		if(beg)[[likely]]
			alloc.deallocate(beg,buffer_size);
	}
	Allocator get_allocator() const{ return alloc;}
};


template<input_stream Ihandler,typename Buf=basic_buf_handler<typename Ihandler::char_type>>
class basic_ibuf
{
public:
	Ihandler ih;
	Buf ibuffer;

	using native_handle_type = Ihandler;
	using buffer_type = Buf;
	using char_type = typename Buf::char_type;
	template<typename... Args>
	requires std::constructible_from<Ihandler,Args...>
	basic_ibuf(Args&&... args):ih(std::forward<Args>(args)...){}
	inline constexpr auto& native_handle()
	{
		return ih;
	}
};
template<input_stream Ihandler,typename Buf>
[[nodiscard]] inline constexpr bool underflow(basic_ibuf<Ihandler,Buf>& ib)
{
	if(ib.ibuffer.end==nullptr)
		ib.ibuffer.init_space();
	ib.ibuffer.end=read(ib.ih,ib.ibuffer.beg,ib.ibuffer.beg+Buf::size);
	ib.ibuffer.curr=ib.ibuffer.beg;
	return ib.ibuffer.end!=ib.ibuffer.beg;
}

template<input_stream Ihandler,typename Buf>
[[nodiscard]] inline constexpr auto ibuffer_begin(basic_ibuf<Ihandler,Buf>& ib)
{
	return ib.ibuffer.beg;
}
template<input_stream Ihandler,typename Buf>
[[nodiscard]] inline constexpr auto ibuffer_curr(basic_ibuf<Ihandler,Buf>& ib)
{
	return ib.ibuffer.curr;
}
template<input_stream Ihandler,typename Buf>
[[nodiscard]] inline constexpr auto ibuffer_end(basic_ibuf<Ihandler,Buf>& ib)
{
	return ib.ibuffer.end;
}

template<input_stream Ihandler,typename Buf>
inline constexpr void ibuffer_set_curr(basic_ibuf<Ihandler,Buf>& ib,typename Ihandler::char_type* ptr)
{
	ib.ibuffer.curr=ptr;
}


template<buffer_output_stream Ihandler,typename Buf>
inline constexpr decltype(auto) overflow(basic_ibuf<Ihandler,Buf>& ib,typename Ihandler::char_type ch)
{
	return overflow(ib.oh,ch);
}

template<buffer_output_stream Ihandler,typename Buf>
[[nodiscard]] inline constexpr decltype(auto) obuffer_begin(basic_ibuf<Ihandler,Buf>& ib)
{
	return obuffer_begin(ib.oh);
}

template<buffer_output_stream Ihandler,typename Buf>
[[nodiscard]] inline constexpr decltype(auto) obuffer_curr(basic_ibuf<Ihandler,Buf>& ib)
{
	return obuffer_curr(ib.oh);
}
template<buffer_output_stream Ihandler,typename Buf>
[[nodiscard]] inline constexpr decltype(auto) obuffer_end(basic_ibuf<Ihandler,Buf>& ib)
{
	return obuffer_end(ib.oh);
}

template<buffer_output_stream Ihandler,typename Buf>
[[nodiscard]] inline constexpr decltype(auto) obuffer_set_curr(basic_ibuf<Ihandler,Buf>& ib,typename Ihandler::char_type* ptr)
{
	return obuffer_set_curr(ib.oh,ptr);
}

template<output_stream Ihandler,typename Buf>
inline constexpr void flush(basic_ibuf<Ihandler,Buf>& ib)
{
	flush(ib.native_handle());
}

template<output_stream Ihandler,typename Buf,std::contiguous_iterator Iter>
inline constexpr decltype(auto) write(basic_ibuf<Ihandler,Buf>& ib,Iter b,Iter e)
{
	return write(ib.native_handle(),b,e);
}

template<redirect_stream Ihandler,typename Buf>
inline constexpr decltype(auto) redirect_handle(basic_ibuf<Ihandler,Buf>& ib)
{
	return redirect_handle(ib.native_handle());
}

template<typename T,typename Iter>
concept write_read_punned_constraints = (std::contiguous_iterator<Iter>&&sizeof(typename T::char_type)==1) ||
	(std::random_access_iterator<Iter>&&std::same_as<typename T::char_type,typename std::iterator_traits<Iter>::value_type>);

namespace details
{
template<std::size_t buffer_size,bool punning=false,typename T,typename Iter>
requires std::same_as<std::iter_value_t<Iter>,typename std::remove_cvref_t<T>::char_type>
inline constexpr Iter ibuf_read_cold(T& ib,Iter begin,Iter end)
{
	std::size_t n(end-begin);
	std::size_t const buffer_remain(ib.ibuffer.end-ib.ibuffer.curr);
	if(ib.ibuffer.end==nullptr)
	{
		if(buffer_size<=n)
		{
			return read(ib.native_handle(),begin,end);
		}
		ib.ibuffer.init_space();
		ib.ibuffer.curr=ib.ibuffer.end=ib.ibuffer.beg;
	}
	if constexpr(punning)
	{
		std::memcpy(begin,ib.ibuffer.curr,buffer_remain*sizeof(std::iter_value_t<Iter>));
		begin+=buffer_remain;
	}
	else
		begin=std::copy_n(ib.ibuffer.curr,buffer_remain,begin);
	if(begin+buffer_size<end)
	{
//			if constexpr(std::contiguous_iterator<Iter>)
			begin=read(ib.native_handle(),begin,end);
/*			else
		{
			
		}*/
		if(begin!=end)
		{
			ib.ibuffer.end=ib.ibuffer.curr=ib.ibuffer.beg;
			return begin;
		}
	}
	ib.ibuffer.end=read(ib.native_handle(),ib.ibuffer.beg,ib.ibuffer.beg+buffer_size);
	ib.ibuffer.curr=ib.ibuffer.beg;
	n=end-begin;
	std::size_t const sz(ib.ibuffer.end-ib.ibuffer.beg);
	if(sz<n)
		n=sz;
	if constexpr(punning)
	{
		std::memcpy(begin,ib.ibuffer.curr,n*sizeof(std::iter_value_t<Iter>));
		begin+=n;
	}
	else
		begin=std::copy_n(ib.ibuffer.curr,n,begin);
	ib.ibuffer.curr+=n;
	return begin;
}

template<std::size_t buffer_size,bool punning=false,typename T,typename Iter>
requires std::same_as<std::iter_value_t<Iter>,typename std::remove_cvref_t<T>::char_type>
inline constexpr Iter ibuf_read(T& ib,Iter begin,Iter end)
{
	std::size_t n(end-begin);
	if(ib.ibuffer.curr+n<ib.ibuffer.end)[[unlikely]]			//cache miss
		return ibuf_read_cold<buffer_size,punning>(ib,begin,end);
	if constexpr(punning)
	{
		std::memcpy(begin,ib.ibuffer.curr,n*sizeof(std::iter_value_t<Iter>));
		begin+=n;
	}
	else
		begin=std::copy_n(ib.ibuffer.curr,n,begin);
	ib.ibuffer.curr+=n;
	return begin;

}
}

template<input_stream Ihandler,typename Buf,std::contiguous_iterator Iter>
requires (write_read_punned_constraints<basic_ibuf<Ihandler,Buf>,Iter>)
inline constexpr Iter read(basic_ibuf<Ihandler,Buf>& ib,Iter begin,Iter end)
{
	using char_type = typename basic_ibuf<Ihandler,Buf>::char_type;
	if constexpr(std::same_as<char_type,typename std::iterator_traits<Iter>::value_type>)
	{
		if(std::is_constant_evaluated())
			return details::ibuf_read<Buf::size>(ib,std::to_address(begin),std::to_address(end));
		else
			return details::ibuf_read<Buf::size,true>(ib,std::to_address(begin),std::to_address(end));
	}
	else
	{
		auto b(reinterpret_cast<char const*>(std::to_address(begin)));
		return begin+(details::ibuf_read<Buf::size,true>(ib,b,reinterpret_cast<char const*>(std::to_address(end)))-b)/sizeof(*begin);
	}
}

template<input_stream Ihandler,typename Buf>
requires random_access_stream<Ihandler>
inline constexpr auto seek(basic_ibuf<Ihandler,Buf>& ib,std::common_type_t<std::ptrdiff_t,std::int64_t> u=0,seekdir s=seekdir::cur)
{
	std::common_type_t<std::ptrdiff_t,std::int64_t> val(u-(ib.end-ib.curr)*sizeof(*ib.curr));
	ib.ibuffer.curr=ib.ibuffer.end;
	return seek(ib.native_handle(),val,s);
}

template<zero_copy_input_stream Ihandler,typename Buf>
inline constexpr decltype(auto) zero_copy_in_handle(basic_ibuf<Ihandler,Buf>& ib)
{
	return zero_copy_in_handle(ib.native_handle());
}

template<zero_copy_output_stream Ohandler,typename Buf>
inline constexpr decltype(auto) zero_copy_out_handle(basic_ibuf<Ohandler,Buf>& ib)
{
	return zero_copy_out_handle(ib.native_handle());
}

template<memory_map_input_stream Ihandler,typename Buf>
inline constexpr decltype(auto) memory_map_in_handle(basic_ibuf<Ihandler,Buf>& handle)
{
	return memory_map_in_handle(handle.native_handle());
}

template<output_stream Ohandler,bool forcecopy=false,typename Buf=basic_buf_handler<typename Ohandler::char_type>>
class basic_obuf
{
public:
	Ohandler oh;
	Buf obuffer;
	inline constexpr void close_impl() noexcept
	{
#ifdef __cpp_exceptions
		try
		{
#endif
			if(obuffer.beg)
				write(oh,obuffer.beg,obuffer.curr);
#ifdef __cpp_exceptions
		}
		catch(...){}
#endif
	}
public:
	using native_handle_type = Ohandler;
	using buffer_type = Buf;
	using char_type = typename Buf::char_type;
	template<typename... Args>
	requires std::constructible_from<Ohandler,Args...>
	basic_obuf(Args&&... args):oh(std::forward<Args>(args)...){}
	~basic_obuf()
	{
		close_impl();
	}
	basic_obuf& operator=(basic_obuf const&)=delete;
	basic_obuf(basic_obuf const&)=delete;
	basic_obuf(basic_obuf&& bmv) noexcept:oh(std::move(bmv.oh)),obuffer(std::move(bmv.obuffer)){}
	basic_obuf& operator=(basic_obuf&& b) noexcept
	{
		if(std::addressof(b)!=this)
		{
			close_impl();
			oh=std::move(b.oh);
			obuffer=std::move(b.obuffer);
		}
		return *this;
	}
	inline constexpr auto& native_handle()
	{
		return oh;
	}
};

template<output_stream Ohandler,bool forcecopy,typename Buf>
[[nodiscard]] inline constexpr auto obuffer_begin(basic_obuf<Ohandler,forcecopy,Buf>& ob)
{
	return ob.obuffer.beg;
}

template<output_stream Ohandler,bool forcecopy,typename Buf>
[[nodiscard]] inline constexpr auto& obuffer_curr(basic_obuf<Ohandler,forcecopy,Buf>& ob)
{
	return ob.obuffer.curr;
}

template<output_stream Ohandler,bool forcecopy,typename Buf>
[[nodiscard]] inline constexpr auto obuffer_end(basic_obuf<Ohandler,forcecopy,Buf>& ob)
{
	return ob.obuffer.end;
}

template<output_stream Ohandler,bool forcecopy,typename Buf>
inline constexpr void obuffer_set_curr(basic_obuf<Ohandler,forcecopy,Buf>& ob,typename Ohandler::char_type* ptr)
{
	ob.obuffer.curr=ptr;
}

template<output_stream Ohandler,bool forcecopy,typename Buf>
inline constexpr void overflow(basic_obuf<Ohandler,forcecopy,Buf>& ob,typename Ohandler::char_type ch)
{
	if(ob.obuffer.beg)
		write(ob.native_handle(),ob.obuffer.beg,ob.obuffer.end);
	else	//cold buffer
	{
		ob.obuffer.init_space();
		ob.obuffer.end=ob.obuffer.beg+Buf::size;
	}
	*(ob.obuffer.curr=ob.obuffer.beg)=ch;
	++ob.obuffer.curr;
}

template<buffer_input_stream Ohandler,bool forcecopy,typename Buf>
[[nodiscard]] inline constexpr decltype(auto) underflow(basic_obuf<Ohandler,forcecopy,Buf>& ob)
{
	return underflow(ob.oh);
}

template<buffer_input_stream Ohandler,bool forcecopy,typename Buf>
[[nodiscard]] inline constexpr decltype(auto) ibuffer_begin(basic_obuf<Ohandler,forcecopy,Buf>& ob)
{
	return ibuffer_begin(ob.oh);
}

template<buffer_input_stream Ohandler,bool forcecopy,typename Buf>
[[nodiscard]] inline constexpr decltype(auto) ibuffer_curr(basic_obuf<Ohandler,forcecopy,Buf>& ob)
{
	return ibuffer_curr(ob.oh);
}

template<buffer_input_stream Ohandler,bool forcecopy,typename Buf>
[[nodiscard]] inline constexpr decltype(auto) ibuffer_end(basic_obuf<Ohandler,forcecopy,Buf>& ob)
{
	return ibuffer_end(ob.oh);
}

template<buffer_input_stream Ohandler,bool forcecopy,typename Buf>
inline constexpr decltype(auto) ibuffer_set_curr(basic_obuf<Ohandler,forcecopy,Buf>& ob,typename Ohandler::char_type* ptr)
{
	return ibuffer_set_curr(ob.oh,ptr);
}

namespace details
{

template<bool punning=false,output_stream Ohandler,bool forcecopy,typename Buf,std::contiguous_iterator Iter>
constexpr void obuf_write_cold(basic_obuf<Ohandler,forcecopy,Buf>& ob,Iter cbegin,Iter cend,std::size_t diff)
{
	if(ob.obuffer.end==nullptr)		//cold buffer
	{
		if(T::buffer_type::size<=diff)
		{
			if constexpr(forcecopy&&!std::same_as<decltype(write(ob,begin,end)),void>)
			{
				auto it{write(ob.oh,begin,end)};
				if(it!=end)
				{
					if(T::buffer_type::size<=end-it)
#ifdef __cpp_exceptions
						throw posix_error(EIO);
#else
						fast_terminate();
#endif
					ob.obuffer.init_space();
					ob.obuffer.end=(ob.obuffer.curr=ob.obuffer.beg)+T::buffer_type::size;
					memcpy(ob.obuffer.beg,std::to_address(it),(end-it)*sizeof(*cbegin));
					ob.obuffer.beg+=end-it;
				}
			}
			else
			{
				write(ob.native_handle(),cbegin,cend);
			}
			return;
		}
		ob.obuffer.init_space();
		ob.obuffer.end=(ob.obuffer.curr=ob.obuffer.beg)+T::buffer_type::size;
		if constexpr(punning)
			memcpy(ob.obuffer.curr,cbegin,diff*sizeof(std::iter_value_t<Iter>));
		else
			std::copy_n(cbegin,diff,ob.obuffer.curr);
		ob.obuffer.curr+=diff;
		return;
	}
	std::size_t n(ob.obuffer.end-ob.obuffer.curr);
	if constexpr(punning)
		memcpy(ob.obuffer.curr,cbegin,n*sizeof(std::iter_value_t<Iter>));
	else
		std::copy_n(cbegin,n,ob.obuffer.curr);		
	cbegin+=n;
	if constexpr(forcecopy&&!std::same_as<decltype(write(ob,begin,end)),void>)
	{
		auto it{write(ob.native_handle(),ob.obuffer.beg,ob.obuffer.end)};
		if(it!=end)
		{
			ob.obuffer.end=(ob.obuffer.curr=ob.obuffer.beg)+T::buffer_type::size;
			memcpy(ob.obuffer.beg,std::to_address(it),(end-it)*sizeof(*cbegin));
			ob.obuffer.beg+=end-it;
		}
	}
	else
	{
		write(ob.native_handle(),ob.obuffer.beg,ob.obuffer.end);
		if(cbegin+(T::buffer_type::size)<cend)
		{
			write(ob.oh,cbegin,cend);
			ob.obuffer.curr=ob.obuffer.beg;
		}
		else
		{
			std::size_t const df(cend-cbegin);
			if constexpr(punning)
				memcpy(ob.obuffer.beg,cbegin,df*sizeof(std::iter_value_t<Iter>));
			else
				std::copy_n(cbegin,df,ob.obuffer.beg);
			ob.obuffer.curr=ob.obuffer.beg+df;
		}
	}
}

template<bool punning=false,output_stream Ohandler,bool forcecopy,typename Buf,std::contiguous_iterator Iter>
requires std::same_as<std::iter_value_t<Iter>,typename std::remove_cvref_t<T>::char_type>
inline constexpr void obuf_write(basic_obuf<Ohandler,forcecopy,Buf>& ob,Iter cbegin,Iter cend)
{
	std::size_t const diff(cend-cbegin);
	auto curr{ob.obuffer.curr};
	auto e{curr+diff};
	if(e<=ob.obuffer.end)[[likely]]
	{
		if constexpr(punning)
			memcpy(curr,cbegin,diff*sizeof(std::iter_value_t<Iter>));
		else
			std::copy_n(cbegin,diff,ob.obuffer.curr);
		ob.obuffer.curr=e;
		return;
	}
	obuf_write_cold<punning>(ob,cbegin,cend,diff);
}

}

template<output_stream Ohandler,bool forcecopy,typename Buf,std::contiguous_iterator Iter>
requires (write_read_punned_constraints<basic_obuf<Ohandler,forcecopy,Buf>,Iter>)
inline constexpr void write(basic_obuf<Ohandler,forcecopy,Buf>& ob,Iter cbegini,Iter cendi)
{
	using char_type = typename basic_obuf<Ohandler,forcecopy,Buf>::char_type;
	if constexpr(std::same_as<char_type,typename std::iterator_traits<Iter>::value_type>)
	{
		if(std::is_constant_evaluated())
			details::obuf_write<false>(ob,std::to_address(cbegini),std::to_address(cendi));
		else
			details::obuf_write<true>(ob,std::to_address(cbegini),std::to_address(cendi));
	}
	else
		details::obuf_write<true>(ob,reinterpret_cast<char const*>(std::to_address(cbegini)),
					reinterpret_cast<char const*>(std::to_address(cendi)));
}

template<output_stream Ohandler,bool forcecopy,typename Buf>
inline constexpr void flush(basic_obuf<Ohandler,forcecopy,Buf>& ob)
{
	if(ob.obuffer.beg==ob.obuffer.curr)
		return;
	write(ob.native_handle(),ob.obuffer.beg,ob.obuffer.curr);
	ob.obuffer.curr=ob.obuffer.beg;
//	flush(oh.native_handle());
}

template<output_stream Ohandler,bool forcecopy,typename Buf,typename... Args>
requires random_access_stream<Ohandler>
inline constexpr decltype(auto) seek(basic_obuf<Ohandler,forcecopy,Buf>& ob,Args&& ...args)
{
	if(ob.obuffer.beg!=ob.obuffer.curr)
	{
		write(ob.native_handle(),ob.obuffer.beg,ob.obuffer.curr);
		ob.obuffer.curr=ob.obuffer.beg;
	}
	return seek(ob.native_handle(),std::forward<Args>(args)...);
}

template<input_stream Ohandler,typename Buf,std::contiguous_iterator Iter>
inline constexpr decltype(auto) read(basic_obuf<Ohandler,forcecopy,Buf>& ob,Iter begin,Iter end)
{
	return read(ob.native_handle(),begin,end);
}

template<zero_copy_output_stream Ohandler,bool forcecopy,typename Buf>
inline constexpr decltype(auto) zero_copy_out_handle(basic_obuf<Ohandler,forcecopy,Buf>& ob)
{
	return zero_copy_out_handle(ob.native_handle());
}

template<zero_copy_input_stream Ohandler,bool forcecopy,typename Buf>
inline constexpr decltype(auto) zero_copy_in_handle(basic_obuf<Ohandler,forcecopy,Buf>& ob)
{
	return zero_copy_in_handle(ob.native_handle());
}

template<redirect_stream Ohandler,bool forcecopy,typename Buf>
inline constexpr decltype(auto) redirect_handle(basic_obuf<Ohandler,forcecopy,Buf>& ob)
{
	return redirect_handle(ob.native_handle());
}

template<memory_map_input_stream Ihandler,typename Buf>
inline constexpr decltype(auto) memory_map_in_handle(basic_obuf<Ihandler,Buf>& handle)
{
	return memory_map_in_handle(handle.native_handle());
}

template<io_stream ioh,bool forcecopy=false,typename bf=basic_buf_handler<typename ioh::char_type>>
using basic_iobuf=basic_obuf<basic_ibuf<ioh,bf>,forcecopy,bf>;

}