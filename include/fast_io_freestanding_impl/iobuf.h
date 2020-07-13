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

template<std::integral CharT,bool need_secure_clear=false,std::size_t buffer_size = details::cal_buffer_size<CharT,true>(),typename Allocator = io_aligned_allocator<CharT>>
requires (buffer_size!=0)
class basic_buf_handler
{
	Allocator alloc;
public:
	using char_type = CharT;
	using allocator_type = Allocator;
	char_type *beg{},*curr{},*end{};
	constexpr basic_buf_handler()=default;
	constexpr basic_buf_handler& operator=(basic_buf_handler const&)=delete;
	constexpr basic_buf_handler(basic_buf_handler const&)=delete;
	static constexpr std::size_t size = buffer_size;
	constexpr basic_buf_handler(basic_buf_handler&& m) noexcept:beg(m.beg),curr(m.curr),end(m.end)
	{
		m.end=m.curr=m.beg=nullptr;
	}
	constexpr basic_buf_handler& operator=(basic_buf_handler&& m) noexcept
	{
		if(std::addressof(m)!=this)[[likely]]
		{
			if(m.beg)
			{
				if constexpr(need_secure_clear)
					secure_clear(beg,buffer_size*sizeof(char_type));
				std::allocator_traits<allocator_type>::deallocate(alloc,beg,buffer_size);
			}
			beg=m.beg;
			curr=m.curr;
			end=m.end;
			m.end=m.curr=m.beg=nullptr;
		}
		return *this;
	}
	constexpr inline void init_space()
	{
		end=curr=beg=std::allocator_traits<allocator_type>::allocate(alloc,buffer_size);
	}
	constexpr inline void release()
	{
		if(beg)[[likely]]
		{
			if constexpr(need_secure_clear)
				secure_clear(beg,buffer_size*sizeof(char_type));
			std::allocator_traits<allocator_type>::deallocate(alloc,beg,buffer_size);
		}
		end=curr=beg=nullptr;
	}
#if __cpp_lib_is_constant_evaluated >= 201811L && __cpp_constexpr_dynamic_alloc >= 201907L
	constexpr
#endif
	~basic_buf_handler()
	{
		if(beg)[[likely]]
		{
			if constexpr(need_secure_clear)
				secure_clear(beg,buffer_size*sizeof(char_type));
			std::allocator_traits<allocator_type>::deallocate(alloc,beg,buffer_size);
		}
	}
	Allocator get_allocator() const{ return alloc;}
};


template<input_stream Ihandler,typename Buf=basic_buf_handler<typename Ihandler::char_type,secure_clear_requirement_stream<Ihandler>>>
class basic_ibuf:public ocrtp<basic_ibuf<Ihandler,Buf>>
{
public:
	Ihandler ih;
	Buf ibuffer;
	constexpr auto& ocrtp_handle() requires output_stream<Ihandler>
	{
		return ih;
	}
	using native_handle_type = Ihandler;
	using buffer_type = Buf;
	using char_type = typename Buf::char_type;
	template<typename... Args>
	requires std::constructible_from<Ihandler,Args...>
	constexpr basic_ibuf(Args&&... args):ih(std::forward<Args>(args)...){}
	inline constexpr auto& native_handle()
	{
		return ih;
	}
};
template<input_stream Ihandler,typename Buf>
inline constexpr bool underflow(basic_ibuf<Ihandler,Buf>& ib)
{
	if(ib.ibuffer.end==nullptr)
		ib.ibuffer.init_space();
	ib.ibuffer.end=read(ib.ih,ib.ibuffer.beg,ib.ibuffer.beg+Buf::size);
	ib.ibuffer.curr=ib.ibuffer.beg;
	return ib.ibuffer.end!=ib.ibuffer.beg;
}

