#pragma once

namespace fast_io::details
{

inline int sys_dup(int old_fd)
{
	auto fd{
#if defined(__linux__)&&(defined(__x86_64__) || defined(__arm64__) || defined(__aarch64__) )
		system_call<
#if defined(__x86_64__)
		32
#elif defined(__arm64__) || defined(__aarch64__) 
		23
#endif
		,int>
#elif _WIN32
		_dup
#else
		dup
#endif
	(old_fd)};
	system_call_throw_error(fd);
	return fd;
}

inline int sys_dup2(int old_fd,int new_fd)
{
	auto fd{
#if defined(__linux__)&&(defined(__x86_64__) || defined(__arm64__) || defined(__aarch64__) )
		system_call<
#if defined(__x86_64__)
		33
#elif defined(__arm64__) || defined(__aarch64__) 
		1041
#endif
		,int>
#elif _WIN32
		_dup2
#else
		dup2
#endif
	(old_fd,new_fd)};
	system_call_throw_error(fd);
	return fd;
}

inline int sys_close(int fd) noexcept
{
	return 
#if defined(__linux__)&&(defined(__x86_64__) || defined(__arm64__) || defined(__aarch64__) )
	system_call<
#if defined(__x86_64__)
		3
#elif defined(__arm64__) || defined(__aarch64__) 
		57
#endif
		,int>
#elif _WIN32
		_close
#else
		close
#endif
	(fd);
}

inline void sys_close_throw_error(int fd)
{
	system_call_throw_error(sys_close(fd));
}

}
