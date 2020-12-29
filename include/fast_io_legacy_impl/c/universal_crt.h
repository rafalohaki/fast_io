#pragma once

namespace fast_io
{
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

namespace details::ucrt_hack
{
static_assert(sizeof(FILE)==sizeof(char*));


struct ucrt_stdio_stream_data_model
{
    char* ptr;
    char*            base;
    int              cnt;
    long             flags;
    long             file;
    int              charbuf;
    int              bufsiz;
    char*            tmpfname;
};


template<std::integral T=char>
inline T* get_fp_ptr(FILE* fp) noexcept
{
	T* b;
	memcpy(std::addressof(b),reinterpret_cast<std::byte*>(fp),sizeof(T*));
	return b;
}
template<std::integral T>
inline void set_fp_ptr(FILE* fp,[[gnu::may_alias]] T* b) noexcept
{
	memcpy(reinterpret_cast<std::byte*>(fp),std::addressof(b),sizeof(T*));
}

template<std::integral T=char>
inline T* get_fp_base(FILE* fp) noexcept
{
	T* b;
	memcpy(std::addressof(b),reinterpret_cast<std::byte*>(fp)+sizeof(uintptr_t),sizeof(T*));
	return b;
}

inline int get_fp_cnt(FILE* fp) noexcept
{
	constexpr std::size_t offset{sizeof(uintptr_t)*2};
	int b;
	memcpy(std::addressof(b),reinterpret_cast<std::byte*>(fp)+offset,sizeof(int));
	return b;
}

inline void set_fp_cnt(FILE* fp,int b) noexcept
{
	constexpr std::size_t offset{sizeof(uintptr_t)*2};
	memcpy(reinterpret_cast<std::byte*>(fp)+offset,
	std::addressof(b),sizeof(int));
}

inline long get_fp_flags(FILE* fp)
{
	constexpr std::size_t offset{sizeof(uintptr_t)*2+sizeof(int)};
	long b;
	memcpy(std::addressof(b),reinterpret_cast<std::byte*>(fp)+offset,sizeof(long));
	return b;
}

inline void set_fp_flags(FILE* fp,long flags)
{
	constexpr std::size_t offset{sizeof(uintptr_t)*2+sizeof(int)};
	memcpy(reinterpret_cast<std::byte*>(fp)+offset,std::addressof(flags),sizeof(long));
}

inline int get_fp_bufsiz(FILE* fp) noexcept
{
	constexpr std::size_t offset{sizeof(uintptr_t)*2+sizeof(int)+sizeof(long)*2+sizeof(int)};
	int b;
	memcpy(std::addressof(b),reinterpret_cast<std::byte*>(fp)+offset,sizeof(int));
	return b;
}

}

inline char* ibuffer_begin(c_io_observer_unlocked cio)
{
	using namespace details::ucrt_hack;
	return get_fp_base(cio.fp);
}

inline char* ibuffer_curr(c_io_observer_unlocked cio)
{
	using namespace details::ucrt_hack;
	return get_fp_ptr(cio.fp);
}

inline char* ibuffer_end(c_io_observer_unlocked cio)
{
	using namespace details::ucrt_hack;
	return get_fp_ptr(cio.fp)+get_fp_cnt(cio.fp);
}

inline void ibuffer_set_curr(c_io_observer_unlocked cio,char* ptr)
{
	using namespace details::ucrt_hack;
	set_fp_cnt(cio.fp,get_fp_ptr(cio.fp)-ptr+get_fp_cnt(cio.fp));
	set_fp_ptr(cio.fp,ptr);
}
//extern "C" int __stdcall __acrt_stdio_refill_and_read_narrow_nolock(FILE*) noexcept;

inline bool underflow(c_io_observer_unlocked cio)
{
	using namespace details::ucrt_hack;
	ibuffer_set_curr(cio,ibuffer_end(cio));
	if(_fgetc_nolock(cio.fp)==EOF)[[unlikely]]
		return false;
	set_fp_cnt(cio.fp,get_fp_cnt(cio.fp)+1);
	set_fp_ptr(cio.fp,get_fp_ptr(cio.fp)-1);
	return true;
}

inline char* obuffer_begin(c_io_observer_unlocked cio)
{
	using namespace details::ucrt_hack;
	return get_fp_base(cio.fp);
}

inline char* obuffer_curr(c_io_observer_unlocked cio)
{
	using namespace details::ucrt_hack;
	return get_fp_ptr(cio.fp);
}

inline char* obuffer_end(c_io_observer_unlocked cio)
{
	using namespace details::ucrt_hack;
	return get_fp_base(cio.fp)+get_fp_bufsiz(cio.fp);
}

inline void obuffer_set_curr(c_io_observer_unlocked cio,char* ptr)
{
	using namespace details::ucrt_hack;
	set_fp_cnt(cio.fp,get_fp_ptr(cio.fp)-ptr+get_fp_cnt(cio.fp));
	set_fp_ptr(cio.fp,ptr);
	set_fp_flags(cio.fp,get_fp_flags(cio.fp)|0x010000);
}

//extern "C" int __stdcall __acrt_stdio_flush_and_write_narrow_nolock(int,FILE*) noexcept;
inline void overflow(c_io_observer_unlocked cio,char ch)
{
	obuffer_set_curr(cio,obuffer_end(cio));
	if(_fputc_nolock(static_cast<int>(static_cast<unsigned char>(ch)),cio.fp)==EOF)[[unlikely]]
		throw_posix_error();
}

[[gnu::may_alias]] inline wchar_t* ibuffer_begin(wc_io_observer_unlocked cio)
{
	return details::ucrt_hack::get_fp_base<wchar_t>(cio.fp);
}

[[gnu::may_alias]] inline wchar_t* ibuffer_curr(wc_io_observer_unlocked cio)
{
	using namespace details::ucrt_hack;
	return get_fp_ptr<wchar_t>(cio.fp);
}

[[gnu::may_alias]] inline wchar_t* ibuffer_end(wc_io_observer_unlocked cio)
{
	using namespace details::ucrt_hack;
	return get_fp_ptr<wchar_t>(cio.fp)+(get_fp_cnt(cio.fp)/sizeof(wchar_t));
}

inline void ibuffer_set_curr(wc_io_observer_unlocked cio, [[gnu::may_alias]] wchar_t* ptr)
{
	using namespace details::ucrt_hack;
	set_fp_cnt(cio.fp,(get_fp_ptr<wchar_t>(cio.fp)-ptr)*sizeof(wchar_t)+get_fp_cnt(cio.fp));
	set_fp_ptr(cio.fp,ptr);
}

inline bool underflow(wc_io_observer_unlocked cio)
{
	using namespace details::ucrt_hack;
	ibuffer_set_curr(cio,ibuffer_end(cio));
	if(_fgetwc_nolock(cio.fp)==EOF)[[unlikely]]
		return false;
	set_fp_cnt(cio.fp,get_fp_cnt(cio.fp)+1);
	set_fp_ptr(cio.fp,get_fp_ptr(cio.fp)-1);
	return true;
}

[[gnu::may_alias]] inline wchar_t* obuffer_begin(wc_io_observer_unlocked cio)
{
	using namespace details::ucrt_hack;
	return get_fp_base<wchar_t>(cio.fp);
}

[[gnu::may_alias]] inline wchar_t* obuffer_curr(wc_io_observer_unlocked cio)
{
	using namespace details::ucrt_hack;
	return get_fp_ptr<wchar_t>(cio.fp);
}

[[gnu::may_alias]] inline wchar_t* obuffer_end(wc_io_observer_unlocked cio)
{
	using namespace details::ucrt_hack;
	return get_fp_base<wchar_t>(cio.fp)+get_fp_bufsiz(cio.fp)/sizeof(wchar_t);
}

inline void obuffer_set_curr(wc_io_observer_unlocked cio,[[gnu::may_alias]] wchar_t* ptr)
{
	using namespace details::ucrt_hack;
	set_fp_cnt(cio.fp,(get_fp_ptr<wchar_t>(cio.fp)-ptr)*sizeof(wchar_t)+get_fp_cnt(cio.fp));
	set_fp_ptr(cio.fp,ptr);
	set_fp_flags(cio.fp,get_fp_flags(cio.fp)|0x010000);
}

//extern "C" wint_t __stdcall __acrt_stdio_flush_and_write_wide_nolock(wint_t,FILE*) noexcept;

inline void overflow(wc_io_observer_unlocked cio,wchar_t ch)
{
	using namespace details::ucrt_hack;
	obuffer_set_curr(cio,obuffer_end(cio));
	if(_fputwc_nolock(static_cast<wint_t>(static_cast<std::make_unsigned_t<wchar_t>>(ch)),cio.fp)==WEOF)[[unlikely]]
		throw_posix_error();
}

inline bool obuffer_is_active(c_io_observer_unlocked cio)
{
	return details::ucrt_hack::get_fp_base(cio.fp);
}

inline bool obuffer_is_active(wc_io_observer_unlocked cio)
{
	return details::ucrt_hack::get_fp_base(cio.fp);
}

static_assert(buffer_io_stream<c_io_observer_unlocked>);
static_assert(buffer_io_stream<wc_io_observer_unlocked>);
static_assert(maybe_buffer_output_stream<c_io_observer_unlocked>);
static_assert(maybe_buffer_output_stream<wc_io_observer_unlocked>);
static_assert(!buffer_io_stream<basic_c_io_observer_unlocked<char8_t>>);
}