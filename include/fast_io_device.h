#pragma once

//fast_io_devices.h defines commonly used io devices and their correlated mutex verisons.
#include"fast_io_hosted.h"


namespace fast_io
{
using ibuf_pipe = basic_ibuf<native_pipe>;

template<std::integral char_type>
using basic_inative_file = input_file_wrapper<basic_native_file<char_type>>;
template<std::integral char_type>
using basic_onative_file = output_file_wrapper<basic_native_file<char_type>>;
template<std::integral char_type>
using basic_ionative_file = io_file_wrapper<basic_native_file<char_type>>;

using dir_file = io_dir_file_wrapper<basic_native_file<char>>;

template<std::integral char_type>
using basic_ibuf_file = basic_ibuf<basic_inative_file<char_type>>;
template<std::integral char_type>
using basic_obuf_file = basic_obuf<basic_onative_file<char_type>>;
template<std::integral char_type>
using basic_iobuf_file = basic_iobuf<basic_ionative_file<char_type>>;

using inative_file = input_file_wrapper<native_file>;
using onative_file = output_file_wrapper<native_file>;
using ionative_file = io_file_wrapper<native_file>;

template<output_stream output>
using basic_obuf_text = basic_obuf<basic_indirect_obuffer_constructor_source_type<typename output::char_type,output,transforms::binary_to_text<>>,true>;

template<input_stream input>
using basic_ibuf_text = basic_indirect_ibuffer_constructor_source_type<basic_ibuf<input>,vector_buffer<typename input::char_type>,transforms::text_to_binary<>>;

using ibuf_file = basic_ibuf<inative_file>;
using obuf_file = basic_obuf<onative_file>;
using iobuf_file = basic_iobuf<ionative_file>;

using ibuf_text_file = basic_ibuf_text<inative_file>;
using obuf_text_file = basic_obuf_text<onative_file>;
#if !defined(__NEWLIB__)||defined(_GLIBCXX_HAS_GTHREADS)
#ifndef __MSDOS__
using ibuf_file_mutex = basic_iomutex<ibuf_file>;
using obuf_file_mutex = basic_iomutex<obuf_file>;
using iobuf_file_mutex = basic_iomutex<iobuf_file>;
#endif
#endif
// utf-8
using u8pipe = u8native_pipe;

using u8inative_file = input_file_wrapper<u8native_file>;
using u8onative_file = output_file_wrapper<u8native_file>;
using u8ionative_file = io_file_wrapper<u8native_file>;


using u8ibuf_file = basic_ibuf<u8inative_file>;
using u8obuf_file = basic_obuf<u8onative_file>;
using u8iobuf_file = basic_iobuf<u8ionative_file>;
#if !defined(__NEWLIB__)||defined(_GLIBCXX_HAS_GTHREADS)
#ifndef __MSDOS__
using u8ibuf_file_mutex = basic_iomutex<u8ibuf_file>;
using u8obuf_file_mutex = basic_iomutex<u8obuf_file>;
using u8iobuf_file_mutex = basic_iomutex<u8iobuf_file>;
#endif
#endif

#ifndef __MSDOS__
using wpipe = wnative_pipe;
using winative_file = input_file_wrapper<wnative_file>;
using wonative_file = output_file_wrapper<wnative_file>;
using wionative_file = io_file_wrapper<wnative_file>;

using wibuf_file = basic_ibuf<winative_file>;
using wobuf_file = basic_obuf<wonative_file>;
using wiobuf_file = basic_iobuf<wionative_file>;
#if !defined(__NEWLIB__)||defined(_GLIBCXX_HAS_GTHREADS)
using wibuf_file_mutex = basic_iomutex<wibuf_file>;
using wobuf_file_mutex = basic_iomutex<wobuf_file>;
using wiobuf_file_mutex = basic_iomutex<wiobuf_file>;
#endif
#endif
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
