#pragma once

namespace fast_io
{

/*
MinGW64 layout
struct _iobuf
{
char *_ptr;
int _cnt;
char *_base;
int _flag;
int _file;
int _charbuf;
int _bufsiz;
char *_tmpfname;
};

*/


inline constexpr char* ibuffer_begin(c_io_observer_unlocked cio)
{
	return cio.fp->_base;
}

inline constexpr char* ibuffer_curr(c_io_observer_unlocked cio)
{
	return cio.fp->_ptr;
}

inline constexpr char* ibuffer_end(c_io_observer_unlocked cio)
{
	return cio.fp->_ptr+cio.fp->_cnt;
}

inline constexpr void ibuffer_set_curr(c_io_observer_unlocked cio,char* ptr)
{
	cio.fp->_cnt-=ptr-cio.fp->_ptr;
	cio.fp->_ptr=ptr;
}

inline bool underflow(c_io_observer_unlocked cio)
{
	if(_filbuf(cio.fp)==EOF)[[unlikely]]
		return false;
	++cio.fp->_cnt;
	--cio.fp->_ptr;
	return true;
}

inline constexpr char* obuffer_begin(c_io_observer_unlocked cio)
{
	return cio.fp->_base;
}

inline constexpr char* obuffer_curr(c_io_observer_unlocked cio)
{
	return cio.fp->_ptr;
}

inline constexpr char* obuffer_end(c_io_observer_unlocked cio)
{
	return cio.fp->_base+cio.fp->_bufsiz;
}

inline constexpr void obuffer_set_curr(c_io_observer_unlocked cio,char* ptr)
{
	cio.fp->_flag|=0x010000;
	cio.fp->_cnt-=ptr-cio.fp->_ptr;
	cio.fp->_ptr=ptr;
}

inline void overflow(c_io_observer_unlocked cio,char ch)
{
	cio.fp->_flag|=0x010000;
	if(_flsbuf(static_cast<int>(static_cast<unsigned char>(ch)),cio.fp)==EOF)[[unlikely]]
		throw_posix_error();
}

inline constexpr bool obuffer_is_active(c_io_observer_unlocked cio)
{
	return cio.fp->_base;
}

[[gnu::may_alias]] inline wchar_t* ibuffer_begin(wc_io_observer_unlocked cio)
{
	return bit_cast<wchar_t*>(cio.fp->_base);
}

[[gnu::may_alias]] inline wchar_t* ibuffer_curr(wc_io_observer_unlocked cio)
{
	return bit_cast<wchar_t*>(cio.fp->_ptr);
}

[[gnu::may_alias]] inline wchar_t* ibuffer_end(wc_io_observer_unlocked cio)
{
	return bit_cast<wchar_t*>(cio.fp->_ptr+cio.fp->_cnt);
}

inline void ibuffer_set_curr(wc_io_observer_unlocked cio, [[gnu::may_alias]] wchar_t* ptr)
{
	cio.fp->_cnt-=reinterpret_cast<char*>(ptr)-cio.fp->_ptr;
	cio.fp->_ptr=reinterpret_cast<char*>(ptr);
}

inline bool underflow(wc_io_observer_unlocked cio)
{
	if(_filbuf(cio.fp)==EOF)[[unlikely]]
		return false;
	++cio.fp->_cnt;
	--cio.fp->_ptr;
	return true;
}

[[gnu::may_alias]] inline wchar_t* obuffer_begin(wc_io_observer_unlocked cio)
{
	return bit_cast<wchar_t*>(cio.fp->_base);
}

[[gnu::may_alias]] inline wchar_t* obuffer_curr(wc_io_observer_unlocked cio)
{
	return bit_cast<wchar_t*>(cio.fp->_ptr);
}

[[gnu::may_alias]] inline wchar_t* obuffer_end(wc_io_observer_unlocked cio)
{
	return bit_cast<wchar_t*>(cio.fp->_base+cio.fp->_bufsiz);
}

inline void obuffer_set_curr(wc_io_observer_unlocked cio,[[gnu::may_alias]] wchar_t* ptr)
{
	cio.fp->_flag|=0x010000;
	cio.fp->_cnt-=bit_cast<char*>(ptr)-cio.fp->_ptr;
	cio.fp->_ptr=bit_cast<char*>(ptr);
}

inline void overflow(wc_io_observer_unlocked cio,wchar_t ch)
{
	obuffer_set_curr(cio,obuffer_end(cio));
	if(fputwc(static_cast<wint_t>(static_cast<std::make_unsigned_t<wchar_t>>(ch)),cio.fp)==WEOF)[[unlikely]]
		throw_posix_error();
}

inline constexpr bool obuffer_is_active(wc_io_observer_unlocked cio)
{
	return cio.fp->_base;
}

static_assert(buffer_io_stream<c_io_observer_unlocked>);
static_assert(maybe_buffer_output_stream<c_io_observer_unlocked>);
static_assert(buffer_io_stream<wc_io_observer_unlocked>);
static_assert(maybe_buffer_output_stream<wc_io_observer_unlocked>);
static_assert(!buffer_io_stream<basic_c_io_observer_unlocked<char8_t>>);
}