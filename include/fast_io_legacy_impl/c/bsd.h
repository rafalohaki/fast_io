#pragma once
//https://github.com/freebsd/freebsd/blob/master/include/stdio.h


namespace fast_io
{

inline char* obuffer_begin(c_io_observer_unlocked cio) noexcept
{
	return reinterpret_cast<char*>(cio.fp->_bf._base);
}

inline char* obuffer_curr(c_io_observer_unlocked cio) noexcept
{
	return reinterpret_cast<char*>(cio.fp->_p);
}

inline char* obuffer_end(c_io_observer_unlocked cio) noexcept
{
	return reinterpret_cast<char*>(cio.fp->_p+cio.fp->_w);
}

inline void obuffer_set_curr(c_io_observer_unlocked cio,char* ptr) noexcept
{
	cio.fp->_w-=reinterpret_cast<char unsigned*>(ptr)-cio.fp->_p;
	cio.fp->_p=reinterpret_cast<char unsigned*>(ptr);
}

inline void overflow(c_io_observer_unlocked cio,char ch)
{
	obuffer_set_curr(cio,obuffer_end(cio));
	if(__sputc(static_cast<int>(static_cast<char unsigned>(ch)),cio.fp)==EOF)[[unlikely]]
		throw_posix_error();
}

inline char* ibuffer_begin(c_io_observer_unlocked cio) noexcept
{
	return reinterpret_cast<char*>(cio.fp->_bf._base);
}

inline char* ibuffer_curr(c_io_observer_unlocked cio) noexcept
{
	return reinterpret_cast<char*>(cio.fp->_p);
}

inline char* ibuffer_end(c_io_observer_unlocked cio) noexcept
{
	return reinterpret_cast<char*>(cio.fp->_p+cio.fp->_r);
}

inline void ibuffer_set_curr(c_io_observer_unlocked cio,char* ptr) noexcept
{
	cio.fp->_r-=reinterpret_cast<char unsigned*>(ptr)-cio.fp->_p;
	cio.fp->_p=reinterpret_cast<char unsigned*>(ptr);
}

extern "C" int	__srget(FILE *) noexcept;

inline bool underflow(c_io_observer_unlocked cio)
{
	bool eof{__srget(cio.fp)!=EOF};
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
	return reinterpret_cast<char8_t*>(cio.fp->_bf._base);
}

#if __has_cpp_attribute(gnu::may_alias)
[[gnu::may_alias]]
#endif
inline char8_t* obuffer_curr(u8c_io_observer_unlocked cio) noexcept
{
	return reinterpret_cast<char8_t*>(cio.fp->_p);
}

#if __has_cpp_attribute(gnu::may_alias)
[[gnu::may_alias]]
#endif
inline char8_t* obuffer_end(u8c_io_observer_unlocked cio) noexcept
{
	return reinterpret_cast<char8_t*>(cio.fp->_p+cio.fp->_w);
}

inline void obuffer_set_curr(u8c_io_observer_unlocked cio,
#if __has_cpp_attribute(gnu::may_alias)
[[gnu::may_alias]]
#endif
char8_t* ptr) noexcept
{
	cio.fp->_w-=reinterpret_cast<char unsigned*>(ptr)-cio.fp->_p;
	cio.fp->_p=reinterpret_cast<char unsigned*>(ptr);
}

inline void overflow(u8c_io_observer_unlocked cio,char8_t ch)
{
	obuffer_set_curr(cio,obuffer_end(cio));
	if(__sputc(static_cast<int>(static_cast<char unsigned>(ch)),cio.fp)==EOF)[[unlikely]]
		throw_posix_error();
}

inline char8_t* ibuffer_begin(u8c_io_observer_unlocked cio) noexcept
{
	return reinterpret_cast<char8_t*>(cio.fp->_bf._base);
}

inline char8_t* ibuffer_curr(u8c_io_observer_unlocked cio) noexcept
{
	return reinterpret_cast<char8_t*>(cio.fp->_p);
}

inline char8_t* ibuffer_end(u8c_io_observer_unlocked cio) noexcept
{
	return reinterpret_cast<char8_t*>(cio.fp->_p+cio.fp->_r);
}

inline void ibuffer_set_curr(u8c_io_observer_unlocked cio,
#if __has_cpp_attribute(gnu::may_alias)
[[gnu::may_alias]]
#endif
char8_t* ptr) noexcept
{
	cio.fp->_r-=reinterpret_cast<char unsigned*>(ptr)-cio.fp->_p;
	cio.fp->_p=reinterpret_cast<char unsigned*>(ptr);
}

inline bool underflow(u8c_io_observer_unlocked cio)
{
	bool eof{__srget(cio.fp)!=EOF};
	if(!eof&&ferror_unlocked(cio.fp))
		throw_posix_error();
	ibuffer_set_curr(cio,ibuffer_begin(cio));
	return eof;
}

static_assert(buffer_io_stream<c_io_observer_unlocked>);
static_assert(buffer_io_stream<u8c_io_observer_unlocked>);

}