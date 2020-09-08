#pragma once
extern "C" char **environ;
namespace fast_io
{
namespace details
{

template<typename... Args>
inline void execve_impl(Args&& ...args)
{
	system_call_throw_error(
#if defined(__linux__)&&(defined(__x86_64__) || defined(__arm64__) || defined(__aarch64__) )
		system_call<
#if defined(__x86_64__)
			59
#elif defined(__arm64__) || defined(__aarch64__)
			221
#endif
			,int>
#elif defined(__WINNT__) || defined(_MSC_VER)
		::_execve
#else
		::execve
#endif
	(std::forward<Args>(args)...));
}
}

inline void posix_exec(std::string path)
{
	std::array<char*,2> arr{path.data()};
	details::execve_impl(path.data(),arr.data(),environ);
}

namespace details
{

template<std::contiguous_iterator Iter>
inline void deal_with_paramters(
	Iter begin,
	Iter end,
	std::contiguous_iterator auto output)
{
	for(;begin!=end;++begin)
	{
		*output=begin->data();
		++output;
	}
	*output={};
}

inline void posix_exec_impl(std::ranges::contiguous_range auto& temp,
	std::string& path,
	std::ranges::contiguous_range auto& args)
{
	temp.front()=path.data();
	deal_with_paramters(
		args.begin(),
		args.end(),
		temp.data()+1);
	execve_impl(path.data(),temp.data(),environ);
}
}

inline void posix_exec(std::string path,std::vector<std::string> args)
{
	if(args.size()<126)[[likely]]
	{
		std::array<char*,128> temp;
		details::posix_exec_impl(temp,path,args);
	}
	else
	{
		std::vector<char*> temp(args.size()+1);
		details::posix_exec_impl(temp,path,args);
	}
}

template<std::size_t n>
inline void posix_exec(std::string path,std::array<std::string,n> args)
{
	if constexpr(n<126)
	{
		std::array<char*,n+2> temp;
		details::posix_exec_impl(temp,path,args);
	}
	else
	{
		std::vector<char*> temp(n+1);
		details::posix_exec_impl(temp,path,args);
	}
}

/*
inline void posix_exec(cstring_view path,std::ranges::sized_range auto& parameters,std::ranges::sized_range auto& environment)
{
	if(std::ranges::size(parameters)<127)[[likely]]
	{
		std::array<char*,128> temp;
		if(std::ranges::size(parameters)<127)
		{
			std::array<char*,128> env_temp;
			details::posix_exec_impl(temp,path,parameters);
		}
	}
	else
	{
		std::vector<char*> temp(std::ranges::size(parameters)+1);
		details::posix_exec_impl(temp,path,parameters);
	}
}
*/

}