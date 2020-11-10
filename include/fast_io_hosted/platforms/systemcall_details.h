#pragma once

namespace fast_io::details
{

#ifdef __MSDOS__
extern "C" int dup(int) noexcept;
extern "C" int dup2(int,int) noexcept;
extern "C" int _close(int) noexcept;
#endif

inline int sys_dup(int old_fd)
{
	auto fd{
#if defined(__linux__)
		system_call<__NR_dup,int>
#elif _WIN32
		_dup
#else
		dup
#endif
	(old_fd)};
	system_call_throw_error(fd);
	return fd;
}

template<bool always_terminate=false>
inline int sys_dup2(int old_fd,int new_fd)
{
	auto fd{
#if defined(__linux__)
		system_call<__NR_dup2,int>
#elif _WIN32
		_dup2
#else
		dup2
#endif
	(old_fd,new_fd)};
	system_call_throw_error<always_terminate>(fd);
	return fd;
}

inline int sys_close(int fd) noexcept
{
	return 
#if defined(__linux__)
	system_call<__NR_close,int>
#elif _WIN32 || __MSDOS__
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
