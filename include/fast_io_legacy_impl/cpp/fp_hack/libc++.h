#pragma once
#include<__std_stream>

namespace fast_io::details::streambuf_hack
{

/*
https://github.com/llvm-mirror/libcxx/blob/master/include/__std_stream
private:
    FILE* __file_;
    const codecvt<char_type, char, state_type>* __cv_;
    state_type* __st_;
    int __encoding_;
    int_type __last_consumed_;
    bool __last_consumed_is_next_;
    bool __always_noconv_;
*/

template<typename T>
inline FILE* stdinbuf_stdoutbuf_fp_hack(T* stdbuf) noexcept
{
	FILE* fp{};
	// we can only do this or ubsanitizer will complain. Do not do down_cast
	memcpy(std::addressof(fp),reinterpret_cast<std::byte*>(stdbuf)+
	sizeof(std::basic_streambuf<typename T::char_type, typename T::traits_type>),sizeof(fp));
	return fp;
}

/*
https://github.com/llvm-mirror/libcxx/blob/master/include/fstream
private:
  char* __extbuf_;
  const char* __extbufnext_;
  const char* __extbufend_;
  char __extbuf_min_[8];
  size_t __ebs_;
  char_type* __intbuf_;
  size_t __ibs_;
  FILE* __file_;
  const codecvt<char_type, char, state_type>* __cv_;
  state_type __st_;
  state_type __st_last_;
  ios_base::openmode __om_;
  ios_base::openmode __cm_;
  bool __owns_eb_;
  bool __owns_ib_;
  bool __always_noconv_;
*/


template<typename char_type,typename traits_type>
inline std::FILE* fp_hack(std::basic_filebuf<char_type,traits_type>* fbuf) noexcept
{
	std::FILE* fp{};
	// we can only do this or ubsanitizer will complain. Do not do down_cast
	memcpy(std::addressof(fp),
	reinterpret_cast<std::byte*>(fbuf)+sizeof(std::basic_streambuf<char_type, traits_type>)+
	4*sizeof(uintptr_t)+8+2*sizeof(std::size_t),sizeof(fp));
	return fp;
}


template<typename T>
requires (std::same_as<T,std::basic_streambuf<typename T::char_type,typename T::traits_type>>)
inline std::FILE* fp_hack(T* stdbuf) noexcept
{
#ifdef __cpp_rtti
	using char_type = typename T::char_type;
	using traits_type = typename T::traits_type;
#ifdef __cpp_exceptions
	try
	{
#endif
		cstring_view stdin_type{typeid(std::__stdinbuf<char_type>).name()};
		cstring_view my_type{typeid(*stdbuf).name()};
		if(my_type==stdin_type)
			return stdinbuf_stdoutbuf_fp_hack(stdbuf);
		cstring_view stdout_type{typeid(std::__stdoutbuf<char_type>).name()};
		if(my_type==stdout_type)
			return stdinbuf_stdoutbuf_fp_hack(stdbuf);
		auto fbf{dynamic_cast<std::basic_filebuf<char_type,traits_type>*>(stdbuf)};
		if(fbf)
			return fp_hack(stdbuf);
#ifdef __cpp_exceptions
	}
	catch(...){}
#endif
#endif
	errno=EBADF;
	return nullptr;

}
}