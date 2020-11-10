#pragma once

namespace fast_io
{

struct posix_wait_status
{
	int wait_loc{};
};

inline constexpr posix_wait_reason reason(posix_wait_status pws) noexcept
{
#ifdef WIFEXITED
	if(WIFEXITED(pws.wait_loc))
		return posix_wait_reason::if_exited;
#endif
#ifdef WIFSIGNALED
	if(WIFSIGNALED(pws.wait_loc))
		return posix_wait_reason::if_signaled;
#endif
#ifdef WIFCORED
	if(WIFCORED(pws.wait_loc))
		return posix_wait_reason::if_core_dump;
#endif
	if(WIFSTOPPED(pws.wait_loc))
		return posix_wait_reason::if_stopped;
	return posix_wait_reason::unknown;
}

inline constexpr int exit_status(posix_wait_status pws) noexcept
{
	return WEXITSTATUS(pws.wait_loc);
}

inline constexpr int core_dump(posix_wait_status pws) noexcept
{
#ifdef WCOREDUMP
	return WCOREDUMP(pws.wait_loc);
#else
	return 0;
#endif
}

inline constexpr int core_signal(posix_wait_status pws) noexcept
{
#ifdef WCORESIG
	return WCORESIG(pws.wait_loc);
#else
	return 0;
#endif
}

inline constexpr int stop_signal(posix_wait_status pws) noexcept
{
	return WSTOPSIG(pws.wait_loc);
}

namespace details
{

inline pid_t posix_fork()
{
	pid_t pid{	
	#if defined(__linux__)
		system_call<__NR_fork,pid_t>()
	#else
		::fork()
	#endif
	};
	system_call_throw_error(pid);
	return pid;
}

inline posix_wait_status posix_waitpid(pid_t pid)
{
	posix_wait_status status;
	system_call_throw_error(
	#if defined(__linux__)
		system_call<__NR_wait4,int>(pid,std::addressof(status.wait_loc),0,nullptr)
	#else
		waitpid(pid,std::addressof(status.wait_loc),0)
	#endif
	);
	return status;
}

inline void posix_waitpid_noexcept(pid_t pid) noexcept
{
#if defined(__linux__)
	system_call<__NR_wait4,int>(pid,nullptr,0,nullptr);
#else
	waitpid(pid,nullptr,0);
#endif
}

inline [[noreturn]] void posix_execveat(int dirfd,char const* path,char* const argv,char* const envp)
{
#ifdef __linux__
	system_call<__NR_execveat,int>(dirfd,path,argv,envp,AT_SYMLINK_NOFOLLOW);
	fast_terminate();
#else
	int fd{::openat(dirfd,path,O_RDONLY|O_EXCL,0644))};
	if(fd==-1)
		fast_terminate();
	::fexecve(fd,argv,envp);
	fast_terminate();
#endif
}

inline int child_process_deal_with_process_io(posix_io_redirection& red,int fd) noexcept
{
	bool is_stdin{fd==0};
	if(red.pipe_fds)
	{
		auto v{red.pipe_fds[!is_stdin]};
		if(v!=-1)
			fd=v;
		int& closefd{red.pipe_fds[is_stdin]};
		if(closefd!=-1)
		{
			sys_close(closefd);
			closefd=-1;
		}
	}
	else if(red.fd!=-1)
		fd=red.fd;
	else if(red.dev_null)
		fd=-1;
	return fd;
}


inline void child_process_execveat(int dirfd,cstring_view csv,std::span<cstring_view> args,std::span<cstring_view> envp,posix_process_io& pio) noexcept
{
	auto args_uptr=new char const*[args.size()+1];
	for(std::size_t i{};i!=args.size();++i)
		args_uptr[i]=args[i].c_str();
	args_uptr[args.size()]=nullptr;
	auto envp_uptr=new char const*[envp.size()+1];
	for(std::size_t i{};i!=envp.size();++i)
		envp_uptr[i]=envp[i].c_str();
	envp_uptr[envp.size()]=nullptr;
	int in_fd{child_process_deal_with_process_io(pio.in,0)};
	int out_fd{child_process_deal_with_process_io(pio.out,1)};
	int err_fd{child_process_deal_with_process_io(pio.err,2)};
	if((in_fd==-1)|(out_fd==-1)|(err_fd==-1))
	{
		posix_file pf{my_posix_open<true>("/dev/null",O_RDWR,0644)};
		if(in_fd==-1)
			sys_dup2<true>(pf.fd,0);
		if(out_fd==-1)
			sys_dup2<true>(pf.fd,1);
		if(err_fd==-1)
			sys_dup2<true>(pf.fd,2);
	}
	if((in_fd!=-1)&(in_fd!=0))
		sys_dup2<true>(in_fd,0);
	if((out_fd!=-1)&(out_fd!=1))
		sys_dup2<true>(out_fd,1);
	if((err_fd!=-1)&(err_fd!=2))
		sys_dup2<true>(err_fd,2);	
	posix_execveat(dirfd,csv.c_str(),args_uptr.data(),envp_uptr.data());
};


template<bool is_stdin>
inline void parent_process_deal_with_process_io(posix_io_redirection& red) noexcept
{
	if(red.pipe_fds)
	{
		int& fd{red.pipe_fds[!is_stdin]};
		if(fd!=-1)
		{
			sys_close(fd);
			fd=-1;
		}
	}
}

inline pid_t posix_fork_execveat_impl(int dirfd,cstring_view csv,std::span<cstring_view> args,std::span<cstring_view> envp,posix_process_io& pio)
{
	pid_t pid{posix_fork()};
	if(pid)
	{
		parent_process_deal_with_process_io<true>(red.in);
		parent_process_deal_with_process_io<false>(red.out);
		parent_process_deal_with_process_io<false>(red.err);
		return pid;
	}
	child_process_execveat(dir,csv,args,envp,pio);
}

}

