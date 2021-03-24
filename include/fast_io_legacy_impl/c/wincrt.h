#pragma once

namespace fast_io::details
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
#if 0
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
    long             _flags;
    long             _file;
    int              _charbuf;
    int              _bufsiz;
    char*            _tmpfname;
};
#endif
#endif

inline constexpr std::size_t wincrt_internal_buffer_size{4096};


[[gnu::cold]]
inline void wincrt_fp_allocate_buffer_impl(std::FILE* __restrict fp)
{
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
}

#if __has_cpp_attribute(gnu::cold)
[[gnu::cold]]
#endif
inline void wincrt_fp_write_cold_malloc_case_impl(std::FILE* __restrict fp,char const* __restrict first,std::size_t diff)
{
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
		posix_write_nolock_impl(static_cast<int>(fp->_file),first,diff);
	auto newbuffer{malloc(allocated_buffer_size)};
	if(newbuffer==nullptr)[[unlikely]]
		fast_terminate();
	my_memcpy(newbuffer,first,diff);
	fp->_ptr=(fp->_base=reinterpret_cast<char*>(newbuffer))+diff;
	fp->_bufsiz=static_cast<int>(allocated_buffer_size);
	fp->_cnt=fp->_bufsiz-diff;
}

inline void wincrt_fp_write_cold_normal_case_impl(std::FILE* __restrict fp,char const* __restrict first,std::size_t diff)
{
	std::size_t remain{static_cast<std::size_t>(static_cast<unsigned int>(fp->_cnt))};
	non_overlapped_copy_n(first,remain,fp->_ptr);
	diff-=remain;
	first+=remain;
	fp->_ptr+=remain;
	fp->_cnt=0;
	posix_write_simple_impl(static_cast<int>(fp->_file),fp->_base,static_cast<std::size_t>(fp->_ptr-fp->_base));
	std::size_t const bufsiz{static_cast<std::size_t>(static_cast<unsigned int>(fp->_bufsiz))};
	if(diff>=bufsiz)
		posix_write_nolock_impl(static_cast<int>(fp->_file),first,diff);
	else
	{
		fp->_ptr=(fp->_base=reinterpret_cast<char*>(newbuffer))+diff;
		fp->_bufsiz=static_cast<int>(allocated_buffer_size);
		fp->_cnt=static_cast<int>(bufsiz-diff);
	}
}

#if __has_cpp_attribute(gnu::cold)
[[gnu::cold]]
#endif
inline void wincrt_fp_write_cold_impl(std::FILE* __restrict fp,char const* __restrict first,std::size_t diff)
{
	if(fp->_base==nullptr)
		wincrt_fp_write_cold_malloc_case_impl(fp,first,diff);
	else
		wincrt_fp_write_cold_normal_case_impl(fp,first,diff);
}

template<std::integral char_type>
inline void wincrt_fp_write_impl(std::FILE* __restrict fp,char_type const* first,char_type const* last)
{
	std::size_t diff{static_cast<std::size_t>(last-first)*sizeof(char_type)};
	std::size_t remain{static_cast<std::size_t>(static_cast<unsigned int>(fp->_cnt))};
	if(diff<remain)[[likely]]
	{
		if(diff)[[likely]]
		{
			my_memcpy(fp->_ptr,diff,first);
			auto intdiff{static_cast<int>(static_cast<unsigned int>(diff))};
			fp->_cnt-=intdiff;
			fp->_ptr+=intdiff;
		}
		return;
	}
	wincrt_fp_write_cold_impl(fp,reinterpret_cast<char const*>(first),diff);
}

inline void wincrt_fp_flush_common_impl(std::FILE* __restrict fp)
{
	posix_write_simple_impl(static_cast<int>(fp->_file),fp->_base,fp->_ptr-fp->_base);
}



template<std::integral char_type>
inline void wincrt_fp_overflow_impl(std::FILE* __restrict fp,char_type ch)
{
	if(fp->_base==nullptr)
		wincrt_fp_fls_impl(fp,ch);
	else
		posix_write_simple_impl(static_cast<int>(fp->_file),fp->_base,fp->_bufsiz);
	fp->_ptr=fp->_base;
	my_memcpy(fp->_ptr,&ch,sizeof(ch));
	fp->_ptr+=sizeof(ch);
	fp->_cnt=static_cast<int>(static_cast<unsigned int>(fp->_bufsiz-sizeof(ch)));
}
[[gnu::cold]]
inline void wincrt_fp_flush_tie_impl()
{
	auto fp{stdout}:
	if(fp->_cnt==0)
		return;
	posix_write_simple_impl(static_cast<int>(fp->_file),fp->_base,fp->_bufsiz);
}

[[gnu::cold]]
inline std::size_t wincrt_fp_read_cold_impl(std::FILE* __restrict fp,char* first,std::size_t diff)
{
	if(fp==stdin)
		wincrt_fp_flush_tie_impl();
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
inline char_type* wincrt_fp_read_impl(std::FILE* __restrict fp,char_type* first,char_type* last)
{
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
	return first+wincrt_fp_read_cold_impl(fp,reinterpret_cast<char*>(first),diff)/sizeof(char_type);
}

template<std::integral char_type>
inline bool wincrt_fp_underflow_impl(std::FILE* __restrict fp)
{
	if(fp==stdin)
		wincrt_fp_flush_stdout_impl();
	if(fp->_base==nullptr)
		wincrt_fp_ibuffer_allocate_buffer_impl(fp);
	auto it{posix_read_impl(static_cast<int>(fp->_file),fp->_base,fp->_base+fp->_bufsiz)};
	if constexpr(sizeof(char_type)==1)
		return it!=fp->_base;
	else
		return sizeof(char_type)<=static_cast<std::size_t>(it-fp->_base);
}

}