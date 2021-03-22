#pragma once

namespace fast_io
{

inline constexpr char* ibuffer_begin(c_io_observer_unlocked cio) noexcept
{
	return cio.fp->_base;
}

inline constexpr char* ibuffer_curr(c_io_observer_unlocked cio) noexcept
{
	return cio.fp->_ptr;
}

inline constexpr char* ibuffer_end(c_io_observer_unlocked cio) noexcept
{
	return cio.fp->_ptr+cio.fp->_cnt;
}

inline constexpr void ibuffer_set_curr(c_io_observer_unlocked cio,char* ptr) noexcept
{
	cio.fp->_cnt-=ptr-cio.fp->_ptr;
	cio.fp->_ptr=ptr;
}
extern int _filbuf(std::FILE *) noexcept asm("_filbuf");
inline bool underflow(c_io_observer_unlocked cio) noexcept
{
	if(_filbuf(cio.fp)==EOF)[[unlikely]]
		return false;
	++cio.fp->_cnt;
	--cio.fp->_ptr;
	return true;
}

inline constexpr char* obuffer_begin(c_io_observer_unlocked cio) noexcept
{
	return cio.fp->_base;
}

inline constexpr char* obuffer_curr(c_io_observer_unlocked cio) noexcept
{
	return cio.fp->_ptr;
}

inline constexpr char* obuffer_end(c_io_observer_unlocked cio) noexcept
{
	return cio.fp->_base+cio.fp->_bufsiz;
}

inline constexpr void obuffer_set_curr(c_io_observer_unlocked cio,char* ptr) noexcept
{
	cio.fp->_flag|=0x010000;
	cio.fp->_cnt-=ptr-cio.fp->_ptr;
	cio.fp->_ptr=ptr;
}

extern int  _flsbuf(int, std::FILE*) noexcept asm("_flsbuf");

inline void overflow(c_io_observer_unlocked cio,char ch)
{
	cio.fp->_flag|=0x010000;
	if(_flsbuf(static_cast<int>(static_cast<unsigned char>(ch)),cio.fp)==EOF)[[unlikely]]
		throw_posix_error();
}


static_assert(buffer_io_stream<c_io_observer_unlocked>);
static_assert(!buffer_io_stream<basic_c_io_observer_unlocked<char8_t>>);

}