class posix_process_observer
{
public:
	using native_handle_type = pid_t;
	pid_t pid{-1};
	constexpr auto& native_handle() noexcept
	{
		return handle;
	}
	constexpr auto& native_handle() const noexcept
	{
		return handle;
	}
	explicit inline constexpr operator bool() const noexcept
	{
		return handle!=-1;
	}
	inline constexpr pid_t release() noexcept
	{
		auto temp{pid};
		pid=-1;
		return temp;
	}
};

inline constexpr bool is_child(posix_process_observer ppob) noexcept
{
	return 0<ppob.pid;
}

inline constexpr bool is_parent(posix_process_observer ppob) noexcept
{
	return ppob.pid==0;
}

inline constexpr pid_t detach(posix_process_observer ppob) noexcept
{
	return ppob.release();
}

inline constexpr posix_wait_status wait(posix_process_observer ppob)
{
	posix_wait_status status{details::details::waitpid(ppob.pid)};
	ppob.pid=-1;
	return status;
}

inline constexpr bool operator==(posix_process_observer a,posix_process_observer b) noexcept
{
	return a.pid==b.pid;
}

inline constexpr auto operator<=>(posix_process_observer a,posix_process_observer b) noexcept
{
	return a.pid<=>b.pid;
}

class posix_process:public posix_process_observer
{
public:
	using native_handle_type = pid_t;
	constexpr posix_process() noexcept =default;
	template<typename native_hd>
	requires std::same_as<native_handle_type,std::remove_cvref_t<native_hd>>
	explicit constexpr posix_process(native_hd pid) noexcept:
		posix_process_observer{pid}{}
	posix_process(posix_at_entry pate,cstring_view filename,std::span<cstring_view> args,std::span<cstring_view> envp,posix_process_io pio):
		posix_process_observer{details::posix_fork_execveat_impl(pate.fd,filename,args,envp,pio)}{}
	posix_process(cstring_view filename,std::span<cstring_view> args,std::span<cstring_view> envp,posix_process_io pio):
		posix_process_observer{details::posix_fork_execveat_impl(AT_FDCWD,filename,args,envp,pio)}{}
	posix_process(posix_process const&)=delete;
	posix_process& operator=(posix_process const&)=delete;
	constexpr posix_process(posix_process&& other) noexcept:posix_process_observer{other.pid}
	{
		other.pid=-1;
	}
	posix_process& operator=(posix_process&& other) noexcept
	{
		if(std::addressof(other)==this)
			return *this;
		details::posix_waitpid_noexcept(this->pid);
		this->pid=other.pid;
		other.pid=-1;
		return *this;
	}
	~posix_process()
	{
		details::posix_waitpid_noexcept(this->pid);
	}
};

}