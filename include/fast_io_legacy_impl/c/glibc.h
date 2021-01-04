#pragma once

namespace fast_io
{

inline constexpr char* ibuffer_begin(c_io_observer_unlocked cio) noexcept
{
	return cio.fp->_IO_read_base;
}

inline constexpr char* ibuffer_curr(c_io_observer_unlocked cio) noexcept
{
	return cio.fp->_IO_read_ptr;
}

inline constexpr char* ibuffer_end(c_io_observer_unlocked cio) noexcept
{
	return cio.fp->_IO_read_end;
}

inline constexpr void ibuffer_set_curr(c_io_observer_unlocked cio,char* ptr) noexcept
{
	cio.fp->_IO_read_ptr=ptr;
}

extern "C" int __underflow (std::FILE*) noexcept;
inline bool underflow(c_io_observer_unlocked cio) noexcept
{
	return __underflow(cio.fp)!=EOF;
}

inline constexpr char* obuffer_begin(c_io_observer_unlocked cio) noexcept
{
	return cio.fp->_IO_write_base;
}

inline constexpr char* obuffer_curr(c_io_observer_unlocked cio) noexcept
{
	return cio.fp->_IO_write_ptr;
}

inline constexpr char* obuffer_end(c_io_observer_unlocked cio) noexcept
{
	return cio.fp->_IO_write_end;
}

inline constexpr void obuffer_set_curr(c_io_observer_unlocked cio,char* ptr) noexcept
{
	cio.fp->_IO_write_ptr=ptr;
}

//extern "C" int __overflow (FILE *,int) noexcept;

inline void overflow(c_io_observer_unlocked cio,char ch)
{
	if(__overflow(cio.fp,static_cast<int>(static_cast<unsigned char>(ch)))==EOF)[[unlikely]]
		throw_posix_error();
}


[[gnu::may_alias]] inline char8_t* ibuffer_begin(u8c_io_observer_unlocked cio) noexcept
{
	return bit_cast<char8_t*>(cio.fp->_IO_read_base);
}

[[gnu::may_alias]] inline char8_t* ibuffer_curr(u8c_io_observer_unlocked cio) noexcept
{
	return bit_cast<char8_t*>(cio.fp->_IO_read_ptr);
}

[[gnu::may_alias]] inline char8_t* ibuffer_end(u8c_io_observer_unlocked cio) noexcept
{
	return bit_cast<char8_t*>(cio.fp->_IO_read_end);
}

inline void ibuffer_set_curr(u8c_io_observer_unlocked cio,char8_t* ptr) noexcept
{
	cio.fp->_IO_read_ptr=bit_cast<char*>(ptr);
}

inline bool underflow(u8c_io_observer_unlocked cio) noexcept
{
	return __underflow(cio.fp)!=EOF;
}

[[gnu::may_alias]] inline char8_t* obuffer_begin(u8c_io_observer_unlocked cio) noexcept
{
	return bit_cast<char8_t*>(cio.fp->_IO_write_base);
}

[[gnu::may_alias]] inline char8_t* obuffer_curr(u8c_io_observer_unlocked cio) noexcept
{
	return bit_cast<char8_t*>(cio.fp->_IO_write_ptr);
}

[[gnu::may_alias]] inline char8_t* obuffer_end(u8c_io_observer_unlocked cio) noexcept
{
	return bit_cast<char8_t*>(cio.fp->_IO_write_end);
}

inline void obuffer_set_curr(u8c_io_observer_unlocked cio,[[gnu::may_alias]] char8_t* ptr) noexcept
{
	cio.fp->_IO_write_ptr=bit_cast<char*>(ptr);
}

inline void overflow(u8c_io_observer_unlocked cio,char8_t ch)
{
	if(__overflow(cio.fp,static_cast<int>(ch))==EOF)[[unlikely]]
		throw_posix_error();
}

namespace details::fp_wide_hack
{
/*
https://github.com/lattera/glibc/blob/master/libio/bits/types/struct_FILE.h
*/
inline std::byte* hack_wide_data(FILE* fp) noexcept
{
	constexpr std::size_t off{sizeof(__off64_t)+2*sizeof(std::uintptr_t)};
	std::byte* value;
	memcpy(std::addressof(value),reinterpret_cast<std::byte*>(std::addressof(fp->_lock))+off,sizeof(std::byte*));
	return value;
}

template<std::size_t position,std::integral char_type=wchar_t>
requires (sizeof(char_type)==sizeof(wchar_t))
inline char_type* hack_wp(FILE* fp) noexcept
{
	constexpr std::size_t off{position*sizeof(uintptr_t)};
	char_type* value;
	memcpy(std::addressof(value),hack_wide_data(fp)+off,sizeof(wchar_t*));
	return value;
}
template<std::size_t position,std::integral char_type>
requires (sizeof(char_type)==sizeof(wchar_t))
inline void hack_wpset(FILE* fp,char_type* ptr) noexcept
{
	constexpr std::size_t off{position*sizeof(uintptr_t)};
	memcpy(hack_wide_data(fp)+off,std::addressof(ptr),sizeof(wchar_t*));
}
}
//wchar_t supports

inline wchar_t* ibuffer_begin(wc_io_observer_unlocked cio) noexcept
{
	return details::fp_wide_hack::hack_wp<2>(cio.fp);
}

inline wchar_t* ibuffer_curr(wc_io_observer_unlocked cio) noexcept
{
	return details::fp_wide_hack::hack_wp<0>(cio.fp);
}

inline wchar_t* ibuffer_end(wc_io_observer_unlocked cio) noexcept
{
	return details::fp_wide_hack::hack_wp<1>(cio.fp);
}

inline void ibuffer_set_curr(wc_io_observer_unlocked cio,wchar_t* ptr) noexcept
{
	details::fp_wide_hack::hack_wpset<0>(cio.fp,ptr);
}

extern "C" std::wint_t __wunderflow (FILE *) noexcept;
inline bool underflow(wc_io_observer_unlocked cio) noexcept
{
	return __wunderflow(cio.fp)!=WEOF;
}

inline wchar_t* obuffer_begin(wc_io_observer_unlocked cio) noexcept
{
	return details::fp_wide_hack::hack_wp<3>(cio.fp);
}

inline wchar_t* obuffer_curr(wc_io_observer_unlocked cio) noexcept
{
	return details::fp_wide_hack::hack_wp<4>(cio.fp);
}

inline wchar_t* obuffer_end(wc_io_observer_unlocked cio) noexcept
{
	return details::fp_wide_hack::hack_wp<5>(cio.fp);
}

inline void obuffer_set_curr(wc_io_observer_unlocked cio,wchar_t* ptr) noexcept
{
	details::fp_wide_hack::hack_wpset<4>(cio.fp,ptr);
}

extern "C" std::wint_t __woverflow (FILE *,std::wint_t) noexcept;

inline void overflow(wc_io_observer_unlocked cio,wchar_t ch)
{
	if(__woverflow(cio.fp,static_cast<std::wint_t>(ch))==WEOF)[[unlikely]]
		throw_posix_error();
}


[[gnu::may_alias]] inline char32_t* ibuffer_begin(u32c_io_observer_unlocked cio) noexcept
{
	return details::fp_wide_hack::hack_wp<2,char32_t>(cio.fp);
}

[[gnu::may_alias]] inline char32_t* ibuffer_curr(u32c_io_observer_unlocked cio) noexcept
{
	return details::fp_wide_hack::hack_wp<0,char32_t>(cio.fp);
}

[[gnu::may_alias]] inline char32_t* ibuffer_end(u32c_io_observer_unlocked cio) noexcept
{
	return details::fp_wide_hack::hack_wp<1,char32_t>(cio.fp);
}

inline void ibuffer_set_curr(u32c_io_observer_unlocked cio,[[gnu::may_alias]] char32_t* ptr) noexcept
{
	details::fp_wide_hack::hack_wpset<0,char32_t>(cio.fp,ptr);
}

inline bool underflow(u32c_io_observer_unlocked cio) noexcept
{
	return __wunderflow(cio.fp)!=WEOF;
}

[[gnu::may_alias]] inline char32_t* obuffer_begin(u32c_io_observer_unlocked cio) noexcept
{
	return details::fp_wide_hack::hack_wp<3,char32_t>(cio.fp);
}

[[gnu::may_alias]] inline char32_t* obuffer_curr(u32c_io_observer_unlocked cio) noexcept
{
	return details::fp_wide_hack::hack_wp<4,char32_t>(cio.fp);
}

[[gnu::may_alias]] inline char32_t* obuffer_end(u32c_io_observer_unlocked cio) noexcept
{
	return details::fp_wide_hack::hack_wp<5,char32_t>(cio.fp);
}

inline void obuffer_set_curr(u32c_io_observer_unlocked cio,[[gnu::may_alias]] char32_t* ptr) noexcept
{
	details::fp_wide_hack::hack_wpset<4,char32_t>(cio.fp,ptr);
}

inline void overflow(u32c_io_observer_unlocked cio,char32_t ch)
{
	if(__woverflow(cio.fp,static_cast<std::wint_t>(ch))==WEOF)[[unlikely]]
		throw_posix_error();
}


extern "C" int __flbf(std::FILE* fp) noexcept;

template<std::integral ch_type>
requires (std::same_as<ch_type,char>||std::same_as<ch_type,wchar_t>||std::same_as<ch_type,char8_t>||std::same_as<ch_type,char32_t>)
inline bool obuffer_is_line_buffering(basic_c_io_observer_unlocked<ch_type> ciou) noexcept
{
	return __flbf(ciou.fp);
}

static_assert(buffer_output_stream<c_io_observer_unlocked>);
static_assert(!noline_buffer_output_stream<c_io_observer_unlocked>);

}