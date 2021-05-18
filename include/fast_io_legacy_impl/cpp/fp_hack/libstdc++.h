#pragma once
//https://www.ginac.de/~kreckel/fileno/
#ifdef __cpp_rtti
#include <ext/stdio_sync_filebuf.h>
#endif

namespace fast_io::details::streambuf_hack
{

template<typename char_type,typename traits_type>
inline std::FILE* fp_hack_impl(std::basic_filebuf<char_type,traits_type>* fbuf) noexcept
{
	std::FILE* fp{};
	// we can only do this or ubsanitizer will complain. Do not do down_cast
	::fast_io::details::my_memcpy(__builtin_addressof(fp),reinterpret_cast<std::byte*>(fbuf)+sizeof(std::basic_streambuf<char_type, traits_type>)+sizeof(std::__c_lock),sizeof(fp));
	return fp;
}

template<typename char_type,typename traits_type>
inline std::FILE* fp_hack(std::basic_filebuf<char_type,traits_type>* fbuf) noexcept
{
	if(fbuf==nullptr)
		return nullptr;
	return fp_hack_impl(fbuf);
}

template<typename T>
requires (std::same_as<T,std::basic_streambuf<typename T::char_type,typename T::traits_type>>)
inline std::FILE* fp_hack([[maybe_unused]] T* cio) noexcept
{
#ifdef __cpp_rtti
	using char_type = typename T::char_type;
	using traits_type = typename T::traits_type;
	if(cio)[[likely]]
	{
		auto fbuf{dynamic_cast<std::basic_filebuf<char_type,traits_type>*>(cio)};
		if(fbuf)
			return fp_hack_impl(fbuf);
		auto sync_fbuf=dynamic_cast<__gnu_cxx::stdio_sync_filebuf<char_type, traits_type>*>(cio);
		if(sync_fbuf)
			return sync_fbuf->file();
	}
#endif
	return nullptr;
}
/*
https://github.com/gcc-mirror/gcc/blob/master/libstdc++-v3/config/io/basic_file_stdio.h

      // Underlying data source/sink.
      __c_file* 	_M_cfile;

      // True iff we opened _M_cfile, and thus must close it ourselves.
      bool 		_M_cfile_created;

*/
struct libstdcxx_basic_file_char_model
{
	std::FILE* M_cfile;
	bool M_cfile_created;
};

template<typename char_type,typename traits_type>
inline void hack_set_close(std::basic_filebuf<char_type,traits_type>* fbuf) noexcept
{
	constexpr std::size_t m_cfile_created_offset{
		sizeof(std::basic_streambuf<char_type, traits_type>)+
		sizeof(std::__c_lock)
		+offsetof(libstdcxx_basic_file_char_model,M_cfile_created)};
	m_cfile_created_offset[reinterpret_cast<std::byte*>(fbuf)]=std::byte{1};
}

}