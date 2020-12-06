#pragma once
//https://www.ginac.de/~kreckel/fileno/
#ifdef __cpp_rtti
#include <ext/stdio_sync_filebuf.h>
#endif

namespace fast_io::details::streambuf_hack
{

template<typename char_type,typename traits_type>
inline std::FILE* fp_hack(std::basic_filebuf<char_type,traits_type>* fbuf) noexcept
{
	std::FILE* fp{};
	// we can only do this or ubsanitizer will complain. Do not do down_cast
	memcpy(std::addressof(fp),reinterpret_cast<std::byte*>(fbuf)+sizeof(std::basic_streambuf<char_type, traits_type>)+sizeof(std::__c_lock),sizeof(fp));
	return fp;
}

template<typename T>
requires (std::same_as<T,std::basic_streambuf<typename T::char_type,typename T::traits_type>>)
inline std::FILE* fp_hack(T* cio) noexcept
{
#ifdef __cpp_rtti
	using char_type = typename T::char_type;
	using traits_type = typename T::traits_type;
	auto fbuf{dynamic_cast<std::basic_filebuf<char_type,traits_type>*>(cio)};
	if(fbuf)
		return fp_hack(fbuf);
	auto sync_fbuf=dynamic_cast<__gnu_cxx::stdio_sync_filebuf<char_type, traits_type>*>(cio);
	if(sync_fbuf)
		return sync_fbuf->file();
#endif
	errno=EBADF;
	return nullptr;
}

}