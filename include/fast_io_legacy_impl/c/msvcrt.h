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

inline void overflow(c_io_observer_unlocked cio,char ch)
{
	cio.fp->_flag|=0x010000;
	if(_flsbuf(static_cast<int>(static_cast<unsigned char>(ch)),cio.fp)==EOF)[[unlikely]]
		throw_posix_error();
}

[[gnu::may_alias]] inline wchar_t* ibuffer_begin(wc_io_observer_unlocked cio) noexcept
{
	return bit_cast<wchar_t*>(cio.fp->_base);
}

[[gnu::may_alias]] inline wchar_t* ibuffer_curr(wc_io_observer_unlocked cio) noexcept
{
	return bit_cast<wchar_t*>(cio.fp->_ptr);
}

[[gnu::may_alias]] inline wchar_t* ibuffer_end(wc_io_observer_unlocked cio) noexcept
{
	return bit_cast<wchar_t*>(cio.fp->_ptr+cio.fp->_cnt);
}

inline void ibuffer_set_curr(wc_io_observer_unlocked cio, [[gnu::may_alias]] wchar_t* ptr) noexcept
{
	cio.fp->_cnt-=reinterpret_cast<char*>(ptr)-cio.fp->_ptr;
	cio.fp->_ptr=reinterpret_cast<char*>(ptr);
}

inline bool underflow(wc_io_observer_unlocked cio) noexcept
{
	if(_filbuf(cio.fp)==WEOF)[[unlikely]]
		return false;
	++cio.fp->_cnt;
	--cio.fp->_ptr;
	return true;
}

[[gnu::may_alias]] inline char8_t* ibuffer_begin(u8c_io_observer_unlocked cio) noexcept
{
	return reinterpret_cast<char8_t*>(cio.fp->_base);
}

[[gnu::may_alias]] inline char8_t* ibuffer_curr(u8c_io_observer_unlocked cio) noexcept
{
	return reinterpret_cast<char8_t*>(cio.fp->_ptr);
}

[[gnu::may_alias]] inline char8_t* ibuffer_end(u8c_io_observer_unlocked cio) noexcept
{
	return reinterpret_cast<char8_t*>(cio.fp->_ptr+cio.fp->_cnt);
}

inline void ibuffer_set_curr(u8c_io_observer_unlocked cio,[[gnu::may_alias]] char8_t* ptr) noexcept
{
	cio.fp->_cnt-=bit_cast<char*>(ptr)-cio.fp->_ptr;
	cio.fp->_ptr=bit_cast<char*>(ptr);
}

inline bool underflow(u8c_io_observer_unlocked cio) noexcept
{
	if(_filbuf(cio.fp)==EOF)[[unlikely]]
		return false;
	++cio.fp->_cnt;
	--cio.fp->_ptr;
	return true;
}

[[gnu::may_alias]] inline char8_t* obuffer_begin(u8c_io_observer_unlocked cio) noexcept
{
	return reinterpret_cast<char8_t*>(cio.fp->_base);
}

[[gnu::may_alias]] inline char8_t* obuffer_curr(u8c_io_observer_unlocked cio) noexcept
{
	return reinterpret_cast<char8_t*>(cio.fp->_ptr);
}

[[gnu::may_alias]] inline char8_t* obuffer_end(u8c_io_observer_unlocked cio) noexcept
{
	return reinterpret_cast<char8_t*>(cio.fp->_base+cio.fp->_bufsiz);
}

inline void obuffer_set_curr(u8c_io_observer_unlocked cio,[[gnu::may_alias]] char8_t* ptr) noexcept
{
	cio.fp->_flag|=0x010000;
	cio.fp->_cnt-=reinterpret_cast<char*>(ptr)-cio.fp->_ptr;
	cio.fp->_ptr=reinterpret_cast<char*>(ptr);
}

inline void overflow(u8c_io_observer_unlocked cio,char8_t ch)
{
	cio.fp->_flag|=0x010000;
	if(_flsbuf(static_cast<int>(static_cast<unsigned char>(ch)),cio.fp)==EOF)[[unlikely]]
		throw_posix_error();
}

[[gnu::may_alias]] inline wchar_t* obuffer_begin(wc_io_observer_unlocked cio) noexcept
{
	return bit_cast<wchar_t*>(cio.fp->_base);
}

[[gnu::may_alias]] inline wchar_t* obuffer_curr(wc_io_observer_unlocked cio) noexcept
{
	return bit_cast<wchar_t*>(cio.fp->_ptr);
}

[[gnu::may_alias]] inline wchar_t* obuffer_end(wc_io_observer_unlocked cio) noexcept
{
	return bit_cast<wchar_t*>(cio.fp->_base+cio.fp->_bufsiz);
}

inline void obuffer_set_curr(wc_io_observer_unlocked cio,[[gnu::may_alias]] wchar_t* ptr) noexcept
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


[[gnu::may_alias]] inline char16_t* obuffer_begin(u16c_io_observer_unlocked cio) noexcept
{
	return bit_cast<char16_t*>(cio.fp->_base);
}

[[gnu::may_alias]] inline char16_t* obuffer_curr(u16c_io_observer_unlocked cio) noexcept
{
	return bit_cast<char16_t*>(cio.fp->_ptr);
}

[[gnu::may_alias]] inline char16_t* obuffer_end(u16c_io_observer_unlocked cio) noexcept
{
	return bit_cast<char16_t*>(cio.fp->_base+cio.fp->_bufsiz);
}

inline void obuffer_set_curr(u16c_io_observer_unlocked cio,[[gnu::may_alias]] char16_t* ptr) noexcept
{
	cio.fp->_flag|=0x010000;
	cio.fp->_cnt-=bit_cast<char*>(ptr)-cio.fp->_ptr;
	cio.fp->_ptr=bit_cast<char*>(ptr);
}

inline void overflow(u16c_io_observer_unlocked cio,char16_t ch)
{
	obuffer_set_curr(cio,obuffer_end(cio));
	if(fputwc(static_cast<wint_t>(static_cast<std::make_unsigned_t<char16_t>>(ch)),cio.fp)==WEOF)[[unlikely]]
		throw_posix_error();
}

[[gnu::may_alias]] inline char16_t* ibuffer_begin(u16c_io_observer_unlocked cio) noexcept
{
	return bit_cast<char16_t*>(cio.fp->_base);
}

[[gnu::may_alias]] inline char16_t* ibuffer_curr(u16c_io_observer_unlocked cio) noexcept
{
	return bit_cast<char16_t*>(cio.fp->_ptr);
}

[[gnu::may_alias]] inline char16_t* ibuffer_end(u16c_io_observer_unlocked cio) noexcept
{
	return bit_cast<char16_t*>(cio.fp->_ptr+cio.fp->_cnt);
}

inline void ibuffer_set_curr(u16c_io_observer_unlocked cio, [[gnu::may_alias]] char16_t* ptr) noexcept
{
	cio.fp->_cnt-=reinterpret_cast<char*>(ptr)-cio.fp->_ptr;
	cio.fp->_ptr=reinterpret_cast<char*>(ptr);
}

inline bool underflow(u16c_io_observer_unlocked cio) noexcept
{
	if(_filbuf(cio.fp)==WEOF)[[unlikely]]
		return false;
	++cio.fp->_cnt;
	--cio.fp->_ptr;
	return true;
}

static_assert(buffer_io_stream<c_io_observer_unlocked>);
static_assert(buffer_io_stream<wc_io_observer_unlocked>);
static_assert(buffer_io_stream<u8c_io_observer_unlocked>);
static_assert(buffer_io_stream<u16c_io_observer_unlocked>);
}