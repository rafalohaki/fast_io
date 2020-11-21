#pragma once

//fast_io_devices.h defines commonly used io devices and their correlated mutex verisons.
#include"fast_io_hosted.h"


namespace fast_io
{

/*
Special file type region. They are usually not used for I/O but for other purpose like I/O redirection.
However, they are still capable of doing I/O.
*/

using dir_file = directory_file_wrapper<basic_native_file<char>>;
using pipe = basic_native_pipe<char>;

/*
template region
*/

template<std::integral char_type>
using basic_ibuf_pipe = basic_ibuf<basic_native_pipe<char_type>>;
template<std::integral char_type>
using basic_obuf_pipe = basic_obuf<basic_native_pipe<char_type>>;
template<std::integral char_type>
using basic_iobuf_pipe = basic_iobuf<basic_native_pipe<char_type>>;

template<std::integral char_type>
using basic_inative_file = input_file_wrapper<basic_native_file<char_type>>;
template<std::integral char_type>
using basic_onative_file = output_file_wrapper<basic_native_file<char_type>>;

template<std::integral char_type>
using basic_ibuf_file = basic_ibuf<basic_inative_file<char_type>>;
template<std::integral char_type>
using basic_obuf_file = basic_obuf<basic_onative_file<char_type>>;
template<std::integral char_type>
using basic_iobuf_file = basic_iobuf<basic_native_file<char_type>>;

template<std::integral char_type>
using basic_ibuf_file_mutex = basic_iomutex<basic_ibuf_file<char_type>>;
template<std::integral char_type>
using basic_obuf_file_mutex = basic_iomutex<basic_obuf_file<char_type>>;
template<std::integral char_type>
using basic_iobuf_file_mutex = basic_iomutex<basic_iobuf_file<char_type>>;

/*
Text stream region. Highly likely change definition in the future due to bad API.
*/

template<output_stream output>
using basic_obuf_text = basic_obuf<basic_indirect_obuffer_constructor_source_type<typename output::char_type,output,transforms::binary_to_text<>>,true>;

template<input_stream input>
using basic_ibuf_text = basic_indirect_ibuffer_constructor_source_type<basic_ibuf<input>,vector_buffer<typename input::char_type>,transforms::text_to_binary<>>;

using ibuf_text_file = basic_ibuf_text<basic_inative_file<char>>;
using obuf_text_file = basic_obuf_text<basic_onative_file<char>>;

/*
char region
*/
using ibuf_pipe = basic_ibuf_pipe<char>;
using obuf_pipe = basic_obuf_pipe<char>;
using iobuf_pipe = basic_iobuf_pipe<char>;

using inative_file = basic_inative_file<char>;
using onative_file = basic_onative_file<char>;

using ibuf_file = basic_ibuf_file<char>;
using obuf_file = basic_obuf_file<char>;
using iobuf_file = basic_iobuf_file<char>;

#if !defined(__NEWLIB__)||defined(_GLIBCXX_HAS_GTHREADS)
#ifndef __MSDOS__
using ibuf_file_mutex = basic_ibuf_file_mutex<char>;
using obuf_file_mutex = basic_obuf_file_mutex<char>;
using iobuf_file_mutex = basic_iobuf_file_mutex<char>;
#endif
#endif

/*
wchar_t region
*/
#ifndef __MSDOS__
/*
libstdc++ bug on GCC 10. wchar_t does not satisfy the std::integral concept on MSDOS DJGPP toolchain.
Remove restriction after GCC 11.0 is released.
*/
using wibuf_pipe = basic_ibuf_pipe<wchar_t>;
using wobuf_pipe = basic_obuf_pipe<wchar_t>;
using wiobuf_pipe = basic_iobuf_pipe<wchar_t>;

using winative_file = basic_inative_file<wchar_t>;
using wonative_file = basic_onative_file<wchar_t>;

using wibuf_file = basic_ibuf_file<wchar_t>;
using wobuf_file = basic_obuf_file<wchar_t>;
using wiobuf_file = basic_iobuf_file<wchar_t>;

#if !defined(__NEWLIB__)||defined(_GLIBCXX_HAS_GTHREADS)
#ifndef __MSDOS__
using wibuf_file_mutex = basic_ibuf_file_mutex<wchar_t>;
using wobuf_file_mutex = basic_obuf_file_mutex<wchar_t>;
using wiobuf_file_mutex = basic_iobuf_file_mutex<wchar_t>;
#endif
#endif
#endif

/*
char8_t region
*/
using u8ibuf_pipe = basic_ibuf_pipe<char8_t>;
using u8obuf_pipe = basic_obuf_pipe<char8_t>;
using u8iobuf_pipe = basic_iobuf_pipe<char8_t>;

using u8inative_file = basic_inative_file<char8_t>;
using u8onative_file = basic_onative_file<char8_t>;

using u8ibuf_file = basic_ibuf_file<char8_t>;
using u8obuf_file = basic_obuf_file<char8_t>;
using u8iobuf_file = basic_iobuf_file<char8_t>;

#if !defined(__NEWLIB__)||defined(_GLIBCXX_HAS_GTHREADS)
#ifndef __MSDOS__
using u8ibuf_file_mutex = basic_ibuf_file_mutex<char8_t>;
using u8obuf_file_mutex = basic_obuf_file_mutex<char8_t>;
using u8iobuf_file_mutex = basic_iobuf_file_mutex<char8_t>;
#endif
#endif

/*
char16_t region
*/
using u16ibuf_pipe = basic_ibuf_pipe<char16_t>;
using u16obuf_pipe = basic_obuf_pipe<char16_t>;
using u16iobuf_pipe = basic_iobuf_pipe<char16_t>;

using u16inative_file = basic_inative_file<char16_t>;
using u16onative_file = basic_onative_file<char16_t>;

using u16ibuf_file = basic_ibuf_file<char16_t>;
using u16obuf_file = basic_obuf_file<char16_t>;
using u16iobuf_file = basic_iobuf_file<char16_t>;

#if !defined(__NEWLIB__)||defined(_GLIBCXX_HAS_GTHREADS)
#ifndef __MSDOS__
using u16ibuf_file_mutex = basic_ibuf_file_mutex<char16_t>;
using u16obuf_file_mutex = basic_obuf_file_mutex<char16_t>;
using u16iobuf_file_mutex = basic_iobuf_file_mutex<char16_t>;
#endif
#endif

/*
char32_t region
*/
using u32ibuf_pipe = basic_ibuf_pipe<char32_t>;
using u32obuf_pipe = basic_obuf_pipe<char32_t>;
using u32iobuf_pipe = basic_iobuf_pipe<char32_t>;

using u32inative_file = basic_inative_file<char32_t>;
using u32onative_file = basic_onative_file<char32_t>;

using u32ibuf_file = basic_ibuf_file<char32_t>;
using u32obuf_file = basic_obuf_file<char32_t>;
using u32iobuf_file = basic_iobuf_file<char32_t>;

#if !defined(__NEWLIB__)||defined(_GLIBCXX_HAS_GTHREADS)
#ifndef __MSDOS__
using u32ibuf_file_mutex = basic_ibuf_file_mutex<char32_t>;
using u32obuf_file_mutex = basic_obuf_file_mutex<char32_t>;
using u32iobuf_file_mutex = basic_iobuf_file_mutex<char32_t>;
#endif
#endif


/*
utf encoding stream region. Highly likely change in the future due to bad API.
*/

template<std::integral new_code_type,output_stream output>
using basic_obuf_utf = basic_obuf<basic_indirect_obuffer_constructor_source_type<new_code_type,output,transforms::utf>,true>;

template<std::integral new_code_type,input_stream input>
using basic_ibuf_utf = basic_indirect_ibuffer_constructor_source_type<basic_ibuf<input>,vector_buffer<new_code_type>,transforms::utf>;

template<std::integral new_code_type,std::integral source_code_type>
using basic_obuf_utf_file = basic_obuf_utf<new_code_type,basic_onative_file<source_code_type>>;

template<std::integral new_code_type,std::integral source_code_type>
using basic_ibuf_utf_file = basic_ibuf_utf<new_code_type,basic_inative_file<source_code_type>>;

template<std::integral new_code_type>
requires (1<sizeof(new_code_type))
using obuf_utf8_file=basic_obuf_utf_file<new_code_type,char8_t>;

template<std::integral new_code_type>
requires (1<sizeof(new_code_type))
using ibuf_utf8_file=basic_ibuf_utf_file<new_code_type,char8_t>;


}
