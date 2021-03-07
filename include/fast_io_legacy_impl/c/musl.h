#pragma once


namespace fast_io
{

namespace details::fp_hack
{
//https://github.com/EOSIO/musl/blob/master/src/internal/stdio_impl.h
struct fp_model
{
	unsigned flags;
	unsigned char *rpos, *rend;
	int (*close)(FILE *);
	unsigned char *wend, *wpos;
	unsigned char *mustbezero_1;
	unsigned char *wbase;
	size_t (*read)(FILE *, unsigned char *, size_t);
	size_t (*write)(FILE *, const unsigned char *, size_t);
	off_t (*seek)(FILE *, off_t, int);
	unsigned char *buf;
	size_t buf_size;
	FILE *prev, *next;
	int fd;
	int pipe_pid;
	long lockcount;
	short dummy3;
	signed char mode;
	signed char lbf;
	volatile int lock;
	volatile int waiters;
	void *cookie;
	off_t off;
	char *getln_buf;
	void *mustbezero_2;
	unsigned char *shend;
	off_t shlim, shcnt;
	FILE *prev_locked, *next_locked;
	void *locale;
};


template<std::size_t position>
requires (position<6)
constexpr std::size_t get_offset()
{
	if constexpr(position==0)
		return offsetof(fp_model,buf);
	else if constexpr(position==1)
		return offsetof(fp_model,rpos);
	else if constexpr(position==2)
		return offsetof(fp_model,rend);
	else if constexpr(position==3)
		return offsetof(fp_model,wbase);
	else if constexpr(position==4)
		return offsetof(fp_model,wpos);
	else if constexpr(position==5)
		return offsetof(fp_model,wend);
}

template<std::integral char_type,std::size_t position>
inline char_type* hack_fp_ptr(FILE* fp)
{
	constexpr std::size_t offset{get_offset<position>()};
	char_type* value;
	::fast_io::details::my_memcpy(std::addressof(value),reinterpret_cast<std::byte*>(fp)+offset,sizeof(char_type*));
	return value;	
}

template<std::size_t position,std::integral char_type>
inline void hack_fp_set_ptr(FILE* fp,char_type* ptr)
{
	constexpr std::size_t offset(get_offset<position>());
	::fast_io::details::my_memcpy(reinterpret_cast<std::byte*>(fp)+offset,std::addressof(ptr),sizeof(char_type*));
}

}

inline char* ibuffer_begin(c_io_observer_unlocked cio) noexcept
{
	return details::fp_hack::hack_fp_ptr<char,0>(cio.fp);
}

inline char* ibuffer_curr(c_io_observer_unlocked cio) noexcept
{
	return details::fp_hack::hack_fp_ptr<char,1>(cio.fp);
}

inline char* ibuffer_end(c_io_observer_unlocked cio) noexcept
{
	return details::fp_hack::hack_fp_ptr<char,2>(cio.fp);
}

inline void ibuffer_set_curr(c_io_observer_unlocked cio,char* ptr) noexcept
{
	details::fp_hack::hack_fp_set_ptr<1>(cio.fp,ptr);
}

#if __has_cpp_attribute(gnu::may_alias)
[[gnu::may_alias]]
#endif
inline char8_t* ibuffer_begin(u8c_io_observer_unlocked cio) noexcept
{
	return details::fp_hack::hack_fp_ptr<char8_t,0>(cio.fp);
}

#if __has_cpp_attribute(gnu::may_alias)
[[gnu::may_alias]]
#endif
inline char8_t* ibuffer_curr(u8c_io_observer_unlocked cio) noexcept
{
	return details::fp_hack::hack_fp_ptr<char8_t,1>(cio.fp);
}

#if __has_cpp_attribute(gnu::may_alias)
[[gnu::may_alias]]
#endif
inline char8_t* ibuffer_end(u8c_io_observer_unlocked cio) noexcept
{
	return details::fp_hack::hack_fp_ptr<char8_t,2>(cio.fp);
}

inline void ibuffer_set_curr(u8c_io_observer_unlocked cio,
#if __has_cpp_attribute(gnu::may_alias)
[[gnu::may_alias]]
#endif
char8_t* ptr) noexcept
{
	details::fp_hack::hack_fp_set_ptr<1>(cio.fp,ptr);
}


extern "C" int __uflow (FILE *) noexcept;
extern "C" int ferror_unlocked(FILE*) noexcept;

namespace details::fp_hack
{

inline bool musl_fp_underflow_impl(std::FILE* fp)
{
	bool eof{__uflow(fp)!=EOF};
	if(!eof&&ferror_unlocked(fp))
		throw_posix_error();
	hack_fp_set_ptr<1>(fp,hack_fp_ptr<char,0>(fp));
	return eof;
}

}

inline bool underflow(c_io_observer_unlocked cio)
{
	return details::fp_hack::musl_fp_underflow_impl(cio.fp);
}

inline bool underflow(u8c_io_observer_unlocked cio)
{
	return details::fp_hack::musl_fp_underflow_impl(cio.fp);
}

inline char* obuffer_begin(c_io_observer_unlocked cio) noexcept
{
	return details::fp_hack::hack_fp_ptr<char,3>(cio.fp);
}

inline char* obuffer_curr(c_io_observer_unlocked cio) noexcept
{
	return details::fp_hack::hack_fp_ptr<char,4>(cio.fp);
}

inline char* obuffer_end(c_io_observer_unlocked cio) noexcept
{
	return details::fp_hack::hack_fp_ptr<char,5>(cio.fp);
}

inline void obuffer_set_curr(c_io_observer_unlocked cio,char* ptr) noexcept
{
	details::fp_hack::hack_fp_set_ptr<4>(cio.fp,ptr);
}

#if __has_cpp_attribute(gnu::may_alias)
[[gnu::may_alias]]
#endif
inline char8_t* obuffer_begin(u8c_io_observer_unlocked cio) noexcept
{
	return details::fp_hack::hack_fp_ptr<char8_t,3>(cio.fp);
}

#if __has_cpp_attribute(gnu::may_alias)
[[gnu::may_alias]]
#endif
inline char8_t* obuffer_curr(u8c_io_observer_unlocked cio) noexcept
{
	return details::fp_hack::hack_fp_ptr<char8_t,4>(cio.fp);
}

#if __has_cpp_attribute(gnu::may_alias)
[[gnu::may_alias]]
#endif
inline char8_t* obuffer_end(u8c_io_observer_unlocked cio) noexcept
{
	return details::fp_hack::hack_fp_ptr<char8_t,5>(cio.fp);
}

inline void obuffer_set_curr(u8c_io_observer_unlocked cio,
#if __has_cpp_attribute(gnu::may_alias)
[[gnu::may_alias]]
#endif
char8_t* ptr) noexcept
{
	details::fp_hack::hack_fp_set_ptr<4>(cio.fp,ptr);
}


extern "C" int __overflow(_IO_FILE *, int) noexcept;

inline void overflow(c_io_observer_unlocked cio,char ch)
{
	if(__overflow(cio.fp,static_cast<int>(static_cast<unsigned char>(ch)))==EOF)[[unlikely]]
		throw_posix_error();
}

inline void overflow(u8c_io_observer_unlocked cio,char8_t ch)
{
	if(__overflow(cio.fp,static_cast<int>(static_cast<unsigned char>(ch)))==EOF)[[unlikely]]
		throw_posix_error();
}

}