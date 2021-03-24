#pragma once

namespace fast_io
{
namespace details
{
/*
the entire stdio on windows is too broken. I want to add tie semantics to stdin and stdout to avoid troubles.
*/

#if defined(_MSC_VER) || defined(_UCRT)

/*

referenced from win10sdk ucrt
C:\Program Files (x86)\Windows Kits\10\Source\10.0.19041.0\ucrt

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//
// Internal Stream Types (__crt_stdio_stream and friends)
//
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+


// Ensure that __crt_stdio_stream_data* and FILE* pointers are freely convertible:


struct ucrt_stdio_stream_data
{
    union
    {
        FILE  public_file;
        char* ptr;
    };

    char*            base;
    int              cnt;
    long             flags;
    long             file;
    int              charbuf;
    int              bufsiz;
    char*            tmpfname;
};

*/
struct
#if __has_cpp_attribute(gnu::may_alias)
[[gnu::may_alias]]
#endif
ucrt_iobuf
{
    char*	     _ptr;
    char*            _base;
// this makes me very curious. Why is the file struct in UCRT in microsoft SDK different from mingw-w64's definition?
// I guess it is probably MinGW-w64 CRT's bug?
    int              _cnt;
    long             _flag;
    long             _file;
    int              _charbuf;
    int              _bufsiz;
    char*            _tmpfname;
};
#endif

inline constexpr std::size_t wincrt_internal_buffer_size{4096};

template<typename fileptr>
inline void wincrt_fp_set_flag_dirty_impl(fileptr* __restrict fp)
{
#if defined(_MSC_VER) || defined(_UCRT)
	fp->_flag|=64;
#else
	fp->_flag|=8;
#endif
}

template<typename fileptr>
inline bool wincrt_fp_is_dirty_impl(fileptr* __restrict fp)
{
	constexpr unsigned int mask{
#if defined(_MSC_VER) || defined(_UCRT)
	64
#else
	8
#endif
	};
	return (fp->_flag&mask)==mask;
}

[[gnu::cold]]
inline void wincrt_fp_allocate_buffer_impl(std::FILE* __restrict fpp)
{
#if defined(_MSC_VER) || defined(_UCRT)
	ucrt_iobuf* fp{reinterpret_cast<ucrt_iobuf*>(fpp)};
#else
	std::FILE* fp{fpp};
#endif
	if(fp->_bufsiz<4)
	{
		fp->_bufsiz=wincrt_internal_buffer_size;
	}
	fp->_bufsiz>>=2;
	fp->_bufsiz<<=2;
	std::size_t allocated_buffer_size{static_cast<std::size_t>(static_cast<unsigned int>(fp->_bufsiz))};
	auto new_buffer{malloc(allocated_buffer_size)};
	if(new_buffer==nullptr)
		fast_terminate();
	fp->_ptr=fp->_base=reinterpret_cast<char*>(new_buffer);
	fp->_cnt=0;
}

#if __has_cpp_attribute(gnu::cold)
[[gnu::cold]]
#endif
inline void wincrt_fp_write_cold_malloc_case_impl(std::FILE* __restrict fpp,char const* __restrict first,std::size_t diff)
{
#if defined(_MSC_VER) || defined(_UCRT)
	ucrt_iobuf* fp{reinterpret_cast<ucrt_iobuf*>(fpp)};
#else
	std::FILE* fp{fpp};
#endif
	if(diff==0)
		return;
	std::size_t allocated_buffer_size{wincrt_internal_buffer_size};
	if(fp->_bufsiz>=4)
	{
		allocated_buffer_size=fp->_bufsiz;
		allocated_buffer_size>>=2;
		allocated_buffer_size<<=2;
	}
	if(diff>=allocated_buffer_size)
	{
		posix_write_nolock_impl(static_cast<int>(fp->_file),first,diff);
		return;
	}
	auto newbuffer{malloc(allocated_buffer_size)};
	if(newbuffer==nullptr)[[unlikely]]
		fast_terminate();
	my_memcpy(newbuffer,first,diff);
	fp->_ptr=(fp->_base=reinterpret_cast<char*>(newbuffer))+diff;
	fp->_bufsiz=static_cast<int>(allocated_buffer_size);
	fp->_cnt=fp->_bufsiz-diff;
	wincrt_fp_set_flag_dirty_impl(fp);
}

inline void wincrt_fp_write_cold_normal_case_impl(std::FILE* __restrict fpp,char const* __restrict first,std::size_t diff)
{
#if defined(_MSC_VER) || defined(_UCRT)
	ucrt_iobuf* fp{reinterpret_cast<ucrt_iobuf*>(fpp)};
#else
	std::FILE* fp{fpp};
#endif
	std::size_t remain{static_cast<std::size_t>(static_cast<unsigned int>(fp->_cnt))};
	non_overlapped_copy_n(first,remain,fp->_ptr);
	diff-=remain;
	first+=remain;
	fp->_ptr+=remain;
	fp->_cnt=0;
	wincrt_fp_set_flag_dirty_impl(fp);
	posix_write_simple_impl(static_cast<int>(fp->_file),fp->_base,static_cast<std::size_t>(fp->_ptr-fp->_base));
	std::size_t const bufsiz{static_cast<std::size_t>(static_cast<unsigned int>(fp->_bufsiz))};	
	if(diff>=bufsiz)
		posix_write_nolock_impl(static_cast<int>(fp->_file),first,diff);
	else
	{
		fp->_ptr=non_overlapped_copy_n(first,diff,fp->_base);
		fp->_cnt=static_cast<int>(bufsiz-diff);
	}
}

#if __has_cpp_attribute(gnu::cold)
[[gnu::cold]]
#endif
inline void wincrt_fp_write_cold_impl(std::FILE* __restrict fp,char const* __restrict first,std::size_t diff)
{
#if defined(_MSC_VER) || defined(_UCRT)
	ucrt_iobuf* fpp{reinterpret_cast<ucrt_iobuf*>(fp)};
#else
	std::FILE* fpp{fp};
#endif
	if(fpp->_base==nullptr)
		wincrt_fp_write_cold_malloc_case_impl(fp,first,diff);
	else
		wincrt_fp_write_cold_normal_case_impl(fp,first,diff);
}

template<std::integral char_type>
inline void wincrt_fp_write_impl(std::FILE* __restrict fpp,char_type const* first,char_type const* last)
{
#if defined(_MSC_VER) || defined(_UCRT)
	ucrt_iobuf* fp{reinterpret_cast<ucrt_iobuf*>(fpp)};
#else
	std::FILE* fp{fpp};
#endif
	std::size_t diff{static_cast<std::size_t>(last-first)*sizeof(char_type)};
	std::size_t remain{static_cast<std::size_t>(static_cast<unsigned int>(fp->_cnt))};
	if(diff<remain)[[likely]]
	{
		if(diff)[[likely]]
		{
			my_memcpy(fp->_ptr,first,diff);
			auto intdiff{static_cast<int>(static_cast<unsigned int>(diff))};
			fp->_cnt-=intdiff;
			fp->_ptr+=intdiff;
		}
		return;
	}
	wincrt_fp_write_cold_impl(fpp,reinterpret_cast<char const*>(first),diff);
}

inline void wincrt_fp_flush_common_impl(std::FILE* __restrict fpp)
{
#if defined(_MSC_VER) || defined(_UCRT)
	ucrt_iobuf* fp{reinterpret_cast<ucrt_iobuf*>(fpp)};
#else
	std::FILE* fp{fpp};
#endif
	posix_write_simple_impl(static_cast<int>(fp->_file),fp->_base,fp->_ptr-fp->_base);
}



template<std::integral char_type>
requires (sizeof(char_type)<=4)
inline void wincrt_fp_overflow_impl(std::FILE* __restrict fpp,char_type ch)
{
#if defined(_MSC_VER) || defined(_UCRT)
	ucrt_iobuf* fp{reinterpret_cast<ucrt_iobuf*>(fpp)};
#else
	std::FILE* fp{fpp};
#endif
	if(fp->_base==nullptr)
		wincrt_fp_allocate_buffer_impl(fpp);
	else
		posix_write_simple_impl(static_cast<int>(fp->_file),fp->_base,fp->_bufsiz);
	fp->_ptr=fp->_base;
	my_memcpy(fp->_ptr,&ch,sizeof(ch));
	fp->_ptr+=sizeof(ch);
	fp->_cnt=static_cast<int>(static_cast<unsigned int>(fp->_bufsiz-sizeof(ch)));
	wincrt_fp_set_flag_dirty_impl(fp);
}
[[gnu::cold]]
inline void wincrt_fp_flush_stdout_impl()
{
#if defined(_MSC_VER) || defined(_UCRT)
	ucrt_iobuf* fp{reinterpret_cast<ucrt_iobuf*>(stdout)};
#else
	std::FILE* fp{stdout};
#endif
	std::size_t diff{static_cast<std::size_t>(fp->_ptr-fp->_base)};
	if(diff==0||!wincrt_fp_is_dirty_impl(fp))
		return;
	posix_write_simple_impl(static_cast<int>(fp->_file),fp->_base,diff);
	fp->_ptr=fp->_base;
}

[[gnu::cold]]
inline std::size_t wincrt_fp_read_cold_impl(std::FILE* __restrict fpp,char* first,std::size_t diff)
{
	if(fpp==stdin)
		wincrt_fp_flush_stdout_impl();
#if defined(_MSC_VER) || defined(_UCRT)
	ucrt_iobuf* fp{reinterpret_cast<ucrt_iobuf*>(fpp)};
#else
	std::FILE* fp{fpp};
#endif
	std::size_t cnt{static_cast<std::size_t>(static_cast<unsigned int>(fp->_cnt))};
	non_overlapped_copy_n(fp->_ptr,cnt,first);
	auto first_temp{first};
	first+=diff;
	diff-=cnt;

	std::size_t allocated_buffer_size{static_cast<std::size_t>(static_cast<unsigned int>(fp->_bufsiz))};
	if(allocated_buffer_size<4)
		allocated_buffer_size=wincrt_internal_buffer_size;

	if(diff>=allocated_buffer_size)
		return posix_read_impl(static_cast<int>(fp->_file),first,diff);
	else
	{
		if(fp->_base==nullptr)
		{
			auto new_buffer{malloc(allocated_buffer_size)};
			if(new_buffer==nullptr)
				fast_terminate();
			fp->_ptr=fp->_base=reinterpret_cast<char*>(new_buffer);
			fp->_cnt=0;
			fp->_bufsiz=static_cast<int>(static_cast<unsigned int>(allocated_buffer_size));
		}
		std::size_t readed{posix_read_impl(static_cast<int>(fp->_file),fp->_base,static_cast<std::size_t>(static_cast<unsigned int>(fp->_bufsiz)))};
		fp->_cnt=static_cast<int>(static_cast<unsigned int>(readed));
		fp->_ptr=fp->_base;
		if(readed<diff)
			diff=readed;
		non_overlapped_copy_n(fp->_ptr,diff,first);
		fp->_ptr+=diff;
		fp->_cnt-=static_cast<int>(static_cast<unsigned int>(diff));
		return static_cast<std::size_t>(first+diff-first_temp);
	}
}

template<std::integral char_type>
inline char_type* wincrt_fp_read_impl(std::FILE* __restrict fpp,char_type* first,char_type* last)
{
#if defined(_MSC_VER) || defined(_UCRT)
	ucrt_iobuf* fp{reinterpret_cast<ucrt_iobuf*>(fpp)};
#else
	std::FILE* fp{fpp};
#endif
	std::size_t diff{static_cast<std::size_t>(last-first)*sizeof(char_type)};
	std::size_t remain{static_cast<std::size_t>(static_cast<unsigned int>(fp->_cnt))};
	if(diff<remain)[[likely]]
	{
		if(diff)[[likely]]
		{
			my_memcpy(first,diff,fp->_ptr);
			auto intdiff{static_cast<int>(static_cast<unsigned int>(diff))};
			fp->_cnt-=intdiff;
			fp->_ptr+=intdiff;
		}
		return last;
	}
	return first+wincrt_fp_read_cold_impl(fpp,reinterpret_cast<char*>(first),diff)/sizeof(char_type);
}

template<std::integral char_type>
inline bool wincrt_fp_underflow_impl(std::FILE* __restrict fpp)
{
	if(fpp==stdin)
		wincrt_fp_flush_stdout_impl();
#if defined(_MSC_VER) || defined(_UCRT)
	ucrt_iobuf* fp{reinterpret_cast<ucrt_iobuf*>(fpp)};
#else
	std::FILE* fp{fpp};
#endif
	if(fp->_base==nullptr)
		wincrt_fp_allocate_buffer_impl(fpp);
	std::size_t size{posix_read_impl(static_cast<int>(fp->_file),fp->_base,fp->_bufsiz)};
	fp->_ptr=fp->_base;
	fp->_cnt=static_cast<int>(static_cast<unsigned int>(size));
	if constexpr(sizeof(char_type)==1)
		return size;
	else
		return sizeof(char_type)<=size;
}

template<typename T,std::size_t num>
#if __has_cpp_attribute(gnu::may_alias)
[[gnu::may_alias]]
#endif
inline T* wincrt_get_buffer_ptr_impl(std::FILE* __restrict fpp) noexcept
{
	static_assert(num<4);
#if defined(_UCRT) || defined(_MSC_VER)
	ucrt_iobuf* fp{reinterpret_cast<ucrt_iobuf*>(fpp)};
#else
	std::FILE* fp{fpp};
#endif
	if constexpr(num==0)
		return reinterpret_cast<T*>(fp->_base);
	else if constexpr(num==1)
		return reinterpret_cast<T*>(fp->_ptr);
	else if constexpr(num==2)
		return reinterpret_cast<T*>(fp->_base+fp->_bufsiz);
	else
		return reinterpret_cast<T*>(fp->_ptr+fp->_cnt);
}


template<typename T>
#if __has_cpp_attribute(gnu::may_alias)
[[gnu::may_alias]]
#endif
inline void wincrt_set_buffer_curr_ptr_impl(std::FILE* __restrict fpp,
#if __has_cpp_attribute(gnu::may_alias)
[[gnu::may_alias]]
#endif
T* ptr) noexcept
{
#if defined(_MSC_VER) || defined(_UCRT)
	ucrt_iobuf* fp{reinterpret_cast<ucrt_iobuf*>(fpp)};
#else
	std::FILE* fp{fpp};
#endif
	fp->_cnt-=static_cast<std::size_t>(reinterpret_cast<char*>(ptr)-fp->_ptr)/sizeof(T);
	fp->_ptr=reinterpret_cast<char*>(ptr);
}

}

template<std::integral char_type>
[[gnu::may_alias]]
inline char_type* ibuffer_begin(basic_c_io_observer_unlocked<char_type> ciob) noexcept
{
	return details::wincrt_get_buffer_ptr_impl<char_type,0>(ciob.fp);
}

template<std::integral char_type>
[[gnu::may_alias]]
inline char_type* ibuffer_curr(basic_c_io_observer_unlocked<char_type> ciob) noexcept
{
	return details::wincrt_get_buffer_ptr_impl<char_type,1>(ciob.fp);
}

template<std::integral char_type>
[[gnu::may_alias]]
inline char_type* ibuffer_end(basic_c_io_observer_unlocked<char_type> ciob) noexcept
{
	return details::wincrt_get_buffer_ptr_impl<char_type,3>(ciob.fp);
}

template<std::integral char_type>
inline void ibuffer_set_curr(basic_c_io_observer_unlocked<char_type> ciob,char_type* ptr) noexcept
{
	details::wincrt_set_buffer_curr_ptr_impl(ciob.fp,ptr);
}

template<std::integral char_type>
inline bool underflow(basic_c_io_observer_unlocked<char_type> ciob)
{
	return details::wincrt_fp_underflow_impl<char_type>(ciob.fp);
}


template<std::integral char_type>
[[gnu::may_alias]]
inline char_type* obuffer_begin(basic_c_io_observer_unlocked<char_type> ciob) noexcept
{
	return details::wincrt_get_buffer_ptr_impl<char_type,0>(ciob.fp);
}

template<std::integral char_type>
[[gnu::may_alias]]
inline char_type* obuffer_curr(basic_c_io_observer_unlocked<char_type> ciob) noexcept
{
	return details::wincrt_get_buffer_ptr_impl<char_type,1>(ciob.fp);
}

template<std::integral char_type>
[[gnu::may_alias]]
inline char_type* obuffer_end(basic_c_io_observer_unlocked<char_type> ciob) noexcept
{
	return details::wincrt_get_buffer_ptr_impl<char_type,2>(ciob.fp);
}

template<std::integral char_type>
inline void obuffer_set_curr(basic_c_io_observer_unlocked<char_type> ciob,char_type* ptr) noexcept
{
	details::wincrt_set_buffer_curr_ptr_impl(ciob.fp,ptr);
}

template<std::integral char_type>
inline void overflow(basic_c_io_observer_unlocked<char_type> ciob,char_type ch)
{
	details::wincrt_fp_overflow_impl(ciob.fp,ch);
}

template<std::integral char_type,std::contiguous_iterator Iter>
requires (std::same_as<char_type,char>||std::same_as<std::iter_value_t<Iter>,char_type>)
inline Iter read(basic_c_io_observer_unlocked<char_type> ciob,Iter bg,Iter ed)
{
	if constexpr(!std::same_as<std::iter_value_t<Iter>,char_type>||!std::is_pointer_v<Iter>)
		return read(ciob,reinterpret_cast<char_type*>(std::to_address(bg)),
				reinterpret_cast<char_type*>(std::to_address(ed)))-
				reinterpret_cast<char_type*>(std::to_address(bg))+bg;
	else
		return details::wincrt_fp_read_impl(ciob.fp,bg,ed);
}


template<std::integral char_type,std::contiguous_iterator Iter>
requires (std::same_as<char_type,char>||std::same_as<std::iter_value_t<Iter>,char_type>)
inline void write(basic_c_io_observer_unlocked<char_type> ciob,Iter bg,Iter ed)
{
	if constexpr(!std::same_as<std::iter_value_t<Iter>,char_type>||!std::is_pointer_v<Iter>)
		write(ciob,reinterpret_cast<char_type*>(std::to_address(bg)),
				reinterpret_cast<char_type*>(std::to_address(ed)));
	else
		details::wincrt_fp_write_impl(ciob.fp,bg,ed);
}
static_assert(input_stream<c_io_observer_unlocked>);

}