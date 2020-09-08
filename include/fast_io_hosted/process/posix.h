#pragma once

namespace fast_io
{

namespace details
{
template<typename... Args>
requires(sizeof...(Args)==2)
inline auto my_dup2(Args&& ...args)
{
	int new_fd(
#if defined(__linux__)&&(defined(__x86_64__) || defined(__arm64__) || defined(__aarch64__) )
		system_call<
#if defined(__x86_64__)
			33
#elif defined(__arm64__) || defined(__aarch64__) 
			1041
#endif
		,int>
#else
		dup2
#endif
	(std::forward<Args>(args)...));
	system_call_throw_error(new_fd);
	return new_fd;
}

inline void my_close(int fd)
{
	if(fd!=-1)
#if defined(__linux__)&&(defined(__x86_64__) || defined(__arm64__) || defined(__aarch64__) )
		system_call<
#if defined(__x86_64__)
			3
#elif defined(__arm64__) || defined(__aarch64__) 
			57
#endif
		,int>(fd);
#else
		close(fd);
#endif
}

template<int number,bool parent=false>
inline void redirect(io_observer& ob)
{
	std::visit([](auto&& arg)
	{
		using T = std::decay_t<decltype(arg)>;
		if constexpr(parent)
		{
			if constexpr(std::same_as<T,std::array<int*,2>>)
			{
				if constexpr(number==0)
				{
					my_close(*arg.front());
					*arg.front()=-1;
				}
				else
				{
					my_close(*arg.back());
					*arg.back()=-1;
				}
			}
		}
		else
		{
			if constexpr(std::same_as<T,int>)
			{
				my_dup2(arg,number);
			}
			else if constexpr(std::same_as<T,std::array<int*,2>>)
			{
				if constexpr(number==0)
				{
					my_dup2(*arg.front(),number);
					my_close(*arg.back());
				}
				else
				{
					my_dup2(*arg.back(),number);
					my_close(*arg.front());
				}
			}
		}
	},ob.variant);
}

template<bool is_child>
inline void prepare_for_exec(cstring_view& path,
				std::vector<cstring_view>& args,
				process_io& io)
{
if constexpr(is_child)
{
	redirect<0>(io.in);
	redirect<1>(io.out);
	redirect<2>(io.err);
	std::string p(path.data(),path.data()+path.size());
	std::vector<std::string> vec;
	vec.reserve(args.size());
	for(auto const& e : args)
		vec.emplace_back(e.data(),e.data()+e.size());
	posix_exec(std::move(p),std::move(vec));
}
else
{
	redirect<0,true>(io.in);
	redirect<1,true>(io.out);
	redirect<2,true>(io.err);
}
}
}

class posix_process
{
	pid_t pid;
public:
	using native_handle_t = pid_t;
	posix_process(native_interface_t):pid(
#if defined(__linux__)&&(defined(__x86_64__) || defined(__arm64__) || defined(__aarch64__) )
	system_call<
#if defined(__x86_64__)
		57
#elif defined(__arm64__) || defined(__aarch64__)
		1079
#endif
	,pid_t>
#else
	fork
#endif
())
	{
		system_call_throw_error(pid);
	}
	posix_process(cstring_view path,
				std::vector<cstring_view> args,
				process_io io):posix_process(native_interface)
	{
		if(!pid)
			details::prepare_for_exec<true>(path,args,io);
		else
			details::prepare_for_exec<false>(path,args,io);
	}
	inline auto native_handle() const
	{
		return pid;
	}
	inline void detach()
	{
		pid=-1;
	}
	void join()
	{
		system_call_throw_error(
	#if defined(__linux__)&&(defined(__x86_64__) || defined(__arm64__) || defined(__aarch64__) )
		system_call<
		#if defined(__x86_64__)
			61
		#elif defined(__arm64__) || defined(__aarch64__) 
			260
		#endif
		,int>(pid,nullptr,0,nullptr)
	#else
		waitpid(pid,nullptr,0)
	#endif
		);
	}
	inline auto id() const
	{
		return pid;
	}
};

inline bool is_child(posix_process const& p)
{
	return !p.native_handle();
}
inline bool is_parent(posix_process const& p)
{
	return 0<p.native_handle();
}

inline bool has_detached(posix_process const& p)
{
	return p.native_handle()<0;
}

class posix_jprocess
{
	pid_t pid;
	void close_impl()
	{
		if(0<pid)
		{
	#if defined(__linux__)&&(defined(__x86_64__) || defined(__arm64__) || defined(__aarch64__) )
		system_call<
		#if defined(__x86_64__)
			61
		#elif defined(__arm64__) || defined(__aarch64__)
			260
		#endif
		,int>(pid,nullptr,0,nullptr);
	#else
		waitpid(pid,nullptr,0);
	#endif
		}
	}
public:
	using native_handle_t = pid_t;
	posix_jprocess(native_interface_t):pid(
#if defined(__linux__)&&(defined(__x86_64__) || defined(__arm64__) || defined(__aarch64__) )
	system_call<
	#if defined(__x86_64__) 
		57
	#elif defined(__arm64__) || defined(__aarch64__)
		1079
	#endif
	,pid_t>
#else
	fork
#endif
())
	{
		system_call_throw_error(pid);
	}
	posix_jprocess(cstring_view path,
				std::vector<cstring_view> args,
				process_io io):posix_jprocess(native_interface)
	{
		if(!pid)
			details::prepare_for_exec<true>(path,args,io);
		else
			details::prepare_for_exec<false>(path,args,io);
	}
	inline auto native_handle() const
	{
		return pid;
	}
	inline void join()
	{
		system_call_throw_error(
	#if defined(__linux__)&&(defined(__x86_64__) || defined(__arm64__) || defined(__aarch64__) )
		system_call<
		#if defined(__x86_64__)
			61
		#elif defined(__arm64__) || defined(__aarch64__)
			260
		#endif
		,int>(pid,nullptr,0,nullptr)
	#else
		waitpid(pid,nullptr,0)
	#endif
		);
	}
	inline void detach()
	{
		pid=-1;
	}
	posix_jprocess(posix_jprocess const&)=delete;
	posix_jprocess& operator=(posix_jprocess const&)=delete;
	posix_jprocess(posix_jprocess&& bmv) noexcept:pid(bmv.pid)
	{
		bmv.pid=-1;
	}
	posix_jprocess& operator=(posix_jprocess&& bmv) noexcept
	{
		if(pid!=bmv.pid)
		{
			close_impl();
			pid=bmv.pid;
			bmv.pid=-1;
		}
		return *this;
	}
	~posix_jprocess()
	{
		close_impl();
	}
	inline auto id() const
	{
		return pid;
	}
};
inline bool is_child(posix_jprocess const& p)
{
	return !p.native_handle();
}
inline bool is_parent(posix_jprocess const& p)
{
	return 0<p.native_handle();
}
inline bool has_detached(posix_jprocess const& p)
{
	return p.native_handle()<0;
}

}