template<input_stream Ihandler,typename Buf>
inline constexpr bool irefill(basic_ibuf<Ihandler,Buf>& ib)
{
	if(ib.ibuffer.end==nullptr)
		ib.ibuffer.init_space();
	ib.ibuffer.end=std::copy(ib.ibuffer.curr,ib.ibuffer.end,ib.ibuffer.beg);
	ib.ibuffer.curr=ib.ibuffer.beg;
	auto ed{ib.ibuffer.end};
/*
	if(ed==ib.ibuffer.beg+Buf::size)
#ifdef __cpp_exceptions
		throw posix_error(EIO);
#else
		fast_terminate();
#endif
*/
	ib.ibuffer.end=read(ib.ih,ed,ib.ibuffer.beg+Buf::size);
	return ib.ibuffer.end!=ed;
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

template<redirect_stream Ihandler,typename Buf>
inline constexpr decltype(auto) redirect_handle(basic_ibuf<Ihandler,Buf>& ib)
{
	return redirect_handle(ib.native_handle());
}

template<stream Ihandler,typename Buf>
requires secure_clear_requirement_stream<Ihandler>
inline constexpr void require_secure_clear(basic_ibuf<Ihandler,Buf>&){}

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

template<input_stream Ihandler,std::integral ch_type,typename Buf>
requires random_access_stream<Ihandler>
inline constexpr auto seek(basic_ibuf<Ihandler,Buf>& ib,seek_type_t<ch_type>,std::intmax_t u=0,seekdir s=seekdir::cur)
{
	std::intmax_t val(u-(ib.end-ib.curr));
	ib.ibuffer.curr=ib.ibuffer.end;
	return seek(ib.native_handle(),seek_type<ch_type>,val,s);
}

template<input_stream Ihandler,typename Buf>
requires random_access_stream<Ihandler>
inline constexpr auto seek(basic_ibuf<Ihandler,Buf>& ib,std::intmax_t u=0,seekdir s=seekdir::cur)
{
	return seek(ib,seek_type<typename basic_ibuf<Ihandler,Buf>::char_type>,u,s);
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

template<output_stream Ohandler,bool forcecopy=false,typename Buf=basic_buf_handler<typename Ohandler::char_type,secure_clear_requirement_stream<Ohandler>>>
class basic_obuf:public icrtp<basic_obuf<Ohandler,forcecopy,Buf>>
{
public:
	Ohandler oh;
	Buf obuffer;
	constexpr auto& icrtp_handle() requires input_stream<Ohandler>
	{
		return oh;
	}
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
	constexpr basic_obuf(Args&&... args):oh(std::forward<Args>(args)...){}
	~basic_obuf()
	{
		close_impl();
	}
	basic_obuf& operator=(basic_obuf const&)=delete;
	basic_obuf(basic_obuf const&)=delete;
	constexpr basic_obuf(basic_obuf&& bmv) noexcept:oh(std::move(bmv.oh)),obuffer(std::move(bmv.obuffer)){}
	constexpr basic_obuf& operator=(basic_obuf&& b) noexcept
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
[[nodiscard]] inline constexpr auto obuffer_curr(basic_obuf<Ohandler,forcecopy,Buf>& ob)
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

template<stream Ohandler,bool forcecopy,typename Buf>
requires secure_clear_requirement_stream<Ohandler>
inline constexpr void require_secure_clear(basic_obuf<Ohandler,forcecopy,Buf>&){}

namespace details
{
template<bool init,bool punning=false,output_stream Ohandler,bool forcecopy,typename Buf,std::contiguous_iterator Iter>
constexpr void obuf_write_force_copy(basic_obuf<Ohandler,forcecopy,Buf>& ob,Iter cbegin,Iter cend)
{
	if constexpr(forcecopy&&!std::same_as<decltype(write(ob.oh,cbegin,cend)),void>)
	{
		auto it{write(ob.oh,cbegin,cend)};
		if(it!=cend)
		{
			if(Buf::size<=static_cast<std::size_t>(cend-it))
#ifdef __cpp_exceptions
				throw posix_error(EIO);
#else
				fast_terminate();
#endif
			if constexpr(init)
			{
				ob.obuffer.init_space();
				ob.obuffer.end=(ob.obuffer.curr=ob.obuffer.beg)+Buf::size;
			}
			else
				ob.obuffer.curr=ob.obuffer.beg;
			memcpy(ob.obuffer.beg,std::to_address(it),(cend-it)*sizeof(*cbegin));
			ob.obuffer.curr=ob.obuffer.beg+(cend-it);
		}
	}
	else
	{
		write(ob.native_handle(),cbegin,cend);
		ob.obuffer.curr=ob.obuffer.beg;
	}
}

}

template<output_stream Ohandler,bool forcecopy,typename Buf>
inline constexpr void overflow(basic_obuf<Ohandler,forcecopy,Buf>& ob,typename Ohandler::char_type ch)
{
	if(ob.obuffer.beg)
	{
		details::obuf_write_force_copy<false>(ob,ob.obuffer.beg,ob.obuffer.end);
	}
	else	//cold buffer
	{
		ob.obuffer.init_space();
		ob.obuffer.end=(ob.obuffer.curr=ob.obuffer.beg)+Buf::size;
	}
	*ob.obuffer.curr=ch;
	++ob.obuffer.curr;
}

namespace details
{

template<output_stream Ohandler,bool forcecopy,typename Buf>
inline constexpr void iobuf_fill_nc_define_code_path(basic_obuf<Ohandler,forcecopy,Buf>& ob,std::size_t n,typename Ohandler::char_type ch)
{
	if(ob.obuffer.end==nullptr)[[unlikely]]
	{
		if(n==0)[[unlikely]]
			return;
		ob.obuffer.init_space();
		ob.obuffer.end=(ob.obuffer.curr=ob.obuffer.beg)+Buf::size;
	}
	auto last{ob.obuffer.end};
	if(ob.obuffer.beg+n<last)
		last=ob.obuffer.beg+n;
	while(n)
	{
		details::my_fill(ob.obuffer.curr,last,ch);
		std::size_t mn(ob.obuffer.end-ob.obuffer.beg);
		if(n<mn)
			mn=n;
		obuf_write_force_copy<false>(ob,ob.obuffer.beg,ob.obuffer.curr+mn);
		last=ob.obuffer.curr;
		n-=mn;
	}
}

}

template<output_stream Ohandler,bool forcecopy,typename Buf>
inline constexpr void fill_nc_define(basic_obuf<Ohandler,forcecopy,Buf>& ob,std::size_t n,typename Ohandler::char_type ch)
{
	auto newcurr{n+ob.obuffer.curr};
	if(ob.obuffer.end<newcurr)
	{
		details::iobuf_fill_nc_define_code_path(ob,n,ch);
		return;
	}
	details::my_fill_n(ob.obuffer.curr,n,ch);
	ob.obuffer.curr=newcurr;
}

namespace details
{

template<bool punning=false,output_stream Ohandler,bool forcecopy,typename Buf,std::contiguous_iterator Iter>
constexpr void obuf_write_cold(basic_obuf<Ohandler,forcecopy,Buf>& ob,Iter cbegin,Iter cend,std::size_t diff)
{
	using T = basic_obuf<Ohandler,forcecopy,Buf>;
	if(ob.obuffer.end==nullptr)		//cold buffer
	{
		if(cend-cbegin==0)
			return;
		if(T::buffer_type::size<=diff)
		{
			obuf_write_force_copy<true,punning>(ob,cbegin,cend);
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

	if constexpr(forcecopy&&!std::same_as<decltype(write(ob.oh,cbegin,cend)),void>)
	{
		while(ob.obuffer.end-ob.obuffer.curr<cend-cbegin)
		{
			std::size_t const need_to_copy(ob.obuffer.end-ob.obuffer.curr);
			memcpy(ob.obuffer.curr,std::to_address(cbegin),need_to_copy*sizeof(*cbegin));
			obuf_write_force_copy<false,punning>(ob,ob.obuffer.curr,ob.obuffer.end);			
			cbegin+=need_to_copy;
		}
		std::size_t const need_to_copy(cend-cbegin);
		memcpy(ob.obuffer.curr,std::to_address(cbegin),need_to_copy*sizeof(*cbegin));
		ob.obuffer.curr+=need_to_copy;
	}
	else
	{
		std::size_t n(ob.obuffer.end-ob.obuffer.curr);
		if constexpr(punning)
			memcpy(ob.obuffer.curr,cbegin,n*sizeof(std::iter_value_t<Iter>));
		else
			std::copy_n(cbegin,n,ob.obuffer.curr);	
		cbegin+=n;
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
requires std::same_as<std::iter_value_t<Iter>,typename Ohandler::char_type>
inline constexpr void obuf_write(basic_obuf<Ohandler,forcecopy,Buf>& ob,Iter cbegin,Iter cend)
{
	std::size_t const diff(cend-cbegin);
	auto curr{ob.obuffer.curr};
	auto e{curr+diff};
	if(e<ob.obuffer.end)[[likely]]
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

template<io_stream ioh,bool forcecopy=false,typename bf=basic_buf_handler<typename ioh::char_type>>
using basic_iobuf=basic_obuf<basic_ibuf<ioh,bf>,forcecopy,bf>;

}
