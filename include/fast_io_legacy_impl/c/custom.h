#pragma once

namespace fast_io
{

template<c_family family,std::integral char_type>
requires requires(FILE* fp)
{
	{::std::posix::__libc_stdio_fp_ibuffer_begin<char_type>(fp)}->char_type*;
}
inline char_type* ibuffer_begin(basic_c_io_observer_unlocked<family,char_type> ciob) noexcept
{
	return ::std::posix::__libc_stdio_fp_ibuffer_begin<char_type>(ciob.fp);
}

template<c_family family,std::integral char_type>
requires requires(FILE* fp)
{
	{::std::posix::__libc_stdio_fp_ibuffer_curr<char_type>(fp)}->char_type*;
}
inline char_type* ibuffer_curr(basic_c_io_observer_unlocked<family,char_type> ciob) noexcept
{
	return ::std::posix::__libc_stdio_fp_ibuffer_curr<char_type>(ciob.fp);
}

template<c_family family,std::integral char_type>
requires requires(FILE* fp)
{
	{::std::posix::__libc_stdio_fp_ibuffer_end<char_type>(fp)}->char_type*;
}
inline char_type* ibuffer_end(c_io_observer_unlocked ciob) noexcept
{
	return ::std::posix::__libc_stdio_fp_ibuffer_end<char_type>(ciob.fp);
}

template<c_family family,std::integral char_type>
requires requires(FILE* fp,char_type* ptr)
{
	{::std::posix::__libc_stdio_fp_ibuffer_set_curr<char_type>(fp)}->char_type*;
}
inline char_type* ibuffer_set_curr(c_io_observer_unlocked ciob,char_type* ptr) noexcept
{
	return ::std::posix::__libc_stdio_fp_ibuffer_set_curr<char_type>(ciob.fp,ptr);
}

template<c_family family,std::integral char_type>
inline bool ibuffer_underflow(c_io_observer_unlocked ciob)
requires requires(FILE* fp,char_type* ptr)
{
	{::std::posix::__libc_stdio_fp_ibuffer_underflow<char_type>(fp)}->bool;
}
{
	return ::std::posix::__libc_stdio_fp_ibuffer_underflow<char_type>(ciob.fp,ptr);
}


template<c_family family,std::integral char_type>
requires requires(FILE* fp)
{
	{::std::posix::__libc_stdio_fp_obuffer_begin<char_type>(fp)}->char_type*;
}
inline char_type* obuffer_begin(basic_c_io_observer_unlocked<family,char_type> ciob) noexcept
{
	return ::std::posix::__libc_stdio_fp_obuffer_begin<char_type>(ciob.fp);
}

template<c_family family,std::integral char_type>
requires requires(FILE* fp)
{
	{::std::posix::__libc_stdio_fp_obuffer_curr<char_type>(fp)}->char_type*;
}
inline char_type* obuffer_curr(basic_c_io_observer_unlocked<family,char_type> ciob) noexcept
{
	return ::std::posix::__libc_stdio_fp_obuffer_curr<char_type>(ciob.fp);
}

template<c_family family,std::integral char_type>
requires requires(FILE* fp)
{
	{::std::posix::__libc_stdio_fp_obuffer_end<char_type>(fp)}->char_type*;
}
inline char_type* obuffer_end(c_io_observer_unlocked ciob) noexcept
{
	return ::std::posix::__libc_stdio_fp_obuffer_end<char_type>(ciob.fp);
}

template<c_family family,std::integral char_type>
requires requires(FILE* fp,char_type* ptr)
{
	{::std::posix::__libc_stdio_fp_obuffer_set_curr<char_type>(fp,ptr)}->char_type*;
}
inline char_type* obuffer_set_curr(c_io_observer_unlocked ciob,char_type* ptr) noexcept
{
	return ::std::posix::__libc_stdio_fp_obuffer_set_curr<char_type>(ciob.fp,ptr);
}

template<c_family family,std::integral char_type>
requires requires(FILE* fp,char_type ch)
{
	{::std::posix::__libc_stdio_fp_obuffer_overflow<char_type>(fp,ch)}->bool;
}
inline bool obuffer_overflow(c_io_observer_unlocked ciob,char_type ch)
{
	return ::std::posix::__libc_stdio_fp_obuffer_overflow<char_type>(ciob.fp,ch);
}

}
