#pragma once
/*
newlib's FILE is very similar to BSD. Maybe it comes from BSD?

Anyway. All FILE implementations are trash tbh.

*/


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
	if(__sputc_r(_REENT,static_cast<int>(static_cast<char unsigned>(ch)),cio.fp)==EOF)[[unlikely]]
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

extern "C" int __ssrefill_r(struct _reent *,FILE *) noexcept;

inline bool underflow(c_io_observer_unlocked cio)
{
	bool eof(__ssrefill_r(_REENT,cio.fp));
	if(!eof&&__sferror(cio.fp))
		throw_posix_error();
	return eof;
}

}