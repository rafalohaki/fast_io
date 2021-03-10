#pragma once

namespace fast_io
{
/*
Google BIONIC folks are losers

Hack this bullshit let those google losers suffer
*/

namespace details
{

struct
#if __has_cpp_attribute(gnu::may_alias)
[[gnu::may_alias]]
#endif
__sbuf {
	unsigned char *_base;
	int	_size;
};

struct
#if __has_cpp_attribute(gnu::may_alias)
[[gnu::may_alias]]
#endif
__sFILE {
	unsigned char *_p;	/* current position in (some) buffer */
	int	_r;		/* read space left for getc() */
	int	_w;		/* write space left for putc() */
	short	_flags;		/* flags, below; this FILE is free if 0 */
	short	_file;		/* fileno, if Unix descriptor, else -1 */
	struct	__sbuf _bf;	/* the buffer (at least 1 byte, if !NULL) */
	int	_lbfsize;	/* 0 or -_bf._size, for inline putc */

	/* operations */
	void	*_cookie;	/* cookie passed to io functions */
	int	(*_close)(void *);
	int	(*_read)(void *, char *, int);
	fpos_t	(*_seek)(void *, fpos_t, int);
	int	(*_write)(void *, const char *, int);

	/* extension data, to avoid further ABI breakage */
	struct	__sbuf _ext;
	/* data for long sequences of ungetc() */
	unsigned char *_up;	/* saved _p when _p is doing ungetc data */
	int	_ur;		/* saved _r when _r is counting ungetc data */

	/* tricks to meet minimum requirements even when malloc() fails */
	unsigned char _ubuf[3];	/* guarantee an ungetc() buffer */
	unsigned char _nbuf[1];	/* guarantee a getc() buffer */

	/* separate buffer for fgetln() when line crosses buffer boundary */
	struct	__sbuf _lb;	/* buffer for fgetln() */

	/* Unix stdio files get aligned to block boundaries on fseek() */
	int	_blksize;	/* stat.st_blksize (may be != _bf._size) */
	fpos_t	_offset;	/* current lseek offset */
};

}

inline char* obuffer_begin(c_io_observer_unlocked cio) noexcept
{
	
	return reinterpret_cast<char*>(reinterpret_cast<details::__sFILE*>(cio.fp)->_bf._base);
}

inline char* obuffer_curr(c_io_observer_unlocked cio) noexcept
{
	return reinterpret_cast<char*>(reinterpret_cast<details::__sFILE*>(cio.fp)->_p);
}

inline char* obuffer_end(c_io_observer_unlocked cio) noexcept
{
	return reinterpret_cast<char*>(reinterpret_cast<details::__sFILE*>(cio.fp)->_p+
		reinterpret_cast<details::__sFILE*>(cio.fp)->_w);
}

inline void obuffer_set_curr(c_io_observer_unlocked cio,char* ptr) noexcept
{
	auto fp{reinterpret_cast<details::__sFILE*>(cio.fp)};
	fp->_w-=reinterpret_cast<char unsigned*>(ptr)-fp->_p;
	fp->_p=reinterpret_cast<char unsigned*>(ptr);
}

inline void overflow(c_io_observer_unlocked cio,char ch)
{
	obuffer_set_curr(cio,obuffer_end(cio));
	if(putc_unlocked(static_cast<int>(static_cast<char unsigned>(ch)),cio.fp)==EOF)[[unlikely]]
		throw_posix_error();
}

inline char* ibuffer_begin(c_io_observer_unlocked cio) noexcept
{
	return reinterpret_cast<char*>(reinterpret_cast<details::__sFILE*>(cio.fp)->_bf._base);
}

inline char* ibuffer_curr(c_io_observer_unlocked cio) noexcept
{
	return reinterpret_cast<char*>(reinterpret_cast<details::__sFILE*>(cio.fp)->_p);
}

inline char* ibuffer_end(c_io_observer_unlocked cio) noexcept
{
	return reinterpret_cast<char*>(reinterpret_cast<details::__sFILE*>(cio.fp)->_p+
				reinterpret_cast<details::__sFILE*>(cio.fp)->_r);
}

inline void ibuffer_set_curr(c_io_observer_unlocked cio,char* ptr) noexcept
{
	reinterpret_cast<details::__sFILE*>(cio.fp)->_r-=reinterpret_cast<char unsigned*>(ptr)-reinterpret_cast<details::__sFILE*>(cio.fp)->_p;
	reinterpret_cast<details::__sFILE*>(cio.fp)->_p=reinterpret_cast<char unsigned*>(ptr);
}

inline bool underflow(c_io_observer_unlocked cio)
{
	bool eof{getc_unlocked(cio.fp)!=EOF};
	if(!eof&&ferror_unlocked(cio.fp))
		throw_posix_error();
	ibuffer_set_curr(cio,ibuffer_begin(cio));
	return eof;
}

#if __has_cpp_attribute(gnu::may_alias)
[[gnu::may_alias]]
#endif
inline char8_t* obuffer_begin(u8c_io_observer_unlocked cio) noexcept
{
	return reinterpret_cast<char8_t*>(reinterpret_cast<details::__sFILE*>(cio.fp)->_bf._base);
}

#if __has_cpp_attribute(gnu::may_alias)
[[gnu::may_alias]]
#endif
inline char8_t* obuffer_curr(u8c_io_observer_unlocked cio) noexcept
{
	return reinterpret_cast<char8_t*>(reinterpret_cast<details::__sFILE*>(cio.fp)->_p);
}

#if __has_cpp_attribute(gnu::may_alias)
[[gnu::may_alias]]
#endif
inline char8_t* obuffer_end(u8c_io_observer_unlocked cio) noexcept
{
	return reinterpret_cast<char8_t*>(reinterpret_cast<details::__sFILE*>(cio.fp)->_p+reinterpret_cast<details::__sFILE*>(cio.fp)->_w);
}

inline void obuffer_set_curr(u8c_io_observer_unlocked cio,
#if __has_cpp_attribute(gnu::may_alias)
[[gnu::may_alias]]
#endif
char8_t* ptr) noexcept
{
	reinterpret_cast<details::__sFILE*>(cio.fp)->_w-=reinterpret_cast<char unsigned*>(ptr)-reinterpret_cast<details::__sFILE*>(cio.fp)->_p;
	reinterpret_cast<details::__sFILE*>(cio.fp)->_p=reinterpret_cast<char unsigned*>(ptr);
}

inline void overflow(u8c_io_observer_unlocked cio,char8_t ch)
{
	obuffer_set_curr(cio,obuffer_end(cio));
	if(putc_unlocked(static_cast<int>(static_cast<char unsigned>(ch)),cio.fp)==EOF)[[unlikely]]
		throw_posix_error();
}

inline char8_t* ibuffer_begin(u8c_io_observer_unlocked cio) noexcept
{
	return reinterpret_cast<char8_t*>(reinterpret_cast<details::__sFILE*>(cio.fp)->_bf._base);
}

inline char8_t* ibuffer_curr(u8c_io_observer_unlocked cio) noexcept
{
	return reinterpret_cast<char8_t*>(reinterpret_cast<details::__sFILE*>(cio.fp)->_p);
}

inline char8_t* ibuffer_end(u8c_io_observer_unlocked cio) noexcept
{
	return reinterpret_cast<char8_t*>(reinterpret_cast<details::__sFILE*>(cio.fp)->_p+reinterpret_cast<details::__sFILE*>(cio.fp)->_r);
}

inline void ibuffer_set_curr(u8c_io_observer_unlocked cio,
#if __has_cpp_attribute(gnu::may_alias)
[[gnu::may_alias]]
#endif
char8_t* ptr) noexcept
{
	reinterpret_cast<details::__sFILE*>(cio.fp)->_r-=reinterpret_cast<char unsigned*>(ptr)-reinterpret_cast<details::__sFILE*>(cio.fp)->_p;
	reinterpret_cast<details::__sFILE*>(cio.fp)->_p=reinterpret_cast<char unsigned*>(ptr);
}

inline bool underflow(u8c_io_observer_unlocked cio)
{
	bool eof{getc_unlocked(cio.fp)!=EOF};
	if(!eof&&ferror_unlocked(cio.fp))
		throw_posix_error();
	ibuffer_set_curr(cio,ibuffer_begin(cio));
	return eof;
}